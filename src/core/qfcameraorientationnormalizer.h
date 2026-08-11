/***************************************************************************
  qfcameraorientationnormalizer.h - CameraOrientationNormalizer

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

#ifndef QFCAMERAORIENTATIONNORMALIZER_H
#define QFCAMERAORIENTATIONNORMALIZER_H

#include <QObject>

/**
 * \brief Compensates for incorrect camera orientation on iOS and Windows,
 * and applies user-requested photo adjustments on all platforms.
 *
 * On iOS and Windows, Qt Multimedia's backend produces an inverted
 * camera preview in landscape mode and writes captured photos with
 * incorrect orientation or bogus EXIF tags (QTBUG-118594). The preview
 * and saved-image corrections below are limited to those platforms.
 *
 * Separately, and on all platforms, this class can bake a user-chosen
 * rotation and mirror into a captured photo before it is saved.
 *
 * This class provides:
 *  \a previewRotation for correcting the live VideoOutput orientation
 *    (iOS and Windows only)
 *  \a cameraPosition to distinguish the front-facing camera, which needs
 *    an additional mirror correction
 *  \a normalizeImageOrientation() for correcting saved JPEG files
 *    (iOS and Windows only)
 *  \a applyEditsToImage() for baking a user-chosen rotation and mirror
 *    into a saved JPEG (all platforms)
 *
 * \ingroup core
 */

class CameraOrientationNormalizer : public QObject
{
    Q_OBJECT

    Q_PROPERTY( int previewRotation READ previewRotation NOTIFY previewRotationChanged )
    Q_PROPERTY( CameraOrientationNormalizer::CameraPosition cameraPosition READ cameraPosition WRITE setCameraPosition NOTIFY cameraPositionChanged )

  public:
    //! Camera position, matching the integer values of QCameraDevice::Position.
    enum CameraPosition
    {
      UnspecifiedPosition = 0,
      BackFace = 1,
      FrontFace = 2
    };
    Q_ENUM( CameraPosition )

    //! Constructs the normalizer and starts tracking screen orientation.
    explicit CameraOrientationNormalizer( QObject *parent = nullptr );

    /**
     * Returns the rotation in degrees that the live camera preview
     * (VideoOutput) must be rotated by to appear upright, compensating
     * for the incorrect preview orientation on iOS and Windows.
     */
    int previewRotation() const;

    /**
     * Returns the active camera's position (front, back, or unspecified),
     * used to apply the additional horizontal mirror correction that
     * front-facing cameras require when normalizing saved photos.
     */
    CameraOrientationNormalizer::CameraPosition cameraPosition() const;

    /**
     * Sets the active camera's \a position. Bound from the QML camera's
     * device position so orientation correction can distinguish the
     * front-facing camera.
     */
    void setCameraPosition( CameraOrientationNormalizer::CameraPosition position );

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
     * Bakes a user-chosen adjustment into the JPEG at \a path, letting the
     * user manually correct a photo before saving. Unlike the orientation
     * compensation, this applies on all platforms. \a rotation is a clockwise
     * angle in degrees (any multiple of 90) and \a mirror applies a horizontal
     * flip. The mirror is applied before the rotation to match the live preview.
     *
     * Returns false without touching the file when there is nothing to apply
     * (zero rotation and no mirror).
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
    CameraPosition mCameraPosition = UnspecifiedPosition;
};

#endif // QFCAMERAORIENTATIONNORMALIZER_H
