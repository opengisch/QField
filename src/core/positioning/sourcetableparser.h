/***************************************************************************
  sourcetableparser.h - SourcetableParser

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

#ifndef SOURCETABLEPARSER_H
#define SOURCETABLEPARSER_H

#include <QByteArray>
#include <QList>
#include <QString>

namespace SourcetableParser
{
  struct MountpointInfo
  {
    QString name;
    QString identifier;
    QString format;
    QString formatDetails;
    int carrier = 0;
    QString navSystem;
    QString network;
    QString country;
    double latitude = 0;
    double longitude = 0;
    bool nmea = false;
    bool networkSolution = false;
    QString generator;
    QString authentication;
    bool fee = false;
    int bitrate = 0;
  };

  QList<MountpointInfo> parse( const QByteArray &sourcetableData );
}

#endif // SOURCETABLEPARSER_H
