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
      UploadAttachmentsStatusRole,
      UploadAttachmentsCountRole,
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

    //! The status of attachments uploading to server.
    enum UploadAttachmentsStatus
    {
      UploadAttachmentsDone,
      UploadAttachmentsInProgress,
    };

    Q_ENUM( UploadAttachmentsStatus )

    //! The status of the running server job for packaging a project.
    enum PackagingStatus
    {
      PackagingErrorStatus,
      PackagingUnstartedStatus,
      PackagingPendingStatus,
      PackagingBusyStatus,
      PackagingFinishedStatus,
      PackagingAbortStatus,
    };

    Q_ENUM( PackagingStatus )

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

    //! Stores the geopackage flusher, write only
    Q_PROPERTY( QgsGpkgFlusher *gpkgFlusher WRITE setGpkgFlusher NOTIFY gpkgFlusherChanged )

    //! Sets the geopackage flusher
    void setGpkgFlusher( QgsGpkgFlusher *flusher );

    //! Stores the cloud project data of the currently opened project. Empty map if missing.
    Q_PROPERTY( QVariant currentProjectData READ currentProjectData NOTIFY currentProjectDataChanged )

    //! Returns the cloud project data of the currently opened project.
    QVariantMap currentProjectData() const;

    //! Returns the cloud project data for given \a projectId.
    Q_INVOKABLE QVariantMap getProjectData( const QString &projectId ) const;

    //! Requests the cloud projects list from the server.
    Q_INVOKABLE void refreshProjectsList();

    //! Downloads a cloud project with given \a projectId and all of its files.
    Q_INVOKABLE void downloadProject( const QString &projectId, bool overwriteProject = false ); //! Downloads a cloud project with given \a projectId and all of its files.

    //! Cancels ongoing cloud project download with \a projectId.
    Q_INVOKABLE void cancelDownloadProject( const QString &projectId );

    //! Pushes all local deltas for given \a projectId. If \a shouldDownloadUpdates is true, also calls `downloadProject`.
    Q_INVOKABLE void uploadProject( const QString &projectId, const bool shouldDownloadUpdates );

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

  signals:
    void cloudConnectionChanged();
    void layerObserverChanged();
    void currentProjectIdChanged();
    void currentProjectDataChanged();
    void canSyncCurrentProjectChanged();
    void gpkgFlusherChanged();
    void warning( const QString &message );
    void projectDownloaded( const QString &projectId, const QString &projectName, const bool hasError, const QString &errorString = QString() );
    void projectStatusChanged( const QString &projectId, const ProjectStatus &projectStatus );

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

    NetworkReply *uploadAttachment( const QString &projectId, const QString &fileName );

    int findProject( const QString &projectId ) const;

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

    struct CloudProject
    {
        CloudProject( const QString &id, bool isPrivate, const QString &owner, const QString &name, const QString &description, const QString &userRole, const QString &updatedAt, const ProjectCheckouts &checkout, const ProjectStatus &status )
          : id( id )
          , isPrivate( isPrivate )
          , owner( owner )
          , name( name )
          , description( description )
          , userRole( userRole )
          , updatedAt( updatedAt )
          , status( status )
          , checkout( checkout )
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
        ProjectStatus status;
        ProjectErrorStatus errorStatus = ProjectErrorStatus::NoErrorStatus;
        ProjectCheckouts checkout;

        ProjectModifications modification = ProjectModification::NoModification;
        QString localPath;

        QString deltaFileId;
        DeltaFileStatus deltaFileUploadStatus = DeltaLocalStatus;
        QString deltaFileUploadStatusString;
        QStringList deltaLayersToDownload;

        PackagingStatus packagingStatus = PackagingUnstartedStatus;
        QString packagingStatusString;
        QStringList packagedLayerErrors;
        QMap<QString, FileTransfer> downloadFileTransfers;
        NetworkReply *apiNetworkReply = nullptr;
        int downloadFilesFinished = 0;
        int downloadFilesFailed = 0;
        int downloadBytesTotal = 0;
        int downloadBytesReceived = 0;
        double downloadProgress = 0.0; // range from 0.0 to 1.0

        UploadAttachmentsStatus uploadAttachmentsStatus = UploadAttachmentsStatus::UploadAttachmentsDone;
        QMap<QString, FileTransfer> uploadAttachments;
        int uploadAttachmentsFailed = 0;

        double uploadDeltaProgress = 0.0; // range from 0.0 to 1.0
        int deltasCount = 0;
        DeltaListModel *deltaListModel = nullptr;

        QString lastExportedAt;
        QString lastExportId;
        QString lastLocalExportedAt;
        QString lastLocalExportId;
        QString lastLocalPushDeltas;
    };

    inline QString layerFileName( const QgsMapLayer *layer ) const;

    QList<CloudProject> mCloudProjects;
    QFieldCloudConnection *mCloudConnection = nullptr;
    QString mCurrentProjectId;
    LayerObserver *mLayerObserver = nullptr;
    QgsProject *mProject = nullptr;
    QgsGpkgFlusher *mGpkgFlusher = nullptr;
    QString mUsername;
    QStringList mActiveProjectFilesToDownload;

    void projectCancelUpload( const QString &projectId );
    void projectCancelUploadAttachments( const QString &projectId );
    void projectUploadAttachments( const QString &projectId );
    void projectApplyDeltas( const QString &projectId );
    void projectGetDeltaStatus( const QString &projectId );
    void projectGetPackagingStatus( const QString &projectId );
    bool projectMoveDownloadedFilesToPermanentStorage( const QString &projectId );
    QString projectGetDir( const QString &projectId, const QString &setting );
    void projectSetSetting( const QString &projectId, const QString &setting, const QVariant &value );
    QVariant projectSetting( const QString &projectId, const QString &setting, const QVariant &defaultValue = QVariant() );

    NetworkReply *downloadFile( const QString &projectId, const QString &fileName );
    void projectDownloadFiles( const QString &projectId );
    void projectDownloadFinishedWithError( const QString &projectId, const QString &errorString );
    void updateActiveProjectFilesToDownload( const QString &projectId );

    bool canSyncProject( const QString &projectId ) const;

    bool deleteGpkgShmAndWal( const QStringList &gpkgFileNames );
    QStringList projectFileNames( const QString &projectPath, const QStringList &fileNames ) const;
    QStringList filterGpkgFileNames( const QStringList &fileNames ) const;

    QFieldCloudProjectsModel::PackagingStatus packagingStatus( const QString &status ) const;

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
