

# File qfsubtitlewriter.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfsubtitlewriter.h**](qfsubtitlewriter_8h.md)

[Go to the documentation of this file](qfsubtitlewriter_8h.md)


```C++
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

class QFIELD_CORE_EXPORT QfSubtitleWriter : public QObject
{
    Q_OBJECT

    Q_PROPERTY( int minimumCueDuration READ minimumCueDuration WRITE setMinimumCueDuration NOTIFY minimumCueDurationChanged )

  public:
    explicit QfSubtitleWriter( QObject *parent = nullptr );

    int minimumCueDuration() const { return mMinimumCueDuration; }

    void setMinimumCueDuration( int duration );

    Q_INVOKABLE void clear();

    Q_INVOKABLE void addCue( qint64 elapsedTime, const QString &text );

    Q_INVOKABLE bool write( const QString &mediaFilePath, qint64 endTime );

    static QString subtitleFilePath( const QString &mediaFilePath );

    static QString formatTimestamp( qint64 milliseconds );

    static QString sanitizeCueText( const QString &text );

  signals:
    void minimumCueDurationChanged();

  private:
    struct Cue
    {
        qint64 start = 0;
        qint64 end = 0;
        QString text;
    };

    void closeOpenCue( qint64 endTime );

    QString toString() const;

    QVector<Cue> mCues;
    Cue mOpenCue;
    bool mHasOpenCue = false;
    int mMinimumCueDuration = 500;
};

#endif // QFSUBTITLEWRITER_H
```


