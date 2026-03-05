

# File qfieldcloudprojectsmodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldcloud**](dir_d27ac98b23548f5810403c734ce9817b.md) **>** [**qfieldcloudprojectsmodel.cpp**](qfieldcloudprojectsmodel_8cpp.md)

[Go to the documentation of this file](qfieldcloudprojectsmodel_8cpp.md)


```C++
/***************************************************************************
    qfieldcloudprojectsmodel.cpp
    ---------------------
    begin                : January 2020
    copyright            : (C) 2020 by Matthias Kuhn
    email                : matthias at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "deltafilewrapper.h"
#include "deltalistmodel.h"
#include "layerobserver.h"
#include "qfieldcloudconnection.h"
#include "qfieldcloudprojectsmodel.h"
#include "qfieldcloudutils.h"

#include <QDir>
#include <QDirIterator>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QSettings>
#include <QTemporaryFile>
#include <qgis.h>
#include <qgsapplication.h>
#include <qgslocalizeddatapathregistry.h>
#include <qgsmessagelog.h>
#include <qgsnetworkaccessmanager.h>
#include <qgsproject.h>
#include <qgsproviderregistry.h>


QFieldCloudProjectsModel::QFieldCloudProjectsModel()
{
  // TODO all of these connects are a bit too much, and I guess not very precise, should be refactored!
  connect( this, &QFieldCloudProjectsModel::dataChanged, this, [this]( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles ) {
    Q_UNUSED( bottomRight )
    Q_UNUSED( roles )

    if ( roles.isEmpty() || roles.contains( StatusRole ) )
    {
      emit busyProjectIdsChanged();
    }
  } );
}

QFieldCloudConnection *QFieldCloudProjectsModel::cloudConnection() const
{
  return mCloudConnection;
}

void QFieldCloudProjectsModel::setCloudConnection( QFieldCloudConnection *cloudConnection )
{
  if ( mCloudConnection == cloudConnection )
  {
    return;
  }

  if ( mCloudConnection )
  {
    disconnect( mCloudConnection, &QFieldCloudConnection::statusChanged, this, &QFieldCloudProjectsModel::connectionStatusChanged );
    disconnect( mCloudConnection, &QFieldCloudConnection::usernameChanged, this, &QFieldCloudProjectsModel::usernameChanged );
    disconnect( mCloudConnection, &QFieldCloudConnection::urlChanged, this, &QFieldCloudProjectsModel::urlChanged );
    disconnect( mCloudConnection, &QFieldCloudConnection::queuedProjectPushRequested, this, nullptr );
  }

  mCloudConnection = cloudConnection;

  if ( mCloudConnection )
  {
    connect( mCloudConnection, &QFieldCloudConnection::statusChanged, this, &QFieldCloudProjectsModel::connectionStatusChanged );
    connect( mCloudConnection, &QFieldCloudConnection::usernameChanged, this, &QFieldCloudProjectsModel::usernameChanged );
    connect( mCloudConnection, &QFieldCloudConnection::urlChanged, this, &QFieldCloudProjectsModel::urlChanged );
    connect( mCloudConnection, &QFieldCloudConnection::queuedProjectPushRequested, this,
             [this]( const QString &projectId ) {
               // queuedProjectPush only provides projectId, so default shouldDownloadUpdates = false
               projectPush( projectId, false );
             } );
    mUsername = mCloudConnection->username();
    mUrl = mCloudConnection->url();
    resetProjects();
  }

  emit cloudConnectionChanged();
}

LayerObserver *QFieldCloudProjectsModel::layerObserver() const
{
  return mLayerObserver;
}

void QFieldCloudProjectsModel::setLayerObserver( LayerObserver *layerObserver )
{
  if ( mLayerObserver == layerObserver )
  {
    return;
  }

  mLayerObserver = layerObserver;

  if ( !layerObserver )
  {
    return;
  }

  emit layerObserverChanged();
}

QString QFieldCloudProjectsModel::currentProjectId() const
{
  return mCurrentProjectId;
}

void QFieldCloudProjectsModel::setCurrentProjectId( const QString &currentProjectId )
{
  if ( mCurrentProjectId == currentProjectId )
  {
    return;
  }

  mCurrentProjectId = currentProjectId;
  mCurrentProject = findProject( mCurrentProjectId );

  if ( mLayerObserver )
  {
    mLayerObserver->setDeltaFileWrapper( mCurrentProject ? mCurrentProject->deltaFileWrapper() : nullptr );
  }

  emit currentProjectIdChanged();
  emit currentProjectChanged();
}

QFieldCloudProject *QFieldCloudProjectsModel::currentProject() const
{
  return mCurrentProject.data();
}

QSet<QString> QFieldCloudProjectsModel::busyProjectIds() const
{
  QSet<QString> result;

  for ( const auto project : mProjects )
  {
    if ( project->status() != QFieldCloudProject::ProjectStatus::Idle && project->status() != QFieldCloudProject::ProjectStatus::Failing )
    {
      result.insert( project->id() );
    }
  }

  return result;
}

void QFieldCloudProjectsModel::refreshProjectsList( bool shouldResetModel, bool shouldFetchPublic, int projectFetchOffset )
{
  switch ( mCloudConnection->status() )
  {
    case QFieldCloudConnection::ConnectionStatus::LoggedIn:
    {
      QString url = shouldFetchPublic ? QStringLiteral( "/api/v1/projects/public/" ) : QStringLiteral( "/api/v1/projects/" );

      QVariantMap params;
      params["limit"] = QString::number( mProjectsPerFetch );
      params["offset"] = QString::number( projectFetchOffset );

      QNetworkRequest request( url );
      request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );
      request.setAttribute( QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::RedirectPolicy::NoLessSafeRedirectPolicy );

      request.setAttribute( static_cast<QNetworkRequest::Attribute>( ProjectsRequestAttribute::ResetModel ), shouldResetModel );
      request.setAttribute( static_cast<QNetworkRequest::Attribute>( ProjectsRequestAttribute::FetchPublicProjects ), shouldFetchPublic );
      request.setAttribute( static_cast<QNetworkRequest::Attribute>( ProjectsRequestAttribute::ProjectsFetchOffset ), projectFetchOffset );

      mIsRefreshing = true;
      emit isRefreshingChanged();

      mCloudConnection->setAuthenticationDetails( request );
      const NetworkReply *reply = mCloudConnection->get( request, url, params );
      connect( reply, &NetworkReply::finished, this, &QFieldCloudProjectsModel::projectListReceived );
      break;
    }
    case QFieldCloudConnection::ConnectionStatus::Disconnected:
    {
      // Nothing done at the moment; since the connection can be disconnected after discovering
      // a token has been invalidated, be careful with what is added here.
      break;
    }
    case QFieldCloudConnection::ConnectionStatus::Connecting:
      // Nothing done for this intermediary status.
      break;
  }
}

QModelIndex QFieldCloudProjectsModel::findProjectIndex( const QString &projectId ) const
{
  if ( projectId.isEmpty() )
  {
    QgsLogger::debug( QStringLiteral( "No project found for an empty project id." ) );
    return QModelIndex();
  }

  for ( int i = 0; i < mProjects.count(); i++ )
  {
    if ( mProjects.at( i )->id() == projectId )
    {
      return createIndex( i, 0 );
    }
  }

  QgsLogger::debug( QStringLiteral( "No project found with the provided id: `%1`" ).arg( projectId ) );

  return QModelIndex();
}

QFieldCloudProject *QFieldCloudProjectsModel::findProject( const QString &projectId ) const
{
  const QModelIndex index = findProjectIndex( projectId );

  if ( index.isValid() )
  {
    return mProjects[index.row()];
  }

  return nullptr;
}

void QFieldCloudProjectsModel::appendProject( const QString &projectId )
{
  if ( !mCloudConnection )
  {
    return;
  }

  const QModelIndex index = findProjectIndex( projectId );
  if ( index.isValid() )
  {
    emit projectAppended( projectId );
    return;
  }

  const QString url = QStringLiteral( "/api/v1/projects/%1/" ).arg( projectId );
  QNetworkRequest request( url );
  request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );
  request.setAttribute( QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::RedirectPolicy::NoLessSafeRedirectPolicy );
  request.setAttribute( static_cast<QNetworkRequest::Attribute>( QFieldCloudProjectsModel::ProjectsRequestAttribute::ProjectId ), projectId );
  mCloudConnection->setAuthenticationDetails( request );

  const NetworkReply *reply = mCloudConnection->get( request, url );
  connect( reply, &NetworkReply::finished, this, &QFieldCloudProjectsModel::projectReceived );
}

void QFieldCloudProjectsModel::removeLocalProject( const QString &projectId )
{
  QDir dir( QStringLiteral( "%1/%2/%3" ).arg( QFieldCloudUtils::localCloudDirectory(), mUsername, projectId ) );

  if ( dir.exists() )
  {
    const QModelIndex projectIndex = findProjectIndex( projectId );
    if ( projectIndex.isValid() )
    {
      QFieldCloudProject *project = mProjects[projectIndex.row()];
      project->removeLocally();

      if ( ( project->status() == QFieldCloudProject::ProjectStatus::Idle || project->status() == QFieldCloudProject::ProjectStatus::Failing ) && project->checkout() & QFieldCloudProject::RemoteCheckout )
      {
        emit dataChanged( projectIndex, projectIndex, QVector<int>() << StatusRole << LocalPathRole << CheckoutRole );
      }
      else
      {
        beginRemoveRows( QModelIndex(), projectIndex.row(), projectIndex.row() );
        delete mProjects.takeAt( projectIndex.row() );
        endRemoveRows();
      }
    }
  }
}

QString QFieldCloudProjectsModel::layerFileName( const QgsMapLayer *layer ) const
{
  return layer->dataProvider()->dataSourceUri().split( '|' )[0];
}

void QFieldCloudProjectsModel::projectCancelDownload( const QString &projectId )
{
  if ( !mCloudConnection )
  {
    return;
  }

  const QModelIndex projectIndex = findProjectIndex( projectId );
  if ( !projectIndex.isValid() )
  {
    return;
  }

  QFieldCloudProject *project = mProjects[projectIndex.row()];

  // before canceling, the project should be downloading
  if ( project->status() != QFieldCloudProject::ProjectStatus::Downloading )
  {
    return;
  }

  project->cancelDownload();

  emit dataChanged( projectIndex, projectIndex, QVector<int>() << StatusRole << ErrorStatusRole << PackagingStatusRole );
}

void QFieldCloudProjectsModel::projectPackageAndDownload( const QString &projectId )
{
  QgsLogger::debug( QStringLiteral( "Project %1: package and download initiated." ).arg( projectId ) );

  if ( !mCloudConnection )
  {
    return;
  }

  const QModelIndex projectIndex = findProjectIndex( projectId );
  if ( !projectIndex.isValid() )
  {
    QgsLogger::debug( QStringLiteral( "Project %1: package and download cancelled, the project is deleted." ).arg( projectId ) );
    return;
  }

  QFieldCloudProject *project = mProjects[projectIndex.row()];

  if ( project->status() != QFieldCloudProject::ProjectStatus::Idle )
  {
    QgsLogger::debug( QStringLiteral( "Project %1: package and download cancelled, the project is currently busy." ).arg( projectId ) );
    emit warning( tr( "Project busy." ) );
    return;
  }

  project->packageAndDownload();

  emit dataChanged( projectIndex, projectIndex );
}

void QFieldCloudProjectsModel::projectPush( const QString &projectId, const bool shouldDownloadUpdates )
{
  const QModelIndex projectIndex = findProjectIndex( projectId );

  if ( !projectIndex.isValid() )
  {
    return;
  }

  QFieldCloudProject *project = mProjects[projectIndex.row()];
  if ( !project )
  {
    return;
  }

  if ( !mCloudConnection )
  {
    return;
  }

  if ( !mCloudConnection->isReachable() )
  {
    mCloudConnection->queueProjectPush( projectId );
    emit warning( tr( "Network is not currently active. "
                      "We will push the changes automatically once you are back online." ) );
    return;
  }

  if ( project->status() != QFieldCloudProject::ProjectStatus::Idle )
  {
    return;
  }

  project->push( shouldDownloadUpdates );
}


void QFieldCloudProjectsModel::refreshProjectDeltaList( const QString &projectId )
{
  const QModelIndex projectIndex = findProjectIndex( projectId );
  if ( !projectIndex.isValid() )
  {
    return;
  }

  QFieldCloudProject *project = mProjects[projectIndex.row()];
  project->refreshDeltaList();
}

void QFieldCloudProjectsModel::connectionStatusChanged()
{
  refreshProjectsList( false );
}

void QFieldCloudProjectsModel::usernameChanged()
{
  if ( mUsername == mCloudConnection->username() )
  {
    return;
  }

  mUsername = mCloudConnection->username();
  resetProjects();
}

void QFieldCloudProjectsModel::urlChanged()
{
  if ( mUrl == mCloudConnection->url() )
  {
    return;
  }

  mUrl = mCloudConnection->url();
}

void QFieldCloudProjectsModel::resetProjects()
{
  if ( !mProjects.isEmpty() )
  {
    beginResetModel();
    qDeleteAll( mProjects );
    mProjects.clear();
    endResetModel();
  }

  // Load locally stored projects
  loadProjects();
}

void QFieldCloudProjectsModel::projectReceived()
{
  NetworkReply *reply = qobject_cast<NetworkReply *>( sender() );
  QNetworkReply *rawReply = reply->currentRawReply();

  Q_ASSERT( rawReply );

  const QString projectId = rawReply->request().attribute( static_cast<QNetworkRequest::Attribute>( ProjectsRequestAttribute::ProjectId ) ).toString();
  if ( rawReply->error() != QNetworkReply::NoError )
  {
    emit projectAppended( projectId, true, QFieldCloudConnection::errorString( rawReply ) );
    return;
  }

  QByteArray response = rawReply->readAll();
  QJsonDocument doc = QJsonDocument::fromJson( response );
  QVariantHash projectDetails = doc.object().toVariantHash();

  QFieldCloudProject *cloudProject = QFieldCloudProject::fromDetails( projectDetails, mCloudConnection, mGpkgFlusher ); // cppcheck-suppress constVariablePointer
  if ( cloudProject )
  {
    insertProjects( QList<QFieldCloudProject *>() << cloudProject );
    emit projectAppended( cloudProject->id() );
  }
}

void QFieldCloudProjectsModel::projectListReceived()
{
  NetworkReply *reply = qobject_cast<NetworkReply *>( sender() );
  QNetworkReply *rawReply = reply->currentRawReply();

  Q_ASSERT( rawReply );

  if ( rawReply->error() != QNetworkReply::NoError )
  {
    mIsRefreshing = false;
    emit isRefreshingChanged();

    emit warning( QFieldCloudConnection::errorString( rawReply ) );
    return;
  }

  const bool resetModel = rawReply->request().attribute( static_cast<QNetworkRequest::Attribute>( ProjectsRequestAttribute::ResetModel ) ).toBool();
  const bool fetchPublic = rawReply->request().attribute( static_cast<QNetworkRequest::Attribute>( ProjectsRequestAttribute::FetchPublicProjects ) ).toBool();
  const int projectFetchOffset = rawReply->request().attribute( static_cast<QNetworkRequest::Attribute>( ProjectsRequestAttribute::ProjectsFetchOffset ) ).toInt();
  if ( resetModel && projectFetchOffset == 0 )
  {
    beginResetModel();
    qDeleteAll( mProjects );
    mProjects.clear();
    endResetModel();

    mCurrentProject.clear();
    emit currentProjectChanged();
  }

  QByteArray response = rawReply->readAll();
  QJsonDocument doc = QJsonDocument::fromJson( response );
  QJsonArray projects = doc.array();

  loadProjects( projects, projectFetchOffset > 0 );

  if ( rawReply->hasRawHeader( QStringLiteral( "X-Next-Page" ) ) )
  {
    refreshProjectsList( resetModel, fetchPublic, projectFetchOffset + mProjectsPerFetch );
  }
  else
  {
    // All projects fetched, refresh current project details if found
    if ( !mCurrentProjectId.isEmpty() )
    {
      mCurrentProject = findProject( mCurrentProjectId );
      emit currentProjectChanged();
    }

    mIsRefreshing = false;
    emit isRefreshingChanged();
  }
}

QHash<int, QByteArray> QFieldCloudProjectsModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[IdRole] = "Id";
  roles[PublicRole] = "Public";
  roles[FeaturedRole] = "Featured";
  roles[OwnerRole] = "Owner";
  roles[NameRole] = "Name";
  roles[DescriptionRole] = "Description";
  roles[ModificationRole] = "Modification";
  roles[CheckoutRole] = "Checkout";
  roles[StatusRole] = "Status";
  roles[ProjectOutdatedRole] = "ProjectOutdated";
  roles[ProjectFileOutdatedRole] = "ProjectFileOutdated";
  roles[ErrorStatusRole] = "ErrorStatus";
  roles[ErrorStringRole] = "ErrorString";
  roles[DownloadSizeRole] = "DownloadSize";
  roles[DownloadProgressRole] = "DownloadProgress";
  roles[PackagingStatusRole] = "PackagingStatus";
  roles[PackagedLayerErrorsRole] = "PackagedLayerErrors";
  roles[PushDeltaProgressRole] = "PushDeltaProgress";
  roles[PushDeltaStatusRole] = "PushDeltaStatus";
  roles[PushDeltaStatusStringRole] = "PushDeltaStatusString";
  roles[LocalDeltasCountRole] = "LocalDeltasCount";
  roles[LocalPathRole] = "LocalPath";
  roles[LastLocalExportedAtRole] = "LastLocalExportedAt";
  roles[LastLocalPushDeltasRole] = "LastLocalPushDeltas";
  roles[UserRoleRole] = "UserRole";
  roles[UserRoleOriginRole] = "UserRoleOrigin";
  roles[DeltaListRole] = "DeltaList";

  return roles;
}

void QFieldCloudProjectsModel::insertProjects( const QList<QFieldCloudProject *> &projects )
{
  int currentCount = static_cast<int>( mProjects.size() );
  int newProjectsCount = 0;
  for ( QFieldCloudProject *project : projects )
  {
    setupProjectConnections( project );

    bool found = false;
    for ( int i = 0; i < mProjects.count(); ++i )
    {
      if ( mProjects[i]->id() == project->id() )
      {
        if ( mProjects[i]->checkout() == QFieldCloudProject::LocalCheckout && project->checkout() != QFieldCloudProject::LocalCheckout )
        {
          mProjects[i]->setCheckout( QFieldCloudProject::LocalAndRemoteCheckout );
          mProjects[i]->setIsPublic( project->isPublic() );
          mProjects[i]->setIsFeatured( project->isFeatured() );
          mProjects[i]->setOwner( project->owner() );
          mProjects[i]->setName( project->name() );
          mProjects[i]->setDescription( project->description() );
          mProjects[i]->setUserRole( project->userRole() );
          mProjects[i]->setUserRoleOrigin( project->userRoleOrigin() );
          mProjects[i]->setCreatedAt( project->createdAt() );
          mProjects[i]->setUpdatedAt( project->updatedAt() );
          mProjects[i]->setCanRepackage( project->canRepackage() );
          mProjects[i]->setNeedsRepackaging( project->needsRepackaging() );
          mProjects[i]->setSharedDatasetsProjectId( project->sharedDatasetsProjectId() );
          mProjects[i]->setIsSharedDatasetsProject( project->isSharedDatasetsProject() );
          mProjects[i]->setDataLastUpdatedAt( project->dataLastUpdatedAt() );
          mProjects[i]->setRestrictedDataLastUpdatedAt( project->restrictedDataLastUpdatedAt() );
          emit dataChanged( index( i, 0 ), index( i, 0 ) );

          delete project;
        }
        found = true;
        break;
      }
    }
    if ( !found )
    {
      newProjectsCount++;
      mProjects.append( project );
    }
  }

  beginInsertRows( QModelIndex(), currentCount, currentCount + newProjectsCount - 1 );
  endInsertRows();
}

void QFieldCloudProjectsModel::setupProjectConnections( QFieldCloudProject *project )
{
  connect( project, &QFieldCloudProject::isProjectOutdatedChanged, this, [this] {
    const QFieldCloudProject *p = static_cast<QFieldCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << ProjectFileOutdatedRole );
  } );

  connect( project, &QFieldCloudProject::downloaded, this, [this]( const QString &name, const QString &error ) {
    const QFieldCloudProject *p = static_cast<QFieldCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit projectDownloaded( p->id(), name, !error.isEmpty(), error );
    emit dataChanged( idx, idx, QVector<int>() << StatusRole << PackagingStatusRole << ErrorStatusRole << ErrorStringRole );
  } );

  connect( project, &QFieldCloudProject::pushFinished, this, [this]( bool isDownloading, const QString &error ) {
    const QFieldCloudProject *p = static_cast<QFieldCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit pushFinished( p->id(), isDownloading, !error.isEmpty(), error );
  } );

  connect( project, &QFieldCloudProject::dataRefreshed, this, [this]( QFieldCloudProject::ProjectRefreshReason reason, const QString &error ) {
    const QFieldCloudProject *p = static_cast<QFieldCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx );
  } );

  connect( project, &QFieldCloudProject::jobFinished, this, [this]( QFieldCloudProject::JobType type, const QString &error ) {
    const QFieldCloudProject *p = static_cast<QFieldCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx );
  } );

  connect( project, &QFieldCloudProject::statusChanged, this, [this] {
    const QFieldCloudProject *p = static_cast<QFieldCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << StatusRole );
  } );

  connect( project, &QFieldCloudProject::errorStatusChanged, this, [this] {
    const QFieldCloudProject *p = static_cast<QFieldCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << ErrorStatusRole );
  } );

  connect( project, &QFieldCloudProject::packagingStatusChanged, this, [this] {
    const QFieldCloudProject *p = static_cast<QFieldCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << PackagingStatusRole );
  } );

  connect( project, &QFieldCloudProject::downloadProgressChanged, this, [this] {
    const QFieldCloudProject *p = static_cast<QFieldCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << DownloadProgressRole );
  } );

  connect( project, &QFieldCloudProject::downloadBytesTotalChanged, this, [this] {
    const QFieldCloudProject *p = static_cast<QFieldCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << DownloadSizeRole );
  } );

  connect( project, &QFieldCloudProject::packagedLayerErrorsChanged, this, [this] {
    const QFieldCloudProject *p = static_cast<QFieldCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << PackagedLayerErrorsRole );
  } );

  connect( project, &QFieldCloudProject::isOutdatedChanged, this, [this] {
    const QFieldCloudProject *p = static_cast<QFieldCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << ProjectOutdatedRole );
  } );

  connect( project, &QFieldCloudProject::localPathChanged, this, [this] {
    const QFieldCloudProject *p = static_cast<QFieldCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << LocalPathRole );
  } );

  connect( project, &QFieldCloudProject::checkoutChanged, this, [this] {
    const QFieldCloudProject *p = static_cast<QFieldCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << CheckoutRole );
  } );

  connect( project, &QFieldCloudProject::lastLocalExportedAtChanged, this, [this] {
    const QFieldCloudProject *p = static_cast<QFieldCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << LastLocalExportedAtRole );
  } );

  connect( project, &QFieldCloudProject::modificationChanged, this, [this] {
    const QFieldCloudProject *p = static_cast<QFieldCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << ModificationRole );
  } );

  connect( project, &QFieldCloudProject::deltaFilePushStatusChanged, this, [this] {
    const QFieldCloudProject *p = static_cast<QFieldCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << PushDeltaStatusRole );
  } );

  connect( project, &QFieldCloudProject::deltaFilePushStatusStringChanged, this, [this] {
    const QFieldCloudProject *p = static_cast<QFieldCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << PushDeltaStatusStringRole );
  } );

  connect( project, &QFieldCloudProject::pushDeltaProgressChanged, this, [this] {
    const QFieldCloudProject *p = static_cast<QFieldCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << PushDeltaProgressRole );
  } );

  connect( project, &QFieldCloudProject::lastLocalPushDeltasChanged, this, [this] {
    const QFieldCloudProject *p = static_cast<QFieldCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << LastLocalPushDeltasRole );
  } );

  connect( project, &QFieldCloudProject::deltasCountChanged, this, [this] {
    const QFieldCloudProject *p = static_cast<QFieldCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << LocalDeltasCountRole );
  } );

  connect( project, &QFieldCloudProject::deltaListModelChanged, this, [this] {
    const QFieldCloudProject *p = static_cast<QFieldCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << DeltaListRole );
    emit deltaListModelChanged();
  } );
}

void QFieldCloudProjectsModel::loadProjects( const QJsonArray &remoteProjects, bool skipLocalProjects )
{
  QList<QFieldCloudProject *> freshCloudProjects;
  for ( const auto project : remoteProjects )
  {
    QVariantHash projectDetails = project.toObject().toVariantHash();
    QFieldCloudProject *cloudProject = QFieldCloudProject::fromDetails( projectDetails, mCloudConnection, mGpkgFlusher );

    if ( cloudProject->isSharedDatasetsProject() )
    {
      delete cloudProject;
    }
    else
    {
      freshCloudProjects.push_back( cloudProject );
    }
  }

  insertProjects( freshCloudProjects );

  if ( !skipLocalProjects )
  {
    QList<QFieldCloudProject *> userSpecificProjects;
    QDirIterator userDirs( QFieldCloudUtils::localCloudDirectory(), QDir::Dirs | QDir::NoDotAndDotDot );
    while ( userDirs.hasNext() )
    {
      userDirs.next();
      const QString username = userDirs.fileName();

      // We skip cloud projects that are not linked to the last successul logged in account
      if ( username != mUsername )
        continue;

      QDirIterator projectDirs( QStringLiteral( "%1/%2" ).arg( QFieldCloudUtils::localCloudDirectory(), username ), QDir::Dirs | QDir::NoDotAndDotDot );
      while ( projectDirs.hasNext() )
      {
        projectDirs.next();

        const QString projectId = projectDirs.fileName();
        if ( findProject( projectId ) )
        {
          // Already covered when receiving cloud projects from the server
          continue;
        }

        QFieldCloudProject *cloudProject = QFieldCloudProject::fromLocalSettings( projectId, mCloudConnection, mGpkgFlusher );
        if ( !cloudProject )
        {
          // Not a previously loaded cloud project
          continue;
        }

        // If the cloud project is a special shared dataset project or if the cloud project
        // had a folder but was not downloaded properly, do not add to the model
        if ( cloudProject->isSharedDatasetsProject() || cloudProject->localPath().isEmpty() )
        {
          delete cloudProject;
        }
        else
        {
          userSpecificProjects.push_back( cloudProject );
        }

        Q_ASSERT( projectId == cloudProject->id() );
      }
    }

    insertProjects( userSpecificProjects );
  }
}

int QFieldCloudProjectsModel::rowCount( const QModelIndex &parent ) const
{
  return !parent.isValid() ? static_cast<int>( mProjects.size() ) : 0;
}

QVariant QFieldCloudProjectsModel::data( const QModelIndex &index, int role ) const
{
  if ( index.row() >= mProjects.size() || index.row() < 0 )
  {
    return QVariant();
  }

  const QFieldCloudProject *project = mProjects.at( index.row() );
  if ( !project )
  {
    return QVariant();
  }

  if ( role == Qt::DisplayRole )
  {
    return QStringLiteral( "%1/%2" ).arg( project->owner(), project->name() );
  }

  switch ( static_cast<ColumnRole>( role ) )
  {
    case IdRole:
      return project->id();

    case PublicRole:
      return project->isPublic();

    case FeaturedRole:
      return project->isFeatured();

    case OwnerRole:
      return project->owner();

    case NameRole:
      return project->name();

    case DescriptionRole:
      return project->description();

    case ModificationRole:
      return static_cast<int>( project->modification() );

    case CheckoutRole:
      return static_cast<int>( project->checkout() );

    case StatusRole:
      return static_cast<int>( project->status() );

    case ProjectOutdatedRole:
      return project->isOutdated();

    case ProjectFileOutdatedRole:
      return project->isProjectOutdated();

    case ErrorStatusRole:
      return static_cast<int>( project->errorStatus() );

    case ErrorStringRole:
      if ( project->errorStatus() == QFieldCloudProject::DownloadErrorStatus )
      {
        return project->packagingStatusString();
      }
      else if ( project->errorStatus() == QFieldCloudProject::PushErrorStatus )
      {
        return project->deltaFilePushStatusString();
      }
      return QString();

    case PackagingStatusRole:
      return project->packagingStatus();

    case PackagedLayerErrorsRole:
      return QVariant( project->packagedLayerErrors() );

    case DownloadSizeRole:
      return project->downloadBytesTotal();

    case DownloadProgressRole:
      return project->downloadProgress();

    case PushDeltaProgressRole:
      return project->pushDeltaProgress();

    case PushDeltaStatusRole:
      return project->deltaFilePushStatus();

    case PushDeltaStatusStringRole:
      return project->deltaFilePushStatusString();

    case LocalDeltasCountRole:
      return project->deltasCount();

    case LocalPathRole:
      return project->localPath();

    case LastLocalExportedAtRole:
      return project->lastLocalExportedAt();

    case LastLocalPushDeltasRole:
      return project->lastLocalPushDeltas();

    case UserRoleRole:
      return project->userRole();

    case UserRoleOriginRole:
      return project->userRoleOrigin();

    case DeltaListRole:
      return QVariant::fromValue<DeltaListModel *>( project->deltaListModel() );
  }

  return QVariant();
}

bool QFieldCloudProjectsModel::revertLocalChangesFromCurrentProject()
{
  const QFieldCloudProject *project = findProject( mCurrentProjectId );
  if ( !project )
  {
    return false;
  }

  DeltaFileWrapper *deltaFileWrapper = mLayerObserver->deltaFileWrapper();

  if ( !deltaFileWrapper->toFile() )
  {
    return false;
  }

  if ( !deltaFileWrapper->applyReversed( QgsProject::instance() ) )
  {
    QgsMessageLog::logMessage( QStringLiteral( "Failed to apply reversed" ) );
    return false;
  }

  deltaFileWrapper->reset();
  deltaFileWrapper->resetId();

  if ( !deltaFileWrapper->toFile() )
  {
    return false;
  }

  return true;
}

bool QFieldCloudProjectsModel::discardLocalChangesFromCurrentProject()
{
  const QFieldCloudProject *project = findProject( mCurrentProjectId );
  if ( !project )
  {
    return false;
  }

  DeltaFileWrapper *deltaFileWrapper = mLayerObserver->deltaFileWrapper();

  if ( !deltaFileWrapper->toFile() )
  {
    QgsMessageLog::logMessage( QStringLiteral( "Failed to write deltas." ) );
  }

  deltaFileWrapper->reset();
  deltaFileWrapper->resetId();

  if ( !deltaFileWrapper->toFile() )
  {
    return false;
  }

  return true;
}

void QFieldCloudProjectsModel::setGpkgFlusher( QgsGpkgFlusher *flusher )
{
  if ( mGpkgFlusher == flusher )
  {
    return;
  }

  mGpkgFlusher = flusher;

  emit gpkgFlusherChanged();
}

void QFieldCloudProjectsModel::updateLocalizedDataPaths( const QString &projectPath )
{
  const QString projectId = QFieldCloudUtils::getProjectId( projectPath );
  QString localizedDataPath;
  if ( !projectId.isEmpty() )
  {
    const QFieldCloudProject *project = findProject( projectId );
    if ( project && !project->sharedDatasetsProjectId().isEmpty() )
    {
      localizedDataPath = QStringLiteral( "%1/%2/%3" ).arg( QFieldCloudUtils::localCloudDirectory(), mUsername, project->sharedDatasetsProjectId() );
    }
  }

  QStringList localizedDataPaths = QgsApplication::instance()->localizedDataPathRegistry()->paths();
  localizedDataPaths.erase( std::remove_if( localizedDataPaths.begin(),
                                            localizedDataPaths.end(),
                                            [&localizedDataPath]( const QString &path ) { return path.startsWith( QFieldCloudUtils::localCloudDirectory() ); } ),
                            localizedDataPaths.end() );
  localizedDataPaths << localizedDataPath;
  QgsApplication::instance()->localizedDataPathRegistry()->setPaths( localizedDataPaths );
}

void QFieldCloudProjectsModel::createProject( QString name )
{
  if ( name.isEmpty() )
  {
    emit projectCreated( QString(), true, tr( "Project creation requires a name" ) );
    return;
  }

  mIsCreating = true;
  emit isCreatingChanged();

  name.replace( QRegularExpression( "[^A-Za-z0-9_]" ), QStringLiteral( "_" ) );

  QString url = QStringLiteral( "/api/v1/projects/?owner=%1" ).arg( mUsername );
  QNetworkRequest request( url );
  request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );
  request.setAttribute( QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::RedirectPolicy::NoLessSafeRedirectPolicy );
  mCloudConnection->setAuthenticationDetails( request );

  const NetworkReply *listingreply = mCloudConnection->get( request, url );
  connect( listingreply, &NetworkReply::finished, this, [this, name]() {
    NetworkReply *reply = qobject_cast<NetworkReply *>( sender() );
    QNetworkReply *rawReply = reply->currentRawReply();
    Q_ASSERT( rawReply );

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      emit projectCreated( QString(), true, mCloudConnection->errorString( rawReply ) );
      return;
    }

    QByteArray response = rawReply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson( response );
    QJsonArray projects = doc.array();
    QStringList projectNames;
    for ( const auto project : projects )
    {
      QVariantHash projectDetails = project.toObject().toVariantHash();
      projectNames << projectDetails.value( QStringLiteral( "name" ) ).toString().toLower();
    }

    int uniqueSuffix = 1;
    QString finalizedName = name;
    while ( projectNames.contains( finalizedName.toLower() ) )
    {
      finalizedName = QStringLiteral( "%1_%2" ).arg( name, QString::number( uniqueSuffix++ ) );
    }

    QString url = QStringLiteral( "/api/v1/projects/" );

    QVariantMap params;
    params.insert( QStringLiteral( "name" ), finalizedName );
    params.insert( QStringLiteral( "owner" ), mUsername );
    params.insert( QStringLiteral( "description" ), QString() );
    params.insert( QStringLiteral( "private" ), true );

    QNetworkRequest request;
    const NetworkReply *creationReply = mCloudConnection->post( request, url, params );
    connect( creationReply, &NetworkReply::finished, this, &QFieldCloudProjectsModel::projectCreationReceived );
  } );
}

void QFieldCloudProjectsModel::projectCreationReceived()
{
  NetworkReply *reply = qobject_cast<NetworkReply *>( sender() );
  QNetworkReply *rawReply = reply->currentRawReply();
  Q_ASSERT( rawReply );

  if ( rawReply->error() != QNetworkReply::NoError )
  {
    emit projectCreated( QString(), true, mCloudConnection->errorString( rawReply ) );

    mIsCreating = false;
    emit isCreatingChanged();
    return;
  }

  QByteArray response = rawReply->readAll();
  QJsonDocument doc = QJsonDocument::fromJson( response );
  QVariantHash projectDetails = doc.object().toVariantHash();
  QFieldCloudProject *cloudProject = QFieldCloudProject::fromDetails( projectDetails, mCloudConnection, mGpkgFlusher ); // cppcheck-suppress constVariablePointer
  if ( cloudProject )
  {
    insertProjects( QList<QFieldCloudProject *>() << cloudProject );
    emit projectCreated( cloudProject->id() );
  }
  else
  {
    emit projectCreated( QString(), true, tr( "Cloud project could not be created." ) );
  }

  mIsCreating = false;
  emit isCreatingChanged();
}

// --

QFieldCloudProjectsFilterModel::QFieldCloudProjectsFilterModel( QObject *parent )
  : QSortFilterProxyModel( parent )
{
  setDynamicSortFilter( true );
  setSortLocaleAware( true );
  sort( 0 );
}

void QFieldCloudProjectsFilterModel::setProjectsModel( QFieldCloudProjectsModel *projectsModel )
{
  if ( mSourceModel == projectsModel )
  {
    return;
  }

  mSourceModel = projectsModel;
  setSourceModel( mSourceModel );

  emit projectsModelChanged();
}

QFieldCloudProjectsModel *QFieldCloudProjectsFilterModel::projectsModel() const
{
  return mSourceModel;
}

void QFieldCloudProjectsFilterModel::setFilter( ProjectsFilter filter )
{
  if ( mFilter == filter )
  {
    return;
  }

  mFilter = filter;
  invalidateFilter();

  emit filterChanged();
}

QFieldCloudProjectsFilterModel::ProjectsFilter QFieldCloudProjectsFilterModel::filter() const
{
  return mFilter;
}

void QFieldCloudProjectsFilterModel::setShowLocalOnly( bool showLocalOnly )
{
  if ( mShowLocalOnly == showLocalOnly )
  {
    return;
  }

  mShowLocalOnly = showLocalOnly;
  invalidateFilter();

  emit showLocalOnlyChanged();
}

bool QFieldCloudProjectsFilterModel::showLocalOnly() const
{
  return mShowLocalOnly;
}

bool QFieldCloudProjectsFilterModel::lessThan( const QModelIndex &sourceLeft, const QModelIndex &sourceRight ) const
{
  if ( !mSourceModel )
  {
    return true;
  }

  if ( mShowFeaturedOnTop )
  {
    const bool isFeaturedLeft = mSourceModel->data( sourceLeft, QFieldCloudProjectsModel::FeaturedRole ).toBool();
    const bool isFeaturedRight = mSourceModel->data( sourceRight, QFieldCloudProjectsModel::FeaturedRole ).toBool();
    if ( isFeaturedLeft != isFeaturedRight )
    {
      return isFeaturedLeft;
    }
  }

  QString left = mSourceModel->data( sourceLeft, QFieldCloudProjectsModel::OwnerRole ).toString();
  QString right = mSourceModel->data( sourceRight, QFieldCloudProjectsModel::OwnerRole ).toString();
  int compare = QString::localeAwareCompare( left, right );
  if ( compare != 0 )
  {
    return compare < 0;
  }

  left = mSourceModel->data( sourceLeft, QFieldCloudProjectsModel::NameRole ).toString();
  right = mSourceModel->data( sourceRight, QFieldCloudProjectsModel::NameRole ).toString();
  compare = QString::localeAwareCompare( left, right );
  return compare < 0;
}

bool QFieldCloudProjectsFilterModel::filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const
{
  const QModelIndex currentRowIndex = mSourceModel->index( source_row, 0, source_parent );
  if ( mShowLocalOnly && mSourceModel->data( currentRowIndex, QFieldCloudProjectsModel::LocalPathRole ).toString().isEmpty() )
  {
    return false;
  }

  bool matchesProjectType = false;
  switch ( mFilter )
  {
    case PrivateProjects:
      // the list will include public "community" projects that are present locally so they can appear in the "My projects" list
      matchesProjectType = mSourceModel->data( currentRowIndex, QFieldCloudProjectsModel::UserRoleOriginRole ).toString() != QStringLiteral( "public" )
                           || !mSourceModel->data( currentRowIndex, QFieldCloudProjectsModel::LocalPathRole ).toString().isEmpty();
      break;
    case PublicProjects:
      matchesProjectType = mSourceModel->data( currentRowIndex, QFieldCloudProjectsModel::UserRoleOriginRole ).toString() == QStringLiteral( "public" );
      break;
  }

  const QString name = mSourceModel->data( currentRowIndex, QFieldCloudProjectsModel::NameRole ).toString();
  const QString description = mSourceModel->data( currentRowIndex, QFieldCloudProjectsModel::DescriptionRole ).toString();
  const QString owner = mSourceModel->data( currentRowIndex, QFieldCloudProjectsModel::OwnerRole ).toString();
  const int status = mSourceModel->data( currentRowIndex, QFieldCloudProjectsModel::StatusRole ).toInt();

  const bool matchesTextFilter = mTextFilter.isEmpty() || name.contains( mTextFilter, Qt::CaseInsensitive ) || description.contains( mTextFilter, Qt::CaseInsensitive ) || owner.contains( mTextFilter, Qt::CaseInsensitive );

  const bool validProjectsFilter = mShowInValidProjects || status != static_cast<int>( QFieldCloudProject::ProjectStatus::Failing );

  return matchesProjectType && matchesTextFilter && validProjectsFilter;
}

void QFieldCloudProjectsFilterModel::setTextFilter( const QString &text )
{
  if ( mTextFilter == text )
  {
    return;
  }
  mTextFilter = text;
  invalidateFilter();
}

QString QFieldCloudProjectsFilterModel::textFilter() const
{
  return mTextFilter;
}

void QFieldCloudProjectsFilterModel::setShowInValidProjects( bool showInValidProjects )
{
  if ( mShowInValidProjects == showInValidProjects )
  {
    return;
  }

  mShowInValidProjects = showInValidProjects;
  invalidateFilter();
}


bool QFieldCloudProjectsFilterModel::showInValidProjects() const
{
  return mShowInValidProjects;
}

void QFieldCloudProjectsFilterModel::setShowFeaturedOnTop( bool showFeaturedOnTop )
{
  if ( mShowFeaturedOnTop == showFeaturedOnTop )
  {
    return;
  }

  mShowFeaturedOnTop = showFeaturedOnTop;
  emit showFeaturedOnTopChanged();

  sort( 0 );
}

bool QFieldCloudProjectsFilterModel::showFeaturedOnTop() const
{
  return mShowFeaturedOnTop;
}
```


