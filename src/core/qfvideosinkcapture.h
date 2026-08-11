/***************************************************************************
 qfvideosinkcapture.h - QfVideoSinkCapture

 ---------------------
 begin                : 02.08.2026
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

#ifndef QFVIDEOSINKCAPTURE_H
#define QFVIDEOSINKCAPTURE_H

#include <QObject>
#include <QVideoSink>


/**
 * This object allows capturing of still frames from a video sink.
 * \ingroup core
 */
class QfVideoSinkCapture : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QVideoSink *videoSink READ videoSink WRITE setVideoSink NOTIFY videoSinkChanged )

  public:
    explicit QfVideoSinkCapture() = default;

    //! Returns the video sink from which still frames will be drawn from.
    QVideoSink *videoSink() const { return mVideoSink; }

    //! Sets the video sink from which still frames will be drawn from.
    void setVideoSink( QVideoSink *videoSink );

    //! Save the current frame from the video sink within a directory \a location.
    Q_INVOKABLE void saveToFile( const QString &location );

  signals:
    void videoSinkChanged();

    void imageSaved( const QString &path = QString() );

  private:
    QVideoSink *mVideoSink = nullptr;
};

#endif // QFVIDEOSINKCAPTURE_H
