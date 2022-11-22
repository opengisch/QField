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
     * Returns if the \layer action has do be handled with QFieldCloud.
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

    //! Sets a \a setting to a given \a value for project with given \a projectId to the permanent storage.
    static void setProjectSetting( const QString &projectId, const QString &setting, const QVariant &value );

    //! Gets a \a setting value for project with given \a projectId from the permanent storage. Return \a defaultValue if not present.
    static const QVariant projectSetting( const QString &projectId, const QString &setting, const QVariant &defaultValue = QVariant() );

    //! Returns the list of attachments that have not yet been uploaded to the cloud.
    static const QMultiMap<QString, QString> getPendingAttachments();

    //! Adds an \a fileName for a given \a projectId to the pending attachments list
    static void addPendingAttachment( const QString &projectId, const QString &fileName );

    //! Adds removes a \a fileName for a given \a projectId to the pending attachments list
    static void removePendingAttachment( const QString &projectId, const QString &fileName );
};

#endif // QFIELDCLOUDUTILS_H
