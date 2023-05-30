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

#if QT_VERSION >= QT_VERSION_CHECK( 6, 0, 0 )
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
#else
AudioRecorder::AudioRecorder( QObject *parent )
  : QAudioRecorder( parent )
{
  QAudioEncoderSettings as = audioSettings();
#ifdef Q_OS_LINUX
  as.setCodec( "audio/x-vorbis" );
  setEncodingSettings( as, QVideoEncoderSettings(), QString( "audio/ogg" ) );
#endif

  connect( this, &QMediaRecorder::stateChanged, this, [=]() {
    mLevel = 0;
    emit levelChanged();
    emit recordingChanged();
  } );

  connect( this, &QMediaRecorder::statusChanged, this, [=]() {
    if ( status() == QMediaRecorder::LoadedStatus )
    {
      emit recordingLoaded();
    }
  } );

  mAudioProbe = std::make_unique<QAudioProbe>();
  if ( mAudioProbe->setSource( this ) )
  {
    connect( mAudioProbe.get(), &QAudioProbe::audioBufferProbed, this, &AudioRecorder::audioBufferProbed );
    mHasLevel = true;
    emit hasLevelChanged();
  }
}

bool AudioRecorder::recording() const
{
  return state() == QMediaRecorder::RecordingState;
}

double AudioRecorder::level() const
{
  return mLevel;
}

bool AudioRecorder::hasLevel() const
{
  return mHasLevel;
}

void AudioRecorder::audioBufferProbed( const QAudioBuffer &buffer )
{
  double previousLevel = mLevel;
  mLevel = 0;

  qreal peakValue;
  if ( buffer.format().sampleType() == QAudioFormat::SignedInt )
  {
    if ( buffer.format().sampleSize() == 32 )
      peakValue = std::numeric_limits<int>::max();
    else if ( buffer.format().sampleSize() == 16 )
      peakValue = std::numeric_limits<short>::max();
    else
      peakValue = std::numeric_limits<char>::max();

    const QAudioBuffer::S16S *data = buffer.data<QAudioBuffer::S16S>();
    for ( int i = 0; i < buffer.frameCount(); i++ )
    {
      mLevel += std::max( std::abs( data[i].left ), std::abs( data[i].right ) ) / peakValue;
    }
  }
  else if ( buffer.format().sampleType() == QAudioFormat::UnSignedInt )
  {
    if ( buffer.format().sampleSize() == 32 )
      peakValue = std::numeric_limits<unsigned int>::max();
    else if ( buffer.format().sampleSize() == 16 )
      peakValue = std::numeric_limits<unsigned short>::max();
    else
      peakValue = std::numeric_limits<unsigned char>::max();

    const QAudioBuffer::S16U *data = buffer.data<QAudioBuffer::S16U>();
    for ( int i = 0; i < buffer.frameCount(); i++ )
    {
      mLevel += std::max( data[i].left, data[i].right ) / peakValue;
    }
  }
  else if ( buffer.format().sampleType() == QAudioFormat::Float )
  {
    peakValue = 1.00003;

    const QAudioBuffer::S32F *data = buffer.data<QAudioBuffer::S32F>();
    for ( int i = 0; i < buffer.frameCount(); i++ )
    {
      mLevel += std::max( std::abs( data[i].left ), std::abs( data[i].right ) ) / peakValue;
    }
  }

  // Smooth level to avoid dizzingly rapid changes
  if ( mLevel <= previousLevel )
  {
    mLevel = std::max( 0.0, std::max( previousLevel - 0.025, mLevel / buffer.frameCount() ) );
  }
  else
  {
    mLevel = std::min( 1.0, std::min( previousLevel + 0.025, mLevel / buffer.frameCount() ) );
  }

  emit levelChanged();
}
#endif
