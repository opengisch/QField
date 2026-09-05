

# File qfcloudprojectsmodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldcloud**](dir_d27ac98b23548f5810403c734ce9817b.md) **>** [**qfcloudprojectsmodel.h**](qfcloudprojectsmodel_8h.md)

[Go to the documentation of this file](qfcloudprojectsmodel_8h.md)


```C++
/***************************************************************************
    qfcloudprojectsmodel.h
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

#ifndef QFCLOUDPROJECTSMODEL_H
#define QFCLOUDPROJECTSMODEL_H

#include "qfcloudproject.h"
#include "qgsgpkgflusher.h"

#include <QAbstractListModel>
#include <QJsonArray>
#include <QNetworkReply>
#include <QPointer>
#include <QSortFilterProxyModel>
#include <QTimer>


class QNetworkRequest;
class QfCloudConnection;
class QfLayerObserver;
class QgsMapLayer;

class QfCloudProjectsModel : public QAbstractListModel
{
    Q_OBJECT

    // moc needs the full definition to build the metaobject.
    Q_MOC_INCLUDE( "qflayerobserver.h" )

    
    Q_PROPERTY( QfCloudConnection *cloudConnection READ cloudConnection WRITE setCloudConnection NOTIFY cloudConnectionChanged )
    Q_PROPERTY( QfLayerObserver *layerObserver READ layerObserver WRITE setLayerObserver NOTIFY layerObserverChanged )
    Q_PROPERTY( QgsGpkgFlusher *gpkgFlusher READ gpkgFlusher WRITE setGpkgFlusher NOTIFY gpkgFlusherChanged )

    Q_PROPERTY( bool isRefreshing READ isRefreshing NOTIFY isRefreshingChanged )

    Q_PROPERTY( bool isCreating READ isCreating NOTIFY isCreatingChanged )

    Q_PROPERTY( bool hasTemplates READ hasTemplates NOTIFY hasTemplatesChanged )

    Q_PROPERTY( QSet<QString> busyProjectIds READ busyProjectIds NOTIFY busyProjectIdsChanged )

    Q_PROPERTY( QString currentProjectId READ currentProjectId WRITE setCurrentProjectId NOTIFY currentProjectIdChanged )
    Q_PROPERTY( QfCloudProject *currentProject READ currentProject NOTIFY currentProjectChanged )

  public:
    enum ColumnRole
    {
      IdRole = Qt::UserRole + 1,
      OwnerRole,
      PublicRole,
      FeaturedRole,
      NameRole,
      DescriptionRole,
      ModificationRole,
      CheckoutRole,
      StatusRole,
      ProjectOutdatedRole,
      ProjectFileOutdatedRole,
      ErrorStatusRole,
      ErrorStringRole,
      DownloadProgressRole,
      DownloadSizeRole,
      PackagingStatusRole,
      PackagedLayerErrorsRole,
      PushDeltaProgressRole,
      PushDeltaStatusRole,
      PushDeltaStatusStringRole,
      LocalDeltasCountRole,
      LocalPathRole,
      LastLocalExportedAtRole,
      LastLocalPushDeltasRole,
      UserRoleRole,
      UserRoleOriginRole,
      ProjectTypeRole
    };

    enum class ProjectsRequestAttribute
    {
      FetchPublicProjects = QNetworkRequest::User + 1,
      ProjectsFetchOffset = QNetworkRequest::User + 2,
      ResetModel = QNetworkRequest::User + 3,
      ProjectId = QNetworkRequest::User + 4,
      ProjectOwnerName = QNetworkRequest::User + 5,
      ProjectSearchTerm = QNetworkRequest::User + 6,
      FromProjectId = QNetworkRequest::User + 7,
    };

    Q_ENUM( ColumnRole )

    QfCloudProjectsModel();

    QfCloudConnection *cloudConnection() const;

    void setCloudConnection( QfCloudConnection *cloudConnection );

    QfLayerObserver *layerObserver() const;

    void setLayerObserver( QfLayerObserver *layerObserver );

    bool isRefreshing() const { return mIsRefreshing; }

    bool isCreating() const { return mIsCreating; }

    QString currentProjectId() const;

    void setCurrentProjectId( const QString &currentProjectId );

    QfCloudProject *currentProject() const;

    QgsGpkgFlusher *gpkgFlusher() const { return mGpkgFlusher; }

    void setGpkgFlusher( QgsGpkgFlusher *flusher );

    QSet<QString> busyProjectIds() const;

    Q_INVOKABLE void refreshProjectsList( bool shouldResetModel = true, int projectFetchOffset = 0 );

    Q_INVOKABLE void projectPush( const QString &projectId, const bool shouldDownloadUpdates );

    Q_INVOKABLE void removeLocalProject( const QString &projectId );

    Q_INVOKABLE bool revertLocalChangesFromCurrentProject();

    Q_INVOKABLE bool discardLocalChangesFromCurrentProject();

    QHash<int, QByteArray> roleNames() const override;

    int rowCount( const QModelIndex &parent ) const override;

    QVariant data( const QModelIndex &index, int role ) const override;

    Q_INVOKABLE void projectPackageAndDownload( const QString &projectId );

    Q_INVOKABLE void projectCancelDownload( const QString &projectId );

    Q_INVOKABLE QfCloudProject *findProject( const QString &projectId ) const;

    Q_INVOKABLE void appendProject( const QString &projectId, bool forceRefresh = false );

    Q_INVOKABLE void appendProjects( const QString &owner, const QString &search, int projectFetchOffset = 0 );

    Q_INVOKABLE QStringList uniqueOwners() const;

    Q_INVOKABLE void createProject( const QString &name, const QString &fromProjectId = QString() );

    bool hasTemplates() const;

  signals:
    void cloudConnectionChanged();
    void layerObserverChanged();
    void isRefreshingChanged();
    void isCreatingChanged();
    void currentProjectIdChanged();
    void currentProjectChanged();
    void busyProjectIdsChanged();
    void gpkgFlusherChanged();
    void warning( const QString &message );

    void projectCreated( const QString &projectId, const QString &fromProjectId, const bool hasError, const QString &errorString );
    void projectAppended( const QString &projectId, const bool hasError = false, const QString &errorString = QString() );
    void projectsAppended( const QString &owner, const QString &search, const bool hasError = false, const QString &errorString = QString() );
    void projectDownloaded( const QString &projectId, const QString &projectName, const QString &projectOwner, const bool hasError = false, const QString &errorString = QString() );
    void pushFinished( const QString &projectId, bool isDownloadingProject, bool hasError = false, const QString &errorString = QString() );

    void projectUploaded( const QString &projectId );

    void hasTemplatesChanged();

  private slots:
    void connectionStatusChanged();
    void usernameChanged();
    void urlChanged();

    void projectListReceived();
    void projectReceived();
    void projectCreationReceived();

  private:
    void setupProjectConnections( QfCloudProject *project );

    QModelIndex findProjectIndex( const QString &projectId ) const;

    void loadProjects( const QJsonArray &remoteProjects = QJsonArray(), bool skipLocalProjects = false );
    void insertProjects( const QList<QfCloudProject *> &projects );
    void resetProjects();

    inline QString layerFileName( const QgsMapLayer *layer ) const;

    void updateHasTemplates();
    bool mHasTemplates = false;

    QList<QfCloudProject *> mProjects;
    QfCloudConnection *mCloudConnection = nullptr;

    bool mIsRefreshing = false;
    bool mIsCreating = false;

    QString mCurrentProjectId;
    QPointer<QfCloudProject> mCurrentProject;

    QfLayerObserver *mLayerObserver = nullptr;
    QgsGpkgFlusher *mGpkgFlusher = nullptr;
    QString mUsername;
    QString mUrl;

    const int mProjectsPerFetch = 250;
};

class QfCloudProjectsFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY( QfCloudProjectsModel *projectsModel READ projectsModel WRITE setProjectsModel NOTIFY projectsModelChanged )
    Q_PROPERTY( QString textFilter READ textFilter WRITE setTextFilter NOTIFY textFilterChanged )
    Q_PROPERTY( bool showLocalOnly READ showLocalOnly WRITE setShowLocalOnly NOTIFY showLocalOnlyChanged )
    Q_PROPERTY( bool showInValidProjects READ showInValidProjects WRITE setShowInValidProjects NOTIFY showInValidProjectsChanged )
    Q_PROPERTY( bool showFeaturedOnTop READ showFeaturedOnTop WRITE setShowFeaturedOnTop NOTIFY showFeaturedOnTopChanged )
    Q_PROPERTY( bool isSearching READ isSearching NOTIFY isSearchingChanged )
    Q_PROPERTY( bool showTemplates READ showTemplates WRITE setShowTemplates NOTIFY showTemplatesChanged )

  public:
    explicit QfCloudProjectsFilterModel( QObject *parent = nullptr );

    QfCloudProjectsModel *projectsModel() const;

    void setProjectsModel( QfCloudProjectsModel *projectsModel );

    bool showLocalOnly() const;

    void setShowLocalOnly( bool showLocalOnly );

    void setTextFilter( const QString &text );

    QString textFilter() const;

    void setShowInValidProjects( bool showInValidProjects );

    bool showInValidProjects() const;

    bool showTemplates() const;

    void setShowTemplates( bool showTemplates );

    void setShowFeaturedOnTop( bool showFeaturedOnTop );

    bool showFeaturedOnTop() const;

    bool isSearching() const;

  signals:

    void projectsModelChanged();
    void filterChanged();
    void showLocalOnlyChanged();
    void textFilterChanged();
    void showInValidProjectsChanged();
    void showFeaturedOnTopChanged();
    void isSearchingChanged();
    void showTemplatesChanged();

  private slots:
    void triggerProjectsAppending();
    void projectsAppended( const QString &owner, const QString &search, const bool hasError = false, const QString &errorString = QString() );

  protected:
    bool lessThan( const QModelIndex &sourceLeft, const QModelIndex &sourceRight ) const override;
    virtual bool filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const override;

  private:
    QfCloudProjectsModel *mSourceModel = nullptr;
    bool mShowLocalOnly = false;
    bool mShowInValidProjects = false;
    bool mShowFeaturedOnTop = false;
    QString mTextFilter;
    QStringList mKeywordFilter;
    QString mOwnerFilter;
    bool mIncludePublic = false;
    bool mIsSearching = false;
    bool mShowTemplates = false;

    QTimer mProjectsAppendingTimer;
};

#endif // QFCLOUDPROJECTSMODEL_H
```


