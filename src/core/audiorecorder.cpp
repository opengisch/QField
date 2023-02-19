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

#include "audiorecorder.h"

AudioRecorder::AudioRecorder( QObject *parent )
  : QAudioRecorder( parent )
{
  QAudioEncoderSettings audioSettings;
  audioSettings.setCodec( "audio/amr" );
  audioSettings.setQuality( QMultimedia::HighQuality );

  setEncodingSettings( audioSettings );

  qDebug() << audioInput();
  qDebug() << audioInputs();
}
