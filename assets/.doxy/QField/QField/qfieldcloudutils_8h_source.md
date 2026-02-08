

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

class QFieldCloudUtils : public QObject
{
    Q_OBJECT

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

  private:
    static inline const QString errorCodeOverQuota { QStringLiteral( "over_quota" ) };

    static void writeToAttachmentsFile( const QString &username, const QString &projectId, const QStringList &fileNames, const QHash<QString, QString> *fileChecksumMap, const bool &checkSumCheck, QFieldCloudConnection *cloudConnection = nullptr );

    static void writeFilesFromDirectory( const QString &dirPath, const QString &projectId, const QHash<QString, QString> *fileChecksumMap, const bool &checkSumCheck, QTextStream &attachmentsStream );

    static void writeFileDetails( const QString &fileName, const QString &projectId, const QHash<QString, QString> *fileChecksumMap, const bool &checkSumCheck, QTextStream &attachmentsStream );
};

#endif // QFIELDCLOUDUTILS_H
```


