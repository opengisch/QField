

# File webdavconnection.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**webdavconnection.h**](webdavconnection_8h.md)

[Go to the documentation of this file](webdavconnection_8h.md)


```C++
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

    QString url() const { return mUrl; }

    void setUrl( const QString &url );

    QString username() const { return mUsername; }

    void setUsername( const QString &username );

    QString password() const { return mPassword; }

    void setPassword( const QString &password );

    bool storePassword() const { return mStorePassword; }

    void setStorePassword( bool storePassword );

    bool isPasswordStored() const { return !mStoredPassword.isEmpty(); }

    bool isFetchingAvailablePaths() const { return mIsFetchingAvailablePaths; }

    bool isImportingPath() const { return mIsImportingPath; }

    bool isDownloadingPath() const { return mIsDownloadingPath; }

    bool isUploadingPath() const { return mIsUploadingPath; }

    double progress() const;

    QStringList checkedPaths() const { return mCheckedPaths; }

    QStringList availablePaths() const { return mIsFetchingAvailablePaths ? QStringList() : mAvailablePaths; }

    QString lastError() const { return mLastError; }

    Q_INVOKABLE void fetchAvailablePaths( const QString &remotePath = QString() );

    Q_INVOKABLE void importPath( const QString &remotePath, const QString &localPath, QString localFolder = QString() );

    Q_INVOKABLE void downloadPath( const QString &localPath );

    Q_INVOKABLE void uploadPaths( const QStringList &localPaths );

    Q_INVOKABLE void confirmRequest();

    Q_INVOKABLE void cancelRequest();

    Q_INVOKABLE void requestUpload( const QString &projectPath, bool force = false );

    Q_INVOKABLE static bool hasWebdavConfiguration( const QString &path );

    Q_INVOKABLE static QVariantMap importHistory();

    Q_INVOKABLE static QStringList findWebdavProjectFolders( const QString &basePath );

    Q_INVOKABLE static QString findWebdavRootForPath( const QString &path );

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
    void finishUpload( bool success, const QString &errorMessage );

    QString getCommonPath( const QString &addressA, const QString &addressB );

    bool isInHiddenDotFolder( const QString &relativePath ) const;

    QString ensureTrailingSlash( QString path ) const;

    bool readWebdavConfig( const QString &rootPath, QVariantMap &outConfig, QString &errorMessage ) const;

    QByteArray computeLocalSignature( const QString &rootPath ) const;


    bool uploadPathsInternal( const QStringList &localPaths, bool requireConfirmation, bool autoUpload, bool force, QString *errorMessage );

    void beginUpload( bool requireConfirmation );

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
```


