

# File qfaudioanalyzer.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfaudioanalyzer.h**](qfaudioanalyzer_8h.md)

[Go to the documentation of this file](qfaudioanalyzer_8h.md)


```C++
/***************************************************************************
 qfaudioanalyzer.h - QfAudioAnalyzer

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

#ifndef QFAUDIOANALYZER_H
#define QFAUDIOANALYZER_H

#include <QAudioDecoder>
#include <QObject>
#include <QThread>
#include <QUrl>

class QfAudioPeaksGatherer : public QThread
{
    Q_OBJECT

  public:
    QfAudioPeaksGatherer( const QUrl &source );

    void run() override;

    void stop();

    QList<float> rawPeaks() const { return mRawPeaks; }

  signals:
    void collectedRawPeaks();

  private slots:
    void processBuffer();
    void finalize();

  private:
    QUrl mSource;
    QAudioDecoder *mDecoder = nullptr;
    QVector<float> mRawPeaks;
};

class QfAudioAnalyzer : public QObject
{
    Q_OBJECT

    Q_PROPERTY( int barCount READ barCount WRITE setBarCount NOTIFY barCountChanged )

  public:
    explicit QfAudioAnalyzer( QObject *parent = nullptr );

    int barCount() const { return mBarCount; }

    void setBarCount( int barCount );

    Q_INVOKABLE void analyze( const QUrl &source );

  signals:

    void ready( const QList<qreal> &bars );

    void barCountChanged();

  private slots:

    void finalize();
    void gathererThreadFinished();

  private:
    QfAudioPeaksGatherer *mGatherer = nullptr;

    int mBarCount = 80;
};

#endif // QFAUDIORECORDER_H
```


