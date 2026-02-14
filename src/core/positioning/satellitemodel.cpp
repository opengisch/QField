/***************************************************************************
  satellitemodel.cpp - SatelliteModel

 ---------------------
 begin                : 13.02.2026
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

#include "satellitemodel.h"

SatelliteModel::SatelliteModel( QObject *parent )
  : QAbstractListModel( parent )
{
}

QHash<int, QByteArray> SatelliteModel::roleNames() const
{
  return {
    { SatId, "satId" },
    { Elevation, "elevation" },
    { Azimuth, "azimuth" },
    { Snr, "snr" },
    { InUse, "inUse" },
    { Constellation, "constellation" },
  };
}

int SatelliteModel::rowCount( const QModelIndex &parent ) const
{
  if ( parent.isValid() )
    return 0;

  return static_cast<int>( mSatellites.size() );
}

QVariant SatelliteModel::data( const QModelIndex &index, int role ) const
{
  if ( !index.isValid() || index.row() < 0 || index.row() >= mSatellites.size() )
    return QVariant();

  const QgsSatelliteInfo &sat = mSatellites.at( index.row() );

  switch ( role )
  {
    case SatId:
      return sat.id;
    case Elevation:
      return sat.elevation;
    case Azimuth:
      return sat.azimuth;
    case Snr:
      return sat.signal;
    case InUse:
      return sat.inUse;
    case Constellation:
      return constellationName( sat );
  }

  return QVariant();
}

void SatelliteModel::updateSatellites( const QList<QgsSatelliteInfo> &satellites )
{
  beginResetModel();
  mSatellites = satellites;
  endResetModel();
}

QString SatelliteModel::constellationName( const QgsSatelliteInfo &info )
{
  // Use satType from NMEA $GxGSV message: P=GPS, L=GLONASS, A=Galileo, B=BeiDou, S=SBAS, Q=QZSS
  const char satTypeChar = info.satType.toLatin1();
  switch ( satTypeChar )
  {
    case 0: // Unknown — fall through to PRN-based detection
      break;
    case 'P':
      return QStringLiteral( "GPS" );
    case 'L':
      return QStringLiteral( "GLONASS" );
    case 'A':
      return QStringLiteral( "Galileo" );
    case 'B':
      return QStringLiteral( "BeiDou" );
    case 'S':
      return QStringLiteral( "SBAS" );
    case 'Q':
      return QStringLiteral( "QZSS" );
    default:
      break;
  }

  // Fallback: derive constellation from PRN range
  const int prn = info.id;
  if ( prn >= 1 && prn <= 32 )
    return QStringLiteral( "GPS" );
  if ( prn >= 65 && prn <= 96 )
    return QStringLiteral( "GLONASS" );
  if ( prn >= 201 && prn <= 237 )
    return QStringLiteral( "BeiDou" );
  if ( prn >= 301 && prn <= 336 )
    return QStringLiteral( "Galileo" );
  if ( prn >= 120 && prn <= 158 )
    return QStringLiteral( "SBAS" );
  if ( prn >= 193 && prn <= 200 )
    return QStringLiteral( "QZSS" );

  return QStringLiteral( "Unknown" );
}
