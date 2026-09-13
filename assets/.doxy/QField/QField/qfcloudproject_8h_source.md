

# File qfcloudproject.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldcloud**](dir_d27ac98b23548f5810403c734ce9817b.md) **>** [**qfcloudproject.h**](qfcloudproject_8h.md)

[Go to the documentation of this file](qfcloudproject_8h.md)


```C++
/***************************************************************************
    qfcloudproject.h
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

#ifndef QFCLOUDPROJECT_H
#define QFCLOUDPROJECT_H

#include "qfdeltafilewrapper.h"
#include "qfnetworkmanager.h"
#include "qfnetworkreply.h"

#include <QObject>
#include <QPointer>
#include <QVariantMap>

class QgsGpkgFlusher;
class QfCloudConnection;

class QfCloudProject : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString id READ id NOTIFY idChanged )
    Q_PROPERTY( QString name READ name NOTIFY nameChanged )
    Q_PROPERTY( QString owner READ owner NOTIFY ownerChanged )
    Q_PROPERTY( QString description READ description NOTIFY descriptionChanged )
    Q_PROPERTY( QString localPath READ localPath NOTIFY localPathChanged )
    Q_PROPERTY( QString thumbnailPath READ thumbnailPath NOTIFY thumbnailPathChanged )

    Q_PROPERTY( QString userRole READ userRole NOTIFY userRoleChanged )
    Q_PROPERTY( QString userRoleOrigin READ userRoleOrigin NOTIFY userRoleOriginChanged )

    Q_PROPERTY( QDateTime createdAt READ createdAt NOTIFY createdAtChanged )
    Q_PROPERTY( QDateTime updatedAt READ updatedAt NOTIFY updatedAtChanged )
    Q_PROPERTY( qint64 remoteSizeBytes READ remoteSizeBytes NOTIFY remoteSizeBytesChanged )
    Q_PROPERTY( QDateTime dataLastUpdatedAt READ dataLastUpdatedAt NOTIFY dataLastUpdatedAtChanged )

    Q_PROPERTY( ProjectStatus status READ status NOTIFY statusChanged )
    Q_PROPERTY( PackagingStatus packagingStatus READ packagingStatus NOTIFY packagingStatusChanged )
    Q_PROPERTY( QStringList packagedLayerErrors READ packagedLayerErrors NOTIFY packagedLayerErrorsChanged )

    Q_PROPERTY( qint64 downloadBytesTotal READ downloadBytesTotal NOTIFY downloadBytesTotalChanged )
    Q_PROPERTY( qint64 downloadBytesReceived READ downloadBytesReceived NOTIFY downloadBytesReceivedChanged )
    Q_PROPERTY( double downloadProgress READ downloadProgress NOTIFY downloadProgressChanged )

    Q_PROPERTY( double pushDeltaProgress READ pushDeltaProgress NOTIFY pushDeltaProgressChanged )
    Q_PROPERTY( DeltaFileStatus deltaFilePushStatus READ deltaFilePushStatus NOTIFY deltaFilePushStatusChanged )
    Q_PROPERTY( int deltasCount READ deltasCount NOTIFY deltasCountChanged )
    Q_PROPERTY( QfDeltaFileWrapper *deltaFileWrapper READ deltaFileWrapper NOTIFY deltaFileWrapperChanged )

    Q_PROPERTY( qint64 uploadBytesTotal READ uploadBytesTotal NOTIFY uploadBytesTotalChanged )
    Q_PROPERTY( qint64 uploadBytesSent READ uploadBytesSent NOTIFY uploadBytesSentChanged )
    Q_PROPERTY( double uploadProgress READ uploadProgress NOTIFY uploadProgressChanged )

    Q_PROPERTY( bool forceAutoPush READ forceAutoPush WRITE setForceAutoPush NOTIFY forceAutoPushChanged )
    Q_PROPERTY( bool autoPushEnabled READ autoPushEnabled WRITE setAutoPushEnabled NOTIFY autoPushEnabledChanged )
    Q_PROPERTY( int autoPushIntervalMins READ autoPushIntervalMins WRITE setAutoPushIntervalMins NOTIFY autoPushIntervalMinsChanged )

    Q_PROPERTY( bool attachmentsOnDemandEnabled READ attachmentsOnDemandEnabled WRITE setAttachmentsOnDemandEnabled NOTIFY attachmentsOnDemandEnabledChanged )

    Q_PROPERTY( QDateTime lastLocalPushDeltas READ lastLocalPushDeltas NOTIFY lastLocalPushDeltasChanged )
    Q_PROPERTY( QDateTime lastLocalExportedAt READ lastLocalExportedAt NOTIFY lastLocalExportedAtChanged )

    Q_PROPERTY( ProjectType type READ type NOTIFY typeChanged )
    Q_PROPERTY( bool isPublic READ isPublic NOTIFY isPublicChanged )
    Q_PROPERTY( bool isFeatured READ isFeatured NOTIFY isFeaturedChanged )
    Q_PROPERTY( bool isOutdated READ isOutdated NOTIFY isOutdatedChanged )
    Q_PROPERTY( bool isProjectOutdated READ isProjectOutdated NOTIFY isProjectOutdatedChanged )

  public:
    struct FileTransfer
    {
        FileTransfer(
          const QString &fileName,
          const qint64 bytesTotal,
          const QString &projectId,
          const QString &etag )
          : fileName( fileName ), bytesTotal( bytesTotal ), projectId( projectId ), etag( etag ) {};

        FileTransfer() = default;

        QString fileName;
        qint64 bytesTotal = 0;
        QString projectId;

        QString etag;
        QString partialFilePath;
        QString tmpFile;
        qint64 bytesTransferred = 0;
        bool isFinished = false;
        QPointer<QfNetworkReply> networkReply;
        QNetworkReply::NetworkError error = QNetworkReply::NoError;
        int redirectsCount = 0;
        QUrl lastRedirectUrl;
        bool resumableDownload = true;
        int retryCount = 0;
    };

    enum class ProjectStatus
    {
      Idle,
      Creating,
      Downloading,
      Pushing,
      Uploading,
      Failing
    };

    Q_ENUM( ProjectStatus )

    
    enum class ProjectType
    {
      Regular,
      SharedDatasets,
      Template,
    };

    Q_ENUM( ProjectType )

    
    enum ProjectErrorStatus
    {
      NoErrorStatus,
      DownloadErrorStatus,
      PushErrorStatus,
    };

    Q_ENUM( ProjectErrorStatus )

    
    enum ProjectCheckout
    {
      RemoteCheckout = 2 << 0,
      LocalCheckout = 2 << 1,
      LocalAndRemoteCheckout = RemoteCheckout | LocalCheckout
    };

    Q_ENUM( ProjectCheckout )
    Q_DECLARE_FLAGS( ProjectCheckouts, ProjectCheckout )
    Q_FLAG( ProjectCheckouts )

    
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

    
    enum PackagingStatus
    {
      PackagingUnstartedStatus,
      PackagingErrorStatus,
      PackagingBusyStatus,
      PackagingFinishedStatus,
      PackagingAbortStatus,
    };

    Q_ENUM( PackagingStatus )

    
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

    
    enum class JobType
    {
      Package,
    };

    Q_ENUM( JobType )

    
    enum class ProjectRefreshReason
    {
      Package,
      DeltaPushed
    };

    Q_ENUM( ProjectRefreshReason )

    QfCloudProject( const QString &id = QString(), QfCloudConnection *connection = nullptr, QgsGpkgFlusher *gpkgFlusher = nullptr );
    ~QfCloudProject() = default;

    QString id() const { return mId; }

    QString sharedDatasetsProjectId() const { return mSharedDatasetsProjectId; }
    void setSharedDatasetsProjectId( const QString &id );

    ProjectType type() const { return mType; }
    void setType( ProjectType projectType );

    static QfCloudProject::ProjectType typeFromString( const QString &type );

    bool isPublic() const { return mIsPublic; }
    void setIsPublic( bool isPublic );

    bool isFeatured() const { return mIsFeatured; }
    void setIsFeatured( bool isFeatured );

    QString owner() const { return mOwner; }
    void setOwner( const QString &owner );

    QString name() const { return mName; }
    void setName( const QString &name );

    QString description() const { return mDescription; }
    void setDescription( const QString &description );

    QString userRole() const { return mUserRole; }
    void setUserRole( const QString &userRole );

    QString userRoleOrigin() const { return mUserRoleOrigin; }
    void setUserRoleOrigin( const QString &userRoleOrigin );

    ProjectErrorStatus errorStatus() const { return mErrorStatus; }
    void setErrorStatus( ProjectErrorStatus errorStatus );

    ProjectCheckouts checkout() const { return mCheckout; }
    void setCheckout( ProjectCheckouts checkout );

    ProjectStatus status() const { return mStatus; }
    void setStatus( ProjectStatus status );

    QDateTime createdAt() const { return mCreatedAt; }
    void setCreatedAt( const QDateTime &createdAt );

    QDateTime updatedAt() const { return mUpdatedAt; }
    void setUpdatedAt( const QDateTime &updatedAt );

    qint64 remoteSizeBytes() const { return mRemoteSizeBytes; }
    void setRemoteSizeBytes( qint64 remoteSizeBytes );

    QDateTime dataLastUpdatedAt() const { return mDataLastUpdatedAt; }
    void setDataLastUpdatedAt( const QDateTime &dataLastUpdatedAt );

    QDateTime restrictedDataLastUpdatedAt() const { return mRestrictedDataLastUpdatedAt; }
    void setRestrictedDataLastUpdatedAt( const QDateTime &restrictedDataLastUpdatedAt );

    bool canRepackage() const { return mCanRepackage; }
    void setCanRepackage( bool canRepackage );

    bool needsRepackaging() const { return mNeedsRepackaging; }
    void setNeedsRepackaging( bool needsRepackaging );

    bool isOutdated() const { return mIsOutdated; }
    void setIsOutdated( bool isOutdated );

    bool isProjectOutdated() const { return mIsProjectOutdated; }
    void setIsProjectOutdated( bool isProjectOutdated );

    ProjectModifications modification() const { return mModification; }
    void setModification( ProjectModification modification );

    QString localPath() const { return mLocalPath; }
    void setLocalPath( const QString &localPath );

    QString deltaFileId() const { return mDeltaFileId; }
    void setDeltaFileId( const QString &deltaFileId );

    DeltaFileStatus deltaFilePushStatus() const { return mDeltaFilePushStatus; }
    void setDeltaFilePushStatus( DeltaFileStatus deltaFilePushStatus );

    QString deltaFilePushStatusString() const { return mDeltaFilePushStatusString; }
    void setDeltaFilePushStatusString( const QString &deltaFilePushStatusString );

    double pushDeltaProgress() const { return mPushDeltaProgress; }

    QStringList deltaLayersToDownload() const { return mDeltaLayersToDownload; }
    void setDeltaLayersToDownload( const QStringList &deltaLayersToDownload );

    bool isPackagingActive() const { return mIsPackagingActive; }
    void setIsPackagingActive( bool isPackagingActive );

    bool isPackagingFailed() const { return mIsPackagingFailed; }
    void setIsPackagingFailed( bool isPackagingFailed );

    PackagingStatus packagingStatus() const { return mPackagingStatus; }
    void setPackagingStatus( PackagingStatus packagingStatus );

    QString packagingStatusString() const { return mPackagingStatusString; }
    void setPackagingStatusString( const QString &packagingStatusString );

    QStringList packagedLayerErrors() const { return mPackagedLayerErrors; }
    void setPackagedLayerErrors( const QStringList &packagedLayerErrors );

    bool forceAutoPush() const { return mForceAutoPush; }
    void setForceAutoPush( bool force );

    bool autoPushEnabled() const { return mAutoPushEnabled; }
    void setAutoPushEnabled( bool enabled );

    int autoPushIntervalMins() const { return mAutoPushIntervalMins; }
    void setAutoPushIntervalMins( int minutes );

    bool attachmentsOnDemandEnabled() const { return mAttachmentsOnDemandEnabled; }
    void setAttachmentsOnDemandEnabled( bool enabled );

    QDateTime lastLocalPushDeltas() const { return mLastLocalPushDeltas; }
    void setLastLocalPushDeltas( const QDateTime &lastLocalPushDeltas );

    QDateTime lastLocalExportedAt() const { return mLastLocalExportedAt; }
    void setLastLocalExportedAt( const QDateTime &lastLocalExportedAt );

    QString lastLocalExportId() const { return mLastLocalExportId; }
    void setLastLocalExportId( const QString &lastLocalExportId );

    QDateTime lastLocalDataLastUpdatedAt() const { return mLastLocalDataLastUpdatedAt; }
    void setLastLocalDataLastUpdatedAt( const QDateTime &lastLocalDataLastUpdatedAt );

    QDateTime lastLocalRestrictedDataLastUpdatedAt() const { return mLastLocalRestrictedDataLastUpdatedAt; }
    void setLastLocalRestrictedDataLastUpdatedAt( const QDateTime &lastLocalRestrictedDataLastUpdatedAt );

    QDateTime lastRefreshedAt() const { return mLastRefreshedAt; }
    void setLastRefreshedAt( const QDateTime &lastRefreshedAt );

    qint64 downloadBytesTotal() const { return mDownloadBytesTotal; }
    qint64 downloadBytesReceived() const { return mDownloadBytesReceived; }
    double downloadProgress() const { return mDownloadProgress; }

    qint64 uploadBytesTotal() const { return mUploadBytesTotal; }
    qint64 uploadBytesSent() const { return mUploadBytesSent; }
    double uploadProgress() const { return mUploadProgress; }

    int deltasCount() const { return mDeltaFileWrapper ? mDeltaFileWrapper->count() : 0; }
    QfDeltaFileWrapper *deltaFileWrapper() const { return mDeltaFileWrapper.get(); }

    QString thumbnailPath() const { return mThumbnailPath; }
    void setThumbnailPath( const QString &thumbnailPath );

    Q_INVOKABLE void downloadThumbnail();
    Q_INVOKABLE void downloadAttachment( const QString &fileName );

    Q_INVOKABLE void uploadLocalPath( QString localPath, bool deleteAfterSuccessfulUpload = false );

    Q_INVOKABLE void packageAndDownload();
    void cancelDownload();

    Q_INVOKABLE void push( bool shouldDownloadUpdates );

    void ensureProjectCreated();
    void cancelPush();

    void refreshDeltaList();
    void refreshModification();

    void removeLocally();

    static QfCloudProject::JobStatus getJobStatusFromString( const QString &status );
    static QString getJobTypeAsString( QfCloudProject::JobType jobType );

    static QfCloudProject *fromDetails( const QVariantHash &details, QfCloudConnection *connection, QgsGpkgFlusher *gpkgFlusher = nullptr );
    static QfCloudProject *fromLocalSettings( const QString &id, QfCloudConnection *connection, QgsGpkgFlusher *gpkgFlusher = nullptr );
    static void restoreLocalSettings( QfCloudProject *project, const QDir &localPath );

  signals:
    void idChanged();

    void sharedDatasetsProjectIdChanged();
    void typeChanged();

    void isPublicChanged();
    void isFeaturedChanged();
    void ownerChanged();
    void nameChanged();
    void descriptionChanged();
    void userRoleChanged();
    void userRoleOriginChanged();

    void errorStatusChanged();
    void checkoutChanged();
    void statusChanged();

    void createdAtChanged();
    void updatedAtChanged();
    void remoteSizeBytesChanged();

    void dataLastUpdatedAtChanged();
    void restrictedDataLastUpdatedAtChanged();

    void canRepackageChanged();
    void needsRepackagingChanged();
    void isOutdatedChanged();
    void isProjectOutdatedChanged();

    void modificationChanged();
    void localPathChanged();

    void deltaFileIdChanged();
    void deltaFilePushStatusChanged();
    void deltaFilePushStatusStringChanged();
    void deltaLayersToDownloadChanged();

    void pushDeltaProgressChanged();

    void isPackagingActiveChanged();
    void isPackagingFailedChanged();
    void packagingStatusChanged();
    void packagingStatusStringChanged();
    void packagedLayerErrorsChanged();

    void forceAutoPushChanged();
    void autoPushEnabledChanged();
    void autoPushIntervalMinsChanged();

    void attachmentsOnDemandEnabledChanged();

    void lastLocalExportedAtChanged();
    void lastLocalExportIdChanged();
    void lastDataLastUpdatedAtChanged();

    void lastLocalDataLastUpdatedAtChanged();
    void lastLocalRestrictedDataLastUpdatedAtChanged();
    void lastRefreshedAtChanged();
    void lastLocalPushDeltasChanged();

    void downloadBytesTotalChanged();
    void downloadBytesReceivedChanged();
    void downloadProgressChanged();

    void uploadBytesTotalChanged();
    void uploadBytesSentChanged();
    void uploadProgressChanged();

    void deltasCountChanged();
    void deltaFileWrapperChanged();

    void thumbnailPathChanged();

    void downloadAttachmentFinished( const QString &fileName, const QString &error = QString() );
    void downloadFinished( const QString &error = QString() );
    void downloaded( const QString &error = QString() );

    void pushFinished( bool isDownloading, const QString &error = QString() );

    void uploadFinished( const QString &error = QString() );

    void networkDeltaPushed();
    void networkDeltaStatusChecked();

    void dataRefreshed( ProjectRefreshReason reason, const QString &error = QString() );
    void jobFinished( JobType type, const QString &error = QString() );

  private:
    void download();
    void prepareDownloadTransfer( const QString &projectId, const QString &fileName, qint64 fileSize, const QString &cloudEtag );
    void downloadFiles();
    void updateActiveFilesToDownload();
    void downloadFilesCompleted( bool emptyDownload = false );

    void uploadFiles();

    void startJob( JobType type );
    void getJobStatus( JobType type );
    void getDeltaStatus();
    void getCreateProjectJobStatus();

    void refreshData( ProjectRefreshReason reason );

    void setupDeltaFileWrapper();

    void saveSettings();

    QfNetworkReply *downloadFile( const QString &projectId, const QString &fileName, bool fromLatestPackage = true, bool autoRedirect = false );
    void downloadFileConnections( const QString &fileKey );
    void downloadAttachmentConnections( const QString &fileKey );

    bool moveDownloadedFilesToPermanentStorage();
    void logFailedDownload( const QString &fileKey, const QString &errorMessage, const QString &errorMessageDetail );

    struct Job
    {
        Job(
          const QString &id,
          const QString &projectId,
          const QfCloudProject::JobType type )
          : id( id ), projectId( projectId ), type( type ) {};

        Job() = default;

        QString id;
        QString projectId;
        QfCloudProject::JobType type = QfCloudProject::JobType::Package;
        QfCloudProject::JobStatus status = QfCloudProject::JobPendingStatus;
    };

    QString mId;
    QString mSharedDatasetsProjectId;
    ProjectType mType = ProjectType::Regular;

    bool mIsPublic = false;
    bool mIsFeatured = false;

    QString mOwner;
    QString mName;
    QString mDescription;
    QString mUserRole;
    QString mUserRoleOrigin;

    ProjectErrorStatus mErrorStatus = ProjectErrorStatus::NoErrorStatus;
    ProjectCheckouts mCheckout = ProjectCheckout::LocalCheckout;
    ProjectStatus mStatus = ProjectStatus::Idle;
    QDateTime mCreatedAt;
    QDateTime mUpdatedAt;
    qint64 mRemoteSizeBytes = 0;
    QDateTime mDataLastUpdatedAt;
    QDateTime mRestrictedDataLastUpdatedAt;
    bool mCanRepackage = false;
    bool mNeedsRepackaging = false;
    bool mIsOutdated = false;
    bool mIsProjectOutdated = false;
    ProjectModifications mModification = ProjectModification::NoModification;
    QString mLocalPath;

    QString mDeltaFileId;
    DeltaFileStatus mDeltaFilePushStatus = DeltaLocalStatus;
    QString mDeltaFilePushStatusString;
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
    qint64 mDownloadBytesTotal = 0;
    qint64 mDownloadBytesReceived = 0;
    double mDownloadProgress = 0.0;  // range from 0.0 to 1.0
    double mPushDeltaProgress = 0.0; // range from 0.0 to 1.0

    QString mUploadLocalPath;
    bool mUploadDeleteAfterSuccessfulUpload = false;

    QMap<QString, QfCloudProject::FileTransfer> mUploadFileTransfers;
    int mUploadFilesFailed = 0;
    qint64 mUploadBytesTotal = 0;
    qint64 mUploadBytesSent = 0;
    double mUploadProgress = 0.0;

    std::unique_ptr<QfDeltaFileWrapper> mDeltaFileWrapper;

    QString mThumbnailPath;

    QDateTime mLastExportedAt;
    QString mLastExportId;
    QDateTime mLastLocalExportedAt;
    QString mLastLocalExportId;
    QDateTime mLastLocalPushDeltas;

    QDateTime mLastLocalDataLastUpdatedAt;
    QDateTime mLastLocalRestrictedDataLastUpdatedAt;
    QDateTime mLastRefreshedAt;

    bool mForceAutoPush = false;
    bool mAutoPushEnabled = false;
    int mAutoPushIntervalMins = 30;

    bool mAttachmentsOnDemandEnabled = false;
    QMap<QString, FileTransfer> mAttachmentsFileTransfers;

    QMap<JobType, Job> mJobs;

    QfCloudConnection *mCloudConnection = nullptr;
    QString mUsername;

    QgsGpkgFlusher *mGpkgFlusher = nullptr;

    QString mCreateJobId;
    int mCreateJobAttempts = 0;

    static const int sMaxCreateJobAttempts = 20;
    static const int sDelayBeforeStatusRetry = 2000;
};

Q_DECLARE_METATYPE( QfCloudProject::ProjectType )
Q_DECLARE_METATYPE( QfCloudProject::ProjectStatus )
Q_DECLARE_METATYPE( QfCloudProject::ProjectCheckout )
Q_DECLARE_METATYPE( QfCloudProject::ProjectCheckouts )
Q_DECLARE_OPERATORS_FOR_FLAGS( QfCloudProject::ProjectCheckouts )
Q_DECLARE_METATYPE( QfCloudProject::ProjectModification )
Q_DECLARE_METATYPE( QfCloudProject::ProjectModifications )
Q_DECLARE_OPERATORS_FOR_FLAGS( QfCloudProject::ProjectModifications )

#endif // QFCLOUDPROJECT_H
```


