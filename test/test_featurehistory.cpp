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
#include "featurehistory.h"
#include "utils/featureutils.h"

#include <QEventLoop>
#include <qgsproject.h>
#include <qgsvectorlayer.h>
#include <qgsvectorlayerjoininfo.h>


TEST_CASE( "FeatureHistory" )
{
  std::unique_ptr<QgsProject> project = std::make_unique<QgsProject>();
  std::unique_ptr<FeatureHistory> history = std::make_unique<FeatureHistory>( project.get() );

  std::unique_ptr<QgsVectorLayer> vl = std::make_unique<QgsVectorLayer>( QStringLiteral( "Polygon?crs=epsg:3946&field=fid:integer&field=str1:string" ), QStringLiteral( "vl" ), QStringLiteral( "memory" ) );
  std::unique_ptr<QgsVectorLayer> vl2 = std::make_unique<QgsVectorLayer>( QStringLiteral( "Polygon?crs=epsg:3946&field=parent_fid:integer&field=str:string" ), QStringLiteral( "vl2" ), QStringLiteral( "memory" ) );

  QgsGeometry geometry = QgsGeometry::fromWkt( QStringLiteral( "Polygon (((8 8, 9 8, 8 9, 8 8)))" ) );
  QgsFeature f = FeatureUtils::createFeature( vl.get(), geometry );
  vl->dataProvider()->addFeature( f );

  project->addMapLayer( vl.get() );
  project->addMapLayer( vl2.get() );

  QgsVectorLayerJoinInfo joinInfo;
  joinInfo.setJoinLayer( vl2.get() );
  joinInfo.setJoinFieldName( "parent_fid" );
  joinInfo.setTargetFieldName( "fid" );
  vl->addJoin( joinInfo );

  QEventLoop loop;
  QObject::connect( history.get(), &FeatureHistory::isUndoAvailableChanged, &loop, &QEventLoop::quit );
  QObject::connect( history.get(), &FeatureHistory::isRedoAvailableChanged, &loop, &QEventLoop::quit );

  vl->startEditing();
  vl->changeAttributeValue( 1, 1, "changed" );
  vl->commitChanges();
  loop.exec();

  REQUIRE( history->isUndoAvailable() );
  REQUIRE( !history->isRedoAvailable() );

  history->undo();
  REQUIRE( !history->isUndoAvailable() );
  REQUIRE( history->isRedoAvailable() );

  history->redo();
  REQUIRE( history->isUndoAvailable() );
  REQUIRE( !history->isRedoAvailable() );

  f = FeatureUtils::createFeature( vl.get(), geometry );
  vl->startEditing();
  vl->addFeature( f );
  vl->commitChanges();
  loop.exec();

  REQUIRE( !history->isRedoAvailable() );

  history->undo();
  REQUIRE( history->isRedoAvailable() );

  history->redo();
  REQUIRE( !history->isRedoAvailable() );
}
