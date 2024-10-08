/***************************************************************************
  gridmodel.cpp - GridModel

 ---------------------
 begin                : 4.10.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : mathieu@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "gridmodel.h"

GridModel::GridModel( QObject *parent )
  : QObject( parent )
{
}

void GridModel::setEnabled( bool enabled )
{
  if ( mEnabled == enabled )
    return;

  mEnabled = enabled;
  emit enabledChanged();

  if ( mEnabled )
  {
    update();
  }
  else
  {
    if ( !mLines.isEmpty() || !mMarkers.isEmpty() || !mAnnotations.isEmpty() )
    {
      mLines.clear();
      mMarkers.clear();
      mAnnotations.clear();
      emit gridChanged();
    }
  }
}

void GridModel::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
  {
    return;
  }

  if ( mMapSettings )
  {
    disconnect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, &GridModel::update );
  }

  mMapSettings = mapSettings;
  emit mapSettingsChanged();


  if ( mMapSettings )
  {
    connect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, &GridModel::update );
  }
}

void GridModel::setXInterval( double interval )
{
  if ( mXInterval == interval )
    return;

  mXInterval = interval;
  emit xIntervalChanged();

  update();
}

void GridModel::setYInterval( double interval )
{
  if ( mYInterval == interval )
    return;

  mYInterval = interval;
  emit yIntervalChanged();

  update();
}

void GridModel::setXOffset( double offset )
{
  if ( mXOffset == offset )
    return;

  mXOffset = offset;
  emit xOffsetChanged();

  update();
}

void GridModel::setYOffset( double offset )
{
  if ( mYOffset == offset )
    return;

  mYOffset = offset;
  emit yOffsetChanged();

  update();
}

void GridModel::setPrepareLines( bool prepare )
{
  if ( mPrepareLines == prepare )
    return;

  mPrepareLines = prepare;
  emit prepareLinesChanged();

  if ( mPrepareLines )
  {
    update();
  }
  else
  {
    if ( !mLines.isEmpty() )
    {
      mLines.clear();
      emit gridChanged();
    }
  }
}

void GridModel::setPrepareMarkers( bool prepare )
{
  if ( mPrepareMarkers == prepare )
    return;

  mPrepareMarkers = prepare;
  emit prepareMarkersChanged();

  if ( mPrepareMarkers )
  {
    update();
  }
  else
  {
    if ( !mMarkers.isEmpty() )
    {
      mMarkers.clear();
      emit gridChanged();
    }
  }
}

void GridModel::setPrepareAnnotations( bool prepare )
{
  if ( mPrepareAnnotations == prepare )
    return;

  mPrepareAnnotations = prepare;
  emit prepareAnnotationsChanged();

  if ( mPrepareAnnotations )
  {
    update();
  }
  else
  {
    if ( !mAnnotations.isEmpty() )
    {
      mAnnotations.clear();
      emit gridChanged();
    }
  }
}

void GridModel::update()
{
  if ( !mEnabled || !mMapSettings )
  {
    return;
  }

  bool hadGrid = !mLines.isEmpty() || !mMarkers.isEmpty() || !mAnnotations.isEmpty();

  mLines.clear();
  mMarkers.clear();
  mAnnotations.clear();

  const QgsRectangle visibleExtent = mMapSettings->visibleExtent();
  double smallestScreenInterval = std::min( mXInterval / mMapSettings->mapUnitsPerPoint(), mYInterval / mMapSettings->mapUnitsPerPoint() );
  if ( smallestScreenInterval < ( mPrepareMarkers ? 20 : 10 ) )
  {
    if ( hadGrid )
    {
      emit gridChanged();
    }
    return;
  }

  QList<QPointF> line;
  QPointF intersectionPoint;

  if ( mPrepareMarkers )
  {
    double xPos = visibleExtent.xMinimum() - std::fmod( visibleExtent.xMinimum(), mXInterval ) + mXOffset;
    while ( xPos <= visibleExtent.xMaximum() )
    {
      double yPos = visibleExtent.yMinimum() - std::fmod( visibleExtent.yMinimum(), mYInterval ) + mYOffset;
      while ( yPos <= visibleExtent.yMaximum() )
      {
        mMarkers << mMapSettings->coordinateToScreen( QgsPoint( xPos, yPos ) );
        yPos += mYInterval;
      }
      xPos += mXInterval;
    }
  }

  const QSizeF sceneSize = mMapSettings->outputSize() / mMapSettings->devicePixelRatio();
  if ( mPrepareLines || mPrepareAnnotations )
  {
    double xPos = visibleExtent.xMinimum() - std::fmod( visibleExtent.xMinimum(), mXInterval ) + mXOffset;
    const QLineF topBorder( QPointF( 0, 0 ), QPointF( sceneSize.width(), 0 ) );
    const QLineF bottomBorder( QPointF( 0, sceneSize.height() ), QPointF( sceneSize.width(), sceneSize.height() ) );
    while ( xPos <= visibleExtent.xMaximum() )
    {
      const QLineF currentLine( mMapSettings->coordinateToScreen( QgsPoint( xPos, visibleExtent.yMinimum() ) ), mMapSettings->coordinateToScreen( QgsPoint( xPos, visibleExtent.yMaximum() ) ) );

      if ( mPrepareAnnotations )
      {
        if ( currentLine.intersects( topBorder, &intersectionPoint ) )
        {
          mAnnotations << GridAnnotation( GridAnnotation::Top, intersectionPoint, xPos );
        }
        if ( currentLine.intersects( bottomBorder, &intersectionPoint ) )
        {
          mAnnotations << GridAnnotation( GridAnnotation::Bottom, intersectionPoint, xPos );
        }
      }

      if ( mPrepareLines )
      {
        line << currentLine.p1() << currentLine.p2();
        mLines << line;
        line.clear();
      }

      xPos += mXInterval;
    }

    double yPos = visibleExtent.yMinimum() - std::fmod( visibleExtent.yMinimum(), mYInterval ) + mYOffset;
    const QLineF leftBorder( QPointF( 0, 0 ), QPointF( 0, sceneSize.height() ) );
    const QLineF rightBorder( QPointF( sceneSize.width(), 0 ), QPointF( sceneSize.width(), sceneSize.height() ) );
    while ( yPos <= visibleExtent.yMaximum() )
    {
      const QLineF currentLine( mMapSettings->coordinateToScreen( QgsPoint( visibleExtent.xMinimum(), yPos ) ), mMapSettings->coordinateToScreen( QgsPoint( visibleExtent.xMaximum(), yPos ) ) );

      if ( mPrepareAnnotations )
      {
        if ( currentLine.intersects( leftBorder, &intersectionPoint ) )
        {
          mAnnotations << GridAnnotation( GridAnnotation::Left, intersectionPoint, yPos );
        }
        if ( currentLine.intersects( rightBorder, &intersectionPoint ) )
        {
          mAnnotations << GridAnnotation( GridAnnotation::Right, intersectionPoint, yPos );
        }
      }

      if ( mPrepareLines )
      {
        line << currentLine.p1() << currentLine.p2();
        mLines << line;
        line.clear();
      }

      yPos += mYInterval;
    }
  }

  emit gridChanged();
}
