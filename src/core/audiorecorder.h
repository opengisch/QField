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

#include <QAudioProbe>
#include <QAudioRecorder>
#include <QObject>

class AudioRecorder : public QAudioRecorder
{
    Q_OBJECT

    Q_PROPERTY( bool recording READ recording NOTIFY recordingChanged )
    Q_PROPERTY( double level READ level NOTIFY levelChanged )

  public:
    explicit AudioRecorder( QObject *parent = nullptr );

    bool recording() const;

    double level() const;

  signals:
    void recordingChanged();
    void levelChanged();

  private slots:
    void audioBufferProbed( const QAudioBuffer &buffer );

  private:
    std::unique_ptr<QAudioProbe> mAudioProbe;
    double mLevel = 0.0;
};

#endif // AUDIORECORDER_H
