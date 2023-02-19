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

AudioRecorder::AudioRecorder( QObject *parent )
  : QAudioRecorder( parent )
{
  connect( this, &QMediaRecorder::stateChanged, this, [=]() {
    mLevel = 0;
    emit levelChanged();
    emit recordingChanged();
  } );

  mAudioProbe = std::make_unique<QAudioProbe>();
  mAudioProbe->setSource( this );
  connect( mAudioProbe.get(), &QAudioProbe::audioBufferProbed, this, &AudioRecorder::audioBufferProbed );
}

bool AudioRecorder::recording() const
{
  return state() == QMediaRecorder::RecordingState;
}

double AudioRecorder::level() const
{
  return mLevel;
}

void AudioRecorder::audioBufferProbed( const QAudioBuffer &buffer )
{
  double previousLevel = mLevel;
  mLevel = 0;

  qreal peakValue;
  if ( buffer.format().sampleType() == QAudioFormat::SignedInt )
  {
    if ( buffer.format().sampleSize() == 32 )
      peakValue = INT_MAX;
    else if ( buffer.format().sampleSize() == 16 )
      peakValue = SHRT_MAX;
    else
      peakValue = CHAR_MAX;

    const QAudioBuffer::S16S *data = buffer.data<QAudioBuffer::S16S>();
    for ( int i = 0; i < buffer.frameCount(); i++ )
    {
      mLevel += std::max( std::abs( data[i].left ), std::abs( data[i].right ) ) / peakValue;
    }
  }
  else if ( buffer.format().sampleType() == QAudioFormat::UnSignedInt )
  {
    if ( buffer.format().sampleSize() == 32 )
      peakValue = UINT_MAX;
    else if ( buffer.format().sampleSize() == 16 )
      peakValue = USHRT_MAX;
    else
      peakValue = UCHAR_MAX;

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

  mLevel = std::max( 0.0, std::max( previousLevel - 0.025, mLevel / buffer.frameCount() ) );

  emit levelChanged();
}
