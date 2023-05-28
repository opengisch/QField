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

#include <QObject>
#if QT_VERSION >= QT_VERSION_CHECK( 6, 0, 0 )
#include <QMediaCaptureSession>
#include <QMediaRecorder>

class AudioRecorder : public QMediaRecorder
{
    Q_OBJECT

    Q_PROPERTY( bool recording READ recording NOTIFY recordingChanged )
    Q_PROPERTY( bool hasLevel READ hasLevel NOTIFY hasLevelChanged )
    Q_PROPERTY( double level READ level NOTIFY levelChanged )

  public:
    explicit AudioRecorder( QObject *parent = nullptr );

    /**
     * Returns TRUE when audio is being recorded.
     */
    bool recording() const;

    /**
     * Returns TRUE when audio level/amplitude is available during recording.
     * \note this reflects whether a given platform supports QAudioProbe
     * \see level()
     */
    bool hasLevel() const;

    /**
     * Returns the current level (0.0 to 1.0) of the audio being recorded.
     * \see hasLevel()
     */
    double level() const;

  signals:
    /**
     * Emitted when the recording state has changed.
     */
    void recordingChanged();

    /**
     * Emitted upon successfully setting up the audio probe used to return audio levels.
     */
    void hasLevelChanged();

    /**
     * Emitted when the current audio level being recorded has changed.
     */
    void levelChanged();

    /**
     * Emitted when a recorded audio file is finalized and loaded.
     */
    void recordingLoaded();

  private:
    bool mHasLevel = false;
    double mLevel = 0.0;
};
#else
#include <QAudioProbe>
#include <QAudioRecorder>

class AudioRecorder : public QAudioRecorder
{
    Q_OBJECT

    Q_PROPERTY( bool recording READ recording NOTIFY recordingChanged )
    Q_PROPERTY( bool hasLevel READ hasLevel NOTIFY hasLevelChanged )
    Q_PROPERTY( double level READ level NOTIFY levelChanged )

  public:
    explicit AudioRecorder( QObject *parent = nullptr );

    /**
     * Returns TRUE when audio is being recorded.
     */
    bool recording() const;

    /**
     * Returns TRUE when audio level/amplitude is available during recording.
     * \note this reflects whether a given platform supports QAudioProbe
     * \see level()
     */
    bool hasLevel() const;

    /**
     * Returns the current level (0.0 to 1.0) of the audio being recorded.
     * \see hasLevel()
     */
    double level() const;

  signals:
    /**
     * Emitted when the recording state has changed.
     */
    void recordingChanged();

    /**
     * Emitted upon successfully setting up the audio probe used to return audio levels.
     */
    void hasLevelChanged();

    /**
     * Emitted when the current audio level being recorded has changed.
     */
    void levelChanged();

    /**
     * Emitted when a recorded audio file is finalized and loaded.
     */
    void recordingLoaded();

  private slots:
    void audioBufferProbed( const QAudioBuffer &buffer );

  private:
    std::unique_ptr<QAudioProbe> mAudioProbe;

    bool mHasLevel = false;
    double mLevel = 0.0;
};
#endif

#endif // AUDIORECORDER_H
