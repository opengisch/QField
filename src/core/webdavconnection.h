/***************************************************************************
                          webdavconnection.h
                             -------------------
  begin                : January 2025
  copyright            : (C) 2025 by Mathieu Pellerin
  email                : mathieu@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef WEBDAVCONNECTION_H
#define WEBDAVCONNECTION_H

#include <QByteArray>
#include <QDateTime>
#include <QFileInfo>
#include <QList>
#include <QObject>
#include <QStringList>
#include <QVariantMap>
#include <QtWebDAV/qwebdav.h>
#include <QtWebDAV/qwebdavdirparser.h>

#include <memory>

class QLockFile;

/**
 * The WebDAV connection object allows connecting to a WebDAV endpoint and performing
 * push/pull operations for imported content.
 * \ingroup core
 */
class WebdavConnection : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString url READ url WRITE setUrl NOTIFY urlChanged );
    Q_PROPERTY( QString username READ username WRITE setUsername NOTIFY usernameChanged )
    Q_PROPERTY( QString password READ password WRITE setPassword NOTIFY passwordChanged )

    Q_PROPERTY( bool storePassword READ storePassword WRITE setStorePassword NOTIFY storePasswordChanged )

    Q_PROPERTY( bool isPasswordStored READ isPasswordStored NOTIFY isPasswordStoredChanged )
    Q_PROPERTY( bool isFetchingAvailablePaths READ isFetchingAvailablePaths NOTIFY isFetchingAvailablePathsChanged )
    Q_PROPERTY( bool isImportingPath READ isImportingPath NOTIFY isImportingPathChanged )
    Q_PROPERTY( bool isDownloadingPath READ isDownloadingPath NOTIFY isDownloadingPathChanged )
    Q_PROPERTY( bool isUploadingPath READ isUploadingPath NOTIFY isUploadingPathChanged )

    Q_PROPERTY( QStringList availablePaths READ availablePaths NOTIFY availablePathsChanged )
    Q_PROPERTY( QStringList checkedPaths READ checkedPaths NOTIFY checkedPathsChanged )
    Q_PROPERTY( double progress READ progress NOTIFY progressChanged )
    Q_PROPERTY( QString lastError READ lastError NOTIFY lastErrorChanged )

  public:
    explicit WebdavConnection( QObject *parent = nullptr );
    ~WebdavConnection() = default;

    /**
     * Returns the WebDAV URL endpoint used to connect, list, and transfer files.
     */
    QString url() const { return mUrl; }

    /**
     * Sets the WebDAV URL endpoint used to connect, list, and transfer files.
     * \param url the URL endpoint string
     */
    void setUrl( const QString &url );

    /**
     * Returns the authentication username.
     */
    QString username() const { return mUsername; }

    /**
     * Sets the authentication \a username.
     */
    void setUsername( const QString &username );

    /**
     * Returns the authentication password.
     */
    QString password() const { return mPassword; }

    /**
     * Sets the authentication \a password.
     */
    void setPassword( const QString &password );

    /**
     * Returns TRUE if the current authentication password will be saved upon successful list or file transfer operation.
     */
    bool storePassword() const { return mStorePassword; }

    /**
     * Sets whether the current authentication password will be saved upon successful list or file transfer operation
     * \param storePassword set to TRUE to store the current authentication password.
     */
    void setStorePassword( bool storePassword );

    /**
     * Returns TRUE if a password has been stored for a given pair of username and WebDAV URL endpoint.
     */
    bool isPasswordStored() const { return !mStoredPassword.isEmpty(); }

    /**
     * Returns TRUE if the WebDAV connection is conducting a fetching of available remote paths.
     */
    bool isFetchingAvailablePaths() const { return mIsFetchingAvailablePaths; }

    /**
     * Returns TRUE if the WebDAV connection is conducting an import operation.
     */
    bool isImportingPath() const { return mIsImportingPath; }

    /**
     * Returns TRUE if the WebDAV connection is conducting a download operation.
     */
    bool isDownloadingPath() const { return mIsDownloadingPath; }

    /**
     * Returns TRUE if the WebDAV connection is conducting an upload operation.
     */
    bool isUploadingPath() const { return mIsUploadingPath; }

    /**
     * Returns the progress of an ongoing import, download, or upload operation.
     * \note The returned value's range is 0.0 to 1.0.
     */
    double progress() const;

    /**
     * Returns a list of checked paths.
     * \see fetchAvailablePaths
     */
    QStringList checkedPaths() const { return mCheckedPaths; }

    /**
     * Returns a list of fetched available paths.
     * \see fetchAvailablePaths
     */
    QStringList availablePaths() const { return mIsFetchingAvailablePaths ? QStringList() : mAvailablePaths; }

    /**
     * Returns the last error string captured by the WebDAV connection.
     */
    QString lastError() const { return mLastError; }

    /**
     * Fetches available remote paths from the current WebDAV endpoint.
     */
    Q_INVOKABLE void fetchAvailablePaths( const QString &remotePath = QString() );

    /**
     * Imports a remote path into a local path stored on the device.
     * \a remotePath the remote path on the WebDAV endpoint.
     * \a localPath the local path
     * \a localFolder the local folder name containing the imported remote path content
     */
    Q_INVOKABLE void importPath( const QString &remotePath, const QString &localPath, QString localFolder = QString() );

    /**
     * Download new and modified files from an imported remote path.
     * \a localPath the local path within which a remote path was imported into
     * \note This is not a synchronization process; files removed remotely will *not* be removed
     * locally. Furthermore, all files modified remotely will overwrite locally-stored files.
     */
    Q_INVOKABLE void downloadPath( const QString &localPath );

    /**
     * Uploads one or more local files and/or folders to the WebDAV endpoint associated with a project
     * previously imported via WebDAV. The provided \a localPaths must all belong to the same imported
     * project (i.e. share the same WebDAV configuration); otherwise no upload is started and lastError()
     * is set.
     *
     * \note This is not a synchronization process; files removed locally will not be removed remotely.
     * Modified local files will overwrite their remote versions. Hidden dot-folders are ignored, and the
     * WebDAV configuration and lock files are never uploaded.
     *
     * Before the upload starts, confirmationRequested() is emitted so the UI can ask the user to confirm.
     * Call confirmRequest() to proceed or cancelRequest() to abort.
     */
    Q_INVOKABLE void uploadPaths( const QStringList &localPaths );

    /**
     * Launches a requested download or upload operation after the user has confirmed they want to proceed.
     * This is typically called in response to confirmationRequested().
     * \see confirmationRequested
     * \see isDownloadingPath
     * \see isUploadingPath
     */
    Q_INVOKABLE void confirmRequest();

    /**
     * Cancels a requested download or upload operation. If an upload is active, the per-project upload lock
     * is released. For automatic uploads, uploadFinished() is emitted with a failure status.
     * \see confirmationRequested
     * \see isDownloadingPath
     * \see isUploadingPath
     */
    Q_INVOKABLE void cancelRequest();

    /**
     * Requests an automatic upload for the WebDAV imported project that contains \a projectPath. This is intended
     * for background or scheduled upload workflows and will not prompt the user for credentials. The upload is
     * skipped if the project is not WebDAV imported, if another upload is already running for the same project
     * (upload lock), if no stored password is available, or if no local changes are detected since the last
     * successful automatic upload (unless \a force is TRUE).
     *
     * If the upload does not start, uploadSkipped() is emitted with the reason. If it starts, uploadFinished()
     * is emitted when the upload completes.
     */
    Q_INVOKABLE void requestUpload( const QString &projectPath, bool force = false );

    /**
     * Returns TRUE if a given path contains a WebDAV configuration JSON file or is parented
     * to a folder containing such a file.
     */
    Q_INVOKABLE static bool hasWebdavConfiguration( const QString &path );

    /**
     * Returns a list of import history.
     */
    Q_INVOKABLE static QVariantMap importHistory();

    Q_INVOKABLE static void forgetHistory( const QString &url = QString(), const QString &username = QString() );

  signals:
    void urlChanged();
    void usernameChanged();
    void passwordChanged();
    void storePasswordChanged();
    void isPasswordStoredChanged();
    void isFetchingAvailablePathsChanged();
    void isImportingPathChanged();
    void isDownloadingPathChanged();
    void isUploadingPathChanged();
    void availablePathsChanged();
    void checkedPathsChanged();
    void progressChanged();
    void lastErrorChanged();

    void confirmationRequested( const QString &host, const QString &username );

    void importSuccessful( const QString &path );

    void uploadFinished( bool success, const QString &message );
    void uploadSkipped( const QString &reason );

  private slots:
    void processDirParserFinished();
    void processConnectionError( const QString &error );
    void processDirParserError( const QString &error );

  private:
    void checkStoredPassword();
    void applyStoredPassword();
    void setupConnection();
    void getWebdavItems();
    void putLocalItems();
    /**
     * Common upload completion handler.
     * Cleans up auto-upload state and emits appropriate signals.
     */
    void finishUpload( bool success, const QString &errorMessage );

    ///! Computes the common path between two given paths.
    QString getCommonPath( const QString &addressA, const QString &addressB );

    //! Returns TRUE if the relative path is inside a hidden dot-folder.
    bool isInHiddenDotFolder( const QString &relativePath ) const;

    //! Ensures the path ends with a trailing slash.
    QString ensureTrailingSlash( QString path ) const;

    //! Finds the WebDAV project root by walking up from the given path.
    QString findWebdavRootForPath( const QString &path ) const;

    /**
     * Reads and parses the WebDAV config JSON stored at on rootPath.
     * On failure returns FALSE and sets the errorMessage.
     */
    bool readWebdavConfig( const QString &rootPath, QVariantMap &outConfig, QString &errorMessage ) const;

    /**
     * Computes a deterministic signature of the local project tree under on rootPath.
     *
     * The signature is based on each file's relative path, size, and modification time
     * (not file contents). Hidden dot-folders are ignored, and the WebDAV configuration
     * and lock files are excluded.
     *
     * Used to detect local changes for auto-upload.
     */
    QByteArray computeLocalSignature( const QString &rootPath ) const;


    //! Resolves the WebDAV config root for the given localPaths, loads and validates the WebDAV configuration
    bool uploadPathsInternal( const QStringList &localPaths, bool requireConfirmation, bool autoUpload, bool force, QString *errorMessage );

    //! Initializes upload progress state and transitions into "uploading" mode.
    void beginUpload( bool requireConfirmation );

    //! per-project upload lock
    bool tryLockUpload( const QString &root, QString *errorMessage );
    void unlockUpload();

    QString mUrl;
    QString mUsername;
    QString mPassword;

    bool mStorePassword = false;
    QString mStoredPassword;

    QStringList mCheckedPaths;

    bool mIsFetchingAvailablePaths = false;
    QStringList mAvailablePaths;

    bool mIsImportingPath = false;
    bool mIsDownloadingPath = false;
    bool mIsUploadingPath = false;

    QList<QWebdavItem> mWebdavItems;
    QList<QString> mWebdavMkDirs;
    QList<QFileInfo> mLocalItems;
    QList<QString> mWebdavLastModified;

    QString mProcessRemotePath;
    QString mProcessLocalPath;
    qint64 mCurrentBytesProcessed = 0;
    qint64 mBytesProcessed = 0;
    qint64 mBytesTotal = 0;

    qint64 mCurrentUploadFileSize = 0;
    qint64 mCurrentUploadBytesSentMax = 0;

    QWebdav mWebdavConnection;
    QWebdavDirParser mWebdavDirParser;
    QString mLastError;

    bool mAutoUploadActive = false;
    QString mAutoUploadRoot;
    QByteArray mAutoUploadSignature;
    QString mAutoUploadSignatureRoot;
    std::unique_ptr<QLockFile> mUploadLock;
};

#endif // WEBDAVCONNECTION_H
