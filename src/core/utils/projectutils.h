/***************************************************************************
  projectutils.h - ProjectUtils

 ---------------------
 begin                : 19.04.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : mathieu@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PROJECTUTILS_H
#define PROJECTUTILS_H

#include "gnsspositioninformation.h"

#include <QObject>
#include <qgsproject.h>

/**
 * \ingroup core
 */
class ProjectUtils : public QObject
{
    Q_OBJECT

  public:
    explicit ProjectUtils( QObject *parent = nullptr );

    /**
     * Returns a map of all registered layers by layer ID.
     * \note This function mimics QgsProject::mapLayers with a return type
     * that is QML compatible.
     */
    Q_INVOKABLE static QVariantMap mapLayers( QgsProject *project );

    /**
     * Adds a map \a layer to a \a project layers registry.
     */
    Q_INVOKABLE static bool addMapLayer( QgsProject *project, QgsMapLayer *layer );

    /**
     * Removes a map \a layer from a \a project layers registry.
     */
    Q_INVOKABLE static void removeMapLayer( QgsProject *project, QgsMapLayer *layer );

    /**
     * Removes a map layer from a project layers registry matching a given layer ID.
     * \param project the project
     * \param layerId the layer ID
     */
    Q_INVOKABLE static void removeMapLayer( QgsProject *project, const QString &layerId );

    /**
     * Returns the transaction mode for a given \a project.
     * \note To be removed when QField updates to QGIS 3.38.
     */
    Q_INVOKABLE static Qgis::TransactionMode transactionMode( QgsProject *project = nullptr );

    /**
     * Returns the \a project title.
     */
    Q_INVOKABLE static QString title( QgsProject *project = nullptr );

    /**
     * Creates a new project and return the local path of the project file.
     *
     * The available \a options are:
     * - title: the project title
     * - basemap: the basemap type (color, lightgray, darkgray, custom, blank)
     * - basemap_url: a custom basemap URL (XYZ raster layer or vector tile layer style JSON)
     * - notes: set to TRUE to add a notes layer
     * - camera_capture: set to TRUE to add an image/video capture field to the notes layer
     * - tracks: set to TRUE to add a tracks layer
     * - track_on_launch: set to TRUE to start tracking position on project launch
     */
    Q_INVOKABLE static QString createProject( const QVariantMap &options, const GnssPositionInformation &positionInformation = GnssPositionInformation() );
};

#endif // PROJECTUTILS_H
