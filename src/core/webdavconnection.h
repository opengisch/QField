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
    Q_PROPERTY( bool isPasswordStored READ isPasswordStored NOTIFY isPasswordStoredChanged )

    Q_PROPERTY( QStringList availablePaths READ availablePaths NOTIFY availablePathsChanged );
    Q_PROPERTY( bool isFetchingAvailablePaths READ isFetchingAvailablePaths NOTIFY isFetchingAvailablePathsChanged )

  public:
    explicit WebdavConnection( QObject *parent = nullptr );
    ~WebdavConnection() = default;

    QString url() const { return mUrl; }

    void setUrl( const QString &url );

    QString username() const { return mUsername; }

    void setUsername( const QString &username );

    QString password() const { return mPassword; }

    void setPassword( const QString &password );

    bool isPasswordStored() const { return !mStoredPassword.isEmpty(); }

    QStringList availablePaths() const { return isFetchingAvailablePaths() ? QStringList() : mAvailablePaths; }

    bool isFetchingAvailablePaths() const { return !mFetchPendingPaths.isEmpty(); }

    Q_INVOKABLE void fetchAvailablePaths();

  signals:
    void urlChanged();
    void usernameChanged();
    void passwordChanged();
    void isPasswordStoredChanged();
    void availablePathsChanged();
    void isFetchingAvailablePathsChanged();

  private:
    void checkStoredPassword();
    void setupConnection();
    void processDirParserFinished();

    QString mUrl;
    QString mUsername;
    QString mPassword;
    QString mStoredPassword;

    QStringList mAvailablePaths;
    QStringList mFetchPendingPaths;

    QWebdav mWebdavConnection;
    QWebdavDirParser mWebdavDirParser;
};

#endif // WEBDAVCONNECTION_H
