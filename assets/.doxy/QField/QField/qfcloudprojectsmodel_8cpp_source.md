

# File qfcloudprojectsmodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldcloud**](dir_d27ac98b23548f5810403c734ce9817b.md) **>** [**qfcloudprojectsmodel.cpp**](qfcloudprojectsmodel_8cpp.md)

[Go to the documentation of this file](qfcloudprojectsmodel_8cpp.md)


```C++
/***************************************************************************
    qfcloudprojectsmodel.cpp
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

#include "qfcloudconnection.h"
#include "qfcloudprojectsmodel.h"
#include "qfcloudutils.h"
#include "qfdeltafilewrapper.h"
#include "qflayerobserver.h"

#include <QDir>
#include <QDirIterator>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QRegularExpression>
#include <QSettings>
#include <QTemporaryFile>
#include <qgis.h>
#include <qgsapplication.h>
#include <qgslocalizeddatapathregistry.h>
#include <qgsmessagelog.h>
#include <qgsnetworkaccessmanager.h>
#include <qgsproject.h>
#include <qgsproviderregistry.h>

#include <algorithm>


QfCloudProjectsModel::QfCloudProjectsModel()
{
  // TODO all of these connects are a bit too much, and I guess not very precise, should be refactored!
  connect( this, &QfCloudProjectsModel::dataChanged, this, [this]( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles ) {
    Q_UNUSED( bottomRight )
    Q_UNUSED( roles )

    if ( roles.isEmpty() || roles.contains( StatusRole ) )
    {
      emit busyProjectIdsChanged();
    }
  } );
}

QfCloudConnection *QfCloudProjectsModel::cloudConnection() const
{
  return mCloudConnection;
}

void QfCloudProjectsModel::setCloudConnection( QfCloudConnection *cloudConnection )
{
  if ( mCloudConnection == cloudConnection )
  {
    return;
  }

  if ( mCloudConnection )
  {
    disconnect( mCloudConnection, &QfCloudConnection::statusChanged, this, &QfCloudProjectsModel::connectionStatusChanged );
    disconnect( mCloudConnection, &QfCloudConnection::usernameChanged, this, &QfCloudProjectsModel::usernameChanged );
    disconnect( mCloudConnection, &QfCloudConnection::urlChanged, this, &QfCloudProjectsModel::urlChanged );
    disconnect( mCloudConnection, &QfCloudConnection::queuedProjectPushRequested, this, nullptr );
  }

  mCloudConnection = cloudConnection;

  if ( mCloudConnection )
  {
    connect( mCloudConnection, &QfCloudConnection::statusChanged, this, &QfCloudProjectsModel::connectionStatusChanged );
    connect( mCloudConnection, &QfCloudConnection::usernameChanged, this, &QfCloudProjectsModel::usernameChanged );
    connect( mCloudConnection, &QfCloudConnection::urlChanged, this, &QfCloudProjectsModel::urlChanged );
    connect( mCloudConnection, &QfCloudConnection::queuedProjectPushRequested, this,
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

QfLayerObserver *QfCloudProjectsModel::layerObserver() const
{
  return mLayerObserver;
}

void QfCloudProjectsModel::setLayerObserver( QfLayerObserver *layerObserver )
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

QString QfCloudProjectsModel::currentProjectId() const
{
  return mCurrentProjectId;
}

void QfCloudProjectsModel::setCurrentProjectId( const QString &currentProjectId )
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

QfCloudProject *QfCloudProjectsModel::currentProject() const
{
  return mCurrentProject.data();
}

QSet<QString> QfCloudProjectsModel::busyProjectIds() const
{
  QSet<QString> result;

  for ( const auto project : mProjects )
  {
    if ( project->status() != QfCloudProject::ProjectStatus::Idle && project->status() != QfCloudProject::ProjectStatus::Failing )
    {
      result.insert( project->id() );
    }
  }

  return result;
}

void QfCloudProjectsModel::refreshProjectsList( bool shouldResetModel, int projectFetchOffset )
{
  switch ( mCloudConnection->status() )
  {
    case QfCloudConnection::ConnectionStatus::LoggedIn:
    {
      const QString url = QStringLiteral( "/api/v1/projects/" );
      QVariantMap params;
      params["limit"] = QString::number( mProjectsPerFetch );
      params["offset"] = QString::number( projectFetchOffset );

      QNetworkRequest request( url );
      request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );
      request.setAttribute( QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::RedirectPolicy::NoLessSafeRedirectPolicy );

      request.setAttribute( static_cast<QNetworkRequest::Attribute>( ProjectsRequestAttribute::ResetModel ), shouldResetModel );
      request.setAttribute( static_cast<QNetworkRequest::Attribute>( ProjectsRequestAttribute::ProjectsFetchOffset ), projectFetchOffset );

      mIsRefreshing = true;
      emit isRefreshingChanged();

      mCloudConnection->setAuthenticationDetails( request );
      const QfNetworkReply *reply = mCloudConnection->get( request, url, params );
      connect( reply, &QfNetworkReply::finished, this, &QfCloudProjectsModel::projectListReceived );
      break;
    }
    case QfCloudConnection::ConnectionStatus::Disconnected:
    {
      // Nothing done at the moment; since the connection can be disconnected after discovering
      // a token has been invalidated, be careful with what is added here.
      break;
    }
    case QfCloudConnection::ConnectionStatus::Connecting:
      // Nothing done for this intermediary status.
      break;
  }
}

QModelIndex QfCloudProjectsModel::findProjectIndex( const QString &projectId ) const
{
  if ( projectId.isEmpty() )
  {
    QgsLogger::debug( QStringLiteral( "No project found for an empty project id." ) );
    return QModelIndex();
  }

  QString projectOwner;
  QString projectName;
  const int separator = projectId.indexOf( '/' );
  if ( separator > 0 )
  {
    projectOwner = projectId.mid( 0, separator ).trimmed();
    projectName = projectId.mid( separator + 1 ).trimmed();
  }
  bool matchOwnerAndName = !projectOwner.isEmpty() && !projectName.isEmpty();

  for ( int i = 0; i < mProjects.count(); i++ )
  {
    if ( ( !matchOwnerAndName && mProjects.at( i )->id() == projectId ) || ( matchOwnerAndName && mProjects.at( i )->owner() == projectOwner && mProjects.at( i )->name() == projectName ) )
    {
      return createIndex( i, 0 );
    }
  }

  QgsLogger::debug( QStringLiteral( "No project found with the provided id: `%1`" ).arg( projectId ) );
  return QModelIndex();
}

QfCloudProject *QfCloudProjectsModel::findProject( const QString &projectId ) const
{
  const QModelIndex index = findProjectIndex( projectId );

  if ( index.isValid() )
  {
    return mProjects[index.row()];
  }

  return nullptr;
}

void QfCloudProjectsModel::appendProject( const QString &projectId, bool forceRefresh )
{
  if ( !mCloudConnection )
  {
    return;
  }

  if ( !forceRefresh )
  {
    const QfCloudProject *project = findProject( projectId );
    if ( project && ( project->checkout() & QfCloudProject::RemoteCheckout ) )
    {
      emit projectAppended( projectId );
      return;
    }
  }

  QString projectOwner;
  QString projectName;
  const int separator = projectId.indexOf( '/' );
  if ( separator > 0 )
  {
    projectOwner = projectId.mid( 0, separator ).trimmed();
    projectName = projectId.mid( separator + 1 ).trimmed();
  }

  QString url;
  QVariantMap params;
  if ( !projectOwner.isEmpty() && !projectName.isEmpty() )
  {
    params["owner"] = projectOwner;
    params["name"] = projectName;
    params["include_public"] = 1;
    url = QStringLiteral( "/api/v1/projects/" );
  }
  else
  {
    url = QStringLiteral( "/api/v1/projects/%1/" ).arg( projectId );
  }

  QNetworkRequest request( url );
  request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );
  request.setAttribute( QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::RedirectPolicy::NoLessSafeRedirectPolicy );
  request.setAttribute( static_cast<QNetworkRequest::Attribute>( QfCloudProjectsModel::ProjectsRequestAttribute::ProjectId ), projectId );
  mCloudConnection->setAuthenticationDetails( request );

  const QfNetworkReply *reply = mCloudConnection->get( request, url, params );
  connect( reply, &QfNetworkReply::finished, this, &QfCloudProjectsModel::projectReceived );
}

void QfCloudProjectsModel::appendProjects( const QString &owner, const QString &search, int projectFetchOffset )
{
  if ( !mCloudConnection )
  {
    return;
  }

  const QString trimmedOwner = owner.trimmed();
  const QString trimmedSearch = search.trimmed();
  if ( trimmedOwner.isEmpty() && trimmedSearch.isEmpty() )
  {
    emit projectsAppended( owner, search );
    return;
  }

  const QString url = QStringLiteral( "/api/v1/projects/" );

  QVariantMap params;
  params["owner"] = owner;
  params["search"] = search;
  params["include_public"] = 1;
  params["limit"] = QString::number( mProjectsPerFetch );
  params["offset"] = QString::number( projectFetchOffset );

  QNetworkRequest request( url );
  request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );
  request.setAttribute( QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::RedirectPolicy::NoLessSafeRedirectPolicy );
  request.setAttribute( static_cast<QNetworkRequest::Attribute>( ProjectsRequestAttribute::ProjectOwnerName ), owner );
  request.setAttribute( static_cast<QNetworkRequest::Attribute>( ProjectsRequestAttribute::ProjectSearchTerm ), search );
  request.setAttribute( static_cast<QNetworkRequest::Attribute>( ProjectsRequestAttribute::ProjectsFetchOffset ), projectFetchOffset );
  mCloudConnection->setAuthenticationDetails( request );

  const QfNetworkReply *reply = mCloudConnection->get( request, url, params );
  connect( reply, &QfNetworkReply::finished, this, &QfCloudProjectsModel::projectListReceived );
}

QStringList QfCloudProjectsModel::uniqueOwners() const
{
  QStringList owners;
  for ( const QfCloudProject *project : std::as_const( mProjects ) )
  {
    if ( project->userRoleOrigin() == QStringLiteral( "public" ) )
    {
      continue;
    }
    const QString owner = project->owner();
    if ( !owners.contains( owner ) )
    {
      owners.append( owner );
    }
  }
  std::sort( owners.begin(), owners.end() );
  return owners;
}

void QfCloudProjectsModel::removeLocalProject( const QString &projectId )
{
  QDir dir( QStringLiteral( "%1/%2/%3" ).arg( QfCloudUtils::localCloudDirectory(), mUsername, projectId ) );

  if ( dir.exists() )
  {
    const QModelIndex projectIndex = findProjectIndex( projectId );
    if ( projectIndex.isValid() )
    {
      QfCloudProject *project = mProjects[projectIndex.row()];
      project->removeLocally();

      if ( ( project->status() == QfCloudProject::ProjectStatus::Idle || project->status() == QfCloudProject::ProjectStatus::Failing ) && project->checkout() & QfCloudProject::RemoteCheckout )
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

QString QfCloudProjectsModel::layerFileName( const QgsMapLayer *layer ) const
{
  return layer->dataProvider()->dataSourceUri().split( '|' )[0];
}

void QfCloudProjectsModel::projectCancelDownload( const QString &projectId )
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

  QfCloudProject *project = mProjects[projectIndex.row()];

  // before canceling, the project should be downloading
  if ( project->status() != QfCloudProject::ProjectStatus::Downloading )
  {
    return;
  }

  project->cancelDownload();

  emit dataChanged( projectIndex, projectIndex, QVector<int>() << StatusRole << ErrorStatusRole << PackagingStatusRole );
}

void QfCloudProjectsModel::projectPackageAndDownload( const QString &projectId )
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

  QfCloudProject *project = mProjects[projectIndex.row()];

  if ( project->status() != QfCloudProject::ProjectStatus::Idle )
  {
    QgsLogger::debug( QStringLiteral( "Project %1: package and download cancelled, the project is currently busy." ).arg( projectId ) );
    emit warning( tr( "Project busy." ) );
    return;
  }

  project->packageAndDownload();

  emit dataChanged( projectIndex, projectIndex );
}

void QfCloudProjectsModel::projectPush( const QString &projectId, const bool shouldDownloadUpdates )
{
  const QModelIndex projectIndex = findProjectIndex( projectId );

  if ( !projectIndex.isValid() )
  {
    return;
  }

  QfCloudProject *project = mProjects[projectIndex.row()];
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

  if ( project->status() != QfCloudProject::ProjectStatus::Idle )
  {
    return;
  }

  project->push( shouldDownloadUpdates );
}

void QfCloudProjectsModel::connectionStatusChanged()
{
  refreshProjectsList( false );
}

void QfCloudProjectsModel::usernameChanged()
{
  if ( mUsername == mCloudConnection->username() )
  {
    return;
  }

  mUsername = mCloudConnection->username();
  resetProjects();
}

void QfCloudProjectsModel::urlChanged()
{
  if ( mUrl == mCloudConnection->url() )
  {
    return;
  }

  mUrl = mCloudConnection->url();
}

void QfCloudProjectsModel::resetProjects()
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

void QfCloudProjectsModel::projectReceived()
{
  QfNetworkReply *reply = qobject_cast<QfNetworkReply *>( sender() );
  QNetworkReply *rawReply = reply->currentRawReply();

  Q_ASSERT( rawReply );

  const QString projectId = rawReply->request().attribute( static_cast<QNetworkRequest::Attribute>( ProjectsRequestAttribute::ProjectId ) ).toString();
  if ( rawReply->error() != QNetworkReply::NoError )
  {
    emit projectAppended( projectId, true, QfCloudConnection::errorString( rawReply ) );
    return;
  }

  QByteArray response = rawReply->readAll();
  QJsonDocument doc = QJsonDocument::fromJson( response );
  QVariantHash projectDetails;
  if ( doc.isArray() )
  {
    const QJsonArray projects = doc.array();
    if ( !projects.isEmpty() )
    {
      projectDetails = projects.first().toObject().toVariantHash();
    }
  }
  else
  {
    const QJsonObject project = doc.object();
    if ( !project.isEmpty() )
    {
      projectDetails = project.toVariantHash();
    }
  }

  if ( !projectDetails.isEmpty() )
  {
    QfCloudProject *cloudProject = QfCloudProject::fromDetails( projectDetails, mCloudConnection, mGpkgFlusher ); // cppcheck-suppress constVariablePointer
    if ( cloudProject )
    {
      insertProjects( QList<QfCloudProject *>() << cloudProject );
      emit projectAppended( projectId );
      updateHasTemplates();
    }
  }
}

void QfCloudProjectsModel::projectListReceived()
{
  QfNetworkReply *reply = qobject_cast<QfNetworkReply *>( sender() );
  QNetworkReply *rawReply = reply->currentRawReply();

  Q_ASSERT( rawReply );

  const QString projectOwnerName = rawReply->request().attribute( static_cast<QNetworkRequest::Attribute>( ProjectsRequestAttribute::ProjectOwnerName ) ).toString();
  const QString projectSearchTerm = rawReply->request().attribute( static_cast<QNetworkRequest::Attribute>( ProjectsRequestAttribute::ProjectSearchTerm ) ).toString();
  const bool isAppending = !projectOwnerName.isEmpty() || !projectSearchTerm.isEmpty();

  if ( rawReply->error() != QNetworkReply::NoError )
  {
    if ( isAppending )
    {
      emit projectsAppended( projectOwnerName, projectSearchTerm, true, QfCloudConnection::errorString( rawReply ) );
    }
    else
    {
      mIsRefreshing = false;
      emit isRefreshingChanged();
    }
    updateHasTemplates();

    emit warning( QfCloudConnection::errorString( rawReply ) );
    return;
  }

  const bool resetModel = rawReply->request().attribute( static_cast<QNetworkRequest::Attribute>( ProjectsRequestAttribute::ResetModel ) ).toBool();
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

  const bool skipLocalProjects = isAppending || projectFetchOffset > 0;
  loadProjects( projects, skipLocalProjects );

  if ( rawReply->hasRawHeader( QStringLiteral( "X-Next-Page" ) ) )
  {
    if ( isAppending )
    {
      appendProjects( projectOwnerName, projectSearchTerm, projectFetchOffset + mProjectsPerFetch );
    }
    else
    {
      refreshProjectsList( resetModel, projectFetchOffset + mProjectsPerFetch );
    }
    return;
  }

  if ( isAppending )
  {
    emit projectsAppended( projectOwnerName, projectSearchTerm );
  }
  else
  {
    // All projects fetched, refresh current project details if found
    if ( !mCurrentProjectId.isEmpty() )
    {
      mCurrentProject = findProject( mCurrentProjectId );
      emit currentProjectChanged();

      if ( mLayerObserver )
      {
        mLayerObserver->setDeltaFileWrapper( mCurrentProject ? mCurrentProject->deltaFileWrapper() : nullptr );
      }
    }

    mIsRefreshing = false;
    emit isRefreshingChanged();
  }
  updateHasTemplates();
}

QHash<int, QByteArray> QfCloudProjectsModel::roleNames() const
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
  roles[ProjectTypeRole] = "ProjectType";

  return roles;
}

void QfCloudProjectsModel::insertProjects( const QList<QfCloudProject *> &projects )
{
  int currentCount = static_cast<int>( mProjects.size() );
  QList<QfCloudProject *> newProjects;
  for ( QfCloudProject *project : projects )
  {
    bool found = false;
    for ( int i = 0; i < mProjects.count(); ++i )
    {
      if ( mProjects[i]->id() == project->id() )
      {
        if ( mProjects[i]->checkout() == QfCloudProject::LocalCheckout && project->checkout() != QfCloudProject::LocalCheckout )
        {
          mProjects[i]->setCheckout( QfCloudProject::LocalAndRemoteCheckout );
          mProjects[i]->setIsPublic( project->isPublic() );
          mProjects[i]->setIsFeatured( project->isFeatured() );
          mProjects[i]->setOwner( project->owner() );
          mProjects[i]->setName( project->name() );
          mProjects[i]->setDescription( project->description() );
          mProjects[i]->setUserRole( project->userRole() );
          mProjects[i]->setUserRoleOrigin( project->userRoleOrigin() );
          mProjects[i]->setCreatedAt( project->createdAt() );
          mProjects[i]->setUpdatedAt( project->updatedAt() );
          mProjects[i]->setRemoteSizeBytes( project->remoteSizeBytes() );
          mProjects[i]->setCanRepackage( project->canRepackage() );
          mProjects[i]->setNeedsRepackaging( project->needsRepackaging() );
          mProjects[i]->setSharedDatasetsProjectId( project->sharedDatasetsProjectId() );
          mProjects[i]->setType( project->type() );
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
      newProjects.append( project );
    }
  }

  if ( !newProjects.isEmpty() )
  {
    beginInsertRows( QModelIndex(), currentCount, currentCount + newProjects.size() - 1 );
    for ( QfCloudProject *newProject : newProjects ) // cppcheck-suppress constVariablePointer
    {
      mProjects.append( newProject );
      setupProjectConnections( newProject );
    }
    endInsertRows();
  }
}

void QfCloudProjectsModel::setupProjectConnections( QfCloudProject *project )
{
  connect( project, &QfCloudProject::isProjectOutdatedChanged, this, [this] {
    const QfCloudProject *p = static_cast<QfCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << ProjectFileOutdatedRole );
  } );

  connect( project, &QfCloudProject::downloaded, this, [this]( const QString &error ) {
    const QfCloudProject *p = static_cast<QfCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit projectDownloaded( p->id(), p->name(), p->owner(), !error.isEmpty(), error );
    emit dataChanged( idx, idx, QVector<int>() << StatusRole << PackagingStatusRole << ErrorStatusRole << ErrorStringRole );
  } );

  connect( project, &QfCloudProject::pushFinished, this, [this]( bool isDownloading, const QString &error ) {
    const QfCloudProject *p = static_cast<QfCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit pushFinished( p->id(), isDownloading, !error.isEmpty(), error );
  } );

  connect( project, &QfCloudProject::dataRefreshed, this, [this]( QfCloudProject::ProjectRefreshReason reason, const QString &error ) {
    const QfCloudProject *p = static_cast<QfCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx );
  } );

  connect( project, &QfCloudProject::jobFinished, this, [this]( QfCloudProject::JobType type, const QString &error ) {
    const QfCloudProject *p = static_cast<QfCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx );
  } );

  connect( project, &QfCloudProject::statusChanged, this, [this] {
    const QfCloudProject *p = static_cast<QfCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << StatusRole );
  } );

  connect( project, &QfCloudProject::errorStatusChanged, this, [this] {
    const QfCloudProject *p = static_cast<QfCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << ErrorStatusRole );
  } );

  connect( project, &QfCloudProject::packagingStatusChanged, this, [this] {
    const QfCloudProject *p = static_cast<QfCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << PackagingStatusRole );
  } );

  connect( project, &QfCloudProject::downloadProgressChanged, this, [this] {
    const QfCloudProject *p = static_cast<QfCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << DownloadProgressRole );
  } );

  connect( project, &QfCloudProject::downloadBytesTotalChanged, this, [this] {
    const QfCloudProject *p = static_cast<QfCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << DownloadSizeRole );
  } );

  connect( project, &QfCloudProject::packagedLayerErrorsChanged, this, [this] {
    const QfCloudProject *p = static_cast<QfCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << PackagedLayerErrorsRole );
  } );

  connect( project, &QfCloudProject::isOutdatedChanged, this, [this] {
    const QfCloudProject *p = static_cast<QfCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << ProjectOutdatedRole );
  } );

  connect( project, &QfCloudProject::localPathChanged, this, [this] {
    const QfCloudProject *p = static_cast<QfCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << LocalPathRole );
  } );

  connect( project, &QfCloudProject::checkoutChanged, this, [this] {
    const QfCloudProject *p = static_cast<QfCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << CheckoutRole );
  } );

  connect( project, &QfCloudProject::lastLocalExportedAtChanged, this, [this] {
    const QfCloudProject *p = static_cast<QfCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << LastLocalExportedAtRole );
  } );

  connect( project, &QfCloudProject::modificationChanged, this, [this] {
    const QfCloudProject *p = static_cast<QfCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << ModificationRole );
  } );

  connect( project, &QfCloudProject::deltaFilePushStatusChanged, this, [this] {
    const QfCloudProject *p = static_cast<QfCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << PushDeltaStatusRole );
  } );

  connect( project, &QfCloudProject::deltaFilePushStatusStringChanged, this, [this] {
    const QfCloudProject *p = static_cast<QfCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << PushDeltaStatusStringRole );
  } );

  connect( project, &QfCloudProject::pushDeltaProgressChanged, this, [this] {
    const QfCloudProject *p = static_cast<QfCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << PushDeltaProgressRole );
  } );

  connect( project, &QfCloudProject::lastLocalPushDeltasChanged, this, [this] {
    const QfCloudProject *p = static_cast<QfCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << LastLocalPushDeltasRole );
  } );

  connect( project, &QfCloudProject::deltaFileWrapperChanged, this, [this] {
    const QfCloudProject *p = static_cast<QfCloudProject *>( sender() );
    if ( mCurrentProjectId == p->id() )
    {
      if ( mLayerObserver )
      {
        mLayerObserver->setDeltaFileWrapper( p->deltaFileWrapper() );
      }
    }
  } );

  connect( project, &QfCloudProject::deltasCountChanged, this, [this] {
    const QfCloudProject *p = static_cast<QfCloudProject *>( sender() );
    const QModelIndex idx = findProjectIndex( p->id() );
    emit dataChanged( idx, idx, QVector<int>() << LocalDeltasCountRole );
  } );
}

void QfCloudProjectsModel::loadProjects( const QJsonArray &remoteProjects, bool skipLocalProjects )
{
  QList<QfCloudProject *> freshCloudProjects;
  for ( const auto project : remoteProjects )
  {
    QVariantHash projectDetails = project.toObject().toVariantHash();
    QfCloudProject *cloudProject = QfCloudProject::fromDetails( projectDetails, mCloudConnection, mGpkgFlusher );

    if ( cloudProject->type() == QfCloudProject::ProjectType::SharedDatasets )
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
    QList<QfCloudProject *> userSpecificProjects;
    QDirIterator userDirs( QfCloudUtils::localCloudDirectory(), QDir::Dirs | QDir::NoDotAndDotDot );
    while ( userDirs.hasNext() )
    {
      userDirs.next();
      const QString username = userDirs.fileName();

      // We skip cloud projects that are not linked to the last successul logged in account
      if ( username != mUsername )
        continue;

      QDirIterator projectDirs( QStringLiteral( "%1/%2" ).arg( QfCloudUtils::localCloudDirectory(), username ), QDir::Dirs | QDir::NoDotAndDotDot );
      while ( projectDirs.hasNext() )
      {
        projectDirs.next();

        const QString projectId = projectDirs.fileName();
        if ( findProject( projectId ) )
        {
          // Already covered when receiving cloud projects from the server
          continue;
        }

        QfCloudProject *cloudProject = QfCloudProject::fromLocalSettings( projectId, mCloudConnection, mGpkgFlusher );
        if ( !cloudProject )
        {
          // Not a previously loaded cloud project
          continue;
        }

        // If the cloud project is a special shared dataset project or if the cloud project
        // had a folder but was not downloaded properly, do not add to the model
        if ( cloudProject->type() == QfCloudProject::ProjectType::SharedDatasets || cloudProject->localPath().isEmpty() )
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

int QfCloudProjectsModel::rowCount( const QModelIndex &parent ) const
{
  return !parent.isValid() ? static_cast<int>( mProjects.size() ) : 0;
}

QVariant QfCloudProjectsModel::data( const QModelIndex &index, int role ) const
{
  if ( index.row() >= mProjects.size() || index.row() < 0 )
  {
    return QVariant();
  }

  const QfCloudProject *project = mProjects.at( index.row() );
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
      if ( project->errorStatus() == QfCloudProject::DownloadErrorStatus )
      {
        return project->packagingStatusString();
      }
      else if ( project->errorStatus() == QfCloudProject::PushErrorStatus )
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

    case ProjectTypeRole:
      return QVariant::fromValue( project->type() );
  }

  return QVariant();
}

bool QfCloudProjectsModel::revertLocalChangesFromCurrentProject()
{
  const QfCloudProject *project = findProject( mCurrentProjectId );
  if ( !project )
  {
    return false;
  }

  QfDeltaFileWrapper *deltaFileWrapper = mLayerObserver->deltaFileWrapper();

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

bool QfCloudProjectsModel::discardLocalChangesFromCurrentProject()
{
  const QfCloudProject *project = findProject( mCurrentProjectId );
  if ( !project )
  {
    return false;
  }

  QfDeltaFileWrapper *deltaFileWrapper = mLayerObserver->deltaFileWrapper();

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

void QfCloudProjectsModel::setGpkgFlusher( QgsGpkgFlusher *flusher )
{
  if ( mGpkgFlusher == flusher )
  {
    return;
  }

  mGpkgFlusher = flusher;

  emit gpkgFlusherChanged();
}

void QfCloudProjectsModel::createProject( const QString &name, const QString &fromProjectId )
{
  if ( name.isEmpty() )
  {
    emit projectCreated( QString(), fromProjectId, true, tr( "Project creation requires a name" ) );
    return;
  }

  mIsCreating = true;
  emit isCreatingChanged();

  QString sanitizedName = name.normalized( QString::NormalizationForm_KD );
  sanitizedName.replace( QRegularExpression( "[^A-Za-z0-9_]" ), QStringLiteral( "_" ) );

  QString url = QStringLiteral( "/api/v1/projects/?owner=%1" ).arg( mUsername );
  QNetworkRequest request( url );
  request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );
  request.setAttribute( QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::RedirectPolicy::NoLessSafeRedirectPolicy );
  mCloudConnection->setAuthenticationDetails( request );

  const QfNetworkReply *listingreply = mCloudConnection->get( request, url );
  connect( listingreply, &QfNetworkReply::finished, this, [this, sanitizedName, fromProjectId]() {
    QfNetworkReply *reply = qobject_cast<QfNetworkReply *>( sender() );
    QNetworkReply *rawReply = reply->currentRawReply();
    Q_ASSERT( rawReply );

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      emit projectCreated( QString(), fromProjectId, true, mCloudConnection->errorString( rawReply ) );
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
    QString finalizedName = sanitizedName;
    while ( projectNames.contains( finalizedName.toLower() ) )
    {
      finalizedName = QStringLiteral( "%1_%2" ).arg( sanitizedName, QString::number( uniqueSuffix++ ) );
    }

    QString url = QStringLiteral( "/api/v1/projects/" );

    QVariantMap params;
    params.insert( QStringLiteral( "name" ), finalizedName );
    params.insert( QStringLiteral( "owner" ), mUsername );
    params.insert( QStringLiteral( "description" ), QString() );
    params.insert( QStringLiteral( "private" ), true );

    if ( !fromProjectId.isEmpty() )
    {
      params.insert( QStringLiteral( "clone_from_project" ), fromProjectId );
    }

    QNetworkRequest request;
    request.setAttribute( static_cast<QNetworkRequest::Attribute>( ProjectsRequestAttribute::FromProjectId ), fromProjectId );
    const QfNetworkReply *creationReply = mCloudConnection->post( request, url, params );
    connect( creationReply, &QfNetworkReply::finished, this, &QfCloudProjectsModel::projectCreationReceived );
  } );
}

void QfCloudProjectsModel::projectCreationReceived()
{
  QfNetworkReply *reply = qobject_cast<QfNetworkReply *>( sender() );
  QNetworkReply *rawReply = reply->currentRawReply();
  Q_ASSERT( rawReply );

  const QString fromProjectId = rawReply->request().attribute( static_cast<QNetworkRequest::Attribute>( ProjectsRequestAttribute::FromProjectId ) ).toString();

  if ( rawReply->error() != QNetworkReply::NoError )
  {
    emit projectCreated( QString(), fromProjectId, true, mCloudConnection->errorString( rawReply ) );

    mIsCreating = false;
    emit isCreatingChanged();
    return;
  }

  QByteArray response = rawReply->readAll();
  QJsonDocument doc = QJsonDocument::fromJson( response );
  QVariantHash projectDetails = doc.object().toVariantHash();
  QfCloudProject *cloudProject = QfCloudProject::fromDetails( projectDetails, mCloudConnection, mGpkgFlusher ); // cppcheck-suppress constVariablePointer
  if ( cloudProject )
  {
    insertProjects( QList<QfCloudProject *>() << cloudProject );
    emit projectCreated( cloudProject->id(), fromProjectId, false, QString() );

    if ( QfCloudProject *project = findProject( cloudProject->id() ) )
    {
      project->setStatus( QfCloudProject::ProjectStatus::Creating );
      project->ensureProjectCreated();
    }
  }
  else
  {
    emit projectCreated( QString(), fromProjectId, true, tr( "Cloud project could not be created." ) );
  }

  mIsCreating = false;
  emit isCreatingChanged();
}

bool QfCloudProjectsModel::hasTemplates() const
{
  return mHasTemplates;
}

void QfCloudProjectsModel::updateHasTemplates()
{
  const bool hasTemplates = std::any_of( mProjects.begin(), mProjects.end(), []( const QfCloudProject *project ) { return project && project->type() == QfCloudProject::ProjectType::Template; } );
  if ( mHasTemplates != hasTemplates )
  {
    mHasTemplates = hasTemplates;
    emit hasTemplatesChanged();
  }
}

// --

QfCloudProjectsFilterModel::QfCloudProjectsFilterModel( QObject *parent )
  : QSortFilterProxyModel( parent )
{
  setDynamicSortFilter( true );
  setSortLocaleAware( true );
  sort( 0 );

  mProjectsAppendingTimer.setInterval( 500 );
  mProjectsAppendingTimer.setSingleShot( true );
  connect( &mProjectsAppendingTimer, &QTimer::timeout, this, &QfCloudProjectsFilterModel::triggerProjectsAppending );
}

void QfCloudProjectsFilterModel::setProjectsModel( QfCloudProjectsModel *projectsModel )
{
  if ( mSourceModel == projectsModel )
  {
    return;
  }

  if ( mSourceModel )
  {
    disconnect( mSourceModel, &QfCloudProjectsModel::projectsAppended, this, &QfCloudProjectsFilterModel::projectsAppended );
  }

  mSourceModel = projectsModel;
  setSourceModel( mSourceModel );

  if ( mSourceModel )
  {
    connect( mSourceModel, &QfCloudProjectsModel::projectsAppended, this, &QfCloudProjectsFilterModel::projectsAppended );
  }

  emit projectsModelChanged();
}

QfCloudProjectsModel *QfCloudProjectsFilterModel::projectsModel() const
{
  return mSourceModel;
}

void QfCloudProjectsFilterModel::setShowLocalOnly( bool showLocalOnly )
{
  if ( mShowLocalOnly == showLocalOnly )
  {
    return;
  }

  beginFilterChange();
  mShowLocalOnly = showLocalOnly;
  endFilterChange( QSortFilterProxyModel::Direction::Rows );

  emit showLocalOnlyChanged();
}

bool QfCloudProjectsFilterModel::showLocalOnly() const
{
  return mShowLocalOnly;
}

bool QfCloudProjectsFilterModel::lessThan( const QModelIndex &sourceLeft, const QModelIndex &sourceRight ) const
{
  if ( !mSourceModel )
  {
    return true;
  }

  if ( mShowFeaturedOnTop )
  {
    const bool isFeaturedLeft = mSourceModel->data( sourceLeft, QfCloudProjectsModel::FeaturedRole ).toBool();
    const bool isFeaturedRight = mSourceModel->data( sourceRight, QfCloudProjectsModel::FeaturedRole ).toBool();
    if ( isFeaturedLeft != isFeaturedRight )
    {
      return isFeaturedLeft;
    }
  }

  QString left = mSourceModel->data( sourceLeft, QfCloudProjectsModel::OwnerRole ).toString().toLower();
  QString right = mSourceModel->data( sourceRight, QfCloudProjectsModel::OwnerRole ).toString().toLower();
  int compare = QString::localeAwareCompare( left, right );
  if ( compare != 0 )
  {
    return compare < 0;
  }

  left = mSourceModel->data( sourceLeft, QfCloudProjectsModel::NameRole ).toString().toLower();
  right = mSourceModel->data( sourceRight, QfCloudProjectsModel::NameRole ).toString().toLower();
  compare = QString::localeAwareCompare( left, right );
  return compare < 0;
}

bool QfCloudProjectsFilterModel::filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const
{
  const QModelIndex currentRowIndex = mSourceModel->index( source_row, 0, source_parent );
  const QfCloudProject *project = mSourceModel->findProject( mSourceModel->data( currentRowIndex, QfCloudProjectsModel::IdRole ).toString() );
  if ( !project )
  {
    return false;
  }

  if ( mShowLocalOnly && project->localPath().isEmpty() )
  {
    return false;
  }

  const bool isTemplate = project->type() == QfCloudProject::ProjectType::Template;
  if ( ( !mShowTemplates && isTemplate ) || ( mShowTemplates && !isTemplate ) )
  {
    return false;
  }

  const bool isPublic = project->localPath().isEmpty() && project->userRoleOrigin() == QStringLiteral( "public" );
  if ( mIncludePublic && isPublic )
  {
    if ( project->remoteSizeBytes() == 0 )
    {
      // Empty project, skip
      return false;
    }

    if ( project->remoteSizeBytes() < 30000 && project->dataLastUpdatedAt().isNull() )
    {
      // Most likely a created project with a single OSM layer that never was customized, skip
      return false;
    }
  }
  else
  {
    if ( isPublic )
    {
      return false;
    }
  }

  if ( !mShowInValidProjects && project->status() == QfCloudProject::ProjectStatus::Failing )
  {
    return false;
  }

  if ( !mOwnerFilter.isEmpty() )
  {
    if ( project->owner().compare( mOwnerFilter, Qt::CaseInsensitive ) != 0 )
    {
      return false;
    }
  }

  if ( !mKeywordFilter.isEmpty() )
  {
    if ( std::any_of( mKeywordFilter.begin(), mKeywordFilter.end(), [project]( const QString &keyword ) { return !project->name().contains( keyword, Qt::CaseInsensitive ) && !project->description().contains( keyword, Qt::CaseInsensitive ) && !project->owner().contains( keyword, Qt::CaseInsensitive ); } ) )
    {
      return false;
    }
  }

  return true;
}

void QfCloudProjectsFilterModel::projectsAppended( const QString &owner, const QString &search, const bool hasError, const QString &errorString )
{
  if ( mOwnerFilter.isEmpty() && mKeywordFilter.isEmpty() )
  {
    return;
  }

  if ( mOwnerFilter == owner && mKeywordFilter == search.split( QLatin1Char( ' ' ), Qt::SkipEmptyParts ) )
  {
    mIsSearching = false;
    emit isSearchingChanged();
  }
}

void QfCloudProjectsFilterModel::triggerProjectsAppending()
{
  if ( mSourceModel && ( !mOwnerFilter.isEmpty() || !mKeywordFilter.isEmpty() ) )
  {
    mSourceModel->appendProjects( mOwnerFilter, mKeywordFilter.join( QLatin1Char( ' ' ) ) );
  }
}

void QfCloudProjectsFilterModel::setTextFilter( const QString &text )
{
  if ( mTextFilter == text )
  {
    return;
  }

  beginFilterChange();
  mTextFilter = text;

  QString searchTerm;
  QString owner;
  bool includePublic = false;

  const QStringList tokens = text.split( QLatin1Char( ' ' ), Qt::SkipEmptyParts );
  for ( const QString &token : tokens )
  {
    if ( token.startsWith( QStringLiteral( "owner:" ), Qt::CaseInsensitive ) )
    {
      owner = token.mid( 6 ).trimmed();
    }
    else if ( token.compare( QStringLiteral( "include:public" ), Qt::CaseInsensitive ) == 0 )
    {
      includePublic = true;
    }
    else
    {
      if ( !searchTerm.isEmpty() )
      {
        searchTerm += QLatin1Char( ' ' );
      }
      searchTerm += token;
    }
  }

  mKeywordFilter = searchTerm.split( QLatin1Char( ' ' ), Qt::SkipEmptyParts );
  mOwnerFilter = owner;
  mIncludePublic = includePublic;

  if ( mSourceModel && ( !mOwnerFilter.isEmpty() || searchTerm.size() > 1 ) )
  {
    mIsSearching = true;
    emit isSearchingChanged();

    mProjectsAppendingTimer.start();
  }
  else
  {
    if ( mIsSearching )
    {
      mIsSearching = false;
      emit isSearchingChanged();
    }

    mProjectsAppendingTimer.stop();
  }

  endFilterChange( QSortFilterProxyModel::Direction::Rows );

  emit textFilterChanged();
}

QString QfCloudProjectsFilterModel::textFilter() const
{
  return mTextFilter;
}

void QfCloudProjectsFilterModel::setShowInValidProjects( bool showInValidProjects )
{
  if ( mShowInValidProjects == showInValidProjects )
  {
    return;
  }

  beginFilterChange();
  mShowInValidProjects = showInValidProjects;
  endFilterChange( QSortFilterProxyModel::Direction::Rows );

  emit showInValidProjectsChanged();
}

void QfCloudProjectsFilterModel::setShowTemplates( bool showTemplates )
{
  if ( mShowTemplates == showTemplates )
  {
    return;
  }

  beginFilterChange();
  mShowTemplates = showTemplates;
  endFilterChange( QSortFilterProxyModel::Direction::Rows );

  emit showTemplatesChanged();
}

bool QfCloudProjectsFilterModel::showTemplates() const
{
  return mShowTemplates;
}

bool QfCloudProjectsFilterModel::showInValidProjects() const
{
  return mShowInValidProjects;
}

void QfCloudProjectsFilterModel::setShowFeaturedOnTop( bool showFeaturedOnTop )
{
  if ( mShowFeaturedOnTop == showFeaturedOnTop )
  {
    return;
  }

  mShowFeaturedOnTop = showFeaturedOnTop;
  emit showFeaturedOnTopChanged();

  sort( 0 );
}

bool QfCloudProjectsFilterModel::showFeaturedOnTop() const
{
  return mShowFeaturedOnTop;
}

bool QfCloudProjectsFilterModel::isSearching() const
{
  return mIsSearching;
}
```


