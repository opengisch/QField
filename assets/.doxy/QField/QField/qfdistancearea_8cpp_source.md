

# File qfdistancearea.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfdistancearea.cpp**](qfdistancearea_8cpp.md)

[Go to the documentation of this file](qfdistancearea_8cpp.md)


```C++
/***************************************************************************
  qfdistancearea.cpp - QfDistanceArea

 ---------------------
 begin                : 23.2.2017
 copyright            : (C) 2017 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfdistancearea.h"
#include "qfrubberbandmodel.h"

#include <qgis.h>
#include <qgslinestring.h>
#include <qgspolygon.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>

QfDistanceArea::QfDistanceArea( QObject *parent )
  : QObject( parent )
  , mRubberbandModel( nullptr )
  , mProject( nullptr )
{
}

void QfDistanceArea::init()
{
  if ( mProject )
  {
    mDistanceArea.setEllipsoid( mProject->ellipsoid() );
    mDistanceArea.setSourceCrs( mCrs, mProject->transformContext() );
  }
  else
  {
    mDistanceArea.setEllipsoid( Qgis::geoNone() );
  }

  emit lengthUnitsChanged();
  emit areaUnitsChanged();
}

QgsProject *QfDistanceArea::project() const
{
  return mProject;
}

void QfDistanceArea::setProject( QgsProject *project )
{
  if ( mProject == project )
    return;

  if ( mProject )
    disconnect( mProject, &QgsProject::readProject, this, &QfDistanceArea::init );

  mProject = project;

  if ( mProject )
    connect( mProject, &QgsProject::readProject, this, &QfDistanceArea::init );

  init();

  emit projectChanged();
}

QfRubberbandModel *QfDistanceArea::rubberbandModel() const
{
  return mRubberbandModel;
}

void QfDistanceArea::setRubberbandModel( QfRubberbandModel *rubberbandModel )
{
  if ( mRubberbandModel == rubberbandModel )
    return;

  if ( mRubberbandModel )
  {
    disconnect( mRubberbandModel, &QfRubberbandModel::vertexChanged, this, &QfDistanceArea::lengthChanged );
    disconnect( mRubberbandModel, &QfRubberbandModel::vertexChanged, this, &QfDistanceArea::perimeterChanged );
    disconnect( mRubberbandModel, &QfRubberbandModel::vertexChanged, this, &QfDistanceArea::areaChanged );
    disconnect( mRubberbandModel, &QfRubberbandModel::vertexChanged, this, &QfDistanceArea::segmentLengthChanged );
    disconnect( mRubberbandModel, &QfRubberbandModel::vertexChanged, this, &QfDistanceArea::azimuthChanged );
  }

  mRubberbandModel = rubberbandModel;

  if ( mRubberbandModel )
  {
    connect( mRubberbandModel, &QfRubberbandModel::vertexChanged, this, &QfDistanceArea::lengthChanged );
    connect( mRubberbandModel, &QfRubberbandModel::vertexChanged, this, &QfDistanceArea::perimeterChanged );
    connect( mRubberbandModel, &QfRubberbandModel::vertexChanged, this, &QfDistanceArea::areaChanged );
    connect( mRubberbandModel, &QfRubberbandModel::vertexChanged, this, &QfDistanceArea::segmentLengthChanged );
    connect( mRubberbandModel, &QfRubberbandModel::vertexChanged, this, &QfDistanceArea::azimuthChanged );
  }

  emit rubberbandModelChanged();
}

QgsCoordinateReferenceSystem QfDistanceArea::crs() const
{
  return mCrs;
}

void QfDistanceArea::setCrs( const QgsCoordinateReferenceSystem &crs )
{
  if ( mCrs == crs )
    return;

  mCrs = crs;
  init();
  emit crsChanged();
}
#if 0
void QfDistanceArea::setGeometry( QfGeometry *geometry )
{
  if ( mGeometry == geometry )
    return;

  if ( mGeometry )
    disconnect( mGeometry, &QfGeometry::rubberbandModelChanged, this, &QfDistanceArea::rubberbandModelChanged );

  mGeometry = geometry;
  rubberbandModelChanged();

  if ( mGeometry )
    connect( mGeometry, &QfGeometry::rubberbandModelChanged, this, &QfDistanceArea::rubberbandModelChanged );

  init();

  emit geometryChanged();
}
#endif

qreal QfDistanceArea::length() const
{
  double length = std::numeric_limits<double>::quiet_NaN();
  if ( mRubberbandModel )
  {
    try
    {
      length = mDistanceArea.measureLine( mRubberbandModel->flatPointSequence( mCrs ) );
    }
    catch ( const QgsException & )
    {
      length = std::numeric_limits<double>::quiet_NaN();
    }
  }

  return length;
}

bool QfDistanceArea::lengthValid() const
{
  if ( !mRubberbandModel )
    return false;

  switch ( mRubberbandModel->geometryType() )
  {
    case Qgis::GeometryType::Point:
      return false;

    case Qgis::GeometryType::Line:
      [[fallthrough]];
    case Qgis::GeometryType::Polygon:
      return mRubberbandModel->vertexCount() >= 2;

    default:
      return false;
  }
}

qreal QfDistanceArea::perimeter() const
{
  if ( mRubberbandModel )
  {
    QgsGeometry geom( new QgsPolygon( new QgsLineString( mRubberbandModel->flatPointSequence( mCrs ) ) ) );
    return mDistanceArea.measurePerimeter( geom );
  }

  return qQNaN();
}

bool QfDistanceArea::perimeterValid() const
{
  if ( !mRubberbandModel )
    return false;

  switch ( mRubberbandModel->geometryType() )
  {
    case Qgis::GeometryType::Point:
      [[fallthrough]];
    case Qgis::GeometryType::Line:
      return false;

    case Qgis::GeometryType::Polygon:
      return mRubberbandModel->vertexCount() >= 3;

    default:
      return false;
  }
}

qreal QfDistanceArea::area() const
{
  double area = std::numeric_limits<double>::quiet_NaN();
  if ( mRubberbandModel )
  {
    try
    {
      area = mDistanceArea.measurePolygon( mRubberbandModel->flatPointSequence( mCrs ) );
    }
    catch ( const QgsException & )
    {
      area = std::numeric_limits<double>::quiet_NaN();
    }
  }

  return area;
}

bool QfDistanceArea::areaValid() const
{
  if ( !mRubberbandModel )
    return false;

  switch ( mRubberbandModel->geometryType() )
  {
    case Qgis::GeometryType::Point:
      return false;

    case Qgis::GeometryType::Line:
      return false;

    case Qgis::GeometryType::Polygon:
      return mRubberbandModel->vertexCount() >= 3;

    default:
      return false;
  }
}

qreal QfDistanceArea::segmentLength() const
{
  if ( !mRubberbandModel )
    return qQNaN();

  if ( mRubberbandModel->vertexCount() < 2 )
    return qQNaN();

  QVector<QgsPointXY> points = mRubberbandModel->flatPointSequence( mCrs );

  auto pointIt = points.constEnd() - 1;

  QVector<QgsPointXY> flatPoints;

  flatPoints << *pointIt;
  pointIt--;
  flatPoints << *pointIt;

  double length = std::numeric_limits<double>::quiet_NaN();
  try
  {
    length = mDistanceArea.measureLine( flatPoints );
  }
  catch ( const QgsException & )
  {
    length = std::numeric_limits<double>::quiet_NaN();
  }
  return length;
}

qreal QfDistanceArea::azimuth() const
{
  if ( !mRubberbandModel )
    return qQNaN();

  if ( mRubberbandModel->vertexCount() < 2 )
    return qQNaN();

  QVector<QgsPointXY> points = mRubberbandModel->flatPointSequence( mCrs );
  QgsPoint startPoint( points.at( points.size() - 2 ) );
  QgsPoint endPoint( points.at( points.size() - 1 ) );

  return startPoint.azimuth( endPoint );
}

Qgis::DistanceUnit QfDistanceArea::lengthUnits() const
{
  return mDistanceArea.lengthUnits();
}

Qgis::AreaUnit QfDistanceArea::areaUnits() const
{
  return mDistanceArea.areaUnits();
}

double QfDistanceArea::convertLengthMeansurement( double length, Qgis::DistanceUnit toUnits ) const
{
  return mDistanceArea.convertLengthMeasurement( length, toUnits );
}

double QfDistanceArea::convertAreaMeansurement( double area, Qgis::AreaUnit toUnits ) const
{
  return mDistanceArea.convertAreaMeasurement( area, toUnits );
}
```


