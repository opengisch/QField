/***************************************************************************
  cameraorientationnormalizer.cpp - CameraOrientationNormalizer

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

#include "cameraorientationnormalizer.h"

#include <QGuiApplication>
#include <QImage>
#include <QImageReader>
#include <QImageWriter>
#include <QScreen>
#include <QTransform>

CameraOrientationNormalizer::CameraOrientationNormalizer( QObject *parent )
  : QObject( parent )
{
  QScreen *screen = QGuiApplication::primaryScreen();

  if ( screen )
  {
    mCurrentOrientation = screen->orientation();
    connect( screen, &QScreen::orientationChanged, this, &CameraOrientationNormalizer::handleScreenOrientationChanged );
  }
}

int CameraOrientationNormalizer::previewRotation() const
{
  return mPreviewRotation;
}

int CameraOrientationNormalizer::cameraPosition() const
{
  return mCameraPosition;
}

void CameraOrientationNormalizer::setCameraPosition( int position )
{
  if ( mCameraPosition == position )
  {
    return;
  }

  mCameraPosition = position;
  emit cameraPositionChanged();
  updatePreviewRotation();
}

void CameraOrientationNormalizer::recordCaptureOrientation()
{
  QScreen *screen = QGuiApplication::primaryScreen();
  mCaptureOrientation = screen ? screen->orientation() : Qt::PortraitOrientation;
}

bool CameraOrientationNormalizer::normalizeImageOrientation( const QString &path )
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
  const bool needsFrontFlip = ( mCameraPosition == 2 );
  const bool hasExifTag = ( exifTransform != QImageIOHandler::TransformationNone );

  if ( !needsRotation && !needsFlip && !needsFrontFlip && !hasExifTag )
  {
    return false;
  }

  if ( needsRotation || needsFlip || needsFrontFlip )
  {
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
      QTransform transform;
      transform.rotate( angle );
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

void CameraOrientationNormalizer::handleScreenOrientationChanged( Qt::ScreenOrientation orientation )
{
  if ( mCurrentOrientation == orientation )
  {
    return;
  }

  mCurrentOrientation = orientation;
  updatePreviewRotation();
}

void CameraOrientationNormalizer::updatePreviewRotation()
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
