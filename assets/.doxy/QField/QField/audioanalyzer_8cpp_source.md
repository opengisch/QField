

# File audioanalyzer.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**audioanalyzer.cpp**](audioanalyzer_8cpp.md)

[Go to the documentation of this file](audioanalyzer_8cpp.md)


```C++
/***************************************************************************
 audioanalyzer.cpp - AudioRecorder

 ---------------------
 begin                : 12.04.2026
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

#include "audioanalyzer.h"

#include <QAudioBuffer>
#include <QAudioFormat>
#include <QDebug>
#include <QEventLoop>
#include <QUrl>
#include <QVector>

#include <algorithm>
#include <cmath>


AudioAnalyzer::AudioAnalyzer( QObject *parent )
  : QObject( parent )
{
}

void AudioAnalyzer::setBarCount( int barCount )
{
  if ( mBarCount == barCount )
  {
    return;
  }

  mBarCount = barCount;
  emit barCountChanged();
}

void AudioAnalyzer::analyze( const QUrl &source )
{
  if ( mGatherer )
  {
    disconnect( mGatherer, &AudioPeaksGatherer::collectedRawPeaks, this, &AudioAnalyzer::finalize );
    disconnect( mGatherer, &AudioPeaksGatherer::finished, this, &AudioAnalyzer::gathererThreadFinished );
    connect( mGatherer, &AudioPeaksGatherer::finished, mGatherer, &AudioPeaksGatherer::deleteLater );
    mGatherer->stop();
  }

  mGatherer = new AudioPeaksGatherer( source );

  connect( mGatherer, &AudioPeaksGatherer::collectedRawPeaks, this, &AudioAnalyzer::finalize );
  connect( mGatherer, &AudioPeaksGatherer::finished, this, &AudioAnalyzer::gathererThreadFinished );

  mGatherer->start();
}

void AudioAnalyzer::finalize()
{
  if ( !mGatherer )
  {
    emit ready( QList<qreal>() );
    return;
  }

  QList<float> rawPeaks = mGatherer->rawPeaks();
  if ( rawPeaks.isEmpty() )
  {
    emit ready( QList<qreal>() );
    return;
  }

  QList<qreal> finalBars;

  float globalMax = *std::max_element( rawPeaks.begin(), rawPeaks.end() );
  if ( globalMax == 0.0f )
  {
    globalMax = 1.0f;
  }

  const double step = static_cast<double>( rawPeaks.size() ) / mBarCount;
  for ( int i = 0; i < mBarCount; i++ )
  {
    const qsizetype startIndex = static_cast<qsizetype>( i * step );
    const qsizetype endIndex = std::min( static_cast<qsizetype>( ( i + 1 ) * step ), rawPeaks.size() );

    float stepAverage = 0.0f;
    int count = 0;
    for ( int j = startIndex; j < endIndex; j++ )
    {
      stepAverage += rawPeaks[j];
      count++;
    }
    if ( count > 0 )
    {
      stepAverage /= count;
    }

    finalBars.append( stepAverage / globalMax );
  }

  emit ready( finalBars );
}

void AudioAnalyzer::gathererThreadFinished()
{
  if ( sender() != mGatherer )
  {
    return;
  }

  mGatherer->deleteLater();
  mGatherer = nullptr;
}

AudioPeaksGatherer::AudioPeaksGatherer( const QUrl &source )
  : mSource( source )
{
}

void AudioPeaksGatherer::run()
{
  mDecoder = new QAudioDecoder();

  QAudioFormat format;
  format.setChannelCount( 1 );
  format.setSampleFormat( QAudioFormat::Float );
  mDecoder->setAudioFormat( format );

  connect( mDecoder, &QAudioDecoder::bufferReady, this, &AudioPeaksGatherer::processBuffer, Qt::DirectConnection );
  connect( mDecoder, &QAudioDecoder::finished, this, &AudioPeaksGatherer::finalize, Qt::DirectConnection );
  connect( mDecoder, qOverload<QAudioDecoder::Error>( &QAudioDecoder::error ), this, [this]( QAudioDecoder::Error error ) {
    qInfo() << "Audio Analyzer Error:" << error;
    finalize();
  } );

  mDecoder->setSource( mSource );
  mDecoder->start();

  exec();

  delete mDecoder;
  mDecoder = nullptr;
}

void AudioPeaksGatherer::stop()
{
  if ( mDecoder )
  {
    QMetaObject::invokeMethod( mDecoder, "stop", Qt::QueuedConnection );
  }
}

void AudioPeaksGatherer::processBuffer()
{
  while ( mDecoder->bufferAvailable() )
  {
    QAudioBuffer buffer = mDecoder->read();

    // QAudioFormat::Float
    const float *data = buffer.constData<float>();
    const int sampleCount = buffer.sampleCount();

    const int windowSize = 512;
    for ( int i = 0; i < sampleCount; i += windowSize )
    {
      float maxPeak = 0.0f;
      const int end = std::min( i + windowSize, sampleCount );
      for ( int j = i; j < end; ++j )
      {
        const float amplitude = std::abs( data[j] );
        if ( amplitude > maxPeak )
        {
          maxPeak = amplitude;
        }
      }
      mRawPeaks.append( maxPeak );
    }
  }
}

void AudioPeaksGatherer::finalize()
{
  emit collectedRawPeaks();
  quit();
}
```


