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
#include <QtQml/QQmlApplicationEngine>

// QGIS includes
#include <qgsapplication.h>
#include <qgsexiftools.h>
#include <qgsmaplayerproxymodel.h>
#include <qgsconfig.h>

// QGIS mobile includes
#include "multifeaturelistmodel.h"
#include "settings.h"
#include "focusstack.h"
#include "qgsquickutils.h"
#include "qgsgpkgflusher.h"
#include "geometryeditorsmodel.h"

#if VERSION_INT >= 30600
#include "qfieldappauthrequesthandler.h"
#endif

#include "platformutilities.h"
#if defined(Q_OS_ANDROID)
#include "androidplatformutilities.h"
#endif

class AppInterface;
class QgsOfflineEditing;
class QgsQuickMapCanvasMap;
class LayerTreeMapCanvasBridge;
class FlatLayerTreeModel;
class LayerTreeModel;
class LegendImageProvider;
class TrackingModel;
class QgsProject;


#define REGISTER_SINGLETON(uri, _class, name) qmlRegisterSingletonType<_class>( uri, 1, 0, name, [] ( QQmlEngine *engine, QJSEngine *scriptEngine ) -> QObject * { Q_UNUSED(engine); Q_UNUSED(scriptEngine); return new _class(); } )


class QgisMobileapp : public QQmlApplicationEngine
{
    Q_OBJECT
  public:
    QgisMobileapp( QgsApplication *app, QObject *parent = nullptr );
    ~QgisMobileapp() override;

    /**
     * Returns a list of recent projects.
     */
    QList<QPair<QString, QString>> recentProjects();

    /**
     * Saves a list of recent \a projects.
     */
    void saveRecentProjects( QList<QPair<QString, QString>> &projects );

    /**
     * When called loads the last project
     */
    void loadLastProject();

    /**
     * When called loads the project file found at path.
     *
     * @param path The project file to load
     */
    void loadProjectFile( const QString &path );
    /**
     * Loads the project file found at path.
     * It does not reset the Auth Request Handler.
     *
     * @param path The project file to load
     */
    void reloadProjectFile( const QString &path );
    void print( int layoutIndex );

    bool event( QEvent *event ) override;

  signals:
    /**
     * Emitted when a project file is being loaded
     *
     * @param filename The filename of the project that is being loaded
     */
    void loadProjectStarted( const QString &filename );

    /**
     * Emitted when the project is fully loaded
     */
    void loadProjectEnded();

  private slots:

    /**
     * Is called when a project is read.
     * Saves the last project location for auto-load on next start.
     * @param doc The xml content
     */
    void onReadProject( const QDomDocument &doc );

    void onAfterFirstRendering();

  private:
    void initDeclarative();

    void loadProjectQuirks();

    QgsOfflineEditing *mOfflineEditing = nullptr;
    LayerTreeMapCanvasBridge *mLayerTreeCanvasBridge = nullptr;
    FlatLayerTreeModel *mFlatLayerTree = nullptr;
    LayerTreeModel *mLayerTree = nullptr;
    QgsMapLayerProxyModel *mLayerList = nullptr;
    AppInterface *mIface = nullptr;
    Settings mSettings;
    QgsQuickMapCanvasMap *mMapCanvas = nullptr;
    bool mFirstRenderingFlag;
    LegendImageProvider *mLegendImageProvider = nullptr;

    QgsProject *mProject = nullptr;
    std::unique_ptr<QgsGpkgFlusher> mGpkgFlusher;
#if VERSION_INT >= 30600
    QFieldAppAuthRequestHandler *mAuthRequestHandler = nullptr;
#endif
    // Dummy objects. We are not able to call static functions from QML, so we need something here.
    QgsCoordinateReferenceSystem mCrsFactory;
    QgsUnitTypes mUnitTypes;
    QgsExifTools mExifTools;

    TrackingModel *mTrackingModel = nullptr;

#if defined(Q_OS_ANDROID)
    AndroidPlatformUtilities mPlatformUtils;
#else
    PlatformUtilities mPlatformUtils;
#endif
};

Q_DECLARE_METATYPE( QgsWkbTypes::GeometryType )
Q_DECLARE_METATYPE( QgsFeatureId )
Q_DECLARE_METATYPE( QgsFeatureIds )
Q_DECLARE_METATYPE( QgsAttributes )
Q_DECLARE_METATYPE( QVariant::Type )
Q_DECLARE_METATYPE( QgsFieldConstraints )


#endif // QGISMOBILEAPP_H
