

# File qfieldcloudutils.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**qfieldcloudutils.h**](qfieldcloudutils_8h.md)

[Go to the documentation of this file](qfieldcloudutils_8h.md)


```C++
/***************************************************************************
    qfieldcloudutils.h
    ---------------------
    begin                : February 2020
    copyright            : (C) 2020 by Mathieu Pellerin
    email                : nirvn dot asia at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFIELDCLOUDUTILS_H
#define QFIELDCLOUDUTILS_H

#include <qfieldcloudprojectsmodel.h>
#include <qgsmaplayer.h>
#include <qgsproject.h>


class QString;
class QFieldCloudProjectsModel;
class DeltaFileWrapperTest;
class TestLayerObserver;

struct CloudUserInformation
{
    Q_GADGET

    Q_PROPERTY( QString username MEMBER username )
    Q_PROPERTY( QString email MEMBER email )

  public:
    CloudUserInformation() = default;

    CloudUserInformation( const QString &username, const QString &email )
      : username( username )
      , email( email )
    {}

    explicit CloudUserInformation( const QJsonObject cloudUserInformation )
      : username( cloudUserInformation.value( QStringLiteral( "username" ) ).toString() )
      , email( cloudUserInformation.value( QStringLiteral( "email" ) ).toString() )
    {}

    bool operator==( const CloudUserInformation &other ) const
    {
      return username == other.username && email == other.email;
    }

    QJsonObject toJson() const
    {
      QJsonObject cloudUserInformation;

      cloudUserInformation.insert( "username", username );
      cloudUserInformation.insert( "email", email );

      return cloudUserInformation;
    }

    bool isEmpty() const
    {
      return username.isEmpty() && email.isEmpty();
    }

    QString username;
    QString email;
};

struct CloudSubscriptionInformation
{
    Q_GADGET

    Q_PROPERTY( QString plan MEMBER plan )
    Q_PROPERTY( double storageTotal MEMBER storageTotal )
    Q_PROPERTY( double storageUsed MEMBER storageUsed )
    Q_PROPERTY( QString status MEMBER status )

  public:
    CloudSubscriptionInformation() = default;

    explicit CloudSubscriptionInformation( const QJsonObject &subscriptionInformation )
      : plan( subscriptionInformation.value( QStringLiteral( "plan_display_name" ) ).toString() )
      , storageTotal( subscriptionInformation.value( QStringLiteral( "active_storage_total_bytes" ) ).toDouble() )
      , storageUsed( subscriptionInformation.value( QStringLiteral( "storage_used_bytes" ) ).toDouble() )
      , status( subscriptionInformation.value( QStringLiteral( "status" ) ).toString() )
    {}

    bool operator==( const CloudSubscriptionInformation &other ) const
    {
      return plan == other.plan && storageTotal == other.storageTotal && storageUsed == other.storageUsed && status == other.status;
    }

    QString plan;
    double storageTotal = 0;
    double storageUsed = 0;
    QString status;
};

Q_DECLARE_METATYPE( CloudSubscriptionInformation )


struct CloudWhitelabelInformation
{
    Q_GADGET

    Q_PROPERTY( QString siteTitle MEMBER siteTitle )
    Q_PROPERTY( QString logoMain MEMBER logoMain )
    Q_PROPERTY( QString logoNavbar MEMBER logoNavbar )
    Q_PROPERTY( QString favicon MEMBER favicon )

  public:
    CloudWhitelabelInformation() = default;

    explicit CloudWhitelabelInformation( const QVariantMap &whitelabel )
      : siteTitle( whitelabel.value( QStringLiteral( "site_title" ) ).toString() )
      , logoMain( whitelabel.value( QStringLiteral( "logo_main" ) ).toString() )
      , logoNavbar( whitelabel.value( QStringLiteral( "logo_navbar" ) ).toString() )
      , favicon( whitelabel.value( QStringLiteral( "favicon" ) ).toString() )
    {}

    bool operator==( const CloudWhitelabelInformation &other ) const
    {
      return siteTitle == other.siteTitle && logoMain == other.logoMain && logoNavbar == other.logoNavbar && favicon == other.favicon;
    }

    bool operator!=( const CloudWhitelabelInformation &other ) const
    {
      return !( *this == other );
    }

    QVariantMap toVariantMap() const
    {
      return {
        { QStringLiteral( "site_title" ), siteTitle },
        { QStringLiteral( "logo_main" ), logoMain },
        { QStringLiteral( "logo_navbar" ), logoNavbar },
        { QStringLiteral( "favicon" ), favicon },
      };
    }

    QString siteTitle;
    QString logoMain;
    QString logoNavbar;
    QString favicon;
};

Q_DECLARE_METATYPE( CloudWhitelabelInformation )


class QFieldCloudUtils : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString errorCodeOverQuota READ errorCodeOverQuota CONSTANT )
    Q_PROPERTY( QString errorCodePlanInsufficient READ errorCodePlanInsufficient CONSTANT )

  public:
    static void setLocalCloudDirectory( const QString &path );

    static const QString localCloudDirectory();

    static const QString localProjectFilePath( const QString &username, const QString &projectId );

    static bool isCloudAction( const QgsMapLayer *layer );

    Q_INVOKABLE static const QString getProjectId( const QString &fileName );

    Q_INVOKABLE static QString userFriendlyErrorString( const QString &errorString );

    Q_INVOKABLE static QString documentationFromErrorString( const QString &errorString );

    static void setProjectSetting( const QString &projectId, const QString &setting, const QVariant &value );

    static const QVariant projectSetting( const QString &projectId, const QString &setting, const QVariant &defaultValue = QVariant() );

    Q_INVOKABLE static bool hasPendingAttachments( const QString &username );

    static const QMultiMap<QString, QString> getPendingAttachments( const QString &username );

    Q_INVOKABLE static void addPendingAttachments( const QString &username, const QString &projectId, const QStringList &fileNames, QFieldCloudConnection *cloudConnection = nullptr, const bool &checkSumCheck = false );

    Q_INVOKABLE static void removePendingAttachment( const QString &username, const QString &projectId, const QString &fileName );

    Q_INVOKABLE static QString subscriptionManagementUrl( const QString &serverUrl, const QString &plan, const QString &projectOwner, const QString &username );

  private:
    static void writeToAttachmentsFile( const QString &username, const QString &projectId, const QStringList &fileNames, const QHash<QString, QString> *fileChecksumMap, const bool &checkSumCheck, QFieldCloudConnection *cloudConnection = nullptr );
    static void writeFilesFromDirectory( const QString &dirPath, const QString &projectId, const QHash<QString, QString> *fileChecksumMap, const bool &checkSumCheck, QTextStream &attachmentsStream );
    static void writeFileDetails( const QString &fileName, const QString &projectId, const QHash<QString, QString> *fileChecksumMap, const bool &checkSumCheck, QTextStream &attachmentsStream );

    static inline const QString errorCodeOverQuota() { return QStringLiteral( "over_quota" ); };
    static inline const QString errorCodePlanInsufficient() { return QStringLiteral( "permission_denied_plan_insufficient" ); };
};

#endif // QFIELDCLOUDUTILS_H
```


