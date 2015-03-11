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
  , mCrs( new CRS )
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
  mCrs->setCrs( mMapCanvas->mapSettings().destinationCrs() );
  emit crsChanged();
  connect( mMapCanvas, SIGNAL( destinationCrsChanged() ), this, SLOT( onMapCrsChanged() ) );
  connect( mMapCanvas, SIGNAL( extentsChanged() ), this, SIGNAL( extentChanged() ) );
}

QgsMapCanvas* MapSettings::qgsMapCanvas()
{
  return mMapCanvas;
}

void MapSettings::setCenter( const QPointF& center )
{
  QgsVector delta = QgsPoint( center.x(), center.y() ) - mMapCanvas->mapSettings().extent().center();

  QgsRectangle e = mMapCanvas->mapSettings().extent();
  e.setXMinimum( e.xMinimum() + delta.x() );
  e.setXMaximum( e.xMaximum() + delta.x() );
  e.setYMinimum( e.yMinimum() + delta.y() );
  e.setYMaximum( e.yMaximum() + delta.y() );

  setExtent( e );
}

double MapSettings::mapUnitsPerPixel()
{
  return mMapCanvas->mapSettings().mapUnitsPerPixel();
}

const QgsRectangle MapSettings::visibleExtent()
{
  return mMapCanvas->mapSettings().visibleExtent();
}

CRS* MapSettings::crs() const
{
  return mCrs;
}

const QPointF MapSettings::coordinateToScreen( const QPointF& p ) const
{
  if ( mMapCanvas )
  {
    QgsPoint pt( p.x(), p.y() );
    QgsPoint pp = mMapCanvas->mapSettings().mapToPixel().transform( pt );
    return QPointF( pp.x(), pp.y() );
  }
  else
  {
    return p;
  }
}

void MapSettings::onMapCrsChanged()
{
  mCrs->setCrs( mMapCanvas->mapSettings().destinationCrs() );
  emit crsChanged();
}
