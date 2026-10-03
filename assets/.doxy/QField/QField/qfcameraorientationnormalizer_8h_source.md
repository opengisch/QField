

# File qfcameraorientationnormalizer.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfcameraorientationnormalizer.h**](qfcameraorientationnormalizer_8h.md)

[Go to the documentation of this file](qfcameraorientationnormalizer_8h.md)


```C++
/***************************************************************************
  qfcameraorientationnormalizer.h - QfCameraOrientationNormalizer

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

class QfCameraOrientationNormalizer : public QObject
{
    Q_OBJECT

    Q_PROPERTY( int previewRotation READ previewRotation NOTIFY previewRotationChanged )
    Q_PROPERTY( QfCameraOrientationNormalizer::CameraPosition cameraPosition READ cameraPosition WRITE setCameraPosition NOTIFY cameraPositionChanged )

  public:
    enum CameraPosition
    {
      UnspecifiedPosition = 0,
      BackFace = 1,
      FrontFace = 2
    };
    Q_ENUM( CameraPosition )

    
    explicit QfCameraOrientationNormalizer( QObject *parent = nullptr );

    int previewRotation() const;

    QfCameraOrientationNormalizer::CameraPosition cameraPosition() const;

    void setCameraPosition( QfCameraOrientationNormalizer::CameraPosition position );

    Q_INVOKABLE void recordCaptureOrientation();

    Q_INVOKABLE bool normalizeImageOrientation( const QString &path );

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
```


