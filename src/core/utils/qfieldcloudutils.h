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
class TestDeltaFileWrapper;
class TestLayerObserver;

class QFieldCloudUtils : public QObject
{
    Q_OBJECT

  public:
    static const QString localCloudDirectory();
    static const QString localProjectFilePath( const QString &username, const QString &projectId );

    /**
     * Get if the \layer action has do be handled with QFieldCloud.
     *
     * @param layer to be checked
     * @return const bool true if the layer has the action not selected 'no_action' (on online databases) or 'remove'
     */
    static bool isCloudAction( const QgsMapLayer *layer );

    /**
     * Get the cloud project id.
     *
     * @param project project to be checked, const
     * @return const QString either UUID-like string or a null string in case of failure
     */
    Q_INVOKABLE static const QString getProjectId( const QgsProject *project );

    /**
     * Get the cloud project id.
     *
     * @param project project to be checked
     * @return const QString either UUID-like string or a null string in case of failure
     */
    Q_INVOKABLE static const QString getProjectId( QgsProject *project );

  private:
    static QString sQgisSettingsDirPath;

    friend TestDeltaFileWrapper;
    friend TestLayerObserver;
};

#endif // QFIELDCLOUDUTILS_H
