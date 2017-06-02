/***************************************************************************
  coordinatetransformer.h - CoordinateTransformer

 ---------------------
 begin                : 1.6.2017
 copyright            : (C) 2017 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef COORDINATETRANSFORMER_H
#define COORDINATETRANSFORMER_H

#include <QObject>
#include <QtPositioning/QGeoCoordinate>

#include <qgspointv2.h>

class CoordinateTransformer : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QgsPointV2 projectedPosition READ projectedPosition NOTIFY projectedPositionChanged )
    Q_PROPERTY( QGeoCoordinate sourcePosition READ sourcePosition WRITE setSourcePosition NOTIFY sourcePositionChanged )
    Q_PROPERTY( QgsCoordinateReferenceSystem destinationCrs READ destinationCrs WRITE setDestinationCrs NOTIFY destinationCrsChanged )
    Q_PROPERTY( QgsCoordinateReferenceSystem sourceCrs READ sourceCrs WRITE setSourceCrs NOTIFY sourceCrsChanged )

  public:
    explicit CoordinateTransformer( QObject *parent = 0 );
    QgsPointV2 projectedPosition() const;

    QGeoCoordinate sourcePosition() const;
    void setSourcePosition( QGeoCoordinate sourcePosition );

    QgsCoordinateReferenceSystem destinationCrs() const;
    void setDestinationCrs( const QgsCoordinateReferenceSystem& destinationCrs );

    QgsCoordinateReferenceSystem sourceCrs() const;
    void setSourceCrs( const QgsCoordinateReferenceSystem& sourceCrs );

  private:
    void updatePosition();

  signals:
    void projectedPositionChanged();
    void sourcePositionChanged();
    void destinationCrsChanged();
    void sourceCrsChanged();

  private:
    QgsPointV2 mProjectedPosition;
    QGeoCoordinate mSourcePosition;
    QgsCoordinateTransform mCoordinateTransform;
};

#endif // COORDINATETRANSFORMER_H
