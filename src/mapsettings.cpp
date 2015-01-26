/***************************************************************************

               ----------------------------------------------------
              date                 : 27.12.2014
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

#include "mapsettings.h"

MapSettings::MapSettings( QObject* parent )
  : QObject( parent )
  , mMapCanvas( 0 )
{

}

MapSettings::~MapSettings()
{

}

const QgsRectangle MapSettings::extent() const
{
  return mMapCanvas->mapSettings().extent();
}

void MapSettings::setExtent( const QgsRectangle& extent )
{
  if ( mMapCanvas && mMapCanvas->mapSettings().extent() != extent )
  {
    mMapCanvas->setExtent( extent );
    mMapCanvas->refresh();
  }
}

void MapSettings::setQgsMapCanvas( QgsMapCanvas* mapCanvas )
{
  mMapCanvas = mapCanvas;
  connect( mMapCanvas, SIGNAL( extentsChanged() ), this, SIGNAL( extentChanged() ) );
}

QgsMapCanvas* MapSettings::qgsMapCanvas()
{
  return mMapCanvas;
}

double MapSettings::mapUnitsPerPixel()
{
  return mMapCanvas->mapSettings().mapUnitsPerPixel();
}

const QgsRectangle MapSettings::visibleExtent()
{
  return mMapCanvas->mapSettings().visibleExtent();
}
