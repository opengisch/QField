

# File parametizedimage.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**parametizedimage.cpp**](parametizedimage_8cpp.md)

[Go to the documentation of this file](parametizedimage_8cpp.md)


```C++
/***************************************************************************
 parametizedimage.cpp - ParametizedImage

 ---------------------
 begin                : 21.05.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "parametizedimage.h"

#include <QPainter>
#include <QQuickWindow>
#include <QScreen>
#include <qgsapplication.h>
#include <qgsimagecache.h>
#include <qgssvgcache.h>

ParametizedImage::ParametizedImage( QQuickItem *parent )
  : QQuickPaintedItem( parent )
{
}

QString ParametizedImage::source() const
{
  return mSource;
}

void ParametizedImage::setSource( const QString &source )
{
  if ( mSource == source )
    return;

  mSource = source;
  emit sourceChanged();

  QFileInfo fi( mSource );
  mIsValid = fi.exists();
  mIsRaster = fi.suffix().toLower() != QStringLiteral( "svg" );

  if ( mIsValid )
  {
    if ( mIsRaster )
    {
      mSourceSize = QgsApplication::instance()->imageCache()->originalSize( mSource, true );
    }
    else
    {
      mSourceSize = QgsApplication::instance()->svgCache()->svgViewboxSize( mSource, size().width(), QColor( 0, 0, 0 ), QColor( 255, 0, 0 ), 10, 1, 0, true );
    }

    update();
  }
}

QColor ParametizedImage::fillColor() const
{
  return mFillColor;
}

void ParametizedImage::setFillColor( const QColor &color )
{
  if ( mFillColor == color )
    return;

  mFillColor = color;
  emit fillColorChanged();

  if ( mIsValid && !mIsRaster )
    update();
}

QColor ParametizedImage::strokeColor() const
{
  return mStrokeColor;
}

void ParametizedImage::setStrokeColor( const QColor &color )
{
  if ( mStrokeColor == color )
    return;

  mStrokeColor = color;
  emit strokeColorChanged();

  if ( mIsValid && !mIsRaster )
    update();
}

double ParametizedImage::strokeWidth() const
{
  return mStrokeWidth;
}

void ParametizedImage::setStrokeWidth( double width )
{
  if ( mStrokeWidth == width )
    return;

  mStrokeWidth = width;
  emit strokeWidthChanged();

  if ( mIsValid && !mIsRaster )
    update();
}

void ParametizedImage::paint( QPainter *painter )
{
  if ( !mIsValid )
    return;

  const double sourceRatio = mSourceSize.width() / mSourceSize.height();
  const double itemRatio = size().width() / size().height();

  bool fitsInCache = false;
  if ( mIsRaster )
  {
    const double drawnWidth = std::ceil( sourceRatio >= itemRatio ? size().width() : mSourceSize.width() * size().height() / mSourceSize.height() );
    const double drawnHeight = std::ceil( sourceRatio >= itemRatio ? mSourceSize.height() * size().width() / mSourceSize.width() : size().height() );
    const double devicePixelRatio = window()->screen()->devicePixelRatio();
    QImage sourceImage = QgsApplication::instance()->imageCache()->pathAsImage( mSource, QSize( drawnWidth * devicePixelRatio, drawnHeight * devicePixelRatio ), true, 1, fitsInCache, true );
    painter->drawImage( QRectF( ( size().width() - drawnWidth ) / 2, ( size().height() - drawnHeight ) / 2, drawnWidth, drawnHeight ), sourceImage );
  }
  else
  {
    const double drawnWidth = sourceRatio >= itemRatio ? size().width() : size().height() * sourceRatio;
    QPicture sourcePicture = QgsApplication::instance()->svgCache()->svgAsPicture( mSource, drawnWidth, mFillColor, mStrokeColor, mStrokeWidth, 1, fitsInCache, 0, true );
    painter->drawPicture( size().width() / 2, size().height() / 2, sourcePicture );
  }
}
```


