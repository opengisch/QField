

# File qgismobileapp.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qgismobileapp.h**](qgismobileapp_8h.md)

[Go to the documentation of this file](qgismobileapp_8h.md)


```C++
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
#include <qgsunittypes.h>

// QField includes
#include "appcoordinateoperationhandlers.h"
#include "bookmarkmodel.h"
#include "clipboardmanager.h"
#include "cogoregistry.h"
#include "drawingtemplatemodel.h"
#include "pluginmanager.h"
#include "qfield_core_export.h"
#include "qfieldappauthrequesthandler.h"
#include "qfieldurlhandler.h"
#include "qgsgpkgflusher.h"
#include "screendimmer.h"
#include "settings.h"

class AppInterface;
class AppMissingGridHandler;
class BarcodeImageProvider;
class QgsOfflineEditing;
class QgsQuickMapCanvasMap;
class LayerTreeMapCanvasBridge;
class FlatLayerTreeModel;
class LayerTreeModel;
class LegendImageProvider;
class AsyncLegendImageProvider;
class LocalFilesImageProvider;
class ProjectsImageProvider;
class TrackingModel;
class LocatorFiltersModel;
class QgsProject;
class LayerObserver;
class FeatureHistory;
class MessageLogModel;
class QgsPrintLayout;

#define REGISTER_SINGLETON( uri, _class, name ) qmlRegisterSingletonType<_class>( uri, 1, 0, name, []( QQmlEngine *engine, QJSEngine *scriptEngine ) -> QObject * { Q_UNUSED(engine); Q_UNUSED(scriptEngine); return new _class(); } )

class QFIELD_CORE_EXPORT QgisMobileapp : public QQmlApplicationEngine
{
    Q_OBJECT
  public:
    explicit QgisMobileapp( QgsApplication *app, QObject *parent = nullptr );
    ~QgisMobileapp() override;

    QList<QPair<QString, QString>> recentProjects();

    void saveRecentProjects( const QList<QPair<QString, QString>> &projects );

    void removeRecentProject( const QString &path );

    bool loadProjectFile( const QString &path, const QString &name = QString() );

    void reloadProjectFile();

    void readProjectFile();

    QString readProjectEntry( const QString &scope, const QString &key, const QString &def = QString() ) const;

    int readProjectNumEntry( const QString &scope, const QString &key, int def = 0 ) const;

    double readProjectDoubleEntry( const QString &scope, const QString &key, double def = 0.0 ) const;

    bool readProjectBoolEntry( const QString &scope, const QString &key, bool def = false ) const;

    bool print( const QString &layoutName );

    bool printAtlasFeatures( const QString &layoutName, const QList<long long> &featureIds );

    void setScreenDimmerTimeout( int timeoutSeconds );

    bool event( QEvent *event ) override;

    void clearProject();

    static void initDeclarative( QQmlEngine *engine );

  signals:
    void loadProjectTriggered( const QString &filename, const QString &name );

    void loadProjectEnded( const QString &filename, const QString &name );

    void setMapExtent( const QgsRectangle &extent );

  private slots:

    void onAfterFirstRendering();
    void onMapCanvasRefreshed();

  private:
    void registerGlobalVariables();
    void loadProjectQuirks();
    void saveProjectPreviewImage();
    bool printAtlas( QgsPrintLayout *layoutToPrint, const QString &destination );

    QgsOfflineEditing *mOfflineEditing = nullptr;
    LayerTreeMapCanvasBridge *mLayerTreeCanvasBridge = nullptr;
    FlatLayerTreeModel *mFlatLayerTree = nullptr;
    QgsMapLayerProxyModel *mLayerList = nullptr;
    AppInterface *mIface = nullptr;
    Settings mSettings;
    QPointer<QgsQuickMapCanvasMap> mMapCanvas;
    bool mFirstRenderingFlag;
    LegendImageProvider *mLegendImageProvider = nullptr;
    AsyncLegendImageProvider *mAsyncLegendImageProvider = nullptr;
    LocalFilesImageProvider *mLocalFilesImageProvider = nullptr;
    ProjectsImageProvider *mProjectsImageProvider = nullptr;
    BarcodeImageProvider *mBarcodeImageProvider = nullptr;

    QgsProject *mProject = nullptr;
    QString mProjectFilePath;
    QString mProjectFileName;

    std::unique_ptr<QgsGpkgFlusher> mGpkgFlusher;
    std::unique_ptr<LayerObserver> mLayerObserver;
    std::unique_ptr<FeatureHistory> mFeatureHistory;
    std::unique_ptr<ClipboardManager> mClipboardManager;

    QFieldAppAuthRequestHandler *mAuthRequestHandler = nullptr;

    BookmarkModel *mBookmarkModel = nullptr;
    DrawingTemplateModel *mDrawingTemplateModel = nullptr;
    MessageLogModel *mMessageLogModel = nullptr;

    PluginManager *mPluginManager = nullptr;

    std::unique_ptr<CogoRegistry> mCogoRegistry;

    // Dummy objects. We are not able to call static functions from QML, so we need something here.
    QgsCoordinateReferenceSystem mCrsFactory;
    QgsUnitTypes mUnitTypes;
    QgsExifTools mExifTools;

    TrackingModel *mTrackingModel = nullptr;

    AppMissingGridHandler *mAppMissingGridHandler = nullptr;

    std::unique_ptr<ScreenDimmer> mScreenDimmer;
    std::unique_ptr<QFieldUrlHandler> mUrlHandler;
    QgsApplication *mApp;
};


Q_DECLARE_METATYPE( QgsFeatureId )
Q_DECLARE_METATYPE( QgsAttributes )
Q_DECLARE_METATYPE( QgsFieldConstraints )

#endif // QGISMOBILEAPP_H
```


