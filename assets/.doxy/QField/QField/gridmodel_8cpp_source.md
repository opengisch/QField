

# File gridmodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**gridmodel.cpp**](gridmodel_8cpp.md)

[Go to the documentation of this file](gridmodel_8cpp.md)


```C++
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
    clear();
  }
}

void GridModel::setIndeterminate( bool indeterminate )
{
  if ( mIndeterminate == indeterminate )
    return;

  mIndeterminate = indeterminate;
  emit indeterminateChanged();

  if ( mEnabled )
  {
    update();
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
    disconnect( mMapSettings, &QgsQuickMapSettings::backgroundColorChanged, this, &GridModel::updateColors );
    disconnect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, &GridModel::update );
  }

  mMapSettings = mapSettings;
  emit mapSettingsChanged();


  if ( mMapSettings )
  {
    connect( mMapSettings, &QgsQuickMapSettings::backgroundColorChanged, this, &GridModel::updateColors );
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
    if ( !mMajorLines.isEmpty() )
    {
      mMajorLines.clear();
      emit majorLinesChanged();
    }
    if ( !mMinorLines.isEmpty() )
    {
      mMinorLines.clear();
      emit minorLinesChanged();
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
      emit markersChanged();
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
      emit annotationsChanged();
    }
  }
}

void GridModel::setAutoColor( bool autoColor )
{
  if ( mAutoColor == autoColor )
  {
    return;
  }

  mAutoColor = autoColor;
  emit autoColorChanged();

  if ( mAutoColor && mMapSettings )
  {
    updateColors();
  }
}

void GridModel::setMajorLineColor( const QColor &color )
{
  if ( mMajorLineColor == color )
  {
    return;
  }

  mMajorLineColor = color;
  emit majorLineColorChanged();
}

void GridModel::setMinorLineColor( const QColor &color )
{
  if ( mMinorLineColor == color )
  {
    return;
  }

  mMinorLineColor = color;
  emit minorLineColorChanged();
}

void GridModel::setMarkerColor( const QColor &color )
{
  if ( mMarkerColor == color )
  {
    return;
  }

  mMarkerColor = color;
  emit markerColorChanged();
}

void GridModel::setAnnotationColor( const QColor &color )
{
  if ( mAnnotationColor == color )
  {
    return;
  }

  mAnnotationColor = color;
  emit annotationColorChanged();
}

void GridModel::setAnnotationOutlineColor( const QColor &color )
{
  if ( mAnnotationOutlineColor == color )
  {
    return;
  }

  mAnnotationOutlineColor = color;
  emit annotationOutlineColorChanged();
}

void GridModel::setAnnotationHasOutline( bool hasOutline )
{
  if ( mAnnotationHasOutline == hasOutline )
  {
    return;
  }

  mAnnotationHasOutline = hasOutline;
  emit annotationHasOutlineChanged();
}

void GridModel::setAnnotationPrecision( int precision )
{
  if ( mAnnotationPrecision == precision )
  {
    return;
  }

  mAnnotationPrecision = precision;
  emit annotationPrecisionChanged();
}

void GridModel::clear()
{
  if ( !mMajorLines.isEmpty() )
  {
    mMajorLines.clear();
    emit majorLinesChanged();
  }
  if ( !mMinorLines.isEmpty() )
  {
    mMinorLines.clear();
    emit minorLinesChanged();
  }
  if ( !mMarkers.isEmpty() )
  {
    mMarkers.clear();
    emit markersChanged();
  }
  if ( !mAnnotations.isEmpty() )
  {
    mAnnotations.clear();
    emit annotationsChanged();
  }
}

void GridModel::update()
{
  if ( !mEnabled || !mMapSettings )
  {
    return;
  }

  const QgsRectangle visibleExtent = mMapSettings->visibleExtent();
  if ( qgsDoubleNear( mMapSettings->mapUnitsPerPoint(), 0.0 ) || visibleExtent.isEmpty() )
  {
    clear();
    return;
  }

  if ( mIndeterminate )
  {
    const double indeterminateInterval = mMapSettings->mapUnitsPerPoint() * 200;
    if ( qgsDoubleNear( mIndeterminateInterval, 0.0 ) || mIndeterminateInterval / indeterminateInterval > 10 || mIndeterminateInterval / indeterminateInterval < 0.1 )
    {
      mIndeterminateInterval = indeterminateInterval;
    }
    else if ( mIndeterminateInterval / indeterminateInterval >= 2 )
    {
      mIndeterminateInterval = mIndeterminateInterval / 2;
    }
    else if ( mIndeterminateInterval / indeterminateInterval <= 0.5 )
    {
      mIndeterminateInterval = mIndeterminateInterval * 2;
    }
  }
  else
  {
    double smallestScreenInterval = std::min( mXInterval / mMapSettings->mapUnitsPerPoint(), mYInterval / mMapSettings->mapUnitsPerPoint() );
    if ( qgsDoubleNear( smallestScreenInterval, 0.0 ) || smallestScreenInterval < ( mPrepareMarkers ? 20 : 10 ) )
    {
      clear();
      return;
    }
  }

  const bool hadMinorLines = !mMinorLines.isEmpty();

  mMajorLines.clear();
  mMinorLines.clear();
  mMarkers.clear();
  mAnnotations.clear();

  const double xInterval = mIndeterminate ? mIndeterminateInterval : mXInterval;
  const double yInterval = mIndeterminate ? mIndeterminateInterval : mYInterval;
  const double xOffset = mIndeterminate ? 0.0 : mXOffset;
  const double yOffset = mIndeterminate ? 0.0 : mYOffset;

  QList<QPointF> line;
  QPointF intersectionPoint;

  if ( mPrepareMarkers )
  {
    double xPos = visibleExtent.xMinimum() - std::fmod( visibleExtent.xMinimum(), xInterval ) + xOffset;
    while ( xPos <= visibleExtent.xMaximum() )
    {
      double yPos = visibleExtent.yMinimum() - std::fmod( visibleExtent.yMinimum(), yInterval ) + yOffset;
      while ( yPos <= visibleExtent.yMaximum() )
      {
        mMarkers << mMapSettings->coordinateToScreen( QgsPoint( xPos, yPos ) );
        yPos += yInterval;
      }
      xPos += xInterval;
    }
  }

  const QSizeF sceneSize = mMapSettings->outputSize() / mMapSettings->devicePixelRatio();
  if ( mIndeterminate && mPrepareLines )
  {
    const int minorDivisions = 5;
    const double minorInterval = xInterval / minorDivisions;

    double xPos = visibleExtent.xMinimum() - std::fmod( visibleExtent.xMinimum(), xInterval ) + xOffset;
    const QLineF topBorder( QPointF( 0, 0 ), QPointF( sceneSize.width(), 0 ) );
    const QLineF bottomBorder( QPointF( 0, sceneSize.height() ), QPointF( sceneSize.width(), sceneSize.height() ) );
    while ( xPos <= visibleExtent.xMaximum() )
    {
      const QLineF currentLine( mMapSettings->coordinateToScreen( QgsPoint( xPos, visibleExtent.yMinimum() ) ), mMapSettings->coordinateToScreen( QgsPoint( xPos, visibleExtent.yMaximum() ) ) );
      line << currentLine.p1() << currentLine.p2();
      mMajorLines << line;
      line.clear();

      for ( int i = 1; i < minorDivisions - 1; i++ )
      {
        const double minorXPos = xPos + minorInterval * i;
        const QLineF currentMinorLine( mMapSettings->coordinateToScreen( QgsPoint( minorXPos, visibleExtent.yMinimum() ) ), mMapSettings->coordinateToScreen( QgsPoint( minorXPos, visibleExtent.yMaximum() ) ) );
        line << currentMinorLine.p1() << currentMinorLine.p2();
        mMinorLines << line;
        line.clear();
      }

      xPos += xInterval;
    }

    double yPos = visibleExtent.yMinimum() - std::fmod( visibleExtent.yMinimum(), yInterval ) + yOffset;
    const QLineF leftBorder( QPointF( 0, 0 ), QPointF( 0, sceneSize.height() ) );
    const QLineF rightBorder( QPointF( sceneSize.width(), 0 ), QPointF( sceneSize.width(), sceneSize.height() ) );
    while ( yPos <= visibleExtent.yMaximum() )
    {
      const QLineF currentLine( mMapSettings->coordinateToScreen( QgsPoint( visibleExtent.xMinimum(), yPos ) ), mMapSettings->coordinateToScreen( QgsPoint( visibleExtent.xMaximum(), yPos ) ) );

      line << currentLine.p1() << currentLine.p2();
      mMajorLines << line;
      line.clear();

      for ( int i = 1; i < minorDivisions - 1; i++ )
      {
        const double minorYPos = yPos + minorInterval * i;
        const QLineF currentMinorLine( mMapSettings->coordinateToScreen( QgsPoint( visibleExtent.xMinimum(), minorYPos ) ), mMapSettings->coordinateToScreen( QgsPoint( visibleExtent.xMaximum(), minorYPos ) ) );
        line << currentMinorLine.p1() << currentMinorLine.p2();
        mMinorLines << line;
        line.clear();
      }

      yPos += yInterval;
    }
  }
  else if ( mPrepareLines || mPrepareAnnotations )
  {
    double xPos = visibleExtent.xMinimum() - std::fmod( visibleExtent.xMinimum(), xInterval ) + xOffset;
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
        mMajorLines << line;
        line.clear();
      }

      xPos += xInterval;
    }

    double yPos = visibleExtent.yMinimum() - std::fmod( visibleExtent.yMinimum(), yInterval ) + yOffset;
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
        mMajorLines << line;
        line.clear();
      }

      yPos += yInterval;
    }
  }

  if ( mPrepareMarkers )
  {
    emit markersChanged();
  }
  if ( mPrepareLines )
  {
    emit majorLinesChanged();
    if ( !mMinorLines.isEmpty() || hadMinorLines )
    {
      emit minorLinesChanged();
    }
  }
  if ( mPrepareAnnotations )
  {
    emit annotationsChanged();
  }
}

void GridModel::updateColors()
{
  if ( !mAutoColor )
  {
    return;
  }

  QColor backgroundColor = mMapSettings->backgroundColor();
  QColor color = backgroundColor.lightness() > 150 ? backgroundColor.darker( 105 ) : backgroundColor.lighter( 200 );
  setMinorLineColor( color );
  color = backgroundColor.lightness() > 150 ? color.darker( 115 ) : color.lighter( 180 );
  setMajorLineColor( color );
  setMarkerColor( color );
}
```


