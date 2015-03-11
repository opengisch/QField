/***************************************************************************
              crs.cpp
               ----------------------------------------------------
              date                 : 26.2.2015
              copyright            : (C) 2015 by Matthias Kuhn
              email                : matthias (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "crs.h"

#include <QDebug>

CRS::CRS( QObject* parent ) :
  QObject( parent )
{
}

void CRS::setSrid( int srid )
{
  qDebug() << "Setting SRID " << srid;
  if ( mCrs.srsid() != srid )
  {
    mCrs.createFromSrid( srid );
    emit sridChanged();
  }
}

int CRS::srid() const
{
  return mCrs.srsid();
}

void CRS::setCrs( const QgsCoordinateReferenceSystem& crs )
{
  mCrs = crs;
  emit sridChanged();
}

const QgsCoordinateReferenceSystem& CRS::crs()
{
  return mCrs;
}
