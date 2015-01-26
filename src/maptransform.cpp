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

#include <QDebug>

MapTransform::MapTransform()
{
}

MapTransform::~MapTransform()
{
}

void MapTransform::applyTo( QMatrix4x4* matrix ) const
{
  *matrix *= mMatrix;
  matrix->optimize();
}

MapSettings* MapTransform::mapSettings() const
{
  return mMapSettings;
}

void MapTransform::setMapSettings( MapSettings* mapSettings )
{
  if ( mapSettings != mMapSettings )
  {
    mMapSettings = mapSettings;
    connect( mMapSettings, SIGNAL(extentChanged()), SLOT(updateMatrix()));
    emit mapSettingsChanged();
  }
}

void MapTransform::updateMatrix()
{
  QMatrix4x4 matrix;
  float scaleFactor = 1 / mMapSettings->mapUnitsPerPixel();

  matrix.scale( scaleFactor, -scaleFactor );
  matrix.translate( -mMapSettings->visibleExtent().xMinimum(), -mMapSettings->visibleExtent().yMaximum() );

  mMatrix = matrix;
  update();
}
