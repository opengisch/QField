/***************************************************************************
                        test_featurehistory.h
                        ---------------------
  begin                : Aug 2025
  copyright            : (C) 2025 by Mathieu Pellerin
  email                : mathieu@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#define QFIELDTEST_MAIN
#include "catch2.h"
#include "featuremodel.h"
#include "geometry.h"
#include "positioning.h"
#include "qgsquick/qgsquickcoordinatetransformer.h"
#include "rubberbandmodel.h"
#include "tracker.h"
#include "trackingmodel.h"
#include "utils/featureutils.h"

#include <QEventLoop>
#include <QTimer>
#include <qgsproject.h>
#include <qgsvectorlayer.h>
#include <qgsvectorlayerjoininfo.h>


TEST_CASE( "Tracking" )
{
  // Setup dummy project with one empty line vector layer
  std::unique_ptr<QgsProject> project = std::make_unique<QgsProject>();
  std::unique_ptr<QgsVectorLayer> vl = std::make_unique<QgsVectorLayer>( QStringLiteral( "LineString?crs=epsg:3946" ), QStringLiteral( "vl" ), QStringLiteral( "memory" ) );
  project->setCrs( vl->crs() );
  project->addMapLayer( vl.get() );

  // Setup the coordinate transformer to be used to reproject incoming WGS84 positions
  std::unique_ptr<QgsQuickCoordinateTransformer> coordinateTransformer = std::make_unique<QgsQuickCoordinateTransformer>();
  coordinateTransformer->setDestinationCrs( project->crs() );
  coordinateTransformer->setTransformContext( project->transformContext() );

  // Setup positioning
  Positioning *positioning = new Positioning();
  positioning->setDeviceId( QStringLiteral( "file:%1/../nmea_server/TrimbleR1.txt:100" ).arg( TEST_DATA_DIR ) );
  positioning->setCoordinateTransformer( coordinateTransformer.get() );

  // Setup the rubberband model to be used by the tracker
  std::unique_ptr<RubberbandModel> rubberbandModel = std::make_unique<RubberbandModel>();
  rubberbandModel->setCrs( project->crs() );
  rubberbandModel->setVectorLayer( vl.get() );
  rubberbandModel->setGeometryType( Qgis::GeometryType::Line );

  // Setup the geometry object that ties the rubberband model and the feature model together
  std::unique_ptr<Geometry> geometry = std::make_unique<Geometry>();
  geometry->setRubberbandModel( rubberbandModel.get() );
  geometry->setVectorLayer( vl.get() );

  // Setup the feature model to be used by the tracker
  std::unique_ptr<FeatureModel> featureModel = std::make_unique<FeatureModel>();
  featureModel->setCurrentLayer( vl.get() );
  featureModel->setGeometry( geometry.get() );

  // Setup the tracking model and create a tracker on the line layer
  std::unique_ptr<TrackingModel> trackingModel = std::make_unique<TrackingModel>();
  QModelIndex idx = trackingModel->createTracker( vl.get() );
  Tracker *tracker = trackingModel->data( idx, TrackingModel::TrackerPointer ).value<Tracker *>();
  REQUIRE( tracker );
  tracker->setFeatureModel( featureModel.get() );
  tracker->setRubberbandModel( rubberbandModel.get() );


  QObject::connect( positioning, &Positioning::positionInformationChanged, [=]() {
    tracker->processPositionInformation( positioning->positionInformation(), positioning->projectedPosition() );
  } );

  QEventLoop loop;
  QTimer timer;
  timer.setSingleShot( true );
  timer.setInterval( 10000 );
  QObject::connect( &timer, &QTimer::timeout, &loop, &QEventLoop::quit );
  positioning->setActive( true );

  // Reset the feature model and its geometry via updateRubberband()
  featureModel->resetFeature();
  featureModel->resetAttributes();
  featureModel->updateRubberband();

  // Track against no constraints
  tracker->start();
  timer.start();
  loop.exec();
  tracker->stop();

  // Insure the tracked feature has been added
  REQUIRE( vl->featureCount() == 1 );

  // Reset the feature model and its geometry via updateRubberband()
  featureModel->resetFeature();
  featureModel->resetAttributes();
  featureModel->updateRubberband();

  // Track against minimum distance constraints
  tracker->setMinimumDistance( 0.2 );
  tracker->setTimeInterval( 0.0 );
  tracker->start();
  timer.start();
  loop.exec();
  tracker->stop();

  // Insure the tracked feature has been added
  REQUIRE( vl->featureCount() == 2 );

  // Reset the feature model and its geometry via updateRubberband()
  featureModel->resetFeature();
  featureModel->resetAttributes();
  featureModel->updateRubberband();

  // Track against time interval constraints
  tracker->setMinimumDistance( 0.0 );
  tracker->setTimeInterval( 2.0 );
  tracker->start();
  timer.start();
  loop.exec();
  tracker->stop();

  // Insure the tracked feature has been added
  REQUIRE( vl->featureCount() == 3 );

  // Reset the feature model and its geometry via updateRubberband()
  featureModel->resetFeature();
  featureModel->resetAttributes();
  featureModel->updateRubberband();

  // Track against time interval constraints
  tracker->setMinimumDistance( 0.2 );
  tracker->setTimeInterval( 2.0 );
  tracker->start();
  timer.start();
  loop.exec();
  tracker->stop();

  // Insure the tracked feature has been added
  REQUIRE( vl->featureCount() == 4 );

  // Cleanup
  positioning->setActive( false );
  positioning->deleteLater();
  delete positioning;
}
