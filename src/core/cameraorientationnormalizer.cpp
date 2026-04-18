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

void CameraOrientationNormalizer::setCamera( const QCameraDevice &device )
{
  mCameraDevice = device;
  mIsFrontCamera = !device.isNull() && device.position() == QCameraDevice::FrontFace;
  mSensorAngle = device.isNull() ? 0 : static_cast<int>( device.correctionAngle() );
  updatePreviewRotation();
}

void CameraOrientationNormalizer::recordCaptureOrientation()
{
  QScreen *screen = QGuiApplication::primaryScreen();
  mCaptureOrientation = screen ? screen->orientation() : Qt::PortraitOrientation;
}

bool CameraOrientationNormalizer::normalizeImageOrientation( const QString &path )
{
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
  const bool hasExifTag = ( exifTransform != QImageIOHandler::TransformationNone );

  if ( !needsRotation && !hasExifTag )
  {
    return false;
  }

  if ( needsRotation )
  {
    QTransform transform;
    transform.rotate( pixelsAreLandscape ? 90 : 270 );
    image = image.transformed( transform, Qt::SmoothTransformation );
  }

  QImageWriter writer( path );
  writer.setTransformation( QImageIOHandler::TransformationNone );
  writer.setQuality( 95 );
  return writer.write( image );
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
#if defined( Q_OS_ANDROID ) || defined( Q_OS_IOS )
  const QScreen *screen = QGuiApplication::primaryScreen();
  if ( !screen )
  {
    return;
  }

  const int screenAngle = screen->angleBetween( screen->nativeOrientation(), mCurrentOrientation );
  int rotation = 0;

  if ( mSensorAngle == 0 )
  {
    // iOS: AVFoundation reports sensorAngle 0 and handles rotation
    // internally, but produces an inverted preview in landscape
    const bool isLandscape = ( screenAngle == 90 || screenAngle == 270 );
    rotation = isLandscape ? 180 : 0;
  }
  else if ( mIsFrontCamera )
  {
    rotation = ( 360 - mSensorAngle + screenAngle ) % 360;
  }
  else
  {
    rotation = ( mSensorAngle - screenAngle + 360 ) % 360;
  }

  if ( rotation != mPreviewRotation )
  {
    mPreviewRotation = rotation;
    emit previewRotationChanged();
  }
#endif
}
