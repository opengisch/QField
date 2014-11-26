/***************************************************************************
                            qgismobileapp.cpp  -  description
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

#include <QApplication>

#include <QtQml/QQmlEngine>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickItem>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QFileDialog> // Until native looking QML dialogs are implemented (Qt5.4?)
#include <QtWidgets/QMenu> // Until native looking QML dialogs are implemented (Qt5.4?)
#include <QtWidgets/QMenuBar>
#include <QStandardItemModel>

#include <qgsproject.h>
#include <qgsmaplayerregistry.h>
#include <qgsmaptoolidentify.h>
#include <qgsfeature.h>
#include <qgsvectorlayer.h>

#include "qgismobileapp.h"
#include "qgsquickmapcanvasmap.h"

QgisMobileapp::QgisMobileapp( QgsApplication *app, QWindow *parent )
    : QQuickView( parent )
{
  QSurfaceFormat format;
  format.setSamples( 16 );
  format.setRenderableType( QSurfaceFormat::OpenGLES );
  setFormat( format );

  initDeclarative();

  setSource( QUrl( "qrc:/qml/qgismobileapp.qml" ) );

  connect( engine(), SIGNAL( quit() ), app, SLOT( quit() ) );

  setMinimumHeight( 480 );
  setMinimumWidth( 800 );

  QgsQuickMapCanvasMap* mapCanvasBridge = rootObject()->findChild<QgsQuickMapCanvasMap *>();

  Q_ASSERT_X( mapCanvasBridge, "QML Init", "QgsQuickMapCanvasMap not found. It is likely that we failed to load the QML files. Check debug output for related messages." );

  // Setup map canvas
  mMapCanvas = mapCanvasBridge->mapCanvas();

  mMapCanvas->setVisible( true );

  connect( QgsProject::instance(), SIGNAL( readProject(QDomDocument)), this, SLOT( readProject() ) );
  connect( rootObject(), SIGNAL(openProjectDialog()), this, SLOT( openProjectDialog() ) );
  connect( this, SIGNAL( closing(QQuickCloseEvent*)), QgsApplication::instance(), SLOT(quit() ) );

  mLayerTreeCanvasBridge = new QgsLayerTreeMapCanvasBridge( QgsProject::instance()->layerTreeRoot(), mMapCanvas, this );
  connect( QgsProject::instance(), SIGNAL( writeProject( QDomDocument& ) ), mLayerTreeCanvasBridge, SLOT( writeProject( QDomDocument& ) ) );
  connect( QgsProject::instance(), SIGNAL( readProject( QDomDocument ) ), mLayerTreeCanvasBridge, SLOT( readProject( QDomDocument ) ) );
  connect( mapCanvasBridge, SIGNAL( identifyFeature( QPointF ) ), this, SLOT( identifyFeature(QPointF) ) );


  QVariant lastProjectFile = QSettings().value( "/qgis/project/lastProjectFile" );
  if ( lastProjectFile.isValid() )
  {
    QgsProject::instance()->read( lastProjectFile.toString() );
  }

  show();
}

void QgisMobileapp::initDeclarative()
{
  // Register QML custom types
  qmlRegisterType<QgsQuickMapCanvasMap>( "org.qgis", 1, 0, "MapCanvasMap" );
  int dpiX = QApplication::desktop()->physicalDpiX();
  int dpiY = QApplication::desktop()->physicalDpiY();
  int dpi = dpiX < dpiY ? dpiX : dpiY; // In case of asymetrical DPI. Improbable
  float dp = dpi * 0.00768443;
  rootContext()->setContextProperty( "dp", dp );
  rootContext()->setContextProperty( "project", QgsProject::instance() );
}

void QgisMobileapp::readProject()
{
  QSettings().setValue( "/qgis/project/lastProjectFile", QgsProject::instance()->fileName() );
}

void QgisMobileapp::openProjectDialog()
{
  QSettings settings;
  QFileDialog dlg( 0, "Open Project", settings.value( "/qgis/lastProjectOpenDir" ).toString(), "QGIS Project Files (*.qgs)" );
  dlg.setWindowState( dlg.windowState() | Qt::WindowMaximized );

  if ( dlg.exec() )
  {
    QgsProject::instance()->read( dlg.selectedFiles().first() );
    settings.setValue( "/qgis/lastProjectOpenDir", QFileInfo( dlg.selectedFiles().first() ).absolutePath() );
  }
}

QgisMobileapp::~QgisMobileapp()
{
  delete QgsProject::instance();
}
