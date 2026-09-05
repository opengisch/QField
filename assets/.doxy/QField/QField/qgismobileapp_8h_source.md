

# File qgismobileapp.h

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qgismobileapp.h**](qgismobileapp_8h.md)

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
#include "qfappauthrequesthandler.h"
#include "qfappcontroller.h"
#include "qfappcoordinateoperationhandlers.h"
#include "qfbookmarkmodel.h"
#include "qfclipboardmanager.h"
#include "qfcogoregistry.h"
#include "qfdrawingtemplatemodel.h"
#include "qffocusstack.h"
#include "qfield_app_export.h"
#include "qfpluginmanager.h"
#include "qfscreendimmer.h"
#include "qfsettings.h"
#include "qfurlhandler.h"
#include "qgsgpkgflusher.h"

class QfAppInterface;
class QfAppMissingGridHandler;
class QfBarcodeImageProvider;
class QfLayerTreeMapCanvasBridge;
class QfFlatLayerTreeModel;
class LayerTreeModel;
class QfLegendImageProvider;
class QfAsyncLegendImageProvider;
class QfLocalFilesImageProvider;
class QfProjectsImageProvider;
class QfTrackingModel;
class QfLocatorFiltersModel;
class QfLayerObserver;
class QfFeatureHistory;
class QfMessageLogModel;
class QgsOfflineEditing;
class QgsQuickMapCanvasMap;
class QgsProject;
class QgsPrintLayout;

class QFIELD_APP_EXPORT QgisMobileapp : public QQmlApplicationEngine, public QfAppController
{
    Q_OBJECT
  public:
    explicit QgisMobileapp( QgsApplication *app, QObject *parent = nullptr );
    ~QgisMobileapp() override;

    bool loadProjectFile( const QString &path, const QString &name = QString() ) override;

    void reloadProjectFile() override;

    void readProjectFile() override;

    QString readProjectEntry( const QString &scope, const QString &key, const QString &def = QString() ) const override;

    int readProjectNumEntry( const QString &scope, const QString &key, int def = 0 ) const override;

    double readProjectDoubleEntry( const QString &scope, const QString &key, double def = 0.0 ) const override;

    bool readProjectBoolEntry( const QString &scope, const QString &key, bool def = false ) const override;

    bool print( const QString &layoutName ) override;

    bool printAtlasFeatures( const QString &layoutName, const QList<long long> &featureIds ) override;

    void setScreenDimmerTimeout( int timeoutSeconds ) override;

    bool event( QEvent *event ) override;

    void clearProject() override;

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
    void prepareLocalizedDataPaths( const QString &projectFilePath );
    void loadProjectQuirks();
    void saveProjectPreviewImage();
    bool printAtlas( QgsPrintLayout *layoutToPrint, const QString &destination );

    QgsOfflineEditing *mOfflineEditing = nullptr;
    QfLayerTreeMapCanvasBridge *mLayerTreeCanvasBridge = nullptr;
    QfFlatLayerTreeModel *mFlatLayerTree = nullptr;
    QgsMapLayerProxyModel *mLayerList = nullptr;
    QfAppInterface *mIface = nullptr;
    QfSettings mSettings;
    QPointer<QgsQuickMapCanvasMap> mMapCanvas;
    bool mFirstRenderingFlag;
    QfLegendImageProvider *mLegendImageProvider = nullptr;
    QfAsyncLegendImageProvider *mAsyncLegendImageProvider = nullptr;
    QfLocalFilesImageProvider *mLocalFilesImageProvider = nullptr;
    QfProjectsImageProvider *mProjectsImageProvider = nullptr;
    QfBarcodeImageProvider *mBarcodeImageProvider = nullptr;

    QgsProject *mProject = nullptr;
    QString mProjectFilePath;
    QString mProjectFileName;

    std::unique_ptr<QfFocusStack> mFocusStack;
    std::unique_ptr<QgsGpkgFlusher> mGpkgFlusher;
    std::unique_ptr<QfLayerObserver> mLayerObserver;
    std::unique_ptr<QfFeatureHistory> mFeatureHistory;
    std::unique_ptr<QfClipboardManager> mClipboardManager;

    QfAppAuthRequestHandler *mAuthRequestHandler = nullptr;

    QfBookmarkModel *mBookmarkModel = nullptr;
    QfDrawingTemplateModel *mDrawingTemplateModel = nullptr;
    QfMessageLogModel *mMessageLogModel = nullptr;

    QfPluginManager *mPluginManager = nullptr;

    std::unique_ptr<QfCogoRegistry> mCogoRegistry;

    // Dummy objects. We are not able to call static functions from QML, so we need something here.
    QgsWkbTypes mWkbTypes;
    QgsUnitTypes mUnitTypes;
    QgsExifTools mExifTools;

    QfTrackingModel *mTrackingModel = nullptr;

    QfAppMissingGridHandler *mAppMissingGridHandler = nullptr;

    std::unique_ptr<QfScreenDimmer> mScreenDimmer;
    std::unique_ptr<QfUrlHandler> mUrlHandler;

    QgsApplication *mApp;
};


Q_DECLARE_METATYPE( QgsFeatureId )
Q_DECLARE_METATYPE( QgsAttributes )
Q_DECLARE_METATYPE( QgsFieldConstraints )

#endif // QGISMOBILEAPP_H
```


