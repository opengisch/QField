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

/**
 * \brief This class analyzes the peaks of audio clips and generate a list of bars
 * that can be used to visualize the audio's overall texture.
 * \ingroup core
 */
class QfAudioAnalyzer : public QObject
{
    Q_OBJECT

    Q_PROPERTY( int barCount READ barCount WRITE setBarCount NOTIFY barCountChanged )

  public:
    explicit QfAudioAnalyzer( QObject *parent = nullptr );

    /**
     * Returns the bar count that will be returned upon successful audio clip analysis.
     */
    int barCount() const { return mBarCount; }

    /**
     * Sets the bar count that will be returned upon successful audio clip analysis.
     */
    void setBarCount( int barCount );

    /**
     * Run an analysis of the audio \a source.
     */
    Q_INVOKABLE void analyze( const QUrl &source );

  signals:

    /**
     * Emitted when an analysis is over.
     * \note If the analysis failed, the list will be empty.
     */
    void ready( const QList<qreal> &bars );

    /**
     * Emitted when the bar count property has changed.
     */
    void barCountChanged();

  private slots:

    void finalize();
    void gathererThreadFinished();

  private:
    QfAudioPeaksGatherer *mGatherer = nullptr;

    int mBarCount = 80;
};

#endif // QFAUDIORECORDER_H
