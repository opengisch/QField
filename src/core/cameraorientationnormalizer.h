/***************************************************************************
  cameraorientationnormalizer.h - CameraOrientationNormalizer

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

#ifndef CAMERAORIENTATIONNORMALIZER_H
#define CAMERAORIENTATIONNORMALIZER_H

#include <QObject>

/**
 * \brief Compensates for incorrect camera orientation on iOS and Windows.
 *
 * On iOS and Windows, Qt Multimedia's backend produces an inverted
 * camera preview in landscape mode and writes captured photos with
 * incorrect orientation or bogus EXIF tags (QTBUG-118594).
 *
 * This class provides:
 *  \a previewRotation for correcting the live VideoOutput orientation
 *  \a normalizeImageOrientation() for correcting saved JPEG files
 *
 * \ingroup core
 */

class CameraOrientationNormalizer : public QObject
{
    Q_OBJECT

    Q_PROPERTY( int previewRotation READ previewRotation NOTIFY previewRotationChanged )
    Q_PROPERTY( int cameraPosition READ cameraPosition WRITE setCameraPosition NOTIFY cameraPositionChanged )

  public:
    explicit CameraOrientationNormalizer( QObject *parent = nullptr );

    int previewRotation() const;
    int cameraPosition() const;
    void setCameraPosition( int position );

    /**
     * Records the current screen orientation. Call at shutter press
     * so that normalizeImageOrientation() has a ground-truth reference.
     */
    Q_INVOKABLE void recordCaptureOrientation();

    /**
     * Ensures the JPEG at \a path has pixels matching the orientation
     * recorded by recordCaptureOrientation(). Rotates the image if
     * pixel dimensions contradict the capture orientation and strips
     * any non-identity EXIF orientation tag.
     *
     * Returns false without touching the file when no correction is needed.
     */
    Q_INVOKABLE bool normalizeImageOrientation( const QString &path );

    /**
     * Bakes a user-chosen \a rotation (clockwise degrees, multiples of 90)
     * and optional horizontal \a mirror into the JPEG at \a path, letting
     * the user manually adjust a photo before saving.
     *
     * Returns false without touching the file when there is nothing to apply.
     */
    Q_INVOKABLE bool applyEditsToImage( const QString &path, int rotation, bool mirror );

  signals:
    void previewRotationChanged();
    void cameraPositionChanged();

  private slots:
    void handleScreenOrientationChanged( Qt::ScreenOrientation orientation );

  private:
    void updatePreviewRotation();
    Qt::ScreenOrientation mCurrentOrientation = Qt::PortraitOrientation;
    Qt::ScreenOrientation mCaptureOrientation = Qt::PortraitOrientation;
    int mPreviewRotation = 0;
    int mCameraPosition = 0;
};

#endif // CAMERAORIENTATIONNORMALIZER_H
