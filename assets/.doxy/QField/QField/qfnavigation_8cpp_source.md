

# File qfnavigation.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfnavigation.cpp**](qfnavigation_8cpp.md)

[Go to the documentation of this file](qfnavigation_8cpp.md)


```C++
/***************************************************************************
 qfnavigation.cpp - QfNavigation

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

#include "qffeatureutils.h"
#include "qfnavigation.h"
#include "qfnavigationmodel.h"

#include <qgslinestring.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>

QfNavigation::QfNavigation()
  : QObject()
{
  mModel.reset( new QfNavigationModel() );
  mModel->restore();

  connect( mModel.get(), &QfNavigationModel::destinationChanged, this, &QfNavigation::isActiveChanged );
  connect( mModel.get(), &QfNavigationModel::destinationChanged, this, &QfNavigation::destinationChanged );
  connect( mModel.get(), &QfNavigationModel::destinationChanged, this, &QfNavigation::updateDetails );
  connect( mModel.get(), &QfNavigationModel::modelReset, this, &QfNavigation::isActiveChanged );
  connect( mModel.get(), &QfNavigationModel::modelReset, this, &QfNavigation::destinationChanged );
  connect( mModel.get(), &QfNavigationModel::modelReset, this, &QfNavigation::updateDetails );

  mProximityAlarmTimer.setInterval( 250 );
  mProximityAlarmTimer.setSingleShot( false );
  connect( &mProximityAlarmTimer, &QTimer::timeout, this, [this] {
    if ( QDateTime::currentMSecsSinceEpoch() > mLastProximityAlarm + mProximityAlarmInterval )
    {
      triggerProximityAlarm();
    }
  } );
}

QfNavigation::~QfNavigation()
{
}

bool QfNavigation::isActive() const
{
  return !destination().isEmpty();
}

void QfNavigation::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
    return;

  if ( mMapSettings )
  {
    disconnect( mMapSettings, &QgsQuickMapSettings::destinationCrsChanged, this, &QfNavigation::crsChanged );
  }

  mMapSettings = mapSettings;

  connect( mMapSettings, &QgsQuickMapSettings::destinationCrsChanged, this, &QfNavigation::crsChanged );
  crsChanged();

  emit mapSettingsChanged();
}

void QfNavigation::crsChanged()
{
  mDa = QgsDistanceArea();
  mDa.setEllipsoid( QgsProject::instance()->ellipsoid() );
  mDa.setSourceCrs( mMapSettings->destinationCrs(), QgsProject::instance()->transformContext() );
  mModel->setCrs( mMapSettings->destinationCrs() );
}

QgsPoint QfNavigation::location() const
{
  return mLocation;
}

void QfNavigation::setLocation( const QgsPoint &point )
{
  if ( mLocation == point )
    return;

  mLocation = point;
  emit locationChanged();

  updateDetails();
}

QgsPoint QfNavigation::destination() const
{
  return mModel->destination();
}

void QfNavigation::setDestination( const QgsPoint &point )
{
  clearDestinationFeature();
  mModel->setDestination( point );
}

QString QfNavigation::destinationName() const
{
  return mDestinationName;
}

void QfNavigation::setDestinationFeature( const QgsFeature &feature, QgsVectorLayer *layer )
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
    mFeatureName = QfFeatureUtils::displayName( layer, feature );
    mVertexCount = mGeometry.get()->nCoordinates() - ( mGeometry.type() == Qgis::GeometryType::Polygon ? 1 : 0 );
    emit destinationFeatureVertexCountChanged();
    mCurrentVertex = -1;
    nextDestinationVertex();
  }
  else
  {
    mFeatureName.clear();
    mDestinationName.clear();
    emit destinationNameChanged();
    mVertexCount = 0;
    emit destinationFeatureVertexCountChanged();
    mCurrentVertex = -1;
    emit destinationFeatureCurrentVertexChanged();
    mModel->setDestination( QgsPoint() );
  }
}

void QfNavigation::clearDestinationFeature()
{
  if ( !mGeometry.isNull() )
  {
    mGeometry = QgsGeometry();
    mFeatureName.clear();
    mDestinationName.clear();
    emit destinationNameChanged();
    mVertexCount = 0;
    emit destinationFeatureVertexCountChanged();
    mCurrentVertex = -1;
    emit destinationFeatureCurrentVertexChanged();
  }
}

void QfNavigation::nextDestinationVertex()
{
  if ( mGeometry.isNull() )
    return;

  if ( mCurrentVertex >= ( mGeometry.type() == Qgis::GeometryType::Point
                             ? mVertexCount - 1
                             : mVertexCount ) )
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

void QfNavigation::previousDestinationVertex()
{
  if ( mGeometry.isNull() )
    return;

  if ( mCurrentVertex <= 0 )
  {
    mCurrentVertex = mGeometry.type() == Qgis::GeometryType::Point
                       ? mVertexCount - 1
                       : mVertexCount;
  }
  else
  {
    mCurrentVertex--;
  }
  emit destinationFeatureCurrentVertexChanged();

  setDestinationFromCurrentVertex();
}

void QfNavigation::setDestinationFromCurrentVertex()
{
  switch ( mGeometry.type() )
  {
    case Qgis::GeometryType::Point:
      mDestinationName = mFeatureName + ( mVertexCount > 1 ? QStringLiteral( ": %1/%2" ).arg( mCurrentVertex + 1 ).arg( mVertexCount ) : QString() );
      emit destinationNameChanged();
      mModel->setDestination( mGeometry.vertexAt( mCurrentVertex ) );
      break;

    case Qgis::GeometryType::Line:
    case Qgis::GeometryType::Polygon:
      mDestinationName = mFeatureName + ( mCurrentVertex == 0 ? QStringLiteral( " (%1)" ).arg( QObject::tr( "centroid" ) ) : QStringLiteral( ": %1/%2" ).arg( mCurrentVertex ).arg( mVertexCount ) );
      emit destinationNameChanged();
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
      break;

    case Qgis::GeometryType::Unknown:
    case Qgis::GeometryType::Null:
      break;
  }
}

int QfNavigation::destinationFeatureCurrentVertex() const
{
  return mCurrentVertex;
}

int QfNavigation::destinationFeatureVertexCount() const
{
  return mVertexCount;
}

void QfNavigation::updateDetails()
{
  QgsPointSequence points = mModel->points();
  if ( points.isEmpty() || mLocation.isEmpty() )
  {
    mPath = QgsGeometry();
    mDistance = std::numeric_limits<double>::quiet_NaN();
    mVerticalDistance = std::numeric_limits<double>::quiet_NaN();
    mBearing = std::numeric_limits<double>::quiet_NaN();
    emit detailsChanged();
    return;
  }
  points.prepend( mLocation );
  mPath = QgsGeometry( new QgsLineString( points ) );

  const QgsPoint destinationPoint = destination();
  const bool handleZ = QgsWkbTypes::hasZ( mLocation.wkbType() )
                       && QgsWkbTypes::hasZ( destinationPoint.wkbType() );

  try
  {
    mDistance = mDa.measureLine( mLocation, destinationPoint );
  }
  catch ( const QgsException & )
  {
    mDistance = std::numeric_limits<double>::quiet_NaN();
  }

  if ( handleZ )
  {
    mVerticalDistance = destinationPoint.z() - mLocation.z();
  }
  else
  {
    mVerticalDistance = std::numeric_limits<double>::quiet_NaN();
  }
  mBearing = mDa.bearing( mLocation, destinationPoint ) * 180 / M_PI;
  mBearing = std::fmod( mBearing + 360.0, 360.0 );

  emit detailsChanged();

  updateProximityAlarmState();
}

void QfNavigation::updateProximityAlarmState()
{
  if ( mProximityAlarm && mDa.lengthUnits() != Qgis::DistanceUnit::Unknown )
  {
    if ( !std::isnan( mDistance ) && mDistance <= mProximityAlarmThreshold )
    {
      mProximityAlarmInterval = 200 + ( 2000 * mDistance / mProximityAlarmThreshold );
      if ( !mProximityAlarmTimer.isActive() )
      {
        triggerProximityAlarm();
        mProximityAlarmTimer.start();
      }
    }
    else
    {
      if ( mProximityAlarmTimer.isActive() )
      {
        mProximityAlarmTimer.stop();
      }
    }
  }
  else
  {
    if ( mProximityAlarmTimer.isActive() )
    {
      mProximityAlarmTimer.stop();
    }
  }
}

void QfNavigation::setProximityAlarm( const bool enabled )
{
  if ( mProximityAlarm == enabled )
  {
    return;
  }

  mProximityAlarm = enabled;
  emit proximityAlarmChanged();

  updateProximityAlarmState();
}

void QfNavigation::setProximityAlarmThreshold( const double &threshold )
{
  if ( mProximityAlarmThreshold == threshold )
  {
    return;
  }

  mProximityAlarmThreshold = threshold;
  emit proximityAlarmThresholdChanged();

  updateProximityAlarmState();
}

void QfNavigation::clear()
{
  mModel->clear();
  updateDetails();
}

void QfNavigation::triggerProximityAlarm()
{
  if ( !mProximitySound )
  {
    mProximitySound = std::make_unique<QSoundEffect>();
    mProximitySound->setSource( QUrl( QStringLiteral( "qrc:/sounds/proximity_alarm.wav" ) ) );
  }
  mProximitySound->play();
  mLastProximityAlarm = QDateTime::currentMSecsSinceEpoch();
}
```


