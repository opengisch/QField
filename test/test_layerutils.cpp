/***************************************************************************
                        test_layerutils.h
                        --------------------
  begin                : Aug 2026
  copyright            : (C) 2026 by Kaustuv Pokharel
  email                : kaustuv@opengis.ch
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
#include "utils/qflayerutils.h"

#include <qgsfeature.h>
#include <qgsvectorlayer.h>

#include <memory>

TEST_CASE( "LayerUtils AddFeature" )
{
  std::unique_ptr<QgsVectorLayer> layer = std::make_unique<QgsVectorLayer>( QStringLiteral( "Point?crs=EPSG:4326&field=fid:integer&field=name:string" ), QStringLiteral( "vl" ), QStringLiteral( "memory" ) );
  REQUIRE( layer->isValid() );

  QgsFeature f( layer->fields() );
  f.setAttribute( QStringLiteral( "fid" ), 1 );
  f.setAttribute( QStringLiteral( "name" ), QStringLiteral( "a" ) );

  SECTION( "NullLayer" )
  {
    REQUIRE_FALSE( QfLayerUtils::addFeature( nullptr, f ) );
  }

  SECTION( "NotEditing" )
  {
    REQUIRE_FALSE( QfLayerUtils::addFeature( layer.get(), f ) );
    REQUIRE( layer->featureCount() == 0 );
  }

  SECTION( "Editing" )
  {
    REQUIRE( layer->startEditing() );
    REQUIRE( QfLayerUtils::addFeature( layer.get(), f ) );
    REQUIRE( layer->commitChanges() );
    REQUIRE( layer->featureCount() == 1 );

    QgsFeature added = layer->getFeature( 1 );
    REQUIRE( added.isValid() );
    REQUIRE( added.attribute( QStringLiteral( "fid" ) ) == 1 );
    REQUIRE( added.attribute( QStringLiteral( "name" ) ).toString() == QStringLiteral( "a" ) );
  }
}
