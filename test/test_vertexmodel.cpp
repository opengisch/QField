/***************************************************************************
                        test_vertexmodel.h
                        -----------------------
  begin                : Aug 2018
  copyright            : (C) 2018 by Denis Rouzaud
  email                : denis@opengis.ch
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
#include "qgsgeometry.h"
#include "qgsquickmapsettings.h"
#include "vertexmodel.h"

#include <qgsapplication.h>
#include <qgsgeometry.h>
#include <qgslinestring.h>
#include <qgsmessagelog.h>
#include <qgspoint.h>
#include <qgspointxy.h>
#include <qgspolygon.h>

using Catch::Approx;

class VertexModelTest
{
  public:
    static void setCurrentVertex( std::unique_ptr<VertexModel> &model, int newVertex, bool forceUpdate = false )
    {
      model->setCurrentVertex( newVertex, forceUpdate );
    }
};


TEST_CASE( "VertexModel" )
{
  std::unique_ptr<VertexModel> model = std::make_unique<VertexModel>();

  QgsLineString *lineString = new QgsLineString( QVector<QgsPoint>() << QgsPoint( 0, 0 ) << QgsPoint( 2, 2 ) << QgsPoint( 4, 4 ) );
  QgsGeometry lineGeometry( lineString );

  QgsGeometry polygonGeometry = QgsGeometry::fromPolygonXY( QVector<QVector<QgsPointXY>>()
                                                            << ( QVector<QgsPointXY>()
                                                                 << QgsPointXY( 0, 0 )
                                                                 << QgsPointXY( 2, 0 )
                                                                 << QgsPointXY( 2, 2 )
                                                                 << QgsPointXY( 0, 2 )
                                                                 << QgsPointXY( 0, 0 ) ) );

  QgsGeometry ringPolygonGeometry = QgsGeometry::fromPolygonXY( QVector<QVector<QgsPointXY>>()
                                                                << ( QVector<QgsPointXY>()
                                                                     << QgsPointXY( 0, 0 )
                                                                     << QgsPointXY( 4, 0 )
                                                                     << QgsPointXY( 4, 4 )
                                                                     << QgsPointXY( 0, 4 )
                                                                     << QgsPointXY( 0, 0 ) )
                                                                << ( QVector<QgsPointXY>()
                                                                     << QgsPointXY( 1, 1 )
                                                                     << QgsPointXY( 3, 1 )
                                                                     << QgsPointXY( 3, 3 )
                                                                     << QgsPointXY( 1, 3 )
                                                                     << QgsPointXY( 1, 1 ) ) );
  QgsPolygon *polygon = new QgsPolygon( new QgsLineString( QVector<QgsPoint>() << QgsPoint( 0, 0, 0, 0 )
                                                                               << QgsPoint( 4, 0, 0, 0 )
                                                                               << QgsPoint( 4, 4, 0, 0 )
                                                                               << QgsPoint( 0, 4, 0, 0 )
                                                                               << QgsPoint( 0, 0, 0, 0 ) ) );
  QgsGeometry polygonZMGeometry = QgsGeometry( polygon );

  QgsGeometry point2056Geometry = QgsGeometry::fromPointXY( QgsPointXY( 2500000, 1200000 ) );

  QgsGeometry line2056Geometry = QgsGeometry::fromPolylineXY( { QgsPointXY( 2500001, 1200001 ), QgsPointXY( 2500002, 1200002 ), QgsPointXY( 2500004, 1200004 ) } );

  SECTION( "Candidates" )
  {
    model->setGeometry( lineGeometry );
    REQUIRE( model->vertexCount() == 7 );
    REQUIRE( model->vertices().at( 0 ).point == QgsPoint( -.5, -.5 ) );
    REQUIRE( model->vertices().at( 2 ).point == QgsPoint( 1, 1 ) );
    REQUIRE( model->vertices().at( 4 ).point == QgsPoint( 3, 3 ) );
    REQUIRE( model->vertices().at( 6 ).point == QgsPoint( 4.5, 4.5 ) );

    model->setGeometry( polygonGeometry );
    REQUIRE( model->vertexCount() == 8 );
    REQUIRE( model->vertices().at( 0 ).point == QgsPoint( 1, 0 ) );
    REQUIRE( model->vertices().at( 1 ).point == QgsPoint( 2, 0 ) );
    REQUIRE( model->vertices().at( 2 ).point == QgsPoint( 2, 1 ) );
    REQUIRE( model->vertices().at( 3 ).point == QgsPoint( 2, 2 ) );
    REQUIRE( model->vertices().at( 4 ).point == QgsPoint( 1, 2 ) );
    REQUIRE( model->vertices().at( 5 ).point == QgsPoint( 0, 2 ) );
    REQUIRE( model->vertices().at( 6 ).point == QgsPoint( 0, 1 ) );
    REQUIRE( model->vertices().at( 7 ).point == QgsPoint( 0, 0 ) );

    model->setGeometry( ringPolygonGeometry );
    REQUIRE( model->vertexCount() == 16 );
    REQUIRE( model->vertices().at( 0 ).point == QgsPoint( 2, 0 ) );
    REQUIRE( model->vertices().at( 1 ).point == QgsPoint( 4, 0 ) );
    REQUIRE( model->vertices().at( 2 ).point == QgsPoint( 4, 2 ) );
    REQUIRE( model->vertices().at( 3 ).point == QgsPoint( 4, 4 ) );
    REQUIRE( model->vertices().at( 4 ).point == QgsPoint( 2, 4 ) );
    REQUIRE( model->vertices().at( 5 ).point == QgsPoint( 0, 4 ) );
    REQUIRE( model->vertices().at( 6 ).point == QgsPoint( 0, 2 ) );
    REQUIRE( model->vertices().at( 7 ).point == QgsPoint( 0, 0 ) );
    REQUIRE( model->vertices().at( 8 ).point == QgsPoint( 2, 1 ) );
    REQUIRE( model->vertices().at( 9 ).point == QgsPoint( 3, 1 ) );
    REQUIRE( model->vertices().at( 10 ).point == QgsPoint( 3, 2 ) );
    REQUIRE( model->vertices().at( 11 ).point == QgsPoint( 3, 3 ) );
    REQUIRE( model->vertices().at( 12 ).point == QgsPoint( 2, 3 ) );
    REQUIRE( model->vertices().at( 13 ).point == QgsPoint( 1, 3 ) );
    REQUIRE( model->vertices().at( 14 ).point == QgsPoint( 1, 2 ) );
    REQUIRE( model->vertices().at( 15 ).point == QgsPoint( 1, 1 ) );

    model->setGeometry( polygonZMGeometry );
    REQUIRE( model->vertexCount() == 8 );
    REQUIRE( model->vertices().at( 0 ).point == QgsPoint( 2, 0, 0, 0 ) );
    REQUIRE( model->vertices().at( 1 ).point == QgsPoint( 4, 0, 0, 0 ) );
    REQUIRE( model->vertices().at( 2 ).point == QgsPoint( 4, 2, 0, 0 ) );
    REQUIRE( model->vertices().at( 3 ).point == QgsPoint( 4, 4, 0, 0 ) );
    REQUIRE( model->vertices().at( 4 ).point == QgsPoint( 2, 4, 0, 0 ) );
    REQUIRE( model->vertices().at( 5 ).point == QgsPoint( 0, 4, 0, 0 ) );
    REQUIRE( model->vertices().at( 6 ).point == QgsPoint( 0, 2, 0, 0 ) );
    REQUIRE( model->vertices().at( 7 ).point == QgsPoint( 0, 0, 0, 0 ) );
  }

  SECTION( "CanRemoveVertex" )
  {
    // line
    model->setGeometry( lineGeometry );
    REQUIRE( !model->canRemoveVertex() );
    model->setEditingMode( VertexModel::EditVertex );
    REQUIRE( model->canRemoveVertex() );

    model->setEditingMode( VertexModel::NoEditing );
    REQUIRE( model->editingMode() == VertexModel::NoEditing );
    VertexModelTest::setCurrentVertex( model, 1 );
    REQUIRE( model->editingMode() == VertexModel::EditVertex );

    REQUIRE( model->vertexCount() == 7 );
    model->removeCurrentVertex();
    REQUIRE( model->vertexCount() == 5 );
    REQUIRE( !model->canRemoveVertex() );

    // polygon
    model->setGeometry( polygonGeometry );
    REQUIRE( model->editingMode() == VertexModel::NoEditing );
    REQUIRE( !model->canRemoveVertex() );
    VertexModelTest::setCurrentVertex( model, 1 );
    REQUIRE( model->vertexCount() == 8 );
    model->removeCurrentVertex();
    REQUIRE( model->vertexCount() == 6 );
    REQUIRE( !model->canRemoveVertex() );
  }

  SECTION( "AddVertex" )
  {
    model->setGeometry( polygonGeometry );
    REQUIRE( model->vertexCount() == 8 );
    model->setEditingMode( VertexModel::AddVertex );
    VertexModelTest::setCurrentVertex( model, 0 );
    model->setCurrentPoint( QgsPoint( -3, 0 ) );
    REQUIRE( model->vertexCount() == 10 );

    model->setGeometry( polygonZMGeometry );
    REQUIRE( model->vertexCount() == 8 );
    model->setEditingMode( VertexModel::AddVertex );
    VertexModelTest::setCurrentVertex( model, 0 );
    model->setCurrentPoint( QgsPoint( -3, 0 ) );
    REQUIRE( model->vertexCount() == 10 );
    // Test that the added vertex has the extra Z and M dimensions
    REQUIRE( model->vertices().at( 1 ).point == QgsPoint( -3, 0, 0, 0 ) );

    model->setGeometry( lineGeometry );
    model->setEditingMode( VertexModel::AddVertex );
    VertexModelTest::setCurrentVertex( model, 2 );
    REQUIRE( model->currentVertexIndex() == 2 );
    REQUIRE( model->canPreviousVertex() );
    model->previous();
    REQUIRE( !model->canPreviousVertex() );
    REQUIRE( model->currentVertexIndex() == 0 );
    model->next();
    REQUIRE( model->currentVertexIndex() == 2 );

    model->setGeometry( lineGeometry );
    model->setEditingMode( VertexModel::AddVertex );
    VertexModelTest::setCurrentVertex( model, 0 );
    REQUIRE( model->currentVertexIndex() == 0 );
    REQUIRE( model->currentPoint() == QgsPoint( -.5, -.5 ) );
    model->next();
    REQUIRE( model->currentVertexIndex() == 2 );
    REQUIRE( model->currentPoint() == QgsPoint( 1, 1 ) );
    model->next();
    REQUIRE( model->currentVertexIndex() == 4 );
    REQUIRE( model->canNextVertex() );
    model->next();
    REQUIRE( model->currentVertexIndex() == 6 );
    REQUIRE( !model->canNextVertex() );
    model->previous();
    REQUIRE( model->currentVertexIndex() == 4 );
  }

  SECTION( "EditingMode" )
  {
    model->setGeometry( ringPolygonGeometry );
    REQUIRE( model->editingMode() == VertexModel::NoEditing );
    REQUIRE( model->currentVertexIndex() == -1 );
    model->setEditingMode( VertexModel::AddVertex );
    REQUIRE( model->currentVertexIndex() == 0 );
    REQUIRE( model->vertices().at( 0 ).currentVertex == true );
    model->setCurrentPoint( QgsPoint( 1, 0 ) );
    REQUIRE( model->currentVertexIndex() == 1 );
    REQUIRE( model->currentPoint() == QgsPoint( 1, 0 ) );
    REQUIRE( model->vertices().at( 1 ).point == QgsPoint( 1, 0 ) );
  }

  SECTION( "Transform" )
  {
    QgsQuickMapSettings mapSettings;
    mapSettings.setDestinationCrs( QgsCoordinateReferenceSystem::fromEpsgId( 4326 ) );
    model->setMapSettings( &mapSettings );
    REQUIRE( model->mapSettings()->destinationCrs().authid() == QStringLiteral( "EPSG:4326" ) );
    model->setGeometry( point2056Geometry );
    model->setCrs( QgsCoordinateReferenceSystem::fromEpsgId( 2056 ) );
    const auto &point = model->vertex( 0 ).point;

    REQUIRE( point.y() == Approx( 46.9435 ).epsilon( 0.001 ) );
    REQUIRE( point.x() == Approx( 6.12514 ).epsilon( 0.001 ) );
  }

  SECTION( "SelectVertexAtPosition" )
  {
    QgsQuickMapSettings mapSettings;
    mapSettings.setDestinationCrs( QgsCoordinateReferenceSystem::fromEpsgId( 4326 ) );
    // mapSettings.setExtent(  );
    model->setMapSettings( &mapSettings );

    model->setCrs( QgsCoordinateReferenceSystem::fromEpsgId( 2056 ) );
    model->setGeometry( line2056Geometry );
    REQUIRE( model->currentVertexIndex() == -1 );

    model->selectVertexAtPosition( QgsPoint( 6.12515656, 46.943546146 ), 10 );
    REQUIRE( model->currentVertexIndex() == 1 );

    REQUIRE( model->editingMode() == VertexModel::EditVertex );

    model->setEditingMode( VertexModel::AddVertex );
    REQUIRE( model->currentVertexIndex() == 2 );

    // selecting a candidate will make it a vertex
    REQUIRE( model->vertices().count() == 7 );
    model->selectVertexAtPosition( QgsPoint( 6.12515333, 46.94354385 ), 10 );
    REQUIRE( model->editingMode() == VertexModel::EditVertex );
    REQUIRE( model->vertices().count() == 9 );
  }
}
