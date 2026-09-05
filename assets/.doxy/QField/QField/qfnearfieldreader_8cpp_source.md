

# File qfnearfieldreader.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfnearfieldreader.cpp**](qfnearfieldreader_8cpp.md)

[Go to the documentation of this file](qfnearfieldreader_8cpp.md)


```C++
/***************************************************************************
 qfnearfieldreader.cpp - QfNearFieldReader

 ---------------------
 begin                : 27.08.2023
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

#include "qfnearfieldreader.h"

#include <QDebug>
#include <qgsmessagelog.h>
#ifdef WITH_NFC
#include <QNdefMessage>
#include <QNdefNfcTextRecord>
#include <QNearFieldTarget>
#endif
#include <QUrl>

QfNearFieldReader::QfNearFieldReader( QObject *parent )
  : QObject( parent )
{
}

QfNearFieldReader::~QfNearFieldReader()
{
#ifdef WITH_NFC
  if ( mNearFieldManager )
  {
    mNearFieldManager->stopTargetDetection();
  }
#endif
}

QString QfNearFieldReader::readString() const
{
  return mReadString;
}

bool QfNearFieldReader::active() const
{
  return mActive;
}

void QfNearFieldReader::setActive( bool active )
{
  if ( mActive == active )
    return;

  mActive = active;
  emit activeChanged();

#ifdef WITH_NFC
  if ( !mNearFieldManager )
  {
    qDebug() << "NFC..";
    qDebug() << "NFC..";
    qDebug() << "NFC..";
    mNearFieldManager = new QNearFieldManager( this );
    connect( mNearFieldManager, &QNearFieldManager::targetDetected, this, &QfNearFieldReader::handleTargetDetected );
    connect( mNearFieldManager, &QNearFieldManager::targetDetected, this, &QfNearFieldReader::handleTargetLost );
  }

  if ( mActive )
  {
    mNearFieldManager->startTargetDetection( QNearFieldTarget::AnyAccess );
  }
  else
  {
    mNearFieldManager->stopTargetDetection();
  }
#endif
}

bool QfNearFieldReader::targetInRange() const
{
  return mTargetInRange;
}

bool QfNearFieldReader::isSupported()
{
#ifdef WITH_NFC
  QNearFieldManager manager;
  return manager.isSupported();
#else
  return false;
#endif
}

#ifdef WITH_NFC
void QfNearFieldReader::handleTargetDetected( QNearFieldTarget *target )
{
  connect( target, &QNearFieldTarget::ndefMessageRead, this, &QfNearFieldReader::handleNdefMessageRead );
  connect( target, &QNearFieldTarget::error, this, &QfNearFieldReader::handleTargetError );
  mTargetInRange = true;
  emit targetInRangeChanged();

  emit targetDetected( QString( target->uid() ) );

  if ( target->hasNdefMessage() )
  {
    mReadString.clear();
    QNearFieldTarget::RequestId request = target->readNdefMessages();
    if ( !request.isValid() )
    {
      handleTargetError( QNearFieldTarget::NdefReadError, request );
    }
  }
}

void QfNearFieldReader::handleTargetLost( QNearFieldTarget *target )
{
  disconnect( target );
  mTargetInRange = false;
  emit targetInRangeChanged();
}

void QfNearFieldReader::handleNdefMessageRead( const QNdefMessage &message )
{
  qInfo() << QStringLiteral( "Received %1 record message(s) from near-field target" ).arg( message.size() );
  for ( const QNdefRecord &record : message )
  {
    switch ( record.typeNameFormat() )
    {
      case QNdefRecord::NfcRtd:
      {
        if ( record.isRecordType<QNdefNfcTextRecord>() )
        {
          QNdefNfcTextRecord textRecord( record );
          mReadString.append( textRecord.text() );
        }
        else
        {
          mReadString.append( record.payload() );
        }
        emit readStringChanged();
        break;
      }

      case QNdefRecord::Mime:
      case QNdefRecord::Uri:
      case QNdefRecord::ExternalRtd:
      case QNdefRecord::Unknown:
      case QNdefRecord::Empty:
      default:
        qInfo() << QStringLiteral( "Received unsupported record (type %1): %2" ).arg( record.typeNameFormat() ).arg( QString( record.payload() ) );
        break;
    }
  }
}

void QfNearFieldReader::handleTargetError( QNearFieldTarget::Error error, const QNearFieldTarget::RequestId &id )
{
  qWarning() << QStringLiteral( "Near-field target error: %1" ).arg( error );
}
#endif
```


