

# File audiorecorder.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**audiorecorder.cpp**](audiorecorder_8cpp.md)

[Go to the documentation of this file](audiorecorder_8cpp.md)


```C++
/***************************************************************************
 audiorecorder.h - AudioRecorder

 ---------------------
 begin                : 19.02.2023
 copyright            : (C) 2023 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "audiorecorder.h"

#include <QMediaFormat>

AudioRecorder::AudioRecorder( QObject *parent )
  : QMediaRecorder( parent )
{
}

bool AudioRecorder::recording() const
{
  return recorderState() == QMediaRecorder::RecordingState;
}

double AudioRecorder::level() const
{
  return mLevel;
}

bool AudioRecorder::hasLevel() const
{
  return mHasLevel;
}
```


