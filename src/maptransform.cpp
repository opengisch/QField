/***************************************************************************

               ----------------------------------------------------
              date                 : 20.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias.kuhn (at) opengis.ch
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

const QVariant MapTransform::mapSettings() const
{
  return QVariant::fromValue<QgsMapSettings>( mMapSettings );
}

void MapTransform::setMapSettings( const QVariant& ms )
{
  qDebug() << "Mapsettings changed";
  const QgsMapSettings& mapSettings = ms.value<QgsMapSettings>();
  if ( mapSettings.visibleExtent() != mMapSettings.visibleExtent() )
  {
    qDebug() << "New matrix";
    QMatrix4x4 matrix;
    float scaleFactor = 1/mapSettings.mapUnitsPerPixel();

    matrix.scale( scaleFactor, -scaleFactor );
    matrix.translate( -mapSettings.visibleExtent().xMinimum(), -mapSettings.visibleExtent().yMaximum() );

    mMatrix = matrix;
    emit mapSettingsChanged();
    update();
  }
}
