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
#include "qfieldcloudutils.h"

#include <QJsonDocument>
#include <QNetworkInformation>
#include <QObject>
#include <QSet>
#include <QVariantMap>

class QNetworkRequest;


/**
 * \ingroup core
 */
class AuthenticationProvider
{
    Q_GADGET

    Q_PROPERTY( QString id READ id );
    Q_PROPERTY( QString name READ name );
    Q_PROPERTY( QVariantMap details READ details );

  public:
    explicit AuthenticationProvider( const QString &id = QString(), const QString &name = QString(), const QVariantMap &details = QVariantMap() )
      : mId( id )
      , mName( name )
      , mDetails( details )
    {
    }

    QString id() const { return mId; }
    QString name() const { return mName; }
    QVariantMap details() const { return mDetails; }

  private:
    QString mId;
    QString mName;
    QVariantMap mDetails;
};

Q_DECLARE_METATYPE( AuthenticationProvider )


/**
 * \ingroup core
 */
class QFieldCloudConnection : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString provider READ provider WRITE setProvider NOTIFY providerChanged )
    Q_PROPERTY( QString username READ username WRITE setUsername NOTIFY usernameChanged )
    Q_PROPERTY( QString password READ password WRITE setPassword NOTIFY passwordChanged )
    Q_PROPERTY( QString avatarUrl READ avatarUrl NOTIFY avatarUrlChanged )
    Q_PROPERTY( QString url READ url WRITE setUrl NOTIFY urlChanged )
    Q_PROPERTY( QString defaultUrl READ defaultUrl CONSTANT )
    Q_PROPERTY( QStringList urls READ urls NOTIFY urlsChanged )

    Q_PROPERTY( ConnectionStatus status READ status NOTIFY statusChanged )
    Q_PROPERTY( ConnectionState state READ state NOTIFY stateChanged )
    Q_PROPERTY( bool hasToken READ hasToken NOTIFY tokenChanged )
    Q_PROPERTY( bool hasProviderConfiguration READ hasProviderConfiguration NOTIFY providerConfigurationChanged )

    Q_PROPERTY( CloudUserInformation userInformation READ userInformation NOTIFY userInformationChanged )

    Q_PROPERTY( QList<AuthenticationProvider> availableProviders READ availableProviders NOTIFY availableProvidersChanged )
    Q_PROPERTY( bool isFetchingAvailableProviders READ isFetchingAvailableProviders NOTIFY isFetchingAvailableProvidersChanged )

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

    /**
     * Returns the connections URLs successfully logged in in the past.
     */
    QStringList urls() const;

    QString provider() const;
    void setProvider( const QString &provider );

    QString username() const;
    void setUsername( const QString &username );

    QString password() const;
    void setPassword( const QString &password );

    QString token() const;

    QString avatarUrl() const;

    CloudUserInformation userInformation() const;

    Q_INVOKABLE void login( const QString &password = QString() );
    Q_INVOKABLE void logout();

    Q_INVOKABLE void getAuthenticationProviders();
    QList<AuthenticationProvider> availableProviders() const;
    bool isFetchingAvailableProviders() const;

    ConnectionStatus status() const;
    ConnectionState state() const;

    bool hasToken() { return !mToken.isEmpty(); }
    bool hasProviderConfiguration() { return !mProviderConfigId.isEmpty(); }

    /**
     * Sends a post request with the given \a parameters to the given \a endpoint.
     *
     * If this connection is not logged in, will return nullptr.
     * The returned reply needs to be deleted by the caller.
     */
    NetworkReply *post( const QString &endpoint, const QVariantMap &params = QVariantMap(), const QStringList &fileNames = QStringList() );

    /**
     * Sends a post \a request with the given \a parameters to the given \a endpoint.
     *
     * If this connection is not logged in, will return nullptr.
     * The returned reply needs to be deleted by the caller.
     */
    NetworkReply *post( QNetworkRequest &request, const QString &endpoint, const QVariantMap &params = QVariantMap(), const QStringList &fileNames = QStringList() );

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
     * Sets authentication details on a \a request.
     */
    void setAuthenticationDetails( QNetworkRequest &request );

    /**
     * Uploads any pending attachments linked to the logged in user account.
     * \returns the number of attachments to be uploaded.
     */
    qsizetype uploadPendingAttachments();


    /**
     * Called by the push initiator (typically QFieldCloudProject) before starting a push.
     *      * Returns true if the push can start now.
     * If the device is offline, the projectId is queued and will be re-emitted via
     * queuedProjectPushRequested(projectId) once the connection becomes reachable again.
     */
    bool beginProjectPushOrQueue( const QString &projectId );

  signals:
    void providerChanged();
    void usernameChanged();
    void passwordChanged();
    void avatarUrlChanged();
    void urlChanged();
    void urlsChanged();
    void statusChanged();
    void stateChanged();
    void tokenChanged();
    void providerConfigurationChanged();
    void userInformationChanged();
    void pendingAttachmentsUploadFinished( const QString &error = QString() );
    void pendingAttachmentsUploadStatus( const QString &fileName, double fileProgress, qsizetype uploadPending );
    void pendingAttachmentsAdded();
    void error();

    void loginFailed( const QString &reason );

    void availableProvidersChanged();
    void isFetchingAvailableProvidersChanged();

    void isReachable();
    void queuedProjectPushRequested( const QString &projectId );

  private:
    void setStatus( ConnectionStatus status );
    void setState( ConnectionState state );
    void setToken( const QByteArray &token );
    void invalidateToken();
    void processPendingAttachments();

    QString mUrl;

    QString mUsername;
    QString mPassword;
    QByteArray mToken;
    QString mTokenConfigId;

    QMap<QString, AuthenticationProvider> mAvailableProviders;
    bool mIsFetchingAvailableProviders = false;
    QString mProvider;
    QString mProviderConfigId;

    QString mAvatarUrl;
    CloudUserInformation mUserInformation;
    ConnectionStatus mStatus = ConnectionStatus::Disconnected;
    ConnectionState mState = ConnectionState::Idle;

    int mPendingRequests = 0;

    qsizetype mUploadPendingCount = 0;
    qsizetype mUploadDoneCount = 0;
    qsizetype mUploadFailingCount = 0;

    void setClientHeaders( QNetworkRequest &request );

    QSet<QString> mQueuedProjectPushes;
    bool mIsFlushingQueuedProjectPushes = false;

    const QNetworkInformation *mNetworkInformation = nullptr;
    bool isReachableToCloud() const;
    void requestQueuedProjectPush( const QString &projectId );
    void tryFlushQueuedProjectPushes();
};

#endif // QFIELDCLOUDCONNECTION_H
