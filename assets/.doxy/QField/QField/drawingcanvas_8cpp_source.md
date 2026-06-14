

# File drawingcanvas.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**drawingcanvas.cpp**](drawingcanvas_8cpp.md)

[Go to the documentation of this file](drawingcanvas_8cpp.md)


```C++
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

#include <QImageReader>
#include <QPainter>
#include <QPainterPath>
#include <QStandardPaths>
#include <qgsexiftools.h>

#include <cmath>

DrawingCanvas::DrawingCanvas( QQuickItem *parent )
  : QQuickPaintedItem( parent )
{
  setOpaquePainting( true );
  setAntialiasing( true );
  setSmooth( true );
}

void DrawingCanvas::createBlankCanvas( int width, int height, QColor backgroundColor )
{
  clear();

  mBackgroundImage = QImage( QSize( width, height ), QImage::Format_RGB888 );
  mBackgroundImage.fill( backgroundColor );

  mDrawingImage = QImage( QSize( width, height ), QImage::Format_RGB888 );
  mDrawingImage.fill( backgroundColor );

  setIsEmpty( false );
  setIsDirty( false );
  fitCanvas();
}

void DrawingCanvas::createCanvasFromImage( const QString &path )
{
  clear();

  mLoadedImagePath = path;
  QImageReader imageReader( mLoadedImagePath );
  imageReader.setAutoTransform( true );
  QImage image = imageReader.read();
  if ( !image.isNull() )
  {
    mBackgroundImage = QImage( image.size(), QImage::Format_RGB888 );
    mBackgroundImage.fill( Qt::GlobalColor::white );
    QPainter painter( &mBackgroundImage );
    painter.drawImage( 0, 0, image );

    mDrawingImage = mBackgroundImage;
    setIsEmpty( false );
  }
  else
  {
    mLoadedImagePath.clear();
    mDrawingImage = QImage();
    setIsEmpty( false );
  }

  setIsDirty( false );
  fitCanvas();
}

void DrawingCanvas::clear()
{
  mStrokes.clear();

  mLoadedImagePath.clear();
  mBackgroundImage = QImage();
  mDrawingImage = QImage();

  setZoomFactor( 1.0 );
  setOffset( QPointF( 0, 0 ) );
  setIsDirty( false );
  setIsEmpty( true );

  update();
}

QString DrawingCanvas::save() const
{
  QImage image( mBackgroundImage.size(), QImage::Format_RGBA8888 );
  image.fill( Qt::transparent );

  QPainter painter( &image );
  painter.drawImage( 0, 0, mDrawingImage );

  if ( !mLoadedImagePath.isEmpty() )
  {
    QVariantMap metadata = QgsExifTools::readTags( mLoadedImagePath );
    if ( !metadata.isEmpty() )
    {
      QString path = QStandardPaths::writableLocation( QStandardPaths::TempLocation ) + "/sketch.jpg";
      image.save( path, "jpg", 88 );
      for ( const QString &key : metadata.keys() )
      {
        if ( key == QLatin1String( "Exif.Image.Orientation" ) )
        {
          // The rotation transform already happened when we loaded the image, skip the tag
          continue;
        }

        QgsExifTools::tagImage( path, key, metadata[key] );
      }
      return path;
    }
  }

  QString path = QStandardPaths::writableLocation( QStandardPaths::TempLocation ) + "/sketch.png";
  image.save( path, "png", 80 );
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

bool DrawingCanvas::isEmpty() const
{
  return mIsEmpty;
}

void DrawingCanvas::setIsEmpty( bool empty )
{
  if ( mIsEmpty == empty )
  {
    return;
  }

  mIsEmpty = empty;
  emit isEmptyChanged();
}

bool DrawingCanvas::isDirty() const
{
  return mIsDirty;
}

void DrawingCanvas::setIsDirty( bool dirty )
{
  if ( mIsDirty == dirty )
  {
    return;
  }

  mIsDirty = dirty;
  emit isDirtyChanged();
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

DrawingStroke DrawingCanvas::currentStroke() const
{
  return mCurrentStroke;
}

void DrawingCanvas::strokeBegin( const QPointF &point, const QColor color )
{
  mCurrentStroke.points.clear();
  mCurrentStroke.color = color;
  mCurrentStroke.width = DEFAULT_STROKE_WIDTH / mZoomFactor;
  mCurrentStroke.points << itemToCanvas( point );
  mCurrentStroke.scenePoints << point;
}

void DrawingCanvas::strokeMove( const QPointF &point )
{
  if ( mCurrentStroke.points.isEmpty() )
  {
    return;
  }

  const QPointF lastPoint = canvasToItem( mCurrentStroke.points.last() );
  if ( std::pow( point.x() - lastPoint.x(), 2 ) + std::pow( point.y() - lastPoint.y(), 2 ) >= 0.1 )
  {
    mCurrentStroke.points << itemToCanvas( point );
    mCurrentStroke.scenePoints << point;
    emit currentStrokeChanged();
  }
}

void DrawingCanvas::strokeEnd( const QPointF &point )
{
  if ( mCurrentStroke.points.isEmpty() )
  {
    return;
  }

  mCurrentStroke.points << itemToCanvas( point );

  QPainter painter( &mDrawingImage );
  painter.setRenderHint( QPainter::Antialiasing, antialiasing() );
  painter.setRenderHint( QPainter::Antialiasing, smooth() );
  drawStroke( &painter, mCurrentStroke );

  mStrokes << mCurrentStroke;
  mCurrentStroke.points.clear();
  mCurrentStroke.scenePoints.clear();
  emit currentStrokeChanged();

  setIsDirty( true );

  update();
}

void DrawingCanvas::drawStroke( QPainter *painter, const DrawingStroke &stroke, bool onCanvas ) const
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

void DrawingCanvas::undo()
{
  if ( !mStrokes.isEmpty() )
  {
    // Clear current stroke in the off chance an ongoing stroke had not yet ended
    mCurrentStroke.points.clear();
    mStrokes.removeLast();

    // Reset image and redraw remaining strokes
    mDrawingImage = mBackgroundImage;
    for ( const DrawingStroke &stroke : std::as_const( mStrokes ) )
    {
      QPainter painter( &mDrawingImage );
      painter.setRenderHint( QPainter::Antialiasing, antialiasing() );
      painter.setRenderHint( QPainter::Antialiasing, smooth() );
      drawStroke( &painter, stroke );
    }

    setIsDirty( !mStrokes.isEmpty() );

    update();
  }
}

QPointF DrawingCanvas::itemToCanvas( const QPointF &point ) const
{
  const QPointF canvasTopLeft( size().width() / 2 - ( mBackgroundImage.size().width() * mZoomFactor / 2 ) + mOffset.x(),
                               size().height() / 2 - ( mBackgroundImage.size().height() * mZoomFactor / 2 ) + mOffset.y() );
  return QPointF( ( point.x() - canvasTopLeft.x() ) / mZoomFactor,
                  ( point.y() - canvasTopLeft.y() ) / mZoomFactor );
}

QPointF DrawingCanvas::canvasToItem( const QPointF &point ) const
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
```


