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

/**
 * Helper class for transform of coordinates (QgsPoint) to a different coordinate reference system.
 *
 * It requires connection of transformation context from mapSettings, source position and source CRS to
 * calculate projected position in desired destination CRS.
 *
 * \ingroup core
 * \note QML Type: CoordinateTransformer
 *
 */
class QgsQuickCoordinateTransformer : public QObject
{
    Q_OBJECT

    //! Projected (destination) position (in destination CRS)
    Q_PROPERTY( QgsPoint projectedPosition READ projectedPosition NOTIFY projectedPositionChanged )

    //! Source position  (in source CRS)
    Q_PROPERTY( QgsPoint sourcePosition READ sourcePosition WRITE setSourcePosition NOTIFY sourcePositionChanged )

    //! Source coordinate for integrating with QtPositioning, alternative to source position
    Q_PROPERTY( QGeoCoordinate sourceCoordinate READ sourceCoordinate WRITE setSourceCoordinate NOTIFY sourceCoordinateChanged )

    //! Destination CRS
    Q_PROPERTY( QgsCoordinateReferenceSystem destinationCrs READ destinationCrs WRITE setDestinationCrs NOTIFY destinationCrsChanged )

    //! Source CRS, default 4326
    Q_PROPERTY( QgsCoordinateReferenceSystem sourceCrs READ sourceCrs WRITE setSourceCrs NOTIFY sourceCrsChanged )

    //! Transformation context, can be set from QgsQuickMapSettings::transformContext()
    Q_PROPERTY( QgsCoordinateTransformContext transformContext READ transformContext WRITE setTransformContext NOTIFY transformContextChanged )

    /**
     * The altitude value of captured coordinates is corrected by the amount of deltaZ.
     * This can be used to correct the altitude with the antenna height for example.
     */
    Q_PROPERTY( qreal deltaZ READ deltaZ WRITE setDeltaZ NOTIFY deltaZChanged )

    /**
     * Skips any altitude correction handling during CRS transformation. deltaZ will still be applied.
     */
    Q_PROPERTY( bool skipAltitudeTransformation READ skipAltitudeTransformation WRITE setSkipAltitudeTransformation NOTIFY skipAltitudeTransformationChanged )

    /**
     * The vertical grid that will be used to correct the altitude value of captured coordinates
     */
    Q_PROPERTY( QString verticalGrid READ verticalGrid WRITE setVerticalGrid NOTIFY verticalGridChanged )

  public:
    //! Creates new coordinate transformer
    explicit QgsQuickCoordinateTransformer( QObject *parent = nullptr );

    //!\copydoc QgsQuickCoordinateTransformer::projectedPosition
    QgsPoint projectedPosition() const;

    //!\copydoc QgsQuickCoordinateTransformer::sourcePosition
    QgsPoint sourcePosition() const;

    //!\copydoc QgsQuickCoordinateTransformer::sourcePosition
    void setSourcePosition( const QgsPoint &sourcePosition );

    //!\copydoc QgsQuickCoordinateTransformer::destinationCrs
    QgsCoordinateReferenceSystem destinationCrs() const;

    //!\copydoc QgsQuickCoordinateTransformer::destinationCrs
    void setDestinationCrs( const QgsCoordinateReferenceSystem &destinationCrs );

    //!\copydoc QgsQuickCoordinateTransformer::sourceCrs
    QgsCoordinateReferenceSystem sourceCrs() const;

    //!\copydoc QgsQuickCoordinateTransformer::sourceCrs
    void setSourceCrs( const QgsCoordinateReferenceSystem &sourceCrs );

    //!\copydoc QgsQuickCoordinateTransformer::transformContext
    void setTransformContext( const QgsCoordinateTransformContext &context );

    //!\copydoc QgsQuickCoordinateTransformer::transformContext
    QgsCoordinateTransformContext transformContext() const;

    //!\copydoc QgsQuickCoordinateTransformer::deltaZ
    qreal deltaZ() const;

    //!\copydoc QgsQuickCoordinateTransformer::deltaZ
    void setDeltaZ( const qreal &deltaZ );

    //!\copydoc QgsQuickCoordinateTransformer::sourceCoordinate
    QGeoCoordinate sourceCoordinate() const;

    //!\copydoc QgsQuickCoordinateTransformer::sourceCoordinate
    void setSourceCoordinate( const QGeoCoordinate &sourceCoordinate );

    //!\copydoc QgsQuickCoordinateTransformer::skipAltitudeTransformation
    bool skipAltitudeTransformation() const;

    //!\copydoc QgsQuickCoordinateTransformer::skipAltitudeTransformation
    void setSkipAltitudeTransformation( bool skipAltitudeTransformation );

    //!\copydoc QgsQuickCoordinateTransformer::verticalGrid
    QString verticalGrid() const;

    //!\copydoc QgsQuickCoordinateTransformer::verticalGrid
    void setVerticalGrid( const QString &grid );

    Q_INVOKABLE QgsPoint transformPosition( const QgsPoint &position ) const;

  signals:
    //!\copydoc QgsQuickCoordinateTransformer::projectedPosition
    void projectedPositionChanged();

    //!\copydoc QgsQuickCoordinateTransformer::sourcePosition
    void sourcePositionChanged();

    void sourceCoordinateChanged();

    //!\copydoc QgsQuickCoordinateTransformer::destinationCrs
    void destinationCrsChanged();

    //!\copydoc QgsQuickCoordinateTransformer::sourceCrs
    void sourceCrsChanged();

    //!\copydoc QgsQuickCoordinateTransformer::transformContext
    void transformContextChanged();

    //!\copydoc QgsQuickCoordinateTransformer::deltaZChanged
    void deltaZChanged();

    //!\copydoc QgsQuickCoordinateTransformer::skipAltitudeTransformation
    void skipAltitudeTransformationChanged();

    //!\copydoc QgsQuickCoordinateTransformer::verticalGrid
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
