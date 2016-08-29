/***************************************************************************
              crs.h
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

#ifndef CRS_H
#define CRS_H

#include <QObject>

#include <qgscoordinatereferencesystem.h>

class CRS : public QObject
{
    Q_OBJECT

    Q_PROPERTY( int srid READ srid WRITE setSrid NOTIFY sridChanged )

  public:
    explicit CRS( QObject* parent = 0 );

    void setSrid( int srid );
    int srid() const;

    void setCrs( const QgsCoordinateReferenceSystem& crs );

    const QgsCoordinateReferenceSystem& crs();

  signals:
    void sridChanged();

  private:
    QgsCoordinateReferenceSystem mCrs;
};

#endif // CRS_H
