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

#include <QObject>
#include <QtWebDAV/qwebdav.h>
#include <QtWebDAV/qwebdavdirparser.h>

/**
 * The webdav connection objects allows for connection to and push/pull
 * operations of content.
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
    Q_INVOKABLE void fetchAvailablePaths();

    /**
     * Imports a remote path into a local path stored on the device.
     * \a remotePath the remote path on the WebDAV endpoint.
     * \a localPath the local path
     */
    Q_INVOKABLE void importPath( const QString &remotePath, const QString &localPath );

    /**
     * Download new and modified files from an imported remote path.
     * \a localPath the local path within which a remote path was imported into
     * \note This is not a synchronization process; files removed remotely will *not* be removed
     * locally. Furthermore, all files modified remotely will overwrite locally-stored files.
     */
    Q_INVOKABLE void downloadPath( const QString &localPath );

    /**
     * Upload one or more file and/or folder to a WebDAV endpoint tied to the imported remote path.
     * \a localPaths a list of files and folder parented to a local path within which a remote path was imported into
     * \note This is not a synchronization process; files removed locally will *not* be removed
     * remotely. Furthermore, all files modified locally will overwrite remotely-stored files.
     */
    Q_INVOKABLE void uploadPaths( const QStringList &localPaths );

    /**
     * Launches a requested download or upload operation.
     * \see confirmationRequested
     * \see isDownloadingPath
     * \see isUploadingPath
     */
    Q_INVOKABLE void confirmRequest();

    /**
     * Cancels a requested download or upload operation.
     * \see confirmationRequested
     * \see isDownloadingPath
     * \see isUploadingPath
     */
    Q_INVOKABLE void cancelRequest();

    /**
     * Returns TRUE if a given path contains a WebDAV configuration JSON file or is parented
     * to a folder containing such a file.
     */
    Q_INVOKABLE static bool hasWebdavConfiguration( const QString &path );

    /**
     * Returns a list of import history.
     */
    Q_INVOKABLE static QVariantMap importHistory();

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
    void progressChanged();
    void lastErrorChanged();

    void confirmationRequested( const QString &host, const QString &username );

    void importSuccessful( const QString &path );

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

    ///! Computes the common path between two given paths.
    QString getCommonPath( const QString &addressA, const QString &addressB );

    QString mUrl;
    QString mUsername;
    QString mPassword;

    bool mStorePassword = false;
    QString mStoredPassword;

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

    QWebdav mWebdavConnection;
    QWebdavDirParser mWebdavDirParser;
    QString mLastError;
};

#endif // WEBDAVCONNECTION_H
