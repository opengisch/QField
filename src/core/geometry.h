/***************************************************************************
    geometry.h
    ---------------------
    begin                : March 2020
    copyright            : (C) 2020 by David Signer
    email                : david at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "rubberbandmodel.h"

#include <QPointer>
#include <QtPositioning/QGeoCoordinate>
#include <qgsgeometry.h>

class Geometry : public QObject
{
    Q_OBJECT

    Q_PROPERTY( RubberbandModel *rubberbandModel READ rubberbandModel WRITE setRubberbandModel NOTIFY rubberbandModelChanged )
    Q_PROPERTY( QgsVectorLayer *vectorLayer READ vectorLayer WRITE setVectorLayer NOTIFY vectorLayerChanged )

  public:
    explicit Geometry( QObject *parent = nullptr );

    QgsGeometry asQgsGeometry() const;

    RubberbandModel *rubberbandModel() const;
    void setRubberbandModel( RubberbandModel *rubberbandModel );
    void updateRubberband( const QgsGeometry &geometry );

    Q_INVOKABLE void applyRubberband();

    QgsVectorLayer *vectorLayer() const;
    void setVectorLayer( QgsVectorLayer *vectorLayer );

  signals:
    void rubberbandModelChanged();
    void vectorLayerChanged();

  private:
    RubberbandModel *mRubberbandModel = nullptr;
    QPointer<QgsVectorLayer> mVectorLayer;
};

#endif // GEOMETRY_H
