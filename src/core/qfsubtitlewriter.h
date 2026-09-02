/***************************************************************************
                        qfsubtitlewriter.h
                        ------------------
  begin                : August 2026
  copyright            : (C) 2026 by Kaustuv Pokharel
  email                : kaustuv@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFSUBTITLEWRITER_H
#define QFSUBTITLEWRITER_H

#include "qfield_core_export.h"

#include <QObject>
#include <QString>
#include <QVector>

/**
 * \ingroup core
 *
 * \brief Collects timed text and writes it out as a SubRip (.srt) subtitle file.
 *
 * The writer holds a single open cue and closes it only when the text changes, so a caller
 * can feed it every update without filtering out repetitions. Changes arriving faster than
 * minimumCueDuration() are folded into the open cue instead of producing subtitles too
 * short to read.
 *
 * The caller owns the clock: it passes the elapsed time of every cue and the final
 * duration to write(), which keeps the cue timeline expressed against the media itself.
 */
class QFIELD_CORE_EXPORT QfSubtitleWriter : public QObject
{
    Q_OBJECT

  public:
    explicit QfSubtitleWriter( QObject *parent = nullptr );

    //! Returns the shortest duration in milliseconds an individual cue may have.
    int minimumCueDuration() const { return mMinimumCueDuration; }

    //! Sets the shortest \a duration in milliseconds an individual cue may have.
    void setMinimumCueDuration( int duration );

    //! Drops every cue, returning the writer to its initial state.
    Q_INVOKABLE void clear();

    /**
     * Records \a text as being valid from \a elapsedTime milliseconds onwards.
     *
     * Passing the text the open cue already holds does nothing, which is what stops an
     * unchanging value from producing a stream of identical cues. An empty \a text closes
     * the open cue without starting a new one, leaving a gap in the track.
     */
    Q_INVOKABLE void addCue( qint64 elapsedTime, const QString &text );

    /**
     * Writes the cues as a SubRip file next to \a mediaFilePath, closing the open cue at
     * \a endTime milliseconds. Returns FALSE and writes no file when there is nothing to
     * write.
     */
    Q_INVOKABLE bool write( const QString &mediaFilePath, qint64 endTime );

    //! Returns the SubRip text of the cues, closing the open cue at \a endTime milliseconds.
    QString toString( qint64 endTime ) const;

    //! Returns \a mediaFilePath with its suffix replaced by .srt.
    static QString subtitleFilePath( const QString &mediaFilePath );

    //! Returns \a milliseconds as a SubRip timestamp, HH:MM:SS,zzz.
    static QString formatTimestamp( qint64 milliseconds );

    //! Returns \a text with line endings normalized and blank lines removed, as a blank line would end the cue.
    static QString sanitizeCueText( const QString &text );

  private:
    struct Cue
    {
        qint64 start = 0;
        qint64 end = 0;
        QString text;
    };

    void closeOpenCue( qint64 endTime );

    QVector<Cue> mCues;
    Cue mOpenCue;
    bool mHasOpenCue = false;
    int mMinimumCueDuration = 500;
};

#endif // QFSUBTITLEWRITER_H
