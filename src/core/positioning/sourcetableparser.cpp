/***************************************************************************
  sourcetableparser.cpp - SourcetableParser

 ---------------------
 begin                : 14.02.2026
 copyright            : (C) 2026 by Pascal
 email                :
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "sourcetableparser.h"

QList<SourcetableParser::MountpointInfo> SourcetableParser::parse( const QByteArray &sourcetableData )
{
  QList<MountpointInfo> result;

  const QList<QByteArray> lines = sourcetableData.split( '\n' );
  for ( const QByteArray &rawLine : lines )
  {
    QByteArray line = rawLine.trimmed();
    if ( !line.startsWith( "STR;" ) )
      continue;

    const QList<QByteArray> fields = line.split( ';' );
    // STR records have at least 19 fields (index 0 = "STR", 1..18 = data)
    if ( fields.size() < 19 )
      continue;

    MountpointInfo info;
    info.name = QString::fromUtf8( fields[1] );
    info.identifier = QString::fromUtf8( fields[2] );
    info.format = QString::fromUtf8( fields[3] );
    info.formatDetails = QString::fromUtf8( fields[4] );
    info.carrier = fields[5].toInt();
    info.navSystem = QString::fromUtf8( fields[6] );
    info.network = QString::fromUtf8( fields[7] );
    info.country = QString::fromUtf8( fields[8] );
    info.latitude = fields[9].toDouble();
    info.longitude = fields[10].toDouble();
    info.nmea = fields[11].toInt() != 0;
    info.networkSolution = fields[12].toInt() != 0;
    info.generator = QString::fromUtf8( fields[13] );
    // fields[14] = compression, fields[15] = access protection
    info.authentication = QString::fromUtf8( fields[15] );
    info.fee = fields[16] == "Y";
    info.bitrate = fields[17].toInt();

    result.append( info );
  }

  return result;
}
