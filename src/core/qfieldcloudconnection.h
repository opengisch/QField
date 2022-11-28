/***************************************************************************
    qfieldcloudconnection.cpp
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

#ifndef QFIELDCLOUDCONNECTION_H
#define QFIELDCLOUDCONNECTION_H

#include "networkmanager.h"
#include "networkreply.h"

#include <QJsonDocument>
#include <QObject>
#include <QVariantMap>


class QNetworkRequest;

struct CloudUserInformation
{
    Q_GADGET

  public:
    CloudUserInformation() = default;

    CloudUserInformation( const QString &username, const QString &email )
      : username( username )
      , email( email )
    {}

    QString username;
    QString email;
};

class QFieldCloudConnection : public QObject
{
    Q_OBJECT

  public:
    enum class ConnectionStatus
    {
      Disconnected,
      Connecting,
      LoggedIn
    };
    Q_ENUM( ConnectionStatus )

    enum class ConnectionState
    {
      Idle,
      Busy
    };
    Q_ENUM( ConnectionState )

    class CloudError
    {
      public:
        explicit CloudError( QNetworkReply *reply );

        QString message() const { return mMessage; }
        QString qfcCode() const { return mQfcCode; }

      private:
        int mHttpCode = 0;
        QString mMessage;
        QString mPayload;
        QString mQfcCode;
        QJsonDocument mJson;
        QJsonParseError mJsonError;
        QNetworkReply::NetworkError mError;
    };

    QFieldCloudConnection();

    Q_PROPERTY( QString username READ username WRITE setUsername NOTIFY usernameChanged )
    Q_PROPERTY( QString password READ password WRITE setPassword NOTIFY passwordChanged )
    Q_PROPERTY( QString avatarUrl READ avatarUrl NOTIFY avatarUrlChanged )
    Q_PROPERTY( QString url READ url WRITE setUrl NOTIFY urlChanged )
    Q_PROPERTY( QString defaultUrl READ defaultUrl CONSTANT )

    Q_PROPERTY( ConnectionStatus status READ status NOTIFY statusChanged )
    Q_PROPERTY( ConnectionState state READ state NOTIFY stateChanged )
    Q_PROPERTY( bool hasToken READ hasToken NOTIFY tokenChanged )
    Q_PROPERTY( CloudUserInformation userInformation READ userInformation NOTIFY userInformationChanged )

    //!Returns an error string to be shown to the user if \a reply has an error.
    static QString errorString( QNetworkReply *reply );

    //!Contains a map of error codes and translated messages
    static QMap<QString, QString> sErrors;

    /**
     * Returns the currently set server connection URL.
     */
    QString url() const;

    /**
     * Sets the current server connection URL and saves it into QSettings.
     */
    void setUrl( const QString &url );

    /**
     * Default server connection URL, pointing to the production server.
     */
    static QString defaultUrl();

    QString username() const;
    void setUsername( const QString &username );

    QString password() const;
    void setPassword( const QString &password );

    QString token() const;

    QString avatarUrl() const;

    CloudUserInformation userInformation() const;

    Q_INVOKABLE void login();
    Q_INVOKABLE void logout();

    ConnectionStatus status() const;
    ConnectionState state() const;

    bool hasToken() { return !mToken.isEmpty(); }

    /**
     * Sends a post request with the given \a parameters to the given \a endpoint.
     *
     * If this connection is not logged in, will return nullptr.
     * The returned reply needs to be deleted by the caller.
     */
    NetworkReply *post( const QString &endpoint, const QVariantMap &params = QVariantMap(), const QStringList &fileNames = QStringList() );


    /**
     * Sends a get request to the given \a endpoint. Query can be passed via \a params, empty by default.
     *
     * If this connection is not logged in, will return nullptr.
     * The returned reply needs to be deleted by the caller.
     */
    NetworkReply *get( const QString &endpoint, const QVariantMap &params = QVariantMap() );

    /**
     * Sends a get \a request to a given \a endpoint. Additional query can be passed via \a params, empty by default.
     *
     * If this connection is not logged in, will return nullptr.
     * The returned reply needs to be deleted by the caller.
     */
    NetworkReply *get( QNetworkRequest &request, const QString &endpoint, const QVariantMap &params = QVariantMap() );
    NetworkReply *get( QNetworkRequest &request, const QUrl &url, const QVariantMap &params = QVariantMap() );

    /**
     * Sets authentication token on a \a request.
     */
    void setAuthenticationToken( QNetworkRequest &request );

    /**
     * Uploads any pending attachments linked to the logged in user account.
     * \returns the number of attachments to be uploaded.
     */
    int uploadPendingAttachments();

  signals:
    void usernameChanged();
    void passwordChanged();
    void avatarUrlChanged();
    void urlChanged();
    void statusChanged();
    void stateChanged();
    void tokenChanged();
    void userInformationChanged();
    void pendingAttachmentsUploadFinished();
    void error();

    void loginFailed( const QString &reason );

  private:
    void setStatus( ConnectionStatus status );
    void setState( ConnectionState state );
    void setToken( const QByteArray &token );
    void invalidateToken();

    QString mUrl;

    QString mUsername;
    QString mPassword;
    QByteArray mToken;

    QString mAvatarUrl;
    CloudUserInformation mUserInformation;
    ConnectionStatus mStatus = ConnectionStatus::Disconnected;
    ConnectionState mState = ConnectionState::Idle;

    int mPendingRequests = 0;

    bool mUploadingAttachments = false;
    int mUploadCount = 0;

    void setClientHeaders( QNetworkRequest &request );
};

#endif // QFIELDCLOUDCONNECTION_H
