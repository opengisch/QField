

# File qfcameraorientationnormalizer.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfcameraorientationnormalizer.cpp**](qfcameraorientationnormalizer_8cpp.md)

[Go to the documentation of this file](qfcameraorientationnormalizer_8cpp.md)


```C++
/***************************************************************************
  qfcameraorientationnormalizer.cpp - QfCameraOrientationNormalizer

 ---------------------
 begin                : 16.4.2026
 copyright            : (C) 2026 by Kaustuv Pokharel
 email                : kaustuv@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfcameraorientationnormalizer.h"

#include <QGuiApplication>
#include <QImage>
#include <QImageReader>
#include <QImageWriter>
#include <QScreen>
#include <QTransform>

QfCameraOrientationNormalizer::QfCameraOrientationNormalizer( QObject *parent )
  : QObject( parent )
{
  QScreen *screen = QGuiApplication::primaryScreen();

  if ( screen )
  {
    mCurrentOrientation = screen->orientation();
    connect( screen, &QScreen::orientationChanged, this, &QfCameraOrientationNormalizer::handleScreenOrientationChanged );
  }
}

int QfCameraOrientationNormalizer::previewRotation() const
{
  return mPreviewRotation;
}

QfCameraOrientationNormalizer::CameraPosition QfCameraOrientationNormalizer::cameraPosition() const
{
  return mCameraPosition;
}

void QfCameraOrientationNormalizer::setCameraPosition( QfCameraOrientationNormalizer::CameraPosition position )
{
  if ( mCameraPosition == position )
  {
    return;
  }

  mCameraPosition = position;
  emit cameraPositionChanged();
  updatePreviewRotation();
}

void QfCameraOrientationNormalizer::recordCaptureOrientation()
{
  QScreen *screen = QGuiApplication::primaryScreen();
  mCaptureOrientation = screen ? screen->orientation() : Qt::PortraitOrientation;
}

bool QfCameraOrientationNormalizer::normalizeImageOrientation( const QString &path )
{
#if defined( Q_OS_IOS ) || defined( Q_OS_WIN )
  if ( path.isEmpty() )
  {
    return false;
  }

  QImageReader reader( path );
  reader.setAutoTransform( false );
  const QImageIOHandler::Transformations exifTransform = reader.transformation();
  QImage image = reader.read();
  if ( image.isNull() )
  {
    return false;
  }

  const bool capturedInLandscape = ( mCaptureOrientation == Qt::LandscapeOrientation || mCaptureOrientation == Qt::InvertedLandscapeOrientation );
  const bool pixelsAreLandscape = image.width() > image.height();
  const bool needsRotation = ( capturedInLandscape != pixelsAreLandscape );
  const bool needsFlip = ( mCaptureOrientation == Qt::LandscapeOrientation );
  const bool needsFrontFlip = ( mCameraPosition == FrontFace );
  const bool hasExifTag = ( exifTransform != QImageIOHandler::TransformationNone );

  if ( !needsRotation && !needsFlip && !needsFrontFlip && !hasExifTag )
  {
    return false;
  }

  if ( needsRotation || needsFlip || needsFrontFlip )
  {
    QTransform transform;
    int angle = 0;
    if ( needsRotation )
    {
      angle += pixelsAreLandscape ? 90 : 270;
    }
    if ( needsFlip )
    {
      angle += 180;
    }
    if ( needsFrontFlip )
    {
      angle += 180;
    }
    angle %= 360;
    if ( angle != 0 )
    {
      transform.rotate( angle );
    }
    if ( needsFrontFlip )
    {
      transform.scale( -1, 1 );
    }
    if ( !transform.isIdentity() )
    {
      image = image.transformed( transform, Qt::SmoothTransformation );
    }
  }

  QImageWriter writer( path );
  writer.setTransformation( QImageIOHandler::TransformationNone );
  writer.setQuality( 95 );
  return writer.write( image );
#else
  Q_UNUSED( path )
  return false;
#endif
}

bool QfCameraOrientationNormalizer::applyEditsToImage( const QString &path, int rotation, bool mirror )
{
  if ( path.isEmpty() )
  {
    return false;
  }

  const int angle = ( ( rotation % 360 ) + 360 ) % 360;
  if ( angle == 0 && !mirror )
  {
    return false;
  }

  QImageReader reader( path );
  reader.setAutoTransform( false );
  QImage image = reader.read();
  if ( image.isNull() )
  {
    return false;
  }

  QTransform transform;
  if ( mirror )
  {
    transform.scale( -1, 1 );
  }
  if ( angle != 0 )
  {
    transform.rotate( angle );
  }
  image = image.transformed( transform, Qt::SmoothTransformation );

  QImageWriter writer( path );
  writer.setTransformation( QImageIOHandler::TransformationNone );
  writer.setQuality( 95 );
  return writer.write( image );
}

void QfCameraOrientationNormalizer::handleScreenOrientationChanged( Qt::ScreenOrientation orientation )
{
  if ( mCurrentOrientation == orientation )
  {
    return;
  }

  mCurrentOrientation = orientation;
  updatePreviewRotation();
}

void QfCameraOrientationNormalizer::updatePreviewRotation()
{
#if defined( Q_OS_IOS ) || defined( Q_OS_WIN )
  const QScreen *screen = QGuiApplication::primaryScreen();
  if ( !screen )
  {
    return;
  }

  const int screenAngle = screen->angleBetween( screen->nativeOrientation(), mCurrentOrientation );
  const bool isLandscape = ( screenAngle == 90 || screenAngle == 270 );
  const int rotation = isLandscape ? 180 : 0;
  if ( rotation != mPreviewRotation )
  {
    mPreviewRotation = rotation;
    emit previewRotationChanged();
  }
#endif
}
```


