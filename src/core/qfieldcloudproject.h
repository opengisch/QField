/***************************************************************************
    qfieldcloudproject.h
    ---------------------
    begin                : April 2025
    copyright            : (C) 2025 by Mathieu Pellerin
    email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFIELDCLOUDPROJECT_H
#define QFIELDCLOUDPROJECT_H

#include "networkmanager.h"
#include "networkreply.h"

#include <QObject>
#include <QPointer>
#include <QVariantMap>

class DeltaListModel;
class LayerObserver;
class QFieldCloudConnection;

/**
 * \ingroup core
 */
class QFieldCloudProject : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString id READ id NOTIFY idChanged )
    Q_PROPERTY( QString name READ name NOTIFY nameChanged )
    Q_PROPERTY( QString owner READ owner NOTIFY ownerChanged )
    Q_PROPERTY( QString description READ description NOTIFY descriptionChanged )

    Q_PROPERTY( ProjectStatus status READ status NOTIFY statusChanged )
    Q_PROPERTY( PackagingStatus packagingStatus READ packagingStatus NOTIFY packagingStatusChanged )

    Q_PROPERTY( int downloadBytesTotal READ downloadBytesTotal NOTIFY downloadBytesTotalChanged )
    Q_PROPERTY( int downloadBytesReceived READ downloadBytesReceived NOTIFY downloadBytesReceivedChanged )
    Q_PROPERTY( double downloadProgress READ downloadProgress NOTIFY downloadProgressChanged )

    Q_PROPERTY( double uploadDeltaProgress READ uploadDeltaProgress NOTIFY uploadDeltaProgressChanged )
    Q_PROPERTY( DeltaFileStatus deltaFileUploadStatus READ deltaFileUploadStatus NOTIFY deltaFileUploadStatusChanged )
    Q_PROPERTY( DeltaListModel *deltaListModel READ deltaListModel NOTIFY deltaListModelChanged )

    Q_PROPERTY( bool forceAutoPush READ forceAutoPush NOTIFY forceAutoPushChanged )
    Q_PROPERTY( bool autoPushEnabled READ autoPushEnabled WRITE setAutoPushEnabled NOTIFY autoPushEnabledChanged )
    Q_PROPERTY( int autoPushIntervalMins READ autoPushIntervalMins NOTIFY autoPushIntervalMinsChanged )

    Q_PROPERTY( QString lastLocalPushDeltas READ lastLocalPushDeltas NOTIFY lastLocalPushDeltasChanged )
    Q_PROPERTY( QString lastLocalExportedAt READ lastLocalExportedAt NOTIFY lastLocalExportedAtChanged )

    Q_PROPERTY( bool isOutdated READ isOutdated NOTIFY isOutdatedChanged )
    Q_PROPERTY( bool projectFileIsOutdated READ projectFileIsOutdated NOTIFY projectFileIsOutdatedChanged )

  public:
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
      DeltaUploaded
    };

    Q_ENUM( ProjectRefreshReason )

    QFieldCloudProject( const QString &id = QString(), QFieldCloudConnection *connection = nullptr );
    ~QFieldCloudProject();

    QString id() const { return mId; }
    bool isPrivate() const { return mIsPrivate; }
    QString owner() const { return mOwner; }
    QString name() const { return mName; }
    QString description() const { return mDescription; }
    QString userRole() const { return mUserRole; }
    QString userRoleOrigin() const { return mUserRoleOrigin; }

    ProjectErrorStatus errorStatus() const { return mErrorStatus; }
    ProjectCheckouts checkout() const { return mCheckout; }
    ProjectStatus status() const { return mStatus; }
    QDateTime dataLastUpdatedAt() const { return mDataLastUpdatedAt; }

    bool canRepackage() const { return mCanRepackage; }
    bool needsRepackaging() const { return mNeedsRepackaging; }
    bool isOutdated() const { return mIsOutdated; }
    bool projectFileIsOutdated() const { return mProjectFileIsOutdated; }

    ProjectModifications modification() const { return mModification; }
    QString localPath() const { return mLocalPath; }

    QString deltaFileId() const { return mDeltaFileId; }
    DeltaFileStatus deltaFileUploadStatus() const { return mDeltaFileUploadStatus; }
    QString deltaFileUploadStatusString() const { return mDeltaFileUploadStatusString; }
    QStringList deltaLayersToDownload() const { return mDeltaLayersToDownload; }

    bool isPackagingActive() const { return mIsPackagingActive; }
    bool isPackagingFailed() const { return mIsPackagingFailed; }
    PackagingStatus packagingStatus() const { return mPackagingStatus; }
    QString packagingStatusString() const { return mPackagingStatusString; }
    QStringList packagedLayerErrors() const { return mPackagedLayerErrors; }

    bool forceAutoPush() const { return mForceAutoPush; }
    void setForceAutoPush( bool force );
    bool autoPushEnabled() const { return mAutoPushEnabled; }
    void setAutoPushEnabled( bool enabled );
    int autoPushIntervalMins() const { return mAutoPushIntervalMins; }
    void setAutoPushIntervalMins( int minutes );

    QString lastLocalPushDeltas() const { return mLastLocalPushDeltas; }
    QString lastLocalExportedAt() const { return mLastLocalExportId; }
    QString lastLocalExportId() const { return mLastLocalExportId; }
    QDateTime lastLocalDataLastUpdatedAt() const { return mLastLocalDataLastUpdatedAt; }
    QDateTime lastRefreshedAt() const { return mLastRefreshedAt; }

    int downloadFilesFinished() const { return mDownloadFilesFinished; }
    int downloadFilesFailed() const { return mDownloadFilesFailed; }
    int downloadBytesTotal() const { return mDownloadBytesTotal; }
    int downloadBytesReceived() const { return mDownloadBytesReceived; }
    double downloadProgress() const { return mDownloadProgress; }


    double uploadDeltaProgress() const { return mUploadDeltaProgress; }
    int deltasCount() const { return mDeltasCount; }
    DeltaListModel *deltaListModel() const { return mDeltaListModel; }

    // ----------

    void packageAndDownload();
    void cancelDownload();

    void upload( LayerObserver *layerObserver, bool shouldDownloadUpdates );
    void cancelUpload();

    void refreshDeltaList();
    void refreshFileOutdatedStatus();
    void refreshModification( LayerObserver *layerObserver );

    void removeLocally();

    // ----------

    static QFieldCloudProject::JobStatus getJobStatusFromString( const QString &status );
    static QString getJobTypeAsString( QFieldCloudProject::JobType jobType );

    static QFieldCloudProject *fromDetails( const QVariantHash &details, QFieldCloudConnection *connection );
    static QFieldCloudProject *fromLocalSettings( const QString &id, QFieldCloudConnection *connection );
    static void restoreLocalSettings( QFieldCloudProject *project, const QDir &localPath );

  signals:
    void idChanged();
    void isPrivateChanged();
    void ownerChanged();
    void nameChanged();
    void descriptionChanged();
    void userRoleChanged();
    void userRoleOriginChanged();

    void errorStatusChanged();
    void checkoutChanged();
    void statusChanged();
    void dataLastUpdatedAtChanged();

    void canRepackageChanged();
    void needsRepackagingChanged();
    void isOutdatedChanged();
    void projectFileIsOutdatedChanged();

    void modificationChanged();
    void localPathChanged();

    void deltaFileIdChanged();
    void deltaFileUploadStatusChanged();
    void deltaFileUploadStatusStringChanged();
    void deltaLayersToDownloadChanged();

    void isPackagingActiveChanged();
    void isPackagingFailedChanged();
    void packagingStatusChanged();
    void packagingStatusStringChanged();
    void packagedLayerErrorsChanged();

    void forceAutoPushChanged();
    void autoPushEnabledChanged();
    void autoPushIntervalMinsChanged();

    void lastLocalExportedAtChanged();
    void lastLocalExportedIdChanged();
    void lastDataLastUpdatedAtChanged();

    void lastLocalDataLastUpdatedAtChanged();
    void lastRefreshedAtChanged();
    void lastLocalPushDeltasChanged();

    void downloadFilesFinishedChanged();
    void downloadFilesFailedChanged();
    void downloadBytesTotalChanged();
    void downloadBytesReceivedChanged();
    void downloadProgressChanged();

    void uploadDeltaProgressChanged();

    void deltaListModelChanged();

    // ---

    void downloadFinished( QString error = QString() );
    void downloaded( const QString &name, QString error = QString() );

    void uploadFinished( bool isDownloading, QString error = QString() );

    void networkDeltaUploaded();
    void networkDeltaStatusChecked();

    void dataRefreshed( ProjectRefreshReason reason, QString error = QString() );
    void jobFinished( JobType type, QString error = QString() );

  private:
    void download();
    void downloadFiles();
    void updateActiveFilesToDownload();

    void startJob( JobType type );

    void getJobStatus( JobType type );
    void getDeltaStatus();

    void refreshData( ProjectRefreshReason reason );

    NetworkReply *downloadFile( const QString &projectId, const QString &fileName, bool fromLatestPackage = true );
    void downloadFileConnections( const QString &fileKey );

    bool moveDownloadedFilesToPermanentStorage();
    void logFailedDownload( const QString &fileKey, const QString &errorMessage, const QString &errorMessageDetail );

    struct FileTransfer
    {
        FileTransfer(
          const QString &fileName,
          const long long bytesTotal,
          const QString &projectId )
          : fileName( fileName ), bytesTotal( bytesTotal ), projectId( projectId ) {};

        FileTransfer() = default;

        QString fileName;
        long long bytesTotal;
        QString projectId;

        QString tmpFile;
        long long bytesTransferred = 0;
        bool isFinished = false;
        QPointer<NetworkReply> networkReply;
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
          const QFieldCloudProject::JobType type )
          : id( id ), projectId( projectId ), type( type ) {};

        Job() = default;

        QString id;
        QString projectId;
        QFieldCloudProject::JobType type;
        QFieldCloudProject::JobStatus status = QFieldCloudProject::JobPendingStatus;
    };

    QString mId;

    bool mIsPrivate = true;
    QString mOwner;
    QString mName;
    QString mDescription;
    QString mUserRole;
    QString mUserRoleOrigin;
    ProjectErrorStatus mErrorStatus = ProjectErrorStatus::NoErrorStatus;
    ProjectCheckouts mCheckout;
    ProjectStatus mStatus;
    QDateTime mDataLastUpdatedAt;
    bool mCanRepackage = false;
    bool mNeedsRepackaging = false;
    bool mIsOutdated = false;
    bool mProjectFileIsOutdated = false;
    ProjectModifications mModification = ProjectModification::NoModification;
    QString mLocalPath;

    QString mDeltaFileId;
    DeltaFileStatus mDeltaFileUploadStatus = DeltaLocalStatus;
    QString mDeltaFileUploadStatusString;
    QStringList mDeltaLayersToDownload;

    bool mIsPackagingActive = false;
    bool mIsPackagingFailed = false;
    PackagingStatus mPackagingStatus = PackagingUnstartedStatus;
    QString mPackagingStatusString;
    QStringList mPackagedLayerErrors;

    QStringList mActiveFilesToDownload;
    QMap<QString, FileTransfer> mDownloadFileTransfers;
    int mDownloadFilesFinished = 0;
    int mDownloadFilesFailed = 0;
    int mDownloadBytesTotal = 0;
    int mDownloadBytesReceived = 0;
    double mDownloadProgress = 0.0; // range from 0.0 to 1.0

    double mUploadDeltaProgress = 0.0; // range from 0.0 to 1.0
    int mDeltasCount = 0;
    DeltaListModel *mDeltaListModel = nullptr;

    QString mLastExportedAt;
    QString mLastExportId;
    QString mLastLocalExportedAt;
    QString mLastLocalExportId;
    QString mLastLocalPushDeltas;

    QDateTime mLastLocalDataLastUpdatedAt;
    QDateTime mLastRefreshedAt;

    bool mForceAutoPush = false;
    bool mAutoPushEnabled = false;
    int mAutoPushIntervalMins = 30;

    QMap<JobType, Job> mJobs;

    QFieldCloudConnection *mCloudConnection = nullptr;
    QString mUsername;

    static const int sDelayBeforeStatusRetry = 1000;
};

Q_DECLARE_METATYPE( QFieldCloudProject::ProjectStatus )
Q_DECLARE_METATYPE( QFieldCloudProject::ProjectCheckout )
Q_DECLARE_METATYPE( QFieldCloudProject::ProjectCheckouts )
Q_DECLARE_OPERATORS_FOR_FLAGS( QFieldCloudProject::ProjectCheckouts )
Q_DECLARE_METATYPE( QFieldCloudProject::ProjectModification )
Q_DECLARE_METATYPE( QFieldCloudProject::ProjectModifications )
Q_DECLARE_OPERATORS_FOR_FLAGS( QFieldCloudProject::ProjectModifications )

#endif // QFIELDCLOUDPROJECT_H
