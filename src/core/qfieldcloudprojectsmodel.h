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

#include "deltalistmodel.h"
#include "qgsgpkgflusher.h"
#include "qgsnetworkaccessmanager.h"

#include <QAbstractListModel>
#include <QNetworkReply>
#include <QSortFilterProxyModel>
#include <QTimer>


class QNetworkRequest;
class QFieldCloudConnection;
class NetworkReply;
class LayerObserver;
class QgsMapLayer;
class QgsProject;


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
      ErrorStatusRole,
      ErrorStringRole,
      DownloadProgressRole,
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
      DeltaListRole,
    };

    Q_ENUM( ColumnRole )

    //! Whether the project is busy or idle.
    enum class ProjectStatus
    {
      Idle,
      Downloading,
      Uploading,
    };

    Q_ENUM( ProjectStatus )

    //! Whether the project has experienced an error.
    enum ProjectErrorStatus
    {
      NoErrorStatus,
      DownloadErrorStatus,
      UploadErrorStatus,
    };

    Q_ENUM( ProjectErrorStatus )

    //! Whether the project has been available locally and/or remotely.
    enum ProjectCheckout
    {
      RemoteCheckout = 2 << 0,
      LocalCheckout = 2 << 1,
      LocalAndRemoteCheckout = RemoteCheckout | LocalCheckout
    };

    Q_ENUM( ProjectCheckout )
    Q_DECLARE_FLAGS( ProjectCheckouts, ProjectCheckout )
    Q_FLAG( ProjectCheckouts )

    //! Whether the project has no or local and/or remote modification. Indicates wheter can be synced.
    enum ProjectModification
    {
      NoModification = 0,
      LocalModification = 2 << 0,
      RemoteModification = 2 << 1,
      LocalAndRemoteModification = RemoteModification | LocalModification
    };

    Q_ENUM( ProjectModification )
    Q_DECLARE_FLAGS( ProjectModifications, ProjectModification )
    Q_FLAG( ProjectModifications )

    //! The status of the running server job for applying deltas on a project.
    enum DeltaFileStatus
    {
      DeltaErrorStatus,
      DeltaLocalStatus,
      DeltaPendingStatus,
      DeltaBusyStatus,
      DeltaConflictStatus,
      DeltaNotAppliedStatus,
      DeltaAppliedStatus,
    };

    Q_ENUM( DeltaFileStatus )

    //! The status of the running server job for packaging a project.
    enum PackagingStatus
    {
      PackagingUnstartedStatus,
      PackagingErrorStatus,
      PackagingBusyStatus,
      PackagingFinishedStatus,
      PackagingAbortStatus,
    };

    Q_ENUM( PackagingStatus )

    //! The status of the running server job.
    enum JobStatus
    {
      JobPendingStatus,
      JobQueuedStatus,
      JobStartedStatus,
      JobFinishedStatus,
      JobStoppedStatus,
      JobFailedStatus,
    };

    Q_ENUM( JobStatus )

    //! The status of the running server job.
    enum class JobType
    {
      Package,
    };

    Q_ENUM( JobType )

    //! The reason why projectRefreshData was called
    enum class ProjectRefreshReason
    {
      Package,
    };

    Q_ENUM( ProjectRefreshReason )

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

    //! Requests the cloud projects list from the server. If \a shouldRefreshPublic is false, it will refresh only user's project, otherwise will refresh the public projects only.
    Q_INVOKABLE void refreshProjectsList( bool shouldRefreshPublic = false );

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
    Q_INVOKABLE ProjectStatus projectStatus( const QString &projectId ) const;

    //! Returns the cloud project modification for given \a projectId.
    Q_INVOKABLE ProjectModifications projectModification( const QString &projectId ) const;

    //! Updates the project modification for given \a projectId.
    Q_INVOKABLE void refreshProjectModification( const QString &projectId );

    //! Returns the model role names.
    QHash<int, QByteArray> roleNames() const override;

    //! Returns number of rows.
    int rowCount( const QModelIndex &parent ) const override;

    //! Returns the data at given \a index with given \a role.
    QVariant data( const QModelIndex &index, int role ) const override;

    //! Reloads the list of cloud projects with the given list of \a remoteProjects.
    Q_INVOKABLE void reload( const QJsonArray &remoteProjects );

    //! Downloads a cloud project with given \a projectId and all of its files.
    Q_INVOKABLE void projectPackageAndDownload( const QString &projectId );

    //! Cancels ongoing cloud project download with \a projectId.
    Q_INVOKABLE void projectCancelDownload( const QString &projectId );

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
    void projectStatusChanged( const QString &projectId, const ProjectStatus &projectStatus );
    void projectRefreshed( const QString &projectId, const ProjectRefreshReason &refreshReason, const QString &errorString = QString() );
    void projectJobFinished( const QString &projectId, const JobType jobType, const QString &errorString = QString() );
    void projectDownloadFinished( const QString &projectId, const QString &errorString = QString() );
    void deltaListModelChanged();

    //
    void networkDeltaUploaded( const QString &projectId );
    void networkDeltaStatusChecked( const QString &projectId );
    void networkAttachmentsUploaded( const QString &projectId );
    void networkAllAttachmentsUploaded( const QString &projectId );
    void networkLayerDownloaded( const QString &projectId );
    void networkAllLayersDownloaded( const QString &projectId );
    void pushFinished( const QString &projectId, bool hasError, const QString &errorString = QString() );

  private slots:
    void connectionStatusChanged();
    void projectListReceived();

    void layerObserverLayerEdited( const QString &layerId );

  private:
    static const int sDelayBeforeStatusRetry = 1000;

    struct FileTransfer
    {
        FileTransfer(
          const QString &fileName,
          const long long bytesTotal,
          NetworkReply *networkReply = nullptr,
          const QStringList &layerIds = QStringList() )
          : fileName( fileName ), bytesTotal( bytesTotal ), networkReply( networkReply ), layerIds( layerIds ) {};

        FileTransfer() = default;

        QString fileName;
        QString tmpFile;
        long long bytesTotal;
        long long bytesTransferred = 0;
        bool isFinished = false;
        NetworkReply *networkReply;
        QNetworkReply::NetworkError error = QNetworkReply::NoError;
        QStringList layerIds;
        int redirectsCount = 0;
        QUrl lastRedirectUrl;
    };

    //! Tracks the job status (status, error etc) for a particular project. For now 1 project can have only 1 job of a type.
    struct Job
    {
        Job(
          const QString &id,
          const QString &projectId,
          const JobType type )
          : id( id ), projectId( projectId ), type( type ) {};

        Job() = default;

        QString id;
        QString projectId;
        JobType type;
        JobStatus status = JobPendingStatus;
    };

    //! Tracks the cloud project information. It means it is not required CloudProject data to be already downloaded on the device.
    struct CloudProject
    {
        CloudProject(
          const QString &id,
          bool isPrivate,
          const QString &owner,
          const QString &name,
          const QString &description,
          const QString &userRole,
          const QString &updatedAt,
          const ProjectCheckouts &checkout,
          const ProjectStatus &status,
          bool canRepackage,
          bool needsRepackaging )
          : id( id )
          , isPrivate( isPrivate )
          , owner( owner )
          , name( name )
          , description( description )
          , userRole( userRole )
          , updatedAt( updatedAt )
          , checkout( checkout )
          , status( status )
          , canRepackage( canRepackage )
          , needsRepackaging( needsRepackaging )
        {}

        CloudProject() = default;
        ~CloudProject() { delete deltaListModel; }

        QString id;
        bool isPrivate = true;
        QString owner;
        QString name;
        QString description;
        QString userRole;
        QString updatedAt;
        ProjectErrorStatus errorStatus = ProjectErrorStatus::NoErrorStatus;
        ProjectCheckouts checkout;
        ProjectStatus status;
        bool canRepackage = false;
        bool needsRepackaging = false;

        ProjectModifications modification = ProjectModification::NoModification;
        QString localPath;

        QString deltaFileId;
        DeltaFileStatus deltaFileUploadStatus = DeltaLocalStatus;
        QString deltaFileUploadStatusString;
        QStringList deltaLayersToDownload;

        bool isPackagingActive = false;
        bool isPackagingFailed = false;
        PackagingStatus packagingStatus = PackagingUnstartedStatus;
        QString packagingStatusString;
        QStringList packagedLayerErrors;
        QMap<QString, FileTransfer> downloadFileTransfers;
        int downloadFilesFinished = 0;
        int downloadFilesFailed = 0;
        int downloadBytesTotal = 0;
        int downloadBytesReceived = 0;
        double downloadProgress = 0.0; // range from 0.0 to 1.0

        double uploadDeltaProgress = 0.0; // range from 0.0 to 1.0
        int deltasCount = 0;
        DeltaListModel *deltaListModel = nullptr;

        QString lastExportedAt;
        QString lastExportId;
        QString lastLocalExportedAt;
        QString lastLocalExportId;
        QString lastLocalPushDeltas;

        QDateTime lastRefreshedAt;
        QMap<JobType, Job> jobs;
    };

    inline QString layerFileName( const QgsMapLayer *layer ) const;

    QList<CloudProject *> mProjects;
    QFieldCloudConnection *mCloudConnection = nullptr;
    QString mCurrentProjectId;
    LayerObserver *mLayerObserver = nullptr;
    QgsProject *mProject = nullptr;
    QgsGpkgFlusher *mGpkgFlusher = nullptr;
    QString mUsername;
    QStringList mActiveProjectFilesToDownload;

    QModelIndex findProjectIndex( const QString &projectId ) const;
    CloudProject *findProject( const QString &projectId ) const;
    void projectCancelUpload( const QString &projectId );
    void projectGetDeltaStatus( const QString &projectId );
    bool projectMoveDownloadedFilesToPermanentStorage( const QString &projectId );
    void projectRefreshData( const QString &projectId, const ProjectRefreshReason &refreshReason );
    void projectStartJob( const QString &projectId, const JobType jobType );
    void projectGetJobStatus( const QString &projectId, const JobType jobType );
    void projectDownload( const QString &projectId );

    QString projectGetDir( const QString &projectId, const QString &setting );
    void projectSetSetting( const QString &projectId, const QString &setting, const QVariant &value );
    QVariant projectSetting( const QString &projectId, const QString &setting, const QVariant &defaultValue = QVariant() );

    NetworkReply *downloadFile( const QString &projectId, const QString &fileName );
    void projectDownloadFiles( const QString &projectId );
    void updateActiveProjectFilesToDownload( const QString &projectId );

    bool canSyncProject( const QString &projectId ) const;

    bool deleteGpkgShmAndWal( const QStringList &gpkgFileNames );
    QStringList projectFileNames( const QString &projectPath, const QStringList &fileNames ) const;
    QStringList filterGpkgFileNames( const QStringList &fileNames ) const;

    QFieldCloudProjectsModel::JobStatus getJobStatusFromString( const QString &status ) const;
    QString getJobTypeAsString( JobType jobType ) const;

    void downloadFileConnections( const QString &projectId, const QString &fileName );
};

