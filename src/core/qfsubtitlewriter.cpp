/***************************************************************************
                        qfsubtitlewriter.cpp
                        --------------------
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

#include "qfsubtitlewriter.h"

#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QTextStream>

#include <algorithm>
#include <utility>

QfSubtitleWriter::QfSubtitleWriter( QObject *parent )
  : QObject( parent )
{
}

void QfSubtitleWriter::setMinimumCueDuration( int duration )
{
  mMinimumCueDuration = std::max( 0, duration );
}

void QfSubtitleWriter::clear()
{
  mCues.clear();
  mOpenCue = Cue();
  mHasOpenCue = false;
}

void QfSubtitleWriter::addCue( qint64 elapsedTime, const QString &text )
{
  if ( elapsedTime < 0 )
  {
    return;
  }

  const QString cueText = sanitizeCueText( text );

  if ( mHasOpenCue )
  {
    if ( mOpenCue.text == cueText )
    {
      return;
    }
    if ( elapsedTime - mOpenCue.start < mMinimumCueDuration )
    {
      if ( cueText.isEmpty() )
      {
        mHasOpenCue = false;
      }
      else
      {
        mOpenCue.text = cueText;
      }
      return;
    }

    closeOpenCue( elapsedTime );
  }

  if ( cueText.isEmpty() )
  {
    return;
  }

  mOpenCue.start = elapsedTime;
  mOpenCue.end = elapsedTime;
  mOpenCue.text = cueText;
  mHasOpenCue = true;
}

void QfSubtitleWriter::closeOpenCue( qint64 endTime )
{
  if ( !mHasOpenCue )
  {
    return;
  }

  mOpenCue.end = endTime;
  mCues.append( mOpenCue );
  mHasOpenCue = false;
}

QString QfSubtitleWriter::toString( qint64 endTime ) const
{
  QVector<Cue> cues = mCues;

  if ( mHasOpenCue && endTime > mOpenCue.start )
  {
    Cue lastCue = mOpenCue;
    lastCue.end = endTime;
    cues.append( lastCue );
  }

  QString contents;
  int index = 0;
  for ( const Cue &cue : std::as_const( cues ) )
  {
    if ( cue.end <= cue.start )
    {
      continue;
    }

    index++;
    contents += QStringLiteral( "%1\n%2 --> %3\n%4\n\n" )
                  .arg( QString::number( index ), formatTimestamp( cue.start ), formatTimestamp( cue.end ), cue.text );
  }

  return contents;
}

bool QfSubtitleWriter::write( const QString &mediaFilePath, qint64 endTime )
{
  if ( mediaFilePath.isEmpty() )
  {
    return false;
  }

  const QString contents = toString( endTime );
  if ( contents.isEmpty() )
  {
    return false;
  }

  QFile file( subtitleFilePath( mediaFilePath ) );
  if ( !file.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate ) )
  {
    return false;
  }

  QTextStream stream( &file );
  stream.setEncoding( QStringConverter::Utf8 );
  stream << contents;
  stream.flush();
  file.close();

  return stream.status() == QTextStream::Ok;
}

QString QfSubtitleWriter::subtitleFilePath( const QString &mediaFilePath )
{
  if ( mediaFilePath.isEmpty() )
  {
    return QString();
  }

  const QFileInfo fileInfo( mediaFilePath );
  const QString path = fileInfo.path();
  const QString baseName = fileInfo.completeBaseName();

  if ( baseName.isEmpty() )
  {
    return QString();
  }

  return path.isEmpty() || path == QLatin1String( "." )
           ? QStringLiteral( "%1.srt" ).arg( baseName )
           : QStringLiteral( "%1/%2.srt" ).arg( path, baseName );
}

QString QfSubtitleWriter::formatTimestamp( qint64 milliseconds )
{
  const qint64 sanitizedMilliseconds = std::max( static_cast<qint64>( 0 ), milliseconds );

  const qint64 hours = sanitizedMilliseconds / 3600000;
  const qint64 minutes = ( sanitizedMilliseconds % 3600000 ) / 60000;
  const qint64 seconds = ( sanitizedMilliseconds % 60000 ) / 1000;
  const qint64 remainder = sanitizedMilliseconds % 1000;

  return QStringLiteral( "%1:%2:%3,%4" )
    .arg( hours, 2, 10, QLatin1Char( '0' ) )
    .arg( minutes, 2, 10, QLatin1Char( '0' ) )
    .arg( seconds, 2, 10, QLatin1Char( '0' ) )
    .arg( remainder, 3, 10, QLatin1Char( '0' ) );
}

QString QfSubtitleWriter::sanitizeCueText( const QString &text )
{
  QString sanitizedText = text;
  sanitizedText.replace( QLatin1String( "\r\n" ), QLatin1String( "\n" ) );
  sanitizedText.replace( QLatin1Char( '\r' ), QLatin1Char( '\n' ) );
  static const QRegularExpression sBlankLinesRegularExpression( QStringLiteral( "\n[ \t]*(\n[ \t]*)+" ) );
  sanitizedText.replace( sBlankLinesRegularExpression, QStringLiteral( "\n" ) );

  return sanitizedText.trimmed();
}
