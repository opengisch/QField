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
class QgsProject;

/**
 * \ingroup core
 */
class QFieldCloudProjectsModel : public QAbstractListModel
{
    Q_OBJECT

  public:
    enum ColumnRole
    {
      IdRole = Qt::UserRole + 1,
      OwnerRole,
      PrivateRole,
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
      UploadDeltaProgressRole,
      UploadDeltaStatusRole,
      UploadDeltaStatusStringRole,
      LocalDeltasCountRole,
      LocalPathRole,
      CanSyncRole,
      LastLocalExportedAtRole,
      LastLocalPushDeltasRole,
      UserRoleRole,
      UserRoleOriginRole,
      DeltaListRole,
      ForceAutoPushRole,
      AutoPushEnabledRole,
      AutoPushIntervalMinsRole,
    };

    //! Attributes controlling fetching of projects
    enum class ProjectsRequestAttribute
    {
      RefreshPublicProjects = QNetworkRequest::User + 1,
      ProjectsFetchOffset = QNetworkRequest::User + 2
    };

    Q_ENUM( ColumnRole )

    QFieldCloudProjectsModel();

    //! Stores the current cloud connection.
    Q_PROPERTY( QFieldCloudConnection *cloudConnection READ cloudConnection WRITE setCloudConnection NOTIFY cloudConnectionChanged )

    //! Returns the currently used cloud connection.
    QFieldCloudConnection *cloudConnection() const;

    //! Sets the cloud connection.
    void setCloudConnection( QFieldCloudConnection *cloudConnection );

    //! Stores the current layer observer.
    Q_PROPERTY( LayerObserver *layerObserver READ layerObserver WRITE setLayerObserver NOTIFY layerObserverChanged )

    //! Returns the currently used layer observer.
    LayerObserver *layerObserver() const;

    //! Sets the layer observer.
    void setLayerObserver( LayerObserver *layerObserver );

    //! Stores the cloud project id of the currently opened project. Empty string if missing.
    Q_PROPERTY( QString currentProjectId READ currentProjectId WRITE setCurrentProjectId NOTIFY currentProjectIdChanged )

    //! Returns the cloud project id of the currently opened project.
    QString currentProjectId() const;

    //! Sets the cloud project id of the currently opened project.
    void setCurrentProjectId( const QString &currentProjectId );

    //! Stores the geopackage flusher
    Q_PROPERTY( QgsGpkgFlusher *gpkgFlusher READ gpkgFlusher WRITE setGpkgFlusher NOTIFY gpkgFlusherChanged )

    //! Returns the geopackage flusher
    QgsGpkgFlusher *gpkgFlusher() const { return mGpkgFlusher; }

    //! Sets the geopackage flusher
    void setGpkgFlusher( QgsGpkgFlusher *flusher );

    //! Stores the cloud project data of the currently opened project. Empty map if missing.
    Q_PROPERTY( QVariant currentProjectData READ currentProjectData NOTIFY currentProjectDataChanged )

    //! Returns the cloud project data of the currently opened project.
    QVariantMap currentProjectData() const;

    //! Stores a set of the currently busy project ids.
    Q_PROPERTY( QSet<QString> busyProjectIds READ busyProjectIds NOTIFY busyProjectIdsChanged )

    //! Returns a set containing the currently busy project ids.
    QSet<QString> busyProjectIds() const;

    //! Returns the cloud project data for given \a projectId.
    Q_INVOKABLE QVariantMap getProjectData( const QString &projectId ) const;

    //! Requests the cloud projects list from the server. If \a shouldRefreshPublic is false, it will refresh only user's project, otherwise will refresh the public projects only, starting from \a projectFetchOffset for pagination.
    Q_INVOKABLE void refreshProjectsList( bool shouldRefreshPublic = false, int projectFetchOffset = 0 );

    //! Pushes all local deltas for given \a projectId. If \a shouldDownloadUpdates is true, also calls `downloadProject`.
    Q_INVOKABLE void projectUpload( const QString &projectId, const bool shouldDownloadUpdates );

    //! Retreives the delta list for a given \a projectId.
    Q_INVOKABLE void refreshProjectDeltaList( const QString &projectId );

    //! Remove local cloud project with given \a projectId from the device storage
    Q_INVOKABLE void removeLocalProject( const QString &projectId );

    //! Reverts the deltas of the current cloud project. The changes would applied in reverse order and opposite methods, e.g. "delete" becomes "create".
    Q_INVOKABLE bool revertLocalChangesFromCurrentProject();

    //! Discards the delta records of the current cloud project.
    Q_INVOKABLE bool discardLocalChangesFromCurrentProject();

    //! Returns the cloud project status for given \a projectId.
    Q_INVOKABLE QFieldCloudProject::ProjectStatus projectStatus( const QString &projectId ) const;

    //! Returns the cloud project modification for given \a projectId.
    Q_INVOKABLE QFieldCloudProject::ProjectModifications projectModification( const QString &projectId ) const;

    //! Updates the project modification for given \a projectId.
    Q_INVOKABLE void refreshProjectModification( const QString &projectId );

    //! Refreshes the project file (.qgs, .qgz) outdated status.
    Q_INVOKABLE void refreshProjectFileOutdatedStatus( const QString &projectId );

    //! Returns the model role names.
    QHash<int, QByteArray> roleNames() const override;

