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
#include "qgsvectorlayer.h"
#include "rubberbandmodel.h"
#include "utils/geometryutils.h"

#include <QtTest>


class TestGeometryUtils : public QObject
{
    Q_OBJECT
  private slots:
    void initTestCase()
    {
      mModel = std::unique_ptr<RubberbandModel>( new RubberbandModel() );
      mLayer = std::unique_ptr<QgsVectorLayer>( new QgsVectorLayer( QStringLiteral( "Polygon?crs=epsg:3946" ), QStringLiteral( "vl" ), QStringLiteral( "memory" ) ) );

      QgsFeature f( mLayer->fields(), 1 );
      f.setGeometry( QgsGeometry::fromWkt( QStringLiteral( "Polygon ((8 8, 9 8, 8 9, 8 8))" ) ) );

      QVERIFY( mLayer->startEditing() );
      QVERIFY( mLayer->addFeature( f ) );
      QVERIFY( mLayer->commitChanges() );

      mModel->setGeometryType( mLayer->geometryType() );
    }


    void cleanup()
    {
      mModel->reset();
    }


    void testPolygonFromRubberband()
    {
      const QgsCoordinateReferenceSystem crs = QgsCoordinateReferenceSystem::fromEpsgId( 3946 );

      mModel->addVertexFromPoint( QgsPoint( 0, 0 ) );
      mModel->addVertexFromPoint( QgsPoint( 2, 1 ) );
      mModel->addVertexFromPoint( QgsPoint( 1, 2 ) );

      QgsGeometry geom = GeometryUtils::polygonFromRubberband( mModel.get(), crs );

      QCOMPARE( geom.asWkt(), QStringLiteral( "Polygon ((0 0, 2 1, 1 2, 1 2, 0 0))" ) );
    }


    void testAddRingFromRubberband()
    {
      QVERIFY( mLayer->startEditing() );
      QCOMPARE( GeometryUtils::addRingFromRubberband( mLayer.get(), 100, mModel.get() ), QgsGeometry::AddRingNotInExistingFeature );

      mModel->addVertexFromPoint( QgsPoint( 8.1, 8.1 ) );
      mModel->addVertexFromPoint( QgsPoint( 8.9, 8.1 ) );
      mModel->addVertexFromPoint( QgsPoint( 8.1, 8.9 ) );
      mLayer->select( 1 );

      QCOMPARE( GeometryUtils::addRingFromRubberband( mLayer.get(), 1, mModel.get() ), QgsGeometry::Success );
      QVERIFY( mLayer->rollBack() );
    }


    void testSplitFeatureFromRubberband()
    {
      QVERIFY( mLayer->startEditing() );
      QCOMPARE( GeometryUtils::splitFeatureFromRubberband( mLayer.get(), mModel.get() ), QgsGeometry::NothingHappened );

      mModel->addVertexFromPoint( QgsPoint( 7.5, 8.5 ) );
      mModel->addVertexFromPoint( QgsPoint( 9.5, 8.5 ) );
      mLayer->select( 1 );

      QCOMPARE( GeometryUtils::splitFeatureFromRubberband( mLayer.get(), mModel.get() ), QgsGeometry::Success );
      QVERIFY( mLayer->rollBack() );
    }


  private:
    std::unique_ptr<RubberbandModel> mModel;
    std::unique_ptr<QgsVectorLayer> mLayer;
};

QFIELDTEST_MAIN( TestGeometryUtils )
#include "test_geometryutils.moc"
