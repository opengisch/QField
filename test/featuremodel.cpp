/***************************************************************************

               ----------------------------------------------------
              date                 : 4.1.2015
              copyright            : (C) 2015 by Matthias Kuhn
              email                : matthias.kuhn (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QtTest/QtTest>
#include <QtCore/QObject>

#include <qgsvectorlayer.h>
#include <qgsapplication.h>
#include "../src/featuremodel.h"
#include "qt_modeltest/modeltest.h"
#include "qgistestapp.h"

class FeatureModelTest: public QObject, QgisTestApp
{
    Q_OBJECT

  private slots:
    void initTestCase()
    {
      mLayer = new QgsVectorLayer( TESTDATA_DIR "/shapefiles/airports.shp", "airports", "ogr" );
      mModel = new FeatureModel;
      new ModelTest( mModel, mModel );
      Q_ASSERT( mLayer->isValid() );
    }

    void testModelData()
    {
      QgsFeature f;
      mLayer->getFeatures().nextFeature( f );

      mModel->setFeature( Feature( f, mLayer ) );

      int rowcount = mModel->rowCount( QModelIndex() );
      Q_ASSERT( rowcount >= mLayer->pendingFields().count() );

      // Check that the attribute value is ok
      Q_ASSERT( mModel->data( mModel->index( 0, 0 ), FeatureModel::AttributeValue ).toInt() == 1 );
      // Check that the attribute name is ok
      Q_ASSERT( 0 == mModel->data( mModel->index( 0, 0 ), FeatureModel::AttributeName ).toString().compare( "ID" ) );
    }

    void cleanupTestCase()
    {
      delete mModel;
      mModel = 0;
      delete mLayer;
      mLayer = 0;
    }

  private:
    FeatureModel* mModel;
    QgsVectorLayer* mLayer;
};

QTEST_MAIN( FeatureModelTest )

#include "featuremodel.moc"
