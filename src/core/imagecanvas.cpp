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

  fitCanvas();
}

void ImageCanvas::createCanvasFromImage( const QString &path )
{
  mBackgroundImage = QImage( path );

  fitCanvas();
}

void ImageCanvas::fitCanvas()
{
  double scale = 1.0;
  if ( !mBackgroundImage.isNull() )
  {
    const QSizeF itemSize( size().width() - 30, size().height() - 30 );
    const QSizeF backgroundImageSize = mBackgroundImage.size();
    if ( backgroundImageSize.width() > itemSize.width() )
    {
      scale = itemSize.width() / backgroundImageSize.width();
    }
    if ( backgroundImageSize.height() * scale > size().height() )
    {
      scale = itemSize.height() / backgroundImageSize.height();
    }
  }

  mZoomFactor = scale;
  emit zoomFactorChanged();

  mOffset = QPoint( 0, 0 );
  emit offsetChanged();

  update();
}

void ImageCanvas::pan( const QPointF &oldPosition, const QPointF &newPosition )
{
  setOffset( QPointF( mOffset.x() + ( newPosition.x() - oldPosition.x() ),
                      mOffset.y() + ( newPosition.y() - oldPosition.y() ) ) );
}

void ImageCanvas::zoom( double scale )
{
  setZoomFactor( mZoomFactor * scale );
}

double ImageCanvas::zoomFactor() const
{
  return mZoomFactor;
}

void ImageCanvas::setZoomFactor( double factor )
{
  if ( mZoomFactor == factor )
  {
    return;
  }

  mZoomFactor = factor;
  emit zoomFactorChanged();

  update();
}

QPointF ImageCanvas::offset() const
{
  return mOffset;
}

void ImageCanvas::setOffset( const QPointF &offset )
{
  if ( mOffset == offset )
  {
    return;
  }

  mOffset = offset;
  emit offsetChanged();

  update();
}

void ImageCanvas::paint( QPainter *painter )
{
  if ( !mBackgroundImage.isNull() )
  {
    const QSizeF scaledImageSize = mBackgroundImage.size() * mZoomFactor;
    const QRectF imageRect( ( size().width() / 2 - scaledImageSize.width() / 2 ) + mOffset.x(),
                            ( size().height() / 2 - scaledImageSize.height() / 2 ) + mOffset.y(),
                            scaledImageSize.width(),
                            scaledImageSize.height() );

    painter->setPen( Qt::NoPen );
    painter->setBrush( QBrush( fillColor().darker( 125 ) ) );

    painter->drawRect( imageRect.translated( 5, 5 ) );
    painter->drawImage( imageRect, mBackgroundImage );
  }
}
