

# File cameraorientationnormalizer.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**cameraorientationnormalizer.h**](cameraorientationnormalizer_8h.md)

[Go to the documentation of this file](cameraorientationnormalizer_8h.md)


```C++
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

class CameraOrientationNormalizer : public QObject
{
    Q_OBJECT

    Q_PROPERTY( int previewRotation READ previewRotation NOTIFY previewRotationChanged )

  public:
    explicit CameraOrientationNormalizer( QObject *parent = nullptr );

    int previewRotation() const;

    Q_INVOKABLE void recordCaptureOrientation();

    Q_INVOKABLE bool normalizeImageOrientation( const QString &path );

  signals:
    void previewRotationChanged();

  private slots:
    void handleScreenOrientationChanged( Qt::ScreenOrientation orientation );

  private:
    void updatePreviewRotation();
    Qt::ScreenOrientation mCurrentOrientation = Qt::PortraitOrientation;
    Qt::ScreenOrientation mCaptureOrientation = Qt::PortraitOrientation;
    int mPreviewRotation = 0;
};

#endif // CAMERAORIENTATIONNORMALIZER_H
```


