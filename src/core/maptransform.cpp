/***************************************************************************

               ----------------------------------------------------
              date                 : 20.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "maptransform.h"
#include "qgsquickmapsettings.h"

#include <QDebug>

void MapTransform::applyTo( QMatrix4x4 *matrix ) const
{
  *matrix *= mMatrix;
  matrix->optimize();
}

QgsQuickMapSettings *MapTransform::mapSettings() const
{
  return mMapSettings;
}

void MapTransform::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mapSettings == mMapSettings )
    return;

  if ( mMapSettings )
    disconnect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, &MapTransform::updateMatrix );

  mMapSettings = mapSettings;

  if ( mMapSettings )
    connect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, &MapTransform::updateMatrix );

  emit mapSettingsChanged();
}

void MapTransform::updateMatrix()
{
  QMatrix4x4 matrix;
  float scaleFactor = static_cast<float>( 1.0 / mMapSettings->mapUnitsPerPixel() );

  matrix.scale( scaleFactor, -scaleFactor );
  matrix.translate( static_cast<float>( -mMapSettings->visibleExtent().xMinimum( ) ),
                    static_cast<float>( -mMapSettings->visibleExtent().yMaximum() ) );

  mMatrix = matrix;
  update();
}
