

# File qfvideosinkcapture.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfvideosinkcapture.cpp**](qfvideosinkcapture_8cpp.md)

[Go to the documentation of this file](qfvideosinkcapture_8cpp.md)


```C++
/***************************************************************************
 qfvideosinkcapture.h - QfVideoSinkCapture

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

#include "qfvideosinkcapture.h"

#include <QDateTime>
#include <QDebug>
#include <QImage>
#include <QVideoFrame>


void QfVideoSinkCapture::setVideoSink( QVideoSink *videoSink )
{
  if ( mVideoSink == videoSink )
  {
    return;
  }

  mVideoSink = videoSink;
  emit videoSinkChanged();
}

void QfVideoSinkCapture::saveToFile( const QString &location )
{
  if ( !mVideoSink )
  {
    emit imageSaved();
    return;
  }

  const QVideoFrame videoFrame = mVideoSink->videoFrame();
  const QImage image = videoFrame.toImage();
  const QString path = QStringLiteral( "%1/img_%2.jpg" ).arg( location, QDateTime::currentDateTime().toString( QStringLiteral( "yyyyMMddHHmmss" ) ) );
  image.save( path, nullptr, 90 );

  emit imageSaved( path );
}
```


