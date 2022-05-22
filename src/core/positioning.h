/***************************************************************************
  positioning.h - Positioning

 ---------------------
 begin                : 22.05.2022
 copyright            : (C) 2022 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef POSITIONING_H
#define POSITIONING_H

#include "abstractgnssreceiver.h"
#include "gnsspositioninformation.h"
#include "qgsquickcoordinatetransformer.h"

#include <QObject>
#include <qgscoordinatereferencesystem.h>
#include <qgscoordinatetransformcontext.h>
#include <qgspoint.h>

class Positioning : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool active READ active WRITE setActive NOTIFY activeChanged )
    Q_PROPERTY( QString device READ device WRITE setDevice NOTIFY deviceChanged )

    Q_PROPERTY( QgsQuickCoordinateTransformer *coordinateTransformer READ coordinateTransformer WRITE setCoordinateTransformer NOTIFY coordinateTransformerChanged )

    Q_PROPERTY( GnssPositionInformation positionInformation READ positionInformation NOTIFY positionInformationChanged )
    Q_PROPERTY( bool valid READ valid NOTIFY positionInformationChanged )

    Q_PROPERTY( QgsPoint sourcePosition READ sourcePosition NOTIFY positionInformationChanged )
    Q_PROPERTY( QgsPoint projectedPosition READ projectedPosition NOTIFY projectedPositionChanged )
    Q_PROPERTY( double projectedHorizontalAccuracy READ projectedHorizontalAccuracy NOTIFY projectedPositionChanged )

    Q_PROPERTY( bool averagedPosition READ averagedPosition WRITE setAveragedPosition NOTIFY averagedPositionChanged )
    Q_PROPERTY( int averagedPositionCount READ averagedPositionCount NOTIFY averagedPositionCountChanged )

  public:
    explicit Positioning( QObject *parent = nullptr );

    virtual ~Positioning() = default;

    bool active() const { return mActive; }
    void setActive( bool active );

    QString device() const { return mDevice; }
    void setDevice( const QString &device );

    QgsQuickCoordinateTransformer *coordinateTransformer() const { return mCoordinateTransformer; }
    void setCoordinateTransformer( QgsQuickCoordinateTransformer *coordinateTransformer );

    GnssPositionInformation positionInformation() const { return mPositionInformation; };
    bool valid() const { return true; } //mPositionInformation.isValid(); }

    QgsPoint sourcePosition() const;
    QgsPoint projectedPosition() const;
    double projectedHorizontalAccuracy() const;

    bool averagedPosition() const { return mAveragedPosition; }
    void setAveragedPosition( bool averaged );

    int averagedPositionCount() const { return mCollectedPositionInformations.size(); }

  signals:

    void activeChanged();
    void deviceChanged();
    void coordinateTransformerChanged();
    void positionInformationChanged();
    void averagedPositionChanged();
    void averagedPositionCountChanged();
    void projectedPositionChanged();

  private slots:

    void lastGnssPositionInformationChanged( const GnssPositionInformation &lastGnssPositionInformation );
    void projectedPositionTransformed();

  private:
    void setupDevice();

    bool mActive = false;
    QString mDevice;

    QgsCoordinateReferenceSystem mSourceCrs;
    QgsCoordinateReferenceSystem mDestinationCrs;
    QgsCoordinateTransformContext mTransformContext;

    GnssPositionInformation mPositionInformation;
    QList<GnssPositionInformation> mCollectedPositionInformations;

    QgsQuickCoordinateTransformer *mCoordinateTransformer = nullptr;
    QgsPoint mSourcePosition;
    QgsPoint mProjectedPosition;
    double mProjectedHorizontalAccuracy = 0.0;

    bool mAveragedPosition = false;

    std::unique_ptr<AbstractGnssReceiver> mReceiver;
};

#endif // POSITIONING_H
