/***************************************************************************
    qfieldcloudutils.cpp
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

/**
 * \ingroup core
 */
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

/**
 * \ingroup core
 */
class QFieldCloudUtils : public QObject
{
    Q_OBJECT

  public:
    /**
     * Sets the local cloud directory.
     * Must be set before the first call to localCloudDirectory.
     */
    static void setLocalCloudDirectory( const QString &path );

    /**
     * Returns the path to the local cloud directory.
     * By default inside the user profile unless overwritten with setLocalCloudDirectory
     */
    static const QString localCloudDirectory();

    static const QString localProjectFilePath( const QString &username, const QString &projectId );

    /**
     * Returns if the layer action has do be handled with QFieldCloud.
     *
     * @param layer to be checked
     * @return const bool true if the layer has the action not selected 'no_action' (on online databases) or 'remove'
     */
    static bool isCloudAction( const QgsMapLayer *layer );

    /**
     * Returns the cloud project id.
     *
     * @param fileName file name of the project to be checked
     * @return const QString either UUID-like string or a null string in case of failure
     */
    Q_INVOKABLE static const QString getProjectId( const QString &fileName );

    /**
     * Returns a user-friendly error message.
     *
     * @param errorString the error string to be processed.
     * @return A user-friendly error message that will be displayed to the user, translated based on received error code.
     */
    Q_INVOKABLE static QString userFriendlyErrorString( const QString &errorString );

    /**
     * Returns a documentation page hyperlink related to the provided error string.
     *
     * @param errorString the error string to be processed
     * @return The hyperlink to the documentation page related to the provided error code, or an empty string if no match is found.
     */
    Q_INVOKABLE static QString documentationFromErrorString( const QString &errorString );

    //! Sets a \a setting to a given \a value for project with given \a projectId to the permanent storage.
    static void setProjectSetting( const QString &projectId, const QString &setting, const QVariant &value );

    //! Gets a \a setting value for project with given \a projectId from the permanent storage. Return \a defaultValue if not present.
    static const QVariant projectSetting( const QString &projectId, const QString &setting, const QVariant &defaultValue = QVariant() );

    //! Returns TRUE if pending attachments are detected.
    Q_INVOKABLE static bool hasPendingAttachments();

    //! Returns the list of attachments that have not yet been uploaded to the cloud.
    static const QMultiMap<QString, QString> getPendingAttachments();

    //! Adds an array of \a fileNames for a given \a projectId to the pending attachments list
    Q_INVOKABLE static void addPendingAttachments( const QString &projectId, const QStringList &fileNames );

    //! Adds removes a \a fileName for a given \a projectId to the pending attachments list
    static void removePendingAttachment( const QString &projectId, const QString &fileName );

  private:
    static inline const QString errorCodeOverQuota { QStringLiteral( "over_quota" ) };
};

#endif // QFIELDCLOUDUTILS_H
