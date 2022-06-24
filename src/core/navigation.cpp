/***************************************************************************
 navigation.cpp - Navigation

 ---------------------
 begin                : 22.02.2022
 copyright            : (C) 2022 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "featureutils.h"
#include "navigation.h"
#include "navigationmodel.h"

#include <qgslinestring.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>

Navigation::Navigation()
  : QObject()
{
  mModel.reset( new NavigationModel() );
  mModel->restore();

  connect( mModel.get(), &NavigationModel::destinationChanged, this, &Navigation::isActiveChanged );
  connect( mModel.get(), &NavigationModel::destinationChanged, this, &Navigation::destinationChanged );
  connect( mModel.get(), &NavigationModel::destinationChanged, this, &Navigation::updateDetails );
  connect( mModel.get(), &NavigationModel::modelReset, this, &Navigation::isActiveChanged );
  connect( mModel.get(), &NavigationModel::modelReset, this, &Navigation::destinationChanged );
  connect( mModel.get(), &NavigationModel::modelReset, this, &Navigation::updateDetails );
}

Navigation::~Navigation()
{
  mModel->save();
}

bool Navigation::isActive() const
{
  return !destination().isEmpty();
}

void Navigation::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
    return;

  if ( mMapSettings )
  {
    disconnect( mMapSettings, &QgsQuickMapSettings::destinationCrsChanged, this, &Navigation::crsChanged );
  }

  mMapSettings = mapSettings;

  connect( mMapSettings, &QgsQuickMapSettings::destinationCrsChanged, this, &Navigation::crsChanged );
  crsChanged();

  emit mapSettingsChanged();
}

void Navigation::crsChanged()
{
  mDa = QgsDistanceArea();
  mDa.setEllipsoid( QgsProject::instance()->ellipsoid() );
  mDa.setSourceCrs( mMapSettings->destinationCrs(), QgsProject::instance()->transformContext() );
  mModel->setCrs( mMapSettings->destinationCrs() );
}

QgsPoint Navigation::location() const
{
  return mLocation;
}

void Navigation::setLocation( const QgsPoint &point )
{
  if ( mLocation == point )
    return;

  mLocation = point;
  emit locationChanged();

  updateDetails();
}

QgsPoint Navigation::destination() const
{
  return mModel->destination();
}

void Navigation::setDestination( const QgsPoint &point )
{
  clearDestinationFeature();
  mModel->setDestination( point );
}

QString Navigation::destinationName() const
{
  return mDestinationName;
}

void Navigation::setDestinationFeature( const QgsFeature &feature, QgsVectorLayer *layer )
{
  if ( !layer || feature.geometry().isEmpty() )
    return;

  mGeometry = feature.geometry();
  if ( layer->crs() != mMapSettings->destinationCrs() )
  {
    QgsCoordinateTransform transform( layer->crs(), mMapSettings->destinationCrs(), QgsProject::instance()->transformContext() );
    Qgis::GeometryOperationResult result = mGeometry.transform( transform );
    if ( result != Qgis::GeometryOperationResult::Success )
    {
      mGeometry = QgsGeometry();
    }
  }

  if ( !mGeometry.isNull() )
  {
    mDestinationName = FeatureUtils::displayName( layer, feature );
    emit destinationNameChanged();
    mVertexCount = mGeometry.get()->nCoordinates() - ( mGeometry.type() == QgsWkbTypes::PolygonGeometry ? 1 : 0 );
    emit destinationFeatureVertexCountChanged();
    mCurrentVertex = -1;
    nextDestinationVertex();
  }
  else
  {
    mDestinationName = FeatureUtils::displayName( layer, feature );
    emit destinationNameChanged();
    mVertexCount = 0;
    emit destinationFeatureVertexCountChanged();
    mCurrentVertex = -1;
    emit destinationFeatureCurrentVertexChanged();
    mModel->setDestination( QgsPoint() );
  }
}

void Navigation::clearDestinationFeature()
{
  if ( !mGeometry.isNull() )
  {
    mGeometry = QgsGeometry();
    mDestinationName.clear();
    emit destinationNameChanged();
    mVertexCount = 0;
    emit destinationFeatureVertexCountChanged();
    mCurrentVertex = -1;
    emit destinationFeatureCurrentVertexChanged();
  }
}

void Navigation::nextDestinationVertex()
{
  if ( mGeometry.isNull() )
    return;

  if ( mCurrentVertex >= mVertexCount )
  {
    mCurrentVertex = 0;
  }
  else
  {
    mCurrentVertex++;
  }
  emit destinationFeatureCurrentVertexChanged();

  setDestinationFromCurrentVertex();
}

void Navigation::previousDestinationVertex()
{
  if ( mGeometry.isNull() )
    return;

  if ( mCurrentVertex <= 0 )
  {
    mCurrentVertex = mVertexCount;
  }
  else
  {
    mCurrentVertex--;
  }
  emit destinationFeatureCurrentVertexChanged();

  setDestinationFromCurrentVertex();
}

void Navigation::setDestinationFromCurrentVertex()
{
  if ( mCurrentVertex == 0 )
  {
    const QgsGeometry pointOnSurface = mGeometry.pointOnSurface();
    if ( !pointOnSurface.isNull() )
    {
      mModel->setDestination( pointOnSurface.vertexAt( 0 ) );
    }
    else
    {
      mCurrentVertex++;
      mModel->setDestination( mGeometry.vertexAt( mCurrentVertex - 1 ) );
    }
  }
  else
  {
    mModel->setDestination( mGeometry.vertexAt( mCurrentVertex - 1 ) );
  }
}

int Navigation::destinationFeatureCurrentVertex() const
{
  return mCurrentVertex;
}

int Navigation::destinationFeatureVertexCount() const
{
  return mVertexCount;
}

void Navigation::updateDetails()
{
  QgsPointSequence points = mModel->points();
  if ( points.isEmpty() || mLocation.isEmpty() )
  {
    mPath = QgsGeometry();
    mDistance = 0.0;
    mBearing = 0.0;
    emit detailsChanged();
    return;
  }
  points.prepend( mLocation );

  mPath = QgsGeometry( new QgsLineString( points ) );
  mDistance = mDa.measureLine( mLocation, destination() );
  mBearing = mDa.bearing( mLocation, destination() ) * 180 / M_PI;

  emit detailsChanged();
}

void Navigation::clear()
{
  mModel->clear();
  updateDetails();
}
