/***************************************************************************
  snappingutils.cpp

 ---------------------
 begin                : 8.10.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "snappingutils.h"
#include "qgsquickmapsettings.h"
#include "qgsmaptoolcapture.h"
#include "qgsvectorlayer.h"

SnappingUtils::SnappingUtils( QObject *parent )
  : QgsSnappingUtils( parent )
  , mSettings( nullptr )
{
}

void SnappingUtils::onMapSettingsUpdated()
{
  QgsSnappingUtils::setMapSettings( mSettings->mapSettings() );

  snap();
}

QgsPoint SnappingUtils::newPoint( const QgsPointLocator::Match &match )
{
  QgsPoint newPoint( QgsWkbTypes::Point, match.point().x(), match.point().y() );

  // get current layer
  QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( currentLayer() );
  if ( !vlayer || !match.layer() )
  {
    return newPoint;
  }

  // convert to the corresponding type for a full ZM support
  const QgsWkbTypes::Type type = vlayer->wkbType();
  if ( QgsWkbTypes::hasZ( type ) && !QgsWkbTypes::hasM( type ) )
  {
    newPoint.convertTo( QgsWkbTypes::PointZ );
  }
  else if ( !QgsWkbTypes::hasZ( type ) && QgsWkbTypes::hasM( type ) )
  {
    newPoint.convertTo( QgsWkbTypes::PointM );
  }
  else if ( QgsWkbTypes::hasZ( type ) && QgsWkbTypes::hasM( type ) )
  {
    newPoint.convertTo( QgsWkbTypes::PointZM );
  }

  // set z value
  if ( QgsWkbTypes::hasZ( newPoint.wkbType() ) && QgsWkbTypes::hasZ( match.layer()->wkbType() ) )
  {
    const QgsFeature ft = match.layer()->getFeature( match.featureId() );
    newPoint.setZ( ft.geometry().vertexAt( match.vertexIndex() ).z() );
  }

  // set m value
  if ( QgsWkbTypes::hasM( newPoint.wkbType() ) && QgsWkbTypes::hasM( match.layer()->wkbType() ) )
  {
    const QgsFeature ft = match.layer()->getFeature( match.featureId() );
    newPoint.setM( ft.geometry().vertexAt( match.vertexIndex() ).m() );
  }

  return newPoint;
}

void SnappingUtils::snap()
{
  QgsPointLocator::Match match = snapToMap( QPoint( static_cast<int>( mInputCoordinate.x() ), static_cast<int>( mInputCoordinate.y() ) ) );
  mSnappingResult = SnappingResult( match );
  mSnappingResult.setPoint( newPoint( match ) );

  emit snappingResultChanged();
}

QPointF SnappingUtils::inputCoordinate() const
{
  return mInputCoordinate;
}

void SnappingUtils::setInputCoordinate( const QPointF &inputCoordinate )
{
  if ( inputCoordinate == mInputCoordinate )
    return;

  mInputCoordinate = inputCoordinate;

  snap();

  emit inputCoordinateChanged();
}

SnappingResult SnappingUtils::snappingResult() const
{
  return mSnappingResult;
}

void SnappingUtils::prepareIndexStarting( int count )
{
  mIndexLayerCount = count;
  emit indexingStarted( count );
}

void SnappingUtils::prepareIndexProgress( int index )
{
  if ( index == mIndexLayerCount )
    emit indexingFinished();
  else
    emit indexingProgress( index );
}

QgsVectorLayer *SnappingUtils::currentLayer() const
{
  return mCurrentLayer;
}

void SnappingUtils::setCurrentLayer( QgsVectorLayer *currentLayer )
{
  if ( currentLayer == mCurrentLayer )
    return;

  mCurrentLayer = currentLayer;
  QgsSnappingUtils::setCurrentLayer( currentLayer );

  emit currentLayerChanged();
}

QgsQuickMapSettings *SnappingUtils::mapSettings() const
{
  return mSettings;
}

void SnappingUtils::setMapSettings( QgsQuickMapSettings *settings )
{
  if ( mSettings == settings )
    return;

  connect( settings, &QgsQuickMapSettings::extentChanged, this, &SnappingUtils::onMapSettingsUpdated );
  connect( settings, &QgsQuickMapSettings::destinationCrsChanged, this, &SnappingUtils::onMapSettingsUpdated );
  connect( settings, &QgsQuickMapSettings::layersChanged, this, &SnappingUtils::onMapSettingsUpdated );

  mSettings = settings;
  emit mapSettingsChanged();
}
