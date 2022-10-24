/***************************************************************************
                        test_featuremodel.h
                        --------------------
  begin                : Oct 2022
  copyright            : (C) 2022 by Mathieu Pellerin
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

#include "catch2.h"
#include "featuremodel.h"
#include "gnsspositioninformation.h"

#include <qgsvectorlayer.h>

TEST_CASE( "FeatureModel" )
{
  std::unique_ptr<QgsVectorLayer> layer = std::make_unique<QgsVectorLayer>( QStringLiteral( "Point?crs=EPSG:4326&field=name:text&field=horizontal_accuracy:double(12,6)&field=vertical_accuracy:double(12,6)" ), QStringLiteral( "vl" ), QStringLiteral( "memory" ) );
  layer->setDefaultValueDefinition( 1, QgsDefaultValue( QStringLiteral( "@position_horizontal_accuracy" ), true ) );
  layer->setDefaultValueDefinition( 2, QgsDefaultValue( QStringLiteral( "@position_vertical_accuracy" ), true ) );
  std::unique_ptr<FeatureModel> featureModel = std::make_unique<FeatureModel>();

  GnssPositionInformation position( 1.1, 2.2, 50.0, 50.0, 0.0, QList<QgsSatelliteInfo>(), 0, 0, 0, 5.5, 10.5, QDateTime(), QChar(), 0, 100 );

  featureModel->setCurrentLayer( layer.get() );
  featureModel->setPositionInformation( position );
  featureModel->setPositionLocked( true );
  featureModel->resetFeature();
  featureModel->resetAttributes();
  featureModel->setData( featureModel->index( 0, 0 ), QStringLiteral( "created" ), FeatureModel::AttributeValue );
  featureModel->create();

  QgsFeature feature = featureModel->feature();
  REQUIRE( feature.attribute( 0 ).toString() == QStringLiteral( "created" ) );
  REQUIRE( feature.attribute( 1 ).toDouble() == 5.5 );
  REQUIRE( feature.attribute( 2 ).toDouble() == 10.5 );

  featureModel->setData( featureModel->index( 0, 0 ), QStringLiteral( "updated" ), FeatureModel::AttributeValue );
  featureModel->save();

  feature = featureModel->feature();
  REQUIRE( feature.attribute( 0 ).toString() == QStringLiteral( "updated" ) );
  REQUIRE( feature.attribute( 1 ).toDouble() == 5.5 );
  REQUIRE( feature.attribute( 2 ).toDouble() == 10.5 );
}
