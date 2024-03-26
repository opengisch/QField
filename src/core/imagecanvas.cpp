/***************************************************************************
 imagecanvas.cpp - ImageCanvas

 ---------------------
 begin                : 24.03.2024
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

#include "imagecanvas.h"

#include <QPainter>

#include <cmath>

ImageCanvas::ImageCanvas()
  : QQuickPaintedItem()
{
}

void ImageCanvas::createBlankCanvas( int width, int height, QColor backgroundColor )
{
  mBackgroundImage = QImage( QSize( width, height ), QImage::Format_ARGB32 );
  mBackgroundImage.fill( backgroundColor );

  mCanvasScale = -1;
  fitCanvas();
}

void ImageCanvas::createCanvasFromImage( const QString &path )
{
  mBackgroundImage = QImage( path );

  mCanvasScale = -1;
  fitCanvas();
}

void ImageCanvas::fitCanvas()
{
  double scale = 1.0;
  if ( !mBackgroundImage.isNull() )
  {
    const QSize backgroundImageSize = mBackgroundImage.size();
    if ( backgroundImageSize.width() > size().width() )
    {
      scale = size().width() / backgroundImageSize.width();
    }
    if ( backgroundImageSize.height() * scale > size().height() )
    {
      scale = size().height() / backgroundImageSize.height();
    }
  }
  setCanvasScale( scale );
}

double ImageCanvas::canvasScale() const
{
  return mCanvasScale;
}

void ImageCanvas::setCanvasScale( double scale )
{
  if ( mCanvasScale == scale )
  {
    return;
  }

  mCanvasScale = scale;
  emit canvasScaleChanged();

  update();
}

void ImageCanvas::paint( QPainter *painter )
{
  if ( !mBackgroundImage.isNull() )
  {
    const QSize scaledSize = mBackgroundImage.size() * mCanvasScale;
    painter->drawImage( QRect( size().width() / 2 - scaledSize.width() / 2,
                               size().height() / 2 - scaledSize.height() / 2,
                               scaledSize.width(),
                               scaledSize.height() ),
                        mBackgroundImage );
  }
}
