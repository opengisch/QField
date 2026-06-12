/***************************************************************************
                        test_geometryutils.h
                        --------------------
  begin                : Jun 2020
  copyright            : (C) 2020 by Ivan Ivanov
  email                : ivan@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "catch2.h"
#include "rubberbandmodel.h"
#include "utils/geometryutils.h"

#include <qgis.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>


TEST_CASE( "GeometryUtils" )
{
  std::unique_ptr<RubberbandModel> model = std::make_unique<RubberbandModel>();
  std::unique_ptr<QgsVectorLayer> mLayer = std::make_unique<QgsVectorLayer>( QStringLiteral( "Polygon?crs=epsg:3946" ), QStringLiteral( "vl" ), QStringLiteral( "memory" ) );

  QgsFeature f( mLayer->fields(), 1 );
  f.setGeometry( QgsGeometry::fromWkt( QStringLiteral( "Polygon ((8 8, 9 8, 8 9, 8 8))" ) ) );

  REQUIRE( mLayer->startEditing() );
  REQUIRE( mLayer->addFeature( f ) );
  REQUIRE( mLayer->commitChanges() );

  model->setGeometryType( mLayer->geometryType() );

  SECTION( "PolygonFromRubberband" )
  {
    const QgsCoordinateReferenceSystem crs = QgsCoordinateReferenceSystem::fromEpsgId( 3946 );

    model->addVertexFromPoint( QgsPoint( 0, 0 ) );
    model->addVertexFromPoint( QgsPoint( 2, 1 ) );
    model->addVertexFromPoint( QgsPoint( 1, 2 ) );

    QgsGeometry geom = GeometryUtils::polygonFromRubberband( model.get(), crs );

    REQUIRE( geom.asWkt() == QStringLiteral( "Polygon ((0 0, 2 1, 1 2, 1 2, 0 0))" ) );
  }

  SECTION( "AddRingFromRubberband" )
  {
    REQUIRE( mLayer.get() );
    REQUIRE( model.get() );
    REQUIRE( GeometryUtils::addRingFromRubberband( mLayer.get(), 100, model.get() ) == GeometryUtils::GeometryOperationResult::AddRingNotValid );

    model->addVertexFromPoint( QgsPoint( 8.1, 8.1 ) );
    model->addVertexFromPoint( QgsPoint( 8.9, 8.1 ) );
    model->addVertexFromPoint( QgsPoint( 8.1, 8.9 ) );
    mLayer->select( 1 );

    REQUIRE( GeometryUtils::addRingFromRubberband( mLayer.get(), 1, model.get() ) == GeometryUtils::GeometryOperationResult::Success );
  }


  SECTION( "SplitFeatureFromRubberband" )
  {
    REQUIRE( GeometryUtils::splitFeatureFromRubberband( mLayer.get(), 1, model.get() ) == GeometryUtils::GeometryOperationResult::NothingHappened );

    model->addVertexFromPoint( QgsPoint( 7.5, 8.5 ) );
    model->addVertexFromPoint( QgsPoint( 9.5, 8.5 ) );
    mLayer->select( 1 );

    REQUIRE( GeometryUtils::splitFeatureFromRubberband( mLayer.get(), 1, model.get() ) == GeometryUtils::GeometryOperationResult::Success );
  }

  SECTION( "ReshapeFromRubberband" )
  {
    // reshapeFromRubberband reads the topological editing and avoid intersections
    // state off the global project; pin them so the assertions stay deterministic
    QgsProject::instance()->setTopologicalEditing( false );
    QgsProject::instance()->setAvoidIntersectionsMode( Qgis::AvoidIntersectionsMode::AllowIntersections );

    // an unknown feature has no base geometry to reshape
    REQUIRE( GeometryUtils::reshapeFromRubberband( mLayer.get(), 100, model.get() ) == GeometryUtils::GeometryOperationResult::InvalidBaseGeometry );

    const double originalArea = mLayer->getFeature( 1 ).geometry().area();
    model->addVertexFromPoint( QgsPoint( 8.75, 8.25 ) );
    model->addVertexFromPoint( QgsPoint( 9.5, 9.5 ) );
    model->addVertexFromPoint( QgsPoint( 8.25, 8.75 ) );

    // reshapeFromRubberband persists the result through layer->changeGeometry, which
    // requires an open edit session just like the digitizing tool provides at runtime
    REQUIRE( mLayer->startEditing() );
    REQUIRE( GeometryUtils::reshapeFromRubberband( mLayer.get(), 1, model.get() ) == GeometryUtils::GeometryOperationResult::Success );
    REQUIRE( mLayer->commitChanges() );

    const QgsGeometry reshapedGeometry = mLayer->getFeature( 1 ).geometry();
    REQUIRE( !reshapedGeometry.isNull() );
    REQUIRE( reshapedGeometry.isGeosValid() );
    REQUIRE( reshapedGeometry.area() > originalArea );
  }

  SECTION( "EraseFromRubberband" )
  {
    QgsProject::instance()->setTopologicalEditing( false );

    // the rubberband used for erasing carries the buffer width as the M value of its
    // vertices, the geometry type matches the line drawn by the Erase tool
    std::unique_ptr<RubberbandModel> eraseModel = std::make_unique<RubberbandModel>();
    eraseModel->setGeometryType( Qgis::GeometryType::Line );

    // a dedicated square gives room for the M based buffer (width 5 * measureValue) to
    // either nibble a corner or cut clean through, both deterministically
    std::unique_ptr<QgsVectorLayer> squareLayer = std::make_unique<QgsVectorLayer>( QStringLiteral( "Polygon?crs=epsg:3946" ), QStringLiteral( "square" ), QStringLiteral( "memory" ) );
    QgsFeature square( squareLayer->fields(), 1 );
    square.setGeometry( QgsGeometry::fromWkt( QStringLiteral( "Polygon ((0 0, 10 0, 10 10, 0 10, 0 0))" ) ) );
    REQUIRE( squareLayer->startEditing() );
    REQUIRE( squareLayer->addFeature( square ) );
    REQUIRE( squareLayer->commitChanges() );

    // an unknown feature has no base geometry to erase
    REQUIRE( GeometryUtils::eraseFromRubberband( squareLayer.get(), 100, eraseModel.get() ) == GeometryUtils::GeometryOperationResult::InvalidBaseGeometry );

    SECTION( "Nibble a corner" )
    {
      const double originalArea = squareLayer->getFeature( 1 ).geometry().area();

      // measureValue must be set before the vertices are added so each added vertex
      // carries it as its M value; width 5 * 0.1 = 0.5 only clips the corner
      eraseModel->setMeasureValue( 0.1 );
      eraseModel->addVertexFromPoint( QgsPoint( -1, 1 ) );
      eraseModel->addVertexFromPoint( QgsPoint( 1, -1 ) );

      // eraseFromRubberband persists the result through layer->changeGeometry, which
      // requires an open edit session just like the digitizing tool provides at runtime
      REQUIRE( squareLayer->startEditing() );
      REQUIRE( GeometryUtils::eraseFromRubberband( squareLayer.get(), 1, eraseModel.get() ) == GeometryUtils::GeometryOperationResult::Success );
      REQUIRE( squareLayer->commitChanges() );

      const QgsGeometry erasedGeometry = squareLayer->getFeature( 1 ).geometry();
      REQUIRE( !erasedGeometry.isNull() );
      REQUIRE( erasedGeometry.isGeosValid() );
      REQUIRE( erasedGeometry.area() < originalArea );
    }

    SECTION( "Cut into multiple parts" )
    {
      // width 5 * 0.5 = 2.5 erases a band straight across the square, leaving two
      // disjoint pieces, the multipart result cannot fit the single type layer
      eraseModel->setMeasureValue( 0.5 );
      eraseModel->addVertexFromPoint( QgsPoint( 5, -1 ) );
      eraseModel->addVertexFromPoint( QgsPoint( 5, 11 ) );

      REQUIRE( GeometryUtils::eraseFromRubberband( squareLayer.get(), 1, eraseModel.get() ) == GeometryUtils::GeometryOperationResult::AddPartNotMultiGeometry );
    }
  }

  SECTION( "VariableWidthBufferByMFromRubberband" )
  {
    const QgsCoordinateReferenceSystem crs = QgsCoordinateReferenceSystem::fromEpsgId( 3946 );

    std::unique_ptr<RubberbandModel> bufferModel = std::make_unique<RubberbandModel>();
    bufferModel->setGeometryType( Qgis::GeometryType::Line );

    // without a measure value the vertices have no M, the buffer width collapses to zero
    // and an empty geometry comes back
    bufferModel->addVertexFromPoint( QgsPoint( 0, 0 ) );
    bufferModel->addVertexFromPoint( QgsPoint( 10, 0 ) );
    REQUIRE( GeometryUtils::variableWidthBufferByMFromRubberband( bufferModel.get(), crs ).isEmpty() );

    // a positive measure value yields a buffer polygon with an area
    std::unique_ptr<RubberbandModel> widthModel = std::make_unique<RubberbandModel>();
    widthModel->setGeometryType( Qgis::GeometryType::Line );
    widthModel->setMeasureValue( 1.0 );
    widthModel->addVertexFromPoint( QgsPoint( 0, 0 ) );
    widthModel->addVertexFromPoint( QgsPoint( 10, 0 ) );

    const QgsGeometry buffer = GeometryUtils::variableWidthBufferByMFromRubberband( widthModel.get(), crs );
    REQUIRE( !buffer.isEmpty() );
    REQUIRE( buffer.isGeosValid() );
    REQUIRE( buffer.area() > 0.0 );
  }
}
