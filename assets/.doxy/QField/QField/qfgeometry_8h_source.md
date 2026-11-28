

# File qfgeometry.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfgeometry.h**](qfgeometry_8h.md)

[Go to the documentation of this file](qfgeometry_8h.md)


```C++
/***************************************************************************
    qfgeometry.h
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
#ifndef QFGEOMETRY_H
#define QFGEOMETRY_H

#include "qfrubberbandmodel.h"

#include <QPointer>
#include <QtPositioning/QGeoCoordinate>
#include <qgsgeometry.h>

class QfGeometry : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QfRubberbandModel *rubberbandModel READ rubberbandModel WRITE setRubberbandModel NOTIFY rubberbandModelChanged )
    Q_PROPERTY( QgsVectorLayer *vectorLayer READ vectorLayer WRITE setVectorLayer NOTIFY vectorLayerChanged )

  public:
    explicit QfGeometry( QObject *parent = nullptr );

    QgsGeometry asQgsGeometry() const;

    QfRubberbandModel *rubberbandModel() const;
    void setRubberbandModel( QfRubberbandModel *rubberbandModel );
    void updateRubberband( const QgsGeometry &geometry );

    Q_INVOKABLE void applyRubberband();

    QgsVectorLayer *vectorLayer() const;
    void setVectorLayer( QgsVectorLayer *vectorLayer );

  signals:
    void rubberbandModelChanged();
    void vectorLayerChanged();

  private:
    QfRubberbandModel *mRubberbandModel = nullptr;
    QPointer<QgsVectorLayer> mVectorLayer;
};

#endif // QFGEOMETRY_H
```


