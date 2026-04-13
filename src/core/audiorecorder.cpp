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

#include <QMediaDevices>
#include <QMediaFormat>


AudioProbe::AudioProbe( QObject *parent )
  : QIODevice( parent )
{
}

qint64 AudioProbe::readData( char *, qint64 )
{
  return 0;
}

qint64 AudioProbe::writeData( const char *data, qint64 len )
{
  const qint16 *samples = reinterpret_cast<const qint16 *>( data );
  const int sampleCount = len / sizeof( qint16 );

  float maxPeak = 0.0f;
  for ( int i = 0; i < sampleCount; ++i )
  {
    const float amplitude = std::abs( static_cast<float>( samples[i] ) ) / 32768.0f;
    if ( amplitude > maxPeak )
    {
      maxPeak = amplitude;
    }
  }

  emit levelCalculated( maxPeak );

  return len;
}


AudioRecorder::AudioRecorder( QObject *parent )
  : QMediaRecorder( parent )
{
  mProbe = new AudioProbe( this );
  mProbe->open( QIODevice::WriteOnly );
  connect( mProbe, &AudioProbe::levelCalculated, this, [this]( double level ) {
    mLevel = level;
    emit levelChanged();
  } );

  QAudioFormat format;
  format.setSampleRate( 44100 );
  format.setChannelCount( 1 );
  format.setSampleFormat( QAudioFormat::Int16 );

  QAudioDevice defaultMicrophone = QMediaDevices::defaultAudioInput();
  mAudioSource = new QAudioSource( defaultMicrophone, format, this );

  connect( this, &QMediaRecorder::recorderStateChanged, this, [this]( QMediaRecorder::RecorderState state ) {
    if ( state == QMediaRecorder::RecordingState )
    {
      mAudioSource->start( mProbe );
      if ( !mHasLevel )
      {
        mHasLevel = true;
        emit hasLevelChanged();
      }
    }
    else
    {
      mAudioSource->stop();
      mLevel = 0.0;
      emit levelChanged();
    }

    emit recordingChanged();
  } );
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
