/***************************************************************************
 barcodedecoder.cpp - BarcodeDecoder

 ---------------------
 begin                : 22.07.2022
 copyright            : (C) 2022 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "barcodedecoder.h"

BarcodeDecoder::BarcodeDecoder( QObject *parent )
  : QObject( parent )
{
}

void BarcodeDecoder::setDecodedString( const QString &decodedString )
{
  if ( mDecodedString == decodedString )
  {
    return;
  }

  mDecodedString = decodedString;

  emit decodedStringChanged();
}

void BarcodeDecoder::decodeImage( const QImage &image )
{
  return;
}

BarcodeDecoderThread::BarcodeDecoderThread( BarcodeDecoder *decoder, const QImage &image )
  : QThread()
  , mDecoder( decoder )
  , mImage( image )
{
}

void BarcodeDecoderThread::run()
{
  if ( mDecoder )
  {
    mDecoder->decodeImage( mImage );
  }
}