    //! Returns number of rows.
    int rowCount( const QModelIndex &parent ) const override;

    //! Returns the data at given \a index with given \a role.
    QVariant data( const QModelIndex &index, int role ) const override;

    //! Downloads a cloud project with given \a projectId and all of its files.
    Q_INVOKABLE void projectPackageAndDownload( const QString &projectId );

    //! Cancels ongoing cloud project download with \a projectId.
    Q_INVOKABLE void projectCancelDownload( const QString &projectId );

    //! Forces the cloud project auto-push enabled state to be TRUE
    Q_INVOKABLE void projectSetForceAutoPush( const QString &projectId, bool force );

    //! Toggles the cloud project auto-push enabled state
    Q_INVOKABLE void projectSetAutoPushEnabled( const QString &projectId, bool enabled );

    //! Sets the interval in \a minutes between which the project will auto-push changes
    Q_INVOKABLE void projectSetAutoPushIntervalMins( const QString &projectId, int minutes );

    //! Configure localized data paths for cloud projects when available
    Q_INVOKABLE void updateLocalizedDataPaths( const QString &projectPath );

  signals:
    void cloudConnectionChanged();
    void layerObserverChanged();
    void currentProjectIdChanged();
    void currentProjectDataChanged();
    void busyProjectIdsChanged();
    void canSyncCurrentProjectChanged();
    void gpkgFlusherChanged();
    void warning( const QString &message );

    void projectDownloaded( const QString &projectId, const QString &projectName, const bool hasError, const QString &errorString = QString() );
    void pushFinished( const QString &projectId, bool isDownloadingProject, bool hasError, const QString &errorString = QString() );

    void deltaListModelChanged();

  private slots:
    void connectionStatusChanged();
    void usernameChanged();
    void projectListReceived();

    void layerObserverLayerEdited( const QString &layerId );

  private:
    void setupProjectConnections( QFieldCloudProject *project );

    inline QString layerFileName( const QgsMapLayer *layer ) const;

    QList<QFieldCloudProject *> mProjects;
    QFieldCloudConnection *mCloudConnection = nullptr;
    QString mCurrentProjectId;
    LayerObserver *mLayerObserver = nullptr;
    QgsProject *mProject = nullptr;
    QgsGpkgFlusher *mGpkgFlusher = nullptr;
    QString mUsername;
    const int mProjectsPerFetch = 250;
    QMap<QString, QString> mLocalizedDatasetsProjects;

    QModelIndex findProjectIndex( const QString &projectId ) const;
    QFieldCloudProject *findProject( const QString &projectId ) const;

    bool canSyncProject( const QString &projectId ) const;

    bool deleteGpkgShmAndWal( const QStringList &gpkgFileNames );

    void loadProjects( const QJsonArray &remoteProjects = QJsonArray(), bool skipLocalProjects = false );
    void insertProjects( const QList<QFieldCloudProject *> &projects );
};

/**
 * \ingroup core
 */
class QFieldCloudProjectsFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY( QFieldCloudProjectsModel *projectsModel READ projectsModel WRITE setProjectsModel NOTIFY projectsModelChanged )
    Q_PROPERTY( ProjectsFilter filter READ filter WRITE setFilter NOTIFY filterChanged )
    Q_PROPERTY( bool showLocalOnly READ showLocalOnly WRITE setShowLocalOnly NOTIFY showLocalOnlyChanged )
    Q_PROPERTY( QString textFilter READ textFilter WRITE setTextFilter NOTIFY textFilterChanged )

  public:
    enum ProjectsFilter
    {
      PrivateProjects,
      PublicProjects,
    };
    Q_ENUM( ProjectsFilter )

    explicit QFieldCloudProjectsFilterModel( QObject *parent = nullptr );

    /**
     * Returns the source cloud projects model from which the filtered list is derived.
     */
    QFieldCloudProjectsModel *projectsModel() const;

    /**
     * Sets the source cloud projects model from which the filtered list is derived.
     * \param projectsModel the source cloud project model
     */
    void setProjectsModel( QFieldCloudProjectsModel *projectsModel );

    /**
     * Returns the current cloud projects filter.
     */
    ProjectsFilter filter() const;

    /**
     * Sets the cloud project \a filter.
     */
    void setFilter( ProjectsFilter filter );

    /**
     * Returns whether the filtered cloud projects list will only contain those available locally.
     */
    bool showLocalOnly() const;

    /**
     * Sets whether the filtered cloud projects list will only contain those available locally.
     */
    void setShowLocalOnly( bool showLocalOnly );

    /**
     * Sets a \a text string filter projects by matching it against the project's name or description
     * as well well as owner's name.
     */
    void setTextFilter( const QString &text );

    /**
     * Returns the current text string used to filter projects.
     */
    QString textFilter() const;

  signals:

    void projectsModelChanged();
    void filterChanged();
    void showLocalOnlyChanged();
    void textFilterChanged();

  protected:
    virtual bool filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const override;

  private:
    QFieldCloudProjectsModel *mSourceModel = nullptr;
    ProjectsFilter mFilter = PrivateProjects;
    bool mShowLocalOnly = false;
    QString mTextFilter;
};

Q_DECLARE_METATYPE( QFieldCloudProjectsFilterModel::ProjectsFilter )

#endif // QFIELDCLOUDPROJECTSMODEL_H
