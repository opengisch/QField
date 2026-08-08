

# File audiorecorder.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**audiorecorder.h**](audiorecorder_8h.md)

[Go to the documentation of this file](audiorecorder_8h.md)


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

#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <QAudioSource>
#include <QIODevice>
#include <QMediaCaptureSession>
#include <QMediaRecorder>
#include <QObject>


class AudioProbe : public QIODevice
{
    Q_OBJECT

  public:
    explicit AudioProbe( QObject *parent = nullptr );

    qint64 readData( char *data, qint64 maxlen ) override;
    qint64 writeData( const char *data, qint64 len ) override;

  signals:
    void levelCalculated( double level );
};


class AudioRecorder : public QMediaRecorder
{
    Q_OBJECT

    Q_PROPERTY( bool recording READ recording NOTIFY recordingChanged )
    Q_PROPERTY( bool hasLevel READ hasLevel NOTIFY hasLevelChanged )
    Q_PROPERTY( double level READ level NOTIFY levelChanged )

  public:
    explicit AudioRecorder( QObject *parent = nullptr );

    bool recording() const;

    bool hasLevel() const;

    double level() const;

  signals:
    void recordingChanged();

    void hasLevelChanged();

    void levelChanged();

    void recordingLoaded();

  private:
    bool mHasLevel = false;
    double mLevel = 0.0;

    QAudioSource *mAudioSource = nullptr;
    AudioProbe *mProbe = nullptr;
};

#endif // AUDIORECORDER_H
```


