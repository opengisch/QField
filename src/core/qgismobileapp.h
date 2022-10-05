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
#include <qgsconfig.h>
#include <qgsexiftools.h>
#include <qgsmaplayerproxymodel.h>

// QField includes
#include "appcoordinateoperationhandlers.h"
#include "bookmarkmodel.h"
#include "focusstack.h"
#include "geometryeditorsmodel.h"
#include "multifeaturelistmodel.h"
#include "platformutilities.h"
#include "qfield_core_export.h"
#include "qfieldappauthrequesthandler.h"
#include "qgsgpkgflusher.h"
#include "screendimmer.h"
#include "settings.h"

class AppInterface;
class AppMissingGridHandler;
class QgsOfflineEditing;
class QgsQuickMapCanvasMap;
class LayerTreeMapCanvasBridge;
class FlatLayerTreeModel;
class LayerTreeModel;
class LegendImageProvider;
class LocalFilesImageProvider;
class TrackingModel;
class LocatorFiltersModel;
class QgsProject;
class LayerObserver;
class MessageLogModel;
class QgsPrintLayout;

#define REGISTER_SINGLETON( uri, _class, name ) qmlRegisterSingletonType<_class>( uri, 1, 0, name, []( QQmlEngine *engine, QJSEngine *scriptEngine ) -> QObject * { Q_UNUSED(engine); Q_UNUSED(scriptEngine); return new _class(); } )

#define SUPPORTED_PROJECT_EXTENSIONS QStringList( { QStringLiteral( "qgs" ), QStringLiteral( "qgz" ) } )
#define SUPPORTED_VECTOR_EXTENSIONS QStringList( { QStringLiteral( "gpkg" ), QStringLiteral( "shp" ), QStringLiteral( "kml" ), QStringLiteral( "kmz" ), QStringLiteral( "geojson" ), QStringLiteral( "json" ), QStringLiteral( "pdf" ), QStringLiteral( "gpx" ), QStringLiteral( "gml" ), QStringLiteral( "mif" ), QStringLiteral( "fgb" ), QStringLiteral( "db" ), QStringLiteral( "sqlite" ), QStringLiteral( "vrt" ), QStringLiteral( "zip" ) } )
#define SUPPORTED_RASTER_EXTENSIONS QStringList( { QStringLiteral( "tif" ), QStringLiteral( "tiff" ), QStringLiteral( "pdf" ), QStringLiteral( "jpg" ), QStringLiteral( "jpeg" ), QStringLiteral( "png" ), QStringLiteral( "gpkg" ), QStringLiteral( "jp2" ), QStringLiteral( "webp" ), QStringLiteral( "vrt" ), QStringLiteral( "zip" ) } )

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
     * \note It does not reset the Auth Request Handler.
     * \note The actual loading is done in readProjectFile
     */
    void reloadProjectFile();

    /**
     * Reads and opens the project file set in the loadProjectFile function
     */
    void readProjectFile();

    /**
     * Reads a string from the specified \a scope and \a key from the currently opened project
     *
     * \param scope entry scope (group) name
     * \param key entry key name. Keys are '/'-delimited entries, implying a hierarchy of keys and corresponding values
     * \param def default value to return if the specified \a key does not exist within the \a scope
     *
     * \returns entry value as string from \a scope given its \a key
     */
    QString readProjectEntry( const QString &scope, const QString &key, const QString &def = QString() ) const;

    /**
     * Reads an integer from the specified \a scope and \a key from the currently opened project
     *
     * \param scope entry scope (group) name
     * \param key entry key name. Keys are '/'-delimited entries, implying a hierarchy of keys and corresponding values
     * \param def default value to return if the specified \a key does not exist within the \a scope
     *
     * \returns entry value as integer from \a scope given its \a key
     */
    int readProjectNumEntry( const QString &scope, const QString &key, int def = 0 ) const;

    /**
     * Reads a double from the specified \a scope and \a key from the currently opened project
     *
     * \param scope entry scope (group) name
     * \param key entry key name. Keys are '/'-delimited entries, implying a hierarchy of keys and corresponding values
     * \param def default value to return if the specified \a key does not exist within the \a scope
     *
     * \returns entry value as double from \a scope given its \a key
     */
    double readProjectDoubleEntry( const QString &scope, const QString &key, double def = 0.0 ) const;

    /**
     * Prints a given layout from the currently opened project to a PDF file
     * \param layoutName the layout name that will be printed
     * \return TRUE if the layout was successfully printed
     */
    bool print( const QString &layoutName );

    /**
     * Prints a given atlas-driven layout from the currently opened project to one or more PDF files
     * \param layoutName the layout name that will be printed
     * \param featureIds the features from the atlas coverage vector layer that will be used to print the layout
     * \return TRUE if the layout was successfully printed
     */
    bool printAtlasFeatures( const QString &layoutName, const QList<long long> &featureIds );

    /**
     * Sets the screen dimmer timeout in seconds
     * \note setting the timeout value to 0 will disable the screen dimmer
     */
    void setScreenDimmerTimeout( int timeoutSeconds );

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

    void requestQuit();

  private:
    void initDeclarative();
    void loadProjectQuirks();
    bool printAtlas( QgsPrintLayout *layoutToPrint, const QString &destination );

    QgsOfflineEditing *mOfflineEditing = nullptr;
    LayerTreeMapCanvasBridge *mLayerTreeCanvasBridge = nullptr;
    FlatLayerTreeModel *mFlatLayerTree = nullptr;
    QgsMapLayerProxyModel *mLayerList = nullptr;
    AppInterface *mIface = nullptr;
    Settings mSettings;
    QgsQuickMapCanvasMap *mMapCanvas = nullptr;
    bool mFirstRenderingFlag;
    LegendImageProvider *mLegendImageProvider = nullptr;
    LocalFilesImageProvider *mLocalFilesImageProvider = nullptr;

    QgsProject *mProject = nullptr;
    QString mProjectFilePath;
    QString mProjectFileName;

    std::unique_ptr<QgsGpkgFlusher> mGpkgFlusher;
    std::unique_ptr<LayerObserver> mLayerObserver;
    QFieldAppAuthRequestHandler *mAuthRequestHandler = nullptr;

    std::unique_ptr<BookmarkModel> mBookmarkModel;
    std::unique_ptr<MessageLogModel> mMessageLogModel;

    // Dummy objects. We are not able to call static functions from QML, so we need something here.
    QgsCoordinateReferenceSystem mCrsFactory;
    QgsUnitTypes mUnitTypes;
    QgsExifTools mExifTools;

    TrackingModel *mTrackingModel = nullptr;

    AppMissingGridHandler *mAppMissingGridHandler = nullptr;

    std::unique_ptr<ScreenDimmer> mScreenDimmer;
    QgsApplication *mApp;
};

Q_DECLARE_METATYPE( QgsWkbTypes::GeometryType )
Q_DECLARE_METATYPE( QgsFeatureId )
#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
Q_DECLARE_METATYPE( QgsFeatureIds )
#endif
Q_DECLARE_METATYPE( QgsAttributes )
Q_DECLARE_METATYPE( QVariant::Type )
Q_DECLARE_METATYPE( QgsFieldConstraints )

#endif // QGISMOBILEAPP_H