Q_DECLARE_METATYPE( QFieldCloudProjectsModel::ProjectStatus )
Q_DECLARE_METATYPE( QFieldCloudProjectsModel::ProjectCheckout )
Q_DECLARE_METATYPE( QFieldCloudProjectsModel::ProjectCheckouts )
Q_DECLARE_OPERATORS_FOR_FLAGS( QFieldCloudProjectsModel::ProjectCheckouts )
Q_DECLARE_METATYPE( QFieldCloudProjectsModel::ProjectModification )
Q_DECLARE_METATYPE( QFieldCloudProjectsModel::ProjectModifications )
Q_DECLARE_OPERATORS_FOR_FLAGS( QFieldCloudProjectsModel::ProjectModifications )


class QFieldCloudProjectsFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY( QFieldCloudProjectsModel *projectsModel READ projectsModel WRITE setProjectsModel NOTIFY projectsModelChanged )
    Q_PROPERTY( ProjectsFilter filter READ filter WRITE setFilter NOTIFY filterChanged )
    Q_PROPERTY( bool showLocalOnly READ showLocalOnly WRITE setShowLocalOnly NOTIFY showLocalOnlyChanged )

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
     * Sets the the cloud project \a filter.
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

  signals:

    void projectsModelChanged();
    void filterChanged();
    void showLocalOnlyChanged();

  protected:
    virtual bool filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const override;

  private:
    QFieldCloudProjectsModel *mSourceModel = nullptr;
    ProjectsFilter mFilter = PrivateProjects;
    bool mShowLocalOnly = false;
};

Q_DECLARE_METATYPE( QFieldCloudProjectsFilterModel::ProjectsFilter )

#endif // QFIELDCLOUDPROJECTSMODEL_H
