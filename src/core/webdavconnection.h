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

    Q_PROPERTY( QStringList availablePaths READ availablePaths NOTIFY availablePathsChanged )
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

    QStringList availablePaths() const { return mIsFetchingAvailablePaths ? QStringList() : mAvailablePaths; }

    bool isFetchingAvailablePaths() const { return mIsFetchingAvailablePaths; }

    bool isImportingPath() const { return mIsImportingPath; }

    bool isDownloadingPath() const { return mIsDownloadingPath; }

    double progress() const;

    QString lastError() const { return mLastError; }

    Q_INVOKABLE void fetchAvailablePaths();

    Q_INVOKABLE void importPath( const QString &remotePath, const QString &localPath );
    Q_INVOKABLE void downloadPath( const QString &localPath );

  signals:
    void urlChanged();
    void usernameChanged();
    void passwordChanged();
    void storePasswordChanged();
    void isPasswordStoredChanged();
    void isFetchingAvailablePathsChanged();
    void isImportingPathChanged();
    void isDownloadingPathChanged();
    void availablePathsChanged();
    void progressChanged();
    void lastErrorChanged();

  private slots:
    void processDirParserFinished();
    void processConnectionError( const QString &error );
    void processDirParserError( const QString &error );

  private:
    void checkStoredPassword();
    void applyStoredPassword();
    void setupConnection();
    void getWebdavItems();

    QString mUrl;
    QString mUsername;
    QString mPassword;

    bool mStorePassword = false;
    QString mStoredPassword;

    bool mIsFetchingAvailablePaths = false;
    QStringList mAvailablePaths;

    bool mIsImportingPath = false;
    bool mIsDownloadingPath = false;

    QString mGetRemotePath;
    QString mGetLocalPath;

    QList<QWebdavItem> mWebdavItems;
    qint64 mCurrentBytesReceived = 0;
    qint64 mBytesReceived = 0;
    qint64 mBytesTotal = 0;

    QWebdav mWebdavConnection;
    QWebdavDirParser mWebdavDirParser;
    QString mLastError;
};

#endif // WEBDAVCONNECTION_H
