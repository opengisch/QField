/***************************************************************************
  geofencer.h - Geofencer

 ---------------------
 begin                : 27.06.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "geofencer.h"

#include <qgsgeometryengine.h>
#include <qgsproject.h>

Geofencer::Geofencer( QObject *parent )
  : QObject( parent )
{
}

Geofencer::~Geofencer()
{
}

void Geofencer::cleanupGatherer()
{
  if ( mGatherer )
  {
    disconnect( mGatherer, &QThread::finished, this, &Geofencer::processAreas );
    connect( mGatherer, &QThread::finished, mGatherer, &QObject::deleteLater );
    mGatherer->stop();
    mGatherer = nullptr;
  }
}

void Geofencer::gatherAreas()
{
  if ( !mAreasLayer || !mAreasLayer->isValid() || !mPositionCrs.isValid() )
    return;

  QgsFeatureRequest request;
  request.setDestinationCrs( mPositionCrs, QgsProject::instance()->transformContext() );

  QgsExpressionContext context = mAreasLayer->createExpressionContext();
  QgsExpression expression( mAreasLayer->displayExpression() );
  expression.prepare( &context );

  QSet<QString> referencedColumns = expression.referencedColumns();
  QgsFields fields = mAreasLayer->fields();
  request.setSubsetOfAttributes( referencedColumns, fields );

  cleanupGatherer();

  mGatherer = new FeatureExpressionValuesGatherer( mAreasLayer, mAreasLayer->displayExpression(), request );
  connect( mGatherer, &QThread::finished, this, &Geofencer::processAreas );
  mGatherer->start();
}

void Geofencer::processAreas()
{
  if ( !mGatherer )
    return;

  mAreas.clear();

  mAreas = mGatherer->entries();
  mGatherer->deleteLater();
  mGatherer = nullptr;

  if ( mActive )
  {
    checkWithin();
  }
}

void Geofencer::checkWithin()
{
  int isWithinIndex = -1;
  if ( mActive && !mAreas.isEmpty() )
  {
    std::unique_ptr<QgsGeometryEngine> geometryEngine;
    geometryEngine.reset( QgsGeometry::createGeometryEngine( &mPosition ) );
    for ( int i = 0; i < mAreas.size(); i++ )
    {
      if ( geometryEngine->within( mAreas.at( i ).feature.geometry().get() ) )
      {
        isWithinIndex = i;
        break;
      }
    }
  }

  if ( mIsWithinIndex != isWithinIndex )
  {
    if ( mIsWithinIndex != -1 )
    {
      mLastWithinIndex = mIsWithinIndex;
    }

    mIsWithinIndex = isWithinIndex;

    emit isWithinChanged();
  }
}

bool Geofencer::isWithin() const
{
  return mIsWithinIndex > -1;
}

QString Geofencer::isWithinAreaName() const
{
  if ( mIsWithinIndex < 0 || mIsWithinIndex >= mAreas.size() )
  {
    return QString();
  }

  return mAreas.at( mIsWithinIndex ).value;
}

QString Geofencer::lastWithinAreaName() const
{
  if ( mLastWithinIndex < 0 || mLastWithinIndex >= mAreas.size() )
  {
    return QString();
  }

  return mAreas.at( mLastWithinIndex ).value;
}

void Geofencer::setActive( bool active )
{
  if ( mActive == active )
  {
    return;
  }

  mActive = active;
  emit activeChanged();

  checkWithin();
}

void Geofencer::setPosition( const QgsPoint &position )
{
  if ( mPosition == position )
  {
    return;
  }

  mPosition = position;
  emit positionChanged();

  checkWithin();
}

void Geofencer::setPositionCrs( const QgsCoordinateReferenceSystem &crs )
{
  if ( mPositionCrs == crs )
  {
    return;
  }

  mPositionCrs = crs;
  emit positionCrsChanged();

  gatherAreas();
}

void Geofencer::setAreasLayer( QgsVectorLayer *layer )
{
  if ( mAreasLayer == layer )
  {
    return;
  }

  mAreasLayer = layer;
  emit areasLayerChanged();

  gatherAreas();
}
