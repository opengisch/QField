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
#include "appcoordinateoperationhandlers.h"
#include "multifeaturelistmodel.h"
#include "settings.h"
#include "focusstack.h"
#include "qgsquickutils.h"
#include "qgsgpkgflusher.h"
#include "geometryeditorsmodel.h"

#include "qfieldappauthrequesthandler.h"

#include "qfield_core_export.h"

#include "platformutilities.h"
#if defined(Q_OS_ANDROID)
#include "androidplatformutilities.h"
#endif

class AppInterface;
class AppMissingGridHandler;
class QgsOfflineEditing;
class QgsQuickMapCanvasMap;
class LayerTreeMapCanvasBridge;
class FlatLayerTreeModel;
class LayerTreeModel;
class LegendImageProvider;
class TrackingModel;
class QgsProject;


#define REGISTER_SINGLETON(uri, _class, name) qmlRegisterSingletonType<_class>( uri, 1, 0, name, [] ( QQmlEngine *engine, QJSEngine *scriptEngine ) -> QObject * { Q_UNUSED(engine); Q_UNUSED(scriptEngine); return new _class(); } )

#define SUPPORTED_PROJECT_EXTENSIONS QStringList( { QStringLiteral( "qgs" ), QStringLiteral( "qgz" ) } )
#define SUPPORTED_VECTOR_EXTENSIONS  QStringList( { QStringLiteral( "gpkg" ), QStringLiteral( "shp" ), QStringLiteral( "kml" ), QStringLiteral( "kmz" ), QStringLiteral( "geojson" ), QStringLiteral( "json" ), QStringLiteral( "pdf" ), QStringLiteral( "gpx" ), QStringLiteral( "zip" ) } )
#define SUPPORTED_RASTER_EXTENSIONS  QStringList( { QStringLiteral( "tif" ), QStringLiteral( "pdf" ), QStringLiteral( "jpg" ), QStringLiteral( "png" ), QStringLiteral( "gpkg" ), QStringLiteral( "jp2" ), QStringLiteral( "webp" ), QStringLiteral( "zip" ) } )

class QFIELD_CORE_EXPORT QgisMobileapp : public QQmlApplicationEngine
{
    Q_OBJECT
  public:
    explicit QgisMobileapp( QgsApplication *app, QObject *parent = nullptr );
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
     * Removes the project with a given \a path from the list of recent projects
     */
    void removeRecentProject( const QString &path );

    /**
     * When called loads the last project
     */
    void loadLastProject();

    /**
     * Set the project or dataset file path to be loaded.
     *
     * \param path The project or dataset file to load
     * \param name The project name
     * \note The actual loading is done in readProjectFile
     */
    void loadProjectFile( const QString &path, const QString &name = QString() );

    /**
     * Reloads the current project
     *
     * \param path The project file to load
     * \param name The project name
     * \note It does not reset the Auth Request Handler.
     * \note The actual loading is done in readProjectFile
     */
    void reloadProjectFile();

    /**
     * Reads and opens the project file set in the loadProjectFile function
     */
    void readProjectFile();

    void print( int layoutIndex );

    bool event( QEvent *event ) override;

  signals:
    /**
     * Emitted when a project file is being loaded
     *
     * @param filename The filename of the project that is being loaded
     * @param projectname The project name that is being loaded
     */
    void loadProjectTriggered( const QString &filename, const QString &name );

    /**
     * Emitted when the project is fully loaded
     */
    void loadProjectEnded( const QString &filename, const QString &name );

    /**
     * Emitted when a map canvas extent change is needed
     */
    void setMapExtent( const QgsRectangle &extent );

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
    QgsMapLayerProxyModel *mLayerList = nullptr;
    AppInterface *mIface = nullptr;
    Settings mSettings;
    QgsQuickMapCanvasMap *mMapCanvas = nullptr;
    bool mFirstRenderingFlag;
    LegendImageProvider *mLegendImageProvider = nullptr;

    QgsProject *mProject = nullptr;
    QString mProjectFilePath;
    QString mProjectFileName;

    std::unique_ptr<QgsGpkgFlusher> mGpkgFlusher;
    QFieldAppAuthRequestHandler *mAuthRequestHandler = nullptr;

    // Dummy objects. We are not able to call static functions from QML, so we need something here.
    QgsCoordinateReferenceSystem mCrsFactory;
    QgsUnitTypes mUnitTypes;
    QgsExifTools mExifTools;

    TrackingModel *mTrackingModel = nullptr;

    AppMissingGridHandler *mAppMissingGridHandler = nullptr;
};

Q_DECLARE_METATYPE( QgsWkbTypes::GeometryType )
Q_DECLARE_METATYPE( QgsFeatureId )
Q_DECLARE_METATYPE( QgsFeatureIds )
Q_DECLARE_METATYPE( QgsAttributes )
Q_DECLARE_METATYPE( QVariant::Type )
Q_DECLARE_METATYPE( QgsFieldConstraints )


#endif // QGISMOBILEAPP_H
