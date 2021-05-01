/***************************************************************************
                        test_featureutils.h
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
#include "utils/featureutils.h"

#include <QtTest>


class TestFeatureUtils : public QObject
{
    Q_OBJECT
  private slots:


    void testInitFeature()
    {
      std::unique_ptr<QgsVectorLayer> vl( new QgsVectorLayer( QStringLiteral( "Polygon?crs=epsg:3946" ), QStringLiteral( "vl" ), QStringLiteral( "memory" ) ) );

      QgsGeometry geometry = QgsGeometry::fromWkt( QStringLiteral( "Polygon (((8 8, 9 8, 8 9, 8 8)))" ) );

      QgsFeature f = FeatureUtils::initFeature( vl.get(), geometry );

      QCOMPARE( f.fields(), vl->fields() );
      QVERIFY( f.geometry().equals( geometry ) );
    }
};

QFIELDTEST_MAIN( TestFeatureUtils )
#include "test_featureutils.moc"
