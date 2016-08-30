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

#include "qgsmaplayerregistry.h"
#include "qgsmaplayer.h"
#include "qgsproject.h"

MapSettings::MapSettings( QObject* parent )
  : QObject( parent )
{
  // Connect signals for derived values
  connect( this, SIGNAL( mapUnitsChanged() ), this, SIGNAL( mapUnitsPerPixelChanged() ) );
  connect( this, SIGNAL( extentChanged() ), this, SIGNAL( mapUnitsPerPixelChanged() ) );
  connect( this, SIGNAL( outputSizeChanged() ), this, SIGNAL( mapUnitsPerPixelChanged() ) );
  connect( this, SIGNAL( extentChanged() ), this, SIGNAL( visibleExtentChanged() ) );
  connect( this, SIGNAL( rotationChanged() ), this, SIGNAL( visibleExtentChanged() ) );
  connect( QgsProject::instance(), SIGNAL( readProject( QDomDocument ) ), this, SLOT( onReadProject( QDomDocument ) ) );
}

MapSettings::~MapSettings()
{

}

QgsRectangle MapSettings::extent() const
{
  return mMapSettings.extent();
}

void MapSettings::setExtent( const QgsRectangle& extent )
{
  if ( mMapSettings.extent() == extent )
    return;

  mMapSettings.setExtent( extent );
  emit extentChanged();
}

void MapSettings::setCenter( const QPointF& center )
{
  QgsVector delta = QgsPoint( center.x(), center.y() ) - mMapSettings.extent().center();

  QgsRectangle e = mMapSettings.extent();
  e.setXMinimum( e.xMinimum() + delta.x() );
  e.setXMaximum( e.xMaximum() + delta.x() );
  e.setYMinimum( e.yMinimum() + delta.y() );
  e.setYMaximum( e.yMaximum() + delta.y() );

  setExtent( e );
}

double MapSettings::mapUnitsPerPixel() const
{
  return mMapSettings.mapUnitsPerPixel();
}

QgsRectangle MapSettings::visibleExtent() const
{
  return mMapSettings.visibleExtent();
}

const QPointF MapSettings::coordinateToScreen( const QPointF& p ) const
{
  QgsPoint pt( p.x(), p.y() );
  QgsPoint pp = mMapSettings.mapToPixel().transform( pt );
  return QPointF( pp.x(), pp.y() );
}

const QPointF MapSettings::screenToCoordinate( const QPointF& p ) const
{
  const QgsPoint pp = mMapSettings.mapToPixel().toMapCoordinates( p.toPoint() );
  return QPointF( pp.x(), pp.y() );
}

QgsMapSettings MapSettings::mapSettings() const
{
  return mMapSettings;
}

QSize MapSettings::outputSize() const
{
  return mMapSettings.outputSize();
}

void MapSettings::setOutputSize( const QSize& outputSize )
{
  if ( mMapSettings.outputSize() == outputSize )
    return;

  mMapSettings.setOutputSize( outputSize );
  emit outputSizeChanged();
}

double MapSettings::outputDpi() const
{
  return mMapSettings.outputDpi();
}

void MapSettings::setOutputDpi( double outputDpi )
{
  if ( mMapSettings.outputDpi() == outputDpi )
    return;

  mMapSettings.setOutputDpi( outputDpi );
  emit outputDpiChanged();
}

QgsCoordinateReferenceSystem MapSettings::destinationCrs() const
{
  return mMapSettings.destinationCrs();
}

void MapSettings::setDestinationCrs( const QgsCoordinateReferenceSystem& destinationCrs )
{
  if ( mMapSettings.destinationCrs() == destinationCrs )
    return;

  mMapSettings.setDestinationCrs( destinationCrs );
  emit destinationCrsChanged();
}

QgsUnitTypes::DistanceUnit MapSettings::mapUnits() const
{
  return mMapSettings.mapUnits();
}

void MapSettings::setMapUnits( const QgsUnitTypes::DistanceUnit& mapUnits )
{
  if ( mMapSettings.mapUnits() == mapUnits )
    return;

  mMapSettings.setMapUnits( mapUnits );
  emit mapUnitsChanged();
}

bool MapSettings::hasCrsTransformEnabled() const
{
  return mMapSettings.hasCrsTransformEnabled();
}

void MapSettings::setCrsTransformEnabled( bool crsTransformEnabled )
{
  if ( mMapSettings.hasCrsTransformEnabled() == crsTransformEnabled )
    return;

  mMapSettings.setCrsTransformEnabled( crsTransformEnabled );
  emit crsTransformEnabledChanged();
}

QList<QgsMapLayer*> MapSettings::layers() const
{
  QList <QgsMapLayer*> layers;

  Q_FOREACH( const QString& id, mMapSettings.layers() )
  {
    QgsMapLayer* layer = QgsMapLayerRegistry::instance()->mapLayer( id );
    if ( layer )
      layers << layer;
  }
  return layers;
}

void MapSettings::setLayers( const QList<QgsMapLayer*>& layers )
{
  QStringList layerIds;
  Q_FOREACH( QgsMapLayer* layer, layers )
  {
    layerIds << layer->id();
  }

  if ( layerIds == mMapSettings.layers() )
    return;

  mMapSettings.setLayers( layerIds );
  emit layersChanged();
}

void MapSettings::onReadProject( const QDomDocument& doc )
{
  QDomNodeList nodes = doc.elementsByTagName( "mapcanvas" );
  if ( nodes.count() )
  {
    QDomNode node = nodes.item( 0 );

    mMapSettings.readXml( node );
    emit extentChanged();
  }
}

double MapSettings::rotation() const
{
  return mMapSettings.rotation();
}

void MapSettings::setRotation( double rotation )
{
  if ( rotation == mMapSettings.rotation() )
    return;

  mMapSettings.setRotation( rotation );
  emit rotationChanged();
}
