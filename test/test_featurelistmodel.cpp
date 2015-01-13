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
#include "qt_modeltest/modeltest.h"
#include "../src/featurelistmodel.h"
#include "qgistestapp.h"

class FeatureListModelTest: public QObject, QgisTestApp
{
    Q_OBJECT

  private slots:
    void initTestCase()
    {
      mLayer = new QgsVectorLayer( TESTDATA_DIR "/shapefiles/airports.shp", "airports", "ogr" );
      mModel = new FeatureListModel;
      new ModelTest( mModel, mModel );
      Q_ASSERT( mLayer->isValid() );
    }

    void testModelData()
    {
      QMap<QgsVectorLayer*, QgsFeatureRequest> requests;
      requests.insert( mLayer, QgsFeatureRequest().setFilterExpression( "fk_region = 18 " ) );
      mModel->setFeatures( requests );

      Q_ASSERT( mModel->rowCount( QModelIndex() ) == 5 );
    }

    void cleanupTestCase()
    {
      delete mModel;
      mModel = 0;
      delete mLayer;
      mLayer = 0;
    }

  private:
    FeatureListModel* mModel;
    QgsVectorLayer* mLayer;
};

QTEST_MAIN( FeatureListModelTest )

#include "test_featurelistmodel.moc"
