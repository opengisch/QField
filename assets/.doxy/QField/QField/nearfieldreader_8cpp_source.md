

# File nearfieldreader.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**nearfieldreader.cpp**](nearfieldreader_8cpp.md)

[Go to the documentation of this file](nearfieldreader_8cpp.md)


```C++
/***************************************************************************
 nearfieldreader.cpp - NearFieldReader

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

#include "nearfieldreader.h"

#include <QDebug>
#include <qgsmessagelog.h>
#ifdef WITH_NFC
#include <QNdefMessage>
#include <QNdefNfcTextRecord>
#include <QNearFieldTarget>
#endif
#include <QUrl>

NearFieldReader::NearFieldReader( QObject *parent )
  : QObject( parent )
{
#ifdef WITH_NFC
  mNearFieldManager = new QNearFieldManager( this );
  connect( mNearFieldManager, &QNearFieldManager::targetDetected, this, &NearFieldReader::handleTargetDetected );
  connect( mNearFieldManager, &QNearFieldManager::targetDetected, this, &NearFieldReader::handleTargetLost );
#endif
}

NearFieldReader::~NearFieldReader()
{
#ifdef WITH_NFC
  mNearFieldManager->stopTargetDetection();
#endif
}

QString NearFieldReader::readString() const
{
  return mReadString;
}

bool NearFieldReader::active() const
{
  return mActive;
}

void NearFieldReader::setActive( bool active )
{
  if ( mActive == active )
    return;

  mActive = active;
  emit activeChanged();

#ifdef WITH_NFC
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

bool NearFieldReader::targetInRange() const
{
  return mTargetInRange;
}

bool NearFieldReader::isSupported()
{
#ifdef WITH_NFC
  QNearFieldManager manager;
  return manager.isSupported();
#else
  return false;
#endif
}

#ifdef WITH_NFC
void NearFieldReader::handleTargetDetected( QNearFieldTarget *target )
{
  connect( target, &QNearFieldTarget::ndefMessageRead, this, &NearFieldReader::handleNdefMessageRead );
  connect( target, &QNearFieldTarget::error, this, &NearFieldReader::handleTargetError );
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

void NearFieldReader::handleTargetLost( QNearFieldTarget *target )
{
  disconnect( target );
  mTargetInRange = false;
  emit targetInRangeChanged();
}

void NearFieldReader::handleNdefMessageRead( const QNdefMessage &message )
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

void NearFieldReader::handleTargetError( QNearFieldTarget::Error error, const QNearFieldTarget::RequestId &id )
{
  qWarning() << QStringLiteral( "Near-field target error: %1" ).arg( error );
}
#endif
```


