

# File qgsquickcoordinatetransformer.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qgsquick**](dir_19c3d7c8ba6dc95926fecc96295d1560.md) **>** [**qgsquickcoordinatetransformer.h**](qgsquickcoordinatetransformer_8h.md)

[Go to the documentation of this file](qgsquickcoordinatetransformer_8h.md)


```C++
/***************************************************************************
 qgsquickcoordinatetransformer.h
  --------------------------------------
  Date                 : 1.6.2017
  Copyright            : (C) 2017 by Matthias Kuhn
  Email                :  matthias (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSQUICKCOORDINATETRANSFORMER_H
#define QGSQUICKCOORDINATETRANSFORMER_H

#include <QGeoCoordinate>
#include <QObject>
#include <qgscoordinatereferencesystem.h>
#include <qgscoordinatetransform.h>
#include <qgscoordinatetransformcontext.h>
#include <qgspoint.h>

class QgsQuickCoordinateTransformer : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QgsPoint projectedPosition READ projectedPosition NOTIFY projectedPositionChanged )

    
    Q_PROPERTY( QgsPoint sourcePosition READ sourcePosition WRITE setSourcePosition NOTIFY sourcePositionChanged )

    Q_PROPERTY( QGeoCoordinate sourceCoordinate READ sourceCoordinate WRITE setSourceCoordinate NOTIFY sourceCoordinateChanged )

    Q_PROPERTY( QgsCoordinateReferenceSystem destinationCrs READ destinationCrs WRITE setDestinationCrs NOTIFY destinationCrsChanged )

    Q_PROPERTY( QgsCoordinateReferenceSystem sourceCrs READ sourceCrs WRITE setSourceCrs NOTIFY sourceCrsChanged )

    Q_PROPERTY( QgsCoordinateTransformContext transformContext READ transformContext WRITE setTransformContext NOTIFY transformContextChanged )

    Q_PROPERTY( qreal deltaZ READ deltaZ WRITE setDeltaZ NOTIFY deltaZChanged )

    Q_PROPERTY( bool skipAltitudeTransformation READ skipAltitudeTransformation WRITE setSkipAltitudeTransformation NOTIFY skipAltitudeTransformationChanged )

    Q_PROPERTY( QString verticalGrid READ verticalGrid WRITE setVerticalGrid NOTIFY verticalGridChanged )

  public:
    explicit QgsQuickCoordinateTransformer( QObject *parent = nullptr );

    QgsPoint projectedPosition() const;

    QgsPoint sourcePosition() const;

    void setSourcePosition( const QgsPoint &sourcePosition );

    QgsCoordinateReferenceSystem destinationCrs() const;

    void setDestinationCrs( const QgsCoordinateReferenceSystem &destinationCrs );

    QgsCoordinateReferenceSystem sourceCrs() const;

    void setSourceCrs( const QgsCoordinateReferenceSystem &sourceCrs );

    void setTransformContext( const QgsCoordinateTransformContext &context );

    QgsCoordinateTransformContext transformContext() const;

    qreal deltaZ() const;

    void setDeltaZ( const qreal &deltaZ );

    QGeoCoordinate sourceCoordinate() const;

    void setSourceCoordinate( const QGeoCoordinate &sourceCoordinate );

    bool skipAltitudeTransformation() const;

    void setSkipAltitudeTransformation( bool skipAltitudeTransformation );

    QString verticalGrid() const;

    void setVerticalGrid( const QString &grid );

    Q_INVOKABLE QgsPoint transformPosition( const QgsPoint &position ) const;

  signals:
    void projectedPositionChanged();

    void sourcePositionChanged();

    void sourceCoordinateChanged();

    void destinationCrsChanged();

    void sourceCrsChanged();

    void transformContextChanged();

    void deltaZChanged();

    void skipAltitudeTransformationChanged();

    void verticalGridChanged();

  private:
    void updatePosition();
    QgsPoint processPosition( const QgsPoint &position ) const;

    QgsPoint mProjectedPosition;
    QgsPoint mSourcePosition;
    QgsCoordinateTransform mCoordinateTransform;
    QgsCoordinateTransform mCoordinateVerticalGridTransform;

    qreal mDeltaZ = 0;
    bool mSkipAltitudeTransformation = true;
    QString mVerticalGrid;
    QString mVerticalGridPath;
};

#endif // QGSQUICKCOORDINATETRANSFORMER_H
```


