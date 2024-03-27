/***************************************************************************
 drawingcanvas.cpp - DrawingCanvas

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

#include "drawingcanvas.h"

#include <QPainter>
#include <QPainterPath>
#include <QStandardPaths>

#include <cmath>

DrawingCanvas::DrawingCanvas()
  : QQuickPaintedItem()
{
}

void DrawingCanvas::createBlankCanvas( int width, int height, QColor backgroundColor )
{
  mBackgroundImage = QImage( QSize( width, height ), QImage::Format_ARGB32 );
  mBackgroundImage.fill( backgroundColor );

  mDrawingImage = QImage( QSize( width, height ), QImage::Format_ARGB32 );
  mDrawingImage.fill( Qt::transparent );

  fitCanvas();
}

void DrawingCanvas::createCanvasFromImage( const QString &path )
{
  mBackgroundImage = QImage( QUrl( path ).toLocalFile() );

  if ( !mBackgroundImage.isNull() )
  {
    mDrawingImage = QImage( mBackgroundImage.size(), QImage::Format_ARGB32 );
    mDrawingImage.fill( Qt::transparent );
  }
  else
  {
    mDrawingImage = QImage();
  }

  fitCanvas();
}

QString DrawingCanvas::save() const
{
  QImage image( mBackgroundImage.size(), QImage::Format_ARGB32 );
  image.fill( Qt::transparent );

  QPainter painter( &image );
  painter.drawImage( 0, 0, mBackgroundImage );
  painter.drawImage( 0, 0, mDrawingImage );

  const QString path = QStandardPaths::writableLocation( QStandardPaths::TempLocation ) + "/sketch.png";
  image.save( path );

  return path;
}

void DrawingCanvas::fitCanvas()
{
  double scale = 1.0;
  if ( !mBackgroundImage.isNull() )
  {
    const QSizeF itemSize( size().width() - 30, size().height() - 100 );
    const QSizeF backgroundImageSize = mBackgroundImage.size();
    if ( backgroundImageSize.width() > itemSize.width() )
    {
      scale = itemSize.width() / backgroundImageSize.width();
    }
    if ( backgroundImageSize.height() * scale > itemSize.height() )
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

void DrawingCanvas::pan( const QPointF &oldPosition, const QPointF &newPosition )
{
  setOffset( QPointF( mOffset.x() + ( newPosition.x() - oldPosition.x() ),
                      mOffset.y() + ( newPosition.y() - oldPosition.y() ) ) );
}

void DrawingCanvas::zoom( double scale )
{
  setZoomFactor( mZoomFactor * scale );
}

QColor DrawingCanvas::frameColor() const
{
  return mFrameColor;
}

void DrawingCanvas::setFrameColor( const QColor &color )
{
  if ( mFrameColor == color )
  {
    return;
  }

  mFrameColor = color;
  emit frameColorChanged();

  update();
}

double DrawingCanvas::zoomFactor() const
{
  return mZoomFactor;
}

void DrawingCanvas::setZoomFactor( double factor )
{
  if ( mZoomFactor == factor )
  {
    return;
  }

  mZoomFactor = factor;
  emit zoomFactorChanged();

  update();
}

QPointF DrawingCanvas::offset() const
{
  return mOffset;
}

void DrawingCanvas::setOffset( const QPointF &offset )
{
  if ( mOffset == offset )
  {
    return;
  }

  mOffset = offset;
  emit offsetChanged();

  update();
}

void DrawingCanvas::strokeBegin( const QPointF &point, const QColor color )
{
  mCurrentStroke.points.clear();
  mCurrentStroke.color = color;
  mCurrentStroke.points << itemToCanvas( point );
}

void DrawingCanvas::strokeMove( const QPointF &point )
{
  if ( mCurrentStroke.points.isEmpty() )
  {
    return;
  }

  mCurrentStroke.points << itemToCanvas( point );

  update();
}

void DrawingCanvas::strokeEnd( const QPointF &point )
{
  if ( mCurrentStroke.points.isEmpty() )
  {
    return;
  }

  mCurrentStroke.points << itemToCanvas( point );

  QPainter painter( &mDrawingImage );
  painter.setRenderHint( QPainter::Antialiasing, true );
  drawStroke( &painter, mCurrentStroke );

  mCurrentStroke.points.clear();

  update();
}

void DrawingCanvas::drawStroke( QPainter *painter, Stroke &stroke, bool onCanvas )
{
  QPainterPath path( onCanvas ? stroke.points.at( 0 ) : canvasToItem( stroke.points.at( 0 ) ) );
  for ( int i = 1; i < stroke.points.size(); i++ )
  {
    path.lineTo( onCanvas ? stroke.points.at( i ) : canvasToItem( stroke.points.at( i ) ) );
  }

  QPen pen( stroke.color );
  pen.setWidthF( onCanvas ? stroke.width : stroke.width * mZoomFactor );
  painter->setPen( pen );
  QBrush brush( stroke.fillColor );
  painter->setBrush( brush );
  painter->drawPath( path );
}

QPointF DrawingCanvas::itemToCanvas( const QPointF &point )
{
  const QPointF canvasTopLeft( size().width() / 2 - ( mBackgroundImage.size().width() * mZoomFactor / 2 ) + mOffset.x(),
                               size().height() / 2 - ( mBackgroundImage.size().height() * mZoomFactor / 2 ) + mOffset.y() );
  return QPointF( ( point.x() - canvasTopLeft.x() ) / mZoomFactor,
                  ( point.y() - canvasTopLeft.y() ) / mZoomFactor );
}

QPointF DrawingCanvas::canvasToItem( const QPointF &point )
{
  const QPointF canvasTopLeft( size().width() / 2 - ( mBackgroundImage.size().width() * mZoomFactor / 2 ) + mOffset.x(),
                               size().height() / 2 - ( mBackgroundImage.size().height() * mZoomFactor / 2 ) + mOffset.y() );
  return QPointF( canvasTopLeft.x() + ( point.x() * mZoomFactor ),
                  canvasTopLeft.y() + ( point.y() * mZoomFactor ) );
}

void DrawingCanvas::paint( QPainter *painter )
{
  if ( !mBackgroundImage.isNull() )
  {
    painter->setRenderHint( QPainter::Antialiasing, true );
    painter->setRenderHint( QPainter::SmoothPixmapTransform, true );

    const QSizeF scaledImageSize = mBackgroundImage.size() * mZoomFactor;
    const QRectF imageRect( ( size().width() / 2 - scaledImageSize.width() / 2 ) + mOffset.x(),
                            ( size().height() / 2 - scaledImageSize.height() / 2 ) + mOffset.y(),
                            scaledImageSize.width(),
                            scaledImageSize.height() );

    QColor shadowColor = mFrameColor;
    shadowColor.setAlphaF( shadowColor.alphaF() / 2 );

    painter->setPen( Qt::NoPen );
    painter->setBrush( QBrush( shadowColor ) );
    painter->drawRect( imageRect.translated( 3, 3 ) );

    painter->drawImage( imageRect, mBackgroundImage );
    painter->drawImage( imageRect, mDrawingImage );

    if ( mCurrentStroke.points.size() > 1 )
    {
      drawStroke( painter, mCurrentStroke, false );
    }

    painter->setPen( QPen( mFrameColor ) );
    painter->setBrush( Qt::NoBrush );
    painter->drawRect( imageRect );
  }
}
