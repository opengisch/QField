

# File qfcloudconnection.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldcloud**](dir_d27ac98b23548f5810403c734ce9817b.md) **>** [**qfcloudconnection.h**](qfcloudconnection_8h.md)

[Go to the documentation of this file](qfcloudconnection_8h.md)


```C++
/***************************************************************************
    qfcloudconnection.cpp
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

#ifndef QFCLOUDCONNECTION_H
#define QFCLOUDCONNECTION_H

#include "qfcloudutils.h"
#include "qfnetworkmanager.h"
#include "qfnetworkreply.h"

#include <QJsonDocument>
#include <QNetworkInformation>
#include <QObject>
#include <QSet>
#include <QVariantMap>

class QNetworkRequest;


class QfAuthenticationProvider
{
    Q_GADGET

    Q_PROPERTY( QString id READ id );
    Q_PROPERTY( QString name READ name );
    Q_PROPERTY( QVariantMap details READ details );

  public:
    explicit QfAuthenticationProvider( const QString &id = QString(), const QString &name = QString(), const QVariantMap &details = QVariantMap() )
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

Q_DECLARE_METATYPE( QfAuthenticationProvider )



class QfCloudConnection : public QObject
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

    Q_PROPERTY( QList<QfAuthenticationProvider> availableProviders READ availableProviders NOTIFY availableProvidersChanged )
    Q_PROPERTY( bool isFetchingAvailableProviders READ isFetchingAvailableProviders NOTIFY isFetchingAvailableProvidersChanged )

    Q_PROPERTY( QfCloudUserInformation userInformation READ userInformation NOTIFY userInformationChanged )
    Q_PROPERTY( QfCloudServerInformation serverInformation READ serverInformation NOTIFY serverInformationChanged )

    Q_PROPERTY( bool isReachable READ isReachable NOTIFY isReachableChanged )

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

    QfCloudConnection();
    ~QfCloudConnection();

    static QString errorString( QNetworkReply *reply );

    static QMap<QString, QString> sErrors;

    QString url() const;

    void setUrl( const QString &url );

    static QString defaultUrl();

    QStringList urls() const;

    QString provider() const;
    void setProvider( const QString &provider );

    QString username() const;
    void setUsername( const QString &username );

    QString password() const;
    void setPassword( const QString &password );

    QString token() const;

    QString avatarUrl() const;

    QfCloudUserInformation userInformation() const;

    Q_INVOKABLE void login( const QString &password = QString() );
    Q_INVOKABLE void logout();

    Q_INVOKABLE void getUserOrganizations( const QString &user );
    Q_INVOKABLE void getSubscriptionInformation( const QString &user );

    Q_INVOKABLE void getServerInformation();
    QList<QfAuthenticationProvider> availableProviders() const;
    bool isFetchingAvailableProviders() const;

    QfCloudServerInformation serverInformation() const { return mServerInformation; }

    ConnectionStatus status() const;
    ConnectionState state() const;

    bool hasToken() { return !mToken.isEmpty(); }
    bool hasProviderConfiguration() { return !mProviderConfigId.isEmpty(); }

    QfNetworkReply *post( const QString &endpoint, const QVariantMap &params = QVariantMap(), const QStringList &fileNames = QStringList() );

    QfNetworkReply *post( QNetworkRequest &request, const QString &endpoint, const QVariantMap &params = QVariantMap(), const QStringList &fileNames = QStringList() );

    QfNetworkReply *get( const QString &endpoint, const QVariantMap &params = QVariantMap() );

    QfNetworkReply *get( QNetworkRequest &request, const QString &endpoint, const QVariantMap &params = QVariantMap() );
    QfNetworkReply *get( QNetworkRequest &request, const QUrl &url, const QVariantMap &params = QVariantMap() );

    void setAuthenticationDetails( QNetworkRequest &request );

    qsizetype uploadPendingAttachments();

    void queueProjectPush( const QString &projectId );

    bool isReachable() const;

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
    void serverInformationChanged();

    void isReachableChanged();
    void queuedProjectPushRequested( const QString &projectId );

    void userOrganizationsReceived( const QStringList &organizations );
    void subscriptionInformationReceived( const QfCloudSubscriptionInformation &subscriptionInformation );

  private:
    void setStatus( ConnectionStatus status );
    void setState( ConnectionState state );
    void setToken( const QByteArray &token );
    void invalidateToken();
    void processPendingAttachments();
    void fetchLegacyAuthenticationProviders();

    void saveCookies();
    void restoreCookies();

    QString mUrl;

    QString mUsername;
    QString mPassword;
    QByteArray mToken;
    QString mTokenConfigId;

    QMap<QString, QfAuthenticationProvider> mAvailableProviders;
    bool mIsFetchingAvailableProviders = false;
    QString mProvider;
    QString mProviderConfigId;

    QfCloudServerInformation mServerInformation;

    QString mAvatarUrl;
    QfCloudUserInformation mUserInformation;
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
    void tryFlushQueuedProjectPushes();
};

#endif // QFCLOUDCONNECTION_H
```


