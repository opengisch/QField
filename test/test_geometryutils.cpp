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

#include "qfield_testbase.h"

#include "rubberbandmodel.h"
#include "utils/geometryutils.h"

#include <qgis.h>
#include <qgsvectorlayer.h>

#include <gtest/gtest.h>


class TestGeometryUtils : public ::testing::Test
{
  protected:

    void SetUp()
    {
      mModel = std::unique_ptr<RubberbandModel>( new RubberbandModel() );
      mLayer = std::unique_ptr<QgsVectorLayer>( new QgsVectorLayer( QStringLiteral( "Polygon?crs=epsg:3946" ), QStringLiteral( "vl" ), QStringLiteral( "memory" ) ) );

      QgsFeature f( mLayer->fields(), 1 );
      f.setGeometry( QgsGeometry::fromWkt( QStringLiteral( "Polygon ((8 8, 9 8, 8 9, 8 8))" ) ) );

      EXPECT_TRUE( mLayer->startEditing() );
      EXPECT_TRUE( mLayer->addFeature( f ) );
      EXPECT_TRUE( mLayer->commitChanges() );

      mModel->setGeometryType( mLayer->geometryType() );
    }

    std::unique_ptr<RubberbandModel> mModel;
    std::unique_ptr<QgsVectorLayer> mLayer;
};

TEST_F( TestGeometryUtils, PolygonFromRubberband )
{
  const QgsCoordinateReferenceSystem crs = QgsCoordinateReferenceSystem::fromEpsgId( 3946 );

  mModel->addVertexFromPoint( QgsPoint( 0, 0 ) );
  mModel->addVertexFromPoint( QgsPoint( 2, 1 ) );
  mModel->addVertexFromPoint( QgsPoint( 1, 2 ) );

  QgsGeometry geom = GeometryUtils::polygonFromRubberband( mModel.get(), crs );

  EXPECT_EQ( geom.asWkt(), QStringLiteral( "Polygon ((0 0, 2 1, 1 2, 1 2, 0 0))" ) );
}

TEST_F( TestGeometryUtils,    AddRingFromRubberband )
{
  EXPECT_TRUE( mLayer->startEditing() );
#if _QGIS_VERSION_INT >= 32100
  EXPECT_EQ( GeometryUtils::addRingFromRubberband( mLayer.get(), 100, mModel.get() ), Qgis::GeometryOperationResult::AddRingNotInExistingFeature );
#else
  EXPECT_EQ( GeometryUtils::addRingFromRubberband( mLayer.get(), 100, mModel.get() ), QgsGeometry::AddRingNotInExistingFeature );
#endif

  mModel->addVertexFromPoint( QgsPoint( 8.1, 8.1 ) );
  mModel->addVertexFromPoint( QgsPoint( 8.9, 8.1 ) );
  mModel->addVertexFromPoint( QgsPoint( 8.1, 8.9 ) );
  mLayer->select( 1 );

#if _QGIS_VERSION_INT >= 32100
  EXPECT_EQ( GeometryUtils::addRingFromRubberband( mLayer.get(), 1, mModel.get() ), Qgis::GeometryOperationResult::Success );
#else
  EXPECT_EQ( GeometryUtils::addRingFromRubberband( mLayer.get(), 1, mModel.get() ), QgsGeometry::Success );
#endif
  EXPECT_TRUE( mLayer->rollBack() );
}


TEST_F( TestGeometryUtils, SplitFeatureFromRubberband )
{
  EXPECT_TRUE( mLayer->startEditing() );
#if _QGIS_VERSION_INT >= 32100
  EXPECT_EQ( GeometryUtils::splitFeatureFromRubberband( mLayer.get(), mModel.get() ), Qgis::GeometryOperationResult::NothingHappened );
#else
  EXPECT_EQ( GeometryUtils::splitFeatureFromRubberband( mLayer.get(), mModel.get() ), QgsGeometry::NothingHappened );
#endif

  mModel->addVertexFromPoint( QgsPoint( 7.5, 8.5 ) );
  mModel->addVertexFromPoint( QgsPoint( 9.5, 8.5 ) );
  mLayer->select( 1 );

#if _QGIS_VERSION_INT >= 32100
  EXPECT_EQ( GeometryUtils::splitFeatureFromRubberband( mLayer.get(), mModel.get() ), Qgis::GeometryOperationResult::Success );
#else
  EXPECT_EQ( GeometryUtils::splitFeatureFromRubberband( mLayer.get(), mModel.get() ), QgsGeometry::Success );
#endif
  EXPECT_TRUE( mLayer->rollBack() );
}



