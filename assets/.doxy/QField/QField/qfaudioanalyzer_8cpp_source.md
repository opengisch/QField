

# File qfaudioanalyzer.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfaudioanalyzer.cpp**](qfaudioanalyzer_8cpp.md)

[Go to the documentation of this file](qfaudioanalyzer_8cpp.md)


```C++
/***************************************************************************
 qfaudioanalyzer.cpp - QfAudioRecorder

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

#include "qfaudioanalyzer.h"

#include <QAudioBuffer>
#include <QAudioFormat>
#include <QDebug>
#include <QEventLoop>
#include <QUrl>
#include <QVector>

#include <algorithm>
#include <cmath>


QfAudioAnalyzer::QfAudioAnalyzer( QObject *parent )
  : QObject( parent )
{
}

void QfAudioAnalyzer::setBarCount( int barCount )
{
  if ( mBarCount == barCount )
  {
    return;
  }

  mBarCount = barCount;
  emit barCountChanged();
}

void QfAudioAnalyzer::analyze( const QUrl &source )
{
  if ( mGatherer )
  {
    disconnect( mGatherer, &QfAudioPeaksGatherer::collectedRawPeaks, this, &QfAudioAnalyzer::finalize );
    disconnect( mGatherer, &QfAudioPeaksGatherer::finished, this, &QfAudioAnalyzer::gathererThreadFinished );
    connect( mGatherer, &QfAudioPeaksGatherer::finished, mGatherer, &QfAudioPeaksGatherer::deleteLater );
    mGatherer->stop();
  }

  mGatherer = new QfAudioPeaksGatherer( source );

  connect( mGatherer, &QfAudioPeaksGatherer::collectedRawPeaks, this, &QfAudioAnalyzer::finalize );
  connect( mGatherer, &QfAudioPeaksGatherer::finished, this, &QfAudioAnalyzer::gathererThreadFinished );

  mGatherer->start();
}

void QfAudioAnalyzer::finalize()
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

void QfAudioAnalyzer::gathererThreadFinished()
{
  if ( sender() != mGatherer )
  {
    return;
  }

  mGatherer->deleteLater();
  mGatherer = nullptr;
}

QfAudioPeaksGatherer::QfAudioPeaksGatherer( const QUrl &source )
  : mSource( source )
{
}

void QfAudioPeaksGatherer::run()
{
  mDecoder = new QAudioDecoder();

  QAudioFormat format;
  format.setChannelCount( 1 );
  format.setSampleFormat( QAudioFormat::Float );
  mDecoder->setAudioFormat( format );

  connect( mDecoder, &QAudioDecoder::bufferReady, this, &QfAudioPeaksGatherer::processBuffer, Qt::DirectConnection );
  connect( mDecoder, &QAudioDecoder::finished, this, &QfAudioPeaksGatherer::finalize, Qt::DirectConnection );
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

void QfAudioPeaksGatherer::stop()
{
  if ( mDecoder )
  {
    QMetaObject::invokeMethod( mDecoder, "stop", Qt::QueuedConnection );
  }
}

void QfAudioPeaksGatherer::processBuffer()
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

void QfAudioPeaksGatherer::finalize()
{
  emit collectedRawPeaks();
  quit();
}
```


