

# File qfcloudutils.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**qfcloudutils.h**](qfcloudutils_8h.md)

[Go to the documentation of this file](qfcloudutils_8h.md)


```C++
/***************************************************************************
    qfcloudutils.h
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

#ifndef QFCLOUDUTILS_H
#define QFCLOUDUTILS_H

#include <qfcloudprojectsmodel.h>
#include <qgsmaplayer.h>
#include <qgsproject.h>


class QString;
class QfCloudProjectsModel;
class DeltaFileWrapperTest;
class TestLayerObserver;

struct QfCloudUserInformation
{
    Q_GADGET

    Q_PROPERTY( QString username MEMBER username )
    Q_PROPERTY( QString email MEMBER email )
    Q_PROPERTY( QString firstName MEMBER firstName )
    Q_PROPERTY( QString lastName MEMBER lastName )
    Q_PROPERTY( QString fullName MEMBER fullName )

  public:
    QfCloudUserInformation() = default;

    QfCloudUserInformation( const QString &username, const QString &email, const QString &firstName = QString(), const QString &lastName = QString(), const QString &fullName = QString() )
      : username( username )
      , email( email )
      , firstName( firstName )
      , lastName( lastName )
      , fullName( fullName )
    {}

    explicit QfCloudUserInformation( const QJsonObject &cloudUserInformation )
      : username( cloudUserInformation.value( QStringLiteral( "username" ) ).toString() )
      , email( cloudUserInformation.value( QStringLiteral( "email" ) ).toString() )
      , firstName( cloudUserInformation.value( QStringLiteral( "first_name" ) ).toString() )
      , lastName( cloudUserInformation.value( QStringLiteral( "last_name" ) ).toString() )
      , fullName( cloudUserInformation.value( QStringLiteral( "full_name" ) ).toString() )
    {}

    bool operator==( const QfCloudUserInformation &other ) const
    {
      return username == other.username && email == other.email && firstName == other.firstName && lastName == other.lastName && fullName == other.fullName;
    }

    QJsonObject toJson() const
    {
      QJsonObject cloudUserInformation;

      cloudUserInformation.insert( "username", username );
      cloudUserInformation.insert( "email", email );
      cloudUserInformation.insert( "first_name", firstName );
      cloudUserInformation.insert( "last_name", lastName );
      cloudUserInformation.insert( "full_name", fullName );

      return cloudUserInformation;
    }

    bool isEmpty() const
    {
      // Allow for empty first, last, and full name
      return username.isEmpty() && email.isEmpty();
    }

    QString username;
    QString email;
    QString firstName;
    QString lastName;
    QString fullName;
};


struct QfCloudDelta
{
    Q_GADGET

  public:
    enum Status
    {
      PendingStatus,
      BusyStatus,
      AppliedStatus,
      ConflictStatus,
      NotAppliedStatus,
      ErrorStatus,
      IgnoredStatus,
      UnpermittedStatus,
    };

    Q_ENUM( Status )

    QUuid id;
    QUuid deltafileId;
    QString createdBy;
    QDateTime createdAt;
    QDateTime updatedAt;
    Status status = PendingStatus;
    QString summary;
    QString output;
};

Q_DECLARE_METATYPE( QfCloudDelta )


struct QfCloudSubscriptionInformation
{
    Q_GADGET

    Q_PROPERTY( QString plan MEMBER plan )
    Q_PROPERTY( double storageTotal MEMBER storageTotal )
    Q_PROPERTY( double storageUsed MEMBER storageUsed )
    Q_PROPERTY( double storageThresholdWarning MEMBER storageThresholdWarning )
    Q_PROPERTY( double storageThresholdCritical MEMBER storageThresholdCritical )
    Q_PROPERTY( QString status MEMBER status )

  public:
    QfCloudSubscriptionInformation() = default;

    explicit QfCloudSubscriptionInformation( const QJsonObject &subscriptionInformation )
      : plan( subscriptionInformation.value( QStringLiteral( "plan_display_name" ) ).toString() )
      , storageTotal( subscriptionInformation.value( QStringLiteral( "active_storage_total_bytes" ) ).toDouble() )
      , storageUsed( subscriptionInformation.value( QStringLiteral( "storage_used_bytes" ) ).toDouble() )
      , storageThresholdWarning( subscriptionInformation.value( QStringLiteral( "plan_storage_threshold_warning_bytes" ) ).toDouble() )
      , storageThresholdCritical( subscriptionInformation.value( QStringLiteral( "plan_storage_threshold_critical_bytes" ) ).toDouble() )
      , status( subscriptionInformation.value( QStringLiteral( "status" ) ).toString() )
    {}

    bool operator==( const QfCloudSubscriptionInformation &other ) const
    {
      return plan == other.plan && storageTotal == other.storageTotal && storageUsed == other.storageUsed && storageThresholdWarning == other.storageThresholdWarning && storageThresholdCritical == other.storageThresholdCritical && status == other.status;
    }

    QString plan;
    double storageTotal = 0;
    double storageUsed = 0;
    double storageThresholdWarning = 0;
    double storageThresholdCritical = 0;
    QString status;
};

Q_DECLARE_METATYPE( QfCloudSubscriptionInformation )


struct QfCloudWhitelabelInformation
{
    Q_GADGET

    Q_PROPERTY( QString siteTitle MEMBER siteTitle )
    Q_PROPERTY( QString logoMain MEMBER logoMain )
    Q_PROPERTY( QString logoNavbar MEMBER logoNavbar )
    Q_PROPERTY( QString favicon MEMBER favicon )

  public:
    QfCloudWhitelabelInformation() = default;

    explicit QfCloudWhitelabelInformation( const QVariantMap &whitelabel )
      : siteTitle( whitelabel.value( QStringLiteral( "site_title" ) ).toString() )
      , logoMain( whitelabel.value( QStringLiteral( "logo_main" ) ).toString() )
      , logoNavbar( whitelabel.value( QStringLiteral( "logo_navbar" ) ).toString() )
      , favicon( whitelabel.value( QStringLiteral( "favicon" ) ).toString() )
    {}

    bool operator==( const QfCloudWhitelabelInformation &other ) const
    {
      return siteTitle == other.siteTitle && logoMain == other.logoMain && logoNavbar == other.logoNavbar && favicon == other.favicon;
    }

    bool operator!=( const QfCloudWhitelabelInformation &other ) const
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

Q_DECLARE_METATYPE( QfCloudWhitelabelInformation )


struct QfCloudServerInformation
{
    Q_GADGET

    Q_PROPERTY( QfCloudWhitelabelInformation whitelabel MEMBER whitelabel )
    Q_PROPERTY( QString signupUrl MEMBER signupUrl )

  public:
    QfCloudServerInformation() = default;

    explicit QfCloudServerInformation( const QVariantMap &serverInformation )
      : whitelabel( serverInformation.value( QStringLiteral( "whitelabel" ) ).toMap() )
      , signupUrl( serverInformation.value( QStringLiteral( "signup_url" ) ).toString() )
    {}

    bool operator==( const QfCloudServerInformation &other ) const
    {
      return whitelabel == other.whitelabel && signupUrl == other.signupUrl;
    }

    bool operator!=( const QfCloudServerInformation &other ) const
    {
      return !( *this == other );
    }

    QVariantMap toVariantMap() const
    {
      return {
        { QStringLiteral( "whitelabel" ), whitelabel.toVariantMap() },
        { QStringLiteral( "signup_url" ), signupUrl },
      };
    }

    QfCloudWhitelabelInformation whitelabel;
    QString signupUrl;
};

Q_DECLARE_METATYPE( QfCloudServerInformation )


class QfCloudUtils : public QObject
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

    Q_INVOKABLE static void addPendingAttachments( const QString &username, const QString &projectId, const QStringList &fileNames, QfCloudConnection *cloudConnection = nullptr, const bool &checkSumCheck = false );

    Q_INVOKABLE static void removePendingAttachment( const QString &username, const QString &projectId, const QString &fileName );

    Q_INVOKABLE static QString subscriptionManagementUrl( const QString &serverUrl, const QString &plan, const QString &projectOwner, const QString &username );


    static QList<QfCloudDelta> parseDeltaJsonDocument( const QJsonDocument &jsonDocument, QString &errorString, bool &isValid );

    static QString summarizeDeltaContent( const QList<QJsonObject> &deltaObjects, const QString &modificationSeparator = QStringLiteral( ", " ), const QString &layerSeparator = QStringLiteral( " / " ) );

  private:
    static void writeToAttachmentsFile( const QString &username, const QString &projectId, const QStringList &fileNames, const QHash<QString, QString> *fileChecksumMap, const bool &checkSumCheck, QfCloudConnection *cloudConnection = nullptr );
    static void writeFilesFromDirectory( const QString &dirPath, const QString &projectId, const QHash<QString, QString> *fileChecksumMap, const bool &checkSumCheck, QTextStream &attachmentsStream );
    static void writeFileDetails( const QString &fileName, const QString &projectId, const QHash<QString, QString> *fileChecksumMap, const bool &checkSumCheck, QTextStream &attachmentsStream );

    static inline const QString errorCodeOverQuota() { return QStringLiteral( "over_quota" ); };
    static inline const QString errorCodePlanInsufficient() { return QStringLiteral( "permission_denied_plan_insufficient" ); };
};

#endif // QFCLOUDUTILS_H
```


