

# File qfieldcloudprojectsmodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldcloud**](dir_d27ac98b23548f5810403c734ce9817b.md) **>** [**qfieldcloudprojectsmodel.h**](qfieldcloudprojectsmodel_8h.md)

[Go to the documentation of this file](qfieldcloudprojectsmodel_8h.md)


```C++
/***************************************************************************
    qfieldcloudprojectsmodel.h
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

#ifndef QFIELDCLOUDPROJECTSMODEL_H
#define QFIELDCLOUDPROJECTSMODEL_H

#include "qfieldcloudproject.h"
#include "qgsgpkgflusher.h"

#include <QAbstractListModel>
#include <QJsonArray>
#include <QNetworkReply>
#include <QPointer>
#include <QSortFilterProxyModel>
#include <QTimer>


class QNetworkRequest;
class QFieldCloudConnection;
class LayerObserver;
class QgsMapLayer;

class QFieldCloudProjectsModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY( QFieldCloudConnection *cloudConnection READ cloudConnection WRITE setCloudConnection NOTIFY cloudConnectionChanged )
    Q_PROPERTY( LayerObserver *layerObserver READ layerObserver WRITE setLayerObserver NOTIFY layerObserverChanged )
    Q_PROPERTY( QgsGpkgFlusher *gpkgFlusher READ gpkgFlusher WRITE setGpkgFlusher NOTIFY gpkgFlusherChanged )

    Q_PROPERTY( bool isRefreshing READ isRefreshing NOTIFY isRefreshingChanged )

    Q_PROPERTY( bool isCreating READ isCreating NOTIFY isCreatingChanged )

    Q_PROPERTY( QSet<QString> busyProjectIds READ busyProjectIds NOTIFY busyProjectIdsChanged )

    Q_PROPERTY( QString currentProjectId READ currentProjectId WRITE setCurrentProjectId NOTIFY currentProjectIdChanged )
    Q_PROPERTY( QFieldCloudProject *currentProject READ currentProject NOTIFY currentProjectChanged )

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
      DeltaListRole
    };

    enum class ProjectsRequestAttribute
    {
      FetchPublicProjects = QNetworkRequest::User + 1,
      ProjectsFetchOffset = QNetworkRequest::User + 2,
      ResetModel = QNetworkRequest::User + 3,
      ProjectId = QNetworkRequest::User + 4,
    };

    Q_ENUM( ColumnRole )

    QFieldCloudProjectsModel();

    QFieldCloudConnection *cloudConnection() const;

    void setCloudConnection( QFieldCloudConnection *cloudConnection );

    LayerObserver *layerObserver() const;

    void setLayerObserver( LayerObserver *layerObserver );

    bool isRefreshing() const { return mIsRefreshing; }

    bool isCreating() const { return mIsCreating; }

    QString currentProjectId() const;

    void setCurrentProjectId( const QString &currentProjectId );

    QFieldCloudProject *currentProject() const;

    QgsGpkgFlusher *gpkgFlusher() const { return mGpkgFlusher; }

    void setGpkgFlusher( QgsGpkgFlusher *flusher );

    QSet<QString> busyProjectIds() const;

    Q_INVOKABLE void refreshProjectsList( bool shouldResetModel = true, bool shouldFetchPublic = false, int projectFetchOffset = 0 );

    Q_INVOKABLE void projectPush( const QString &projectId, const bool shouldDownloadUpdates );

    Q_INVOKABLE void refreshProjectDeltaList( const QString &projectId );

    Q_INVOKABLE void removeLocalProject( const QString &projectId );

    Q_INVOKABLE bool revertLocalChangesFromCurrentProject();

    Q_INVOKABLE bool discardLocalChangesFromCurrentProject();

    QHash<int, QByteArray> roleNames() const override;

    int rowCount( const QModelIndex &parent ) const override;

    QVariant data( const QModelIndex &index, int role ) const override;

    Q_INVOKABLE void projectPackageAndDownload( const QString &projectId );

    Q_INVOKABLE void projectCancelDownload( const QString &projectId );

    Q_INVOKABLE void updateLocalizedDataPaths( const QString &projectPath );

    Q_INVOKABLE QFieldCloudProject *findProject( const QString &projectId ) const;

    Q_INVOKABLE void appendProject( const QString &projectId );

    Q_INVOKABLE void createProject( const QString name );

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

    void projectCreated( const QString &projectId, const bool hasError = false, const QString &errorString = QString() );
    void projectAppended( const QString &projectId, const bool hasError = false, const QString &errorString = QString() );
    void projectDownloaded( const QString &projectId, const QString &projectName, const bool hasError = false, const QString &errorString = QString() );
    void pushFinished( const QString &projectId, bool isDownloadingProject, bool hasError = false, const QString &errorString = QString() );

    void deltaListModelChanged();

    void projectUploaded( const QString &projectId );

  private slots:
    void connectionStatusChanged();
    void usernameChanged();
    void urlChanged();

    void projectListReceived();
    void projectReceived();
    void projectCreationReceived();

  private:
    void setupProjectConnections( QFieldCloudProject *project );

    QModelIndex findProjectIndex( const QString &projectId ) const;

    void loadProjects( const QJsonArray &remoteProjects = QJsonArray(), bool skipLocalProjects = false );
    void insertProjects( const QList<QFieldCloudProject *> &projects );
    void resetProjects();

    inline QString layerFileName( const QgsMapLayer *layer ) const;

    QList<QFieldCloudProject *> mProjects;
    QFieldCloudConnection *mCloudConnection = nullptr;

    bool mIsRefreshing = false;
    bool mIsCreating = false;

    QString mCurrentProjectId;
    QPointer<QFieldCloudProject> mCurrentProject;

    LayerObserver *mLayerObserver = nullptr;
    QgsGpkgFlusher *mGpkgFlusher = nullptr;
    QString mUsername;
    QString mUrl;

    const int mProjectsPerFetch = 250;
};

class QFieldCloudProjectsFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY( QFieldCloudProjectsModel *projectsModel READ projectsModel WRITE setProjectsModel NOTIFY projectsModelChanged )
    Q_PROPERTY( ProjectsFilter filter READ filter WRITE setFilter NOTIFY filterChanged )
    Q_PROPERTY( bool showLocalOnly READ showLocalOnly WRITE setShowLocalOnly NOTIFY showLocalOnlyChanged )
    Q_PROPERTY( QString textFilter READ textFilter WRITE setTextFilter NOTIFY textFilterChanged )
    Q_PROPERTY( bool showInValidProjects READ showInValidProjects WRITE setShowInValidProjects NOTIFY showInValidProjectsChanged )
    Q_PROPERTY( bool showFeaturedOnTop READ showFeaturedOnTop WRITE setShowFeaturedOnTop NOTIFY showFeaturedOnTopChanged )

  public:
    enum ProjectsFilter
    {
      PrivateProjects,
      PublicProjects,
    };
    Q_ENUM( ProjectsFilter )

    explicit QFieldCloudProjectsFilterModel( QObject *parent = nullptr );

    QFieldCloudProjectsModel *projectsModel() const;

    void setProjectsModel( QFieldCloudProjectsModel *projectsModel );

    ProjectsFilter filter() const;

    void setFilter( ProjectsFilter filter );

    bool showLocalOnly() const;

    void setShowLocalOnly( bool showLocalOnly );

    void setTextFilter( const QString &text );

    QString textFilter() const;

    void setShowInValidProjects( bool showInValidProjects );

    bool showInValidProjects() const;

    void setShowFeaturedOnTop( bool showFeaturedOnTop );

    bool showFeaturedOnTop() const;

  signals:

    void projectsModelChanged();
    void filterChanged();
    void showLocalOnlyChanged();
    void textFilterChanged();
    void showInValidProjectsChanged();
    void showFeaturedOnTopChanged();

  protected:
    bool lessThan( const QModelIndex &sourceLeft, const QModelIndex &sourceRight ) const override;
    virtual bool filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const override;

  private:
    QFieldCloudProjectsModel *mSourceModel = nullptr;
    ProjectsFilter mFilter = PrivateProjects;
    bool mShowLocalOnly = false;
    bool mShowInValidProjects = false;
    bool mShowFeaturedOnTop = false;
    QString mTextFilter;
};

Q_DECLARE_METATYPE( QFieldCloudProjectsFilterModel::ProjectsFilter )

#endif // QFIELDCLOUDPROJECTSMODEL_H
```


