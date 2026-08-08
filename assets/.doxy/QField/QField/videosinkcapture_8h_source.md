

# File videosinkcapture.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**videosinkcapture.h**](videosinkcapture_8h.md)

[Go to the documentation of this file](videosinkcapture_8h.md)


```C++
/***************************************************************************
 videosinkcapture.h - VideoSinkCapture

 ---------------------
 begin                : 02.08.2026
 copyright            : (C) 2026 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef VIDEOSINKCAPTURE_H
#define VIDEOSINKCAPTURE_H

#include <QObject>
#include <QVideoSink>


class VideoSinkCapture : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QVideoSink *videoSink READ videoSink WRITE setVideoSink NOTIFY videoSinkChanged )

  public:
    explicit VideoSinkCapture() = default;

    QVideoSink *videoSink() const { return mVideoSink; }

    void setVideoSink( QVideoSink *videoSink );

    Q_INVOKABLE void saveToFile( const QString &location );

  signals:
    void videoSinkChanged();

    void imageSaved( const QString &path = QString() );

  private:
    QVideoSink *mVideoSink = nullptr;
};

#endif // VIDEOSINKCAPTURE_H
```


