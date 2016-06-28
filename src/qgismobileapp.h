/***************************************************************************
                            qgismobileapp.h
                              -------------------
              begin                : Wed Apr 04 10:48:28 CET 2012
              copyright            : (C) 2012 by Marco Bernasocchi
              email                : marco@bernawebdesign.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGISMOBILEAPP_H
#define QGISMOBILEAPP_H

// Qt includes
#include <QtQuick/QQuickWindow>
#include <QtQuick/QQuickView>

// QGIS includes
#include <qgsapplication.h>
#include <qgslayertreemapcanvasbridge.h>
#include <qgslayertreemodel.h>
#include <qgsmaplayerproxymodel.h>

// QGIS mobile includes
#include "featuremodel.h"
#include "featurelistmodel.h"
#include "settings.h"

#include "platformutilities.h"
#if defined(Q_OS_ANDROID)
#include "androidplatformutilities.h"
#endif

class AppInterface;

class QgisMobileapp : public QQuickView
{
    Q_OBJECT
  public:
    QgisMobileapp( QgsApplication *app, QWindow* parent = 0 );
    ~QgisMobileapp();

    // interface methods
    void identifyFeatures( const QPointF& point );

    void showFeatureForm( const Feature& feature );

    /**
     * When called loads the project file found at path.
     *
     * @param path The project file to load
     */
    void loadProjectFile( const QString& path );

  signals:
    /**
     * Emitted when a project file is being loaded
     *
     * @param filename The filename of the project that is being loaded
     */
    void loadProjectStarted( const QString& filename );

    /**
     * Emitted when the project is fully loaded
     */
    void loadProjectEnded();

  private slots:

    /**
     * When called loads the last project
     */
    void loadLastProject();

    /**
     * Is called when a project is read.
     * Saves the last project location for auto-load on next start.
     * @param doc The xml content
     */
    void onReadProject( const QDomDocument& doc );

    void onAfterFirstRendering();

    void onLayerAdded( QgsMapLayer* ml );

  private:
    void initDeclarative();

    void loadProjectQuirks();

    QgsMapCanvas* mMapCanvas;
    QgsLayerTreeMapCanvasBridge* mLayerTreeCanvasBridge;
    QgsLayerTreeModel* mLayerTree;
    QgsMapLayerProxyModel* mLayerList;
    AppInterface* mIface;
    FeatureModel mFeatureModel;
    FeatureListModel mFeatureListModel;
    Settings mSettings;
    bool mFirstRenderingFlag;
#if defined(Q_OS_ANDROID)
    AndroidPlatformUtilities mPlatformUtils;
#else
    PlatformUtilities mPlatformUtils;
#endif
};

Q_DECLARE_METATYPE( QGis::GeometryType )

#endif // QGISMOBILEAPP_H
