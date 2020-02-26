/***************************************************************************
 tracker.h - Tracker
  ---------------------
 begin                : 20.02.2020
 copyright            : (C) 2020 by David Signer
 email                : david (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TRACKER_H
#define TRACKER_H

#include <QTimer>
#include "qgsvectorlayer.h"

class RubberbandModel;

class Tracker : public QObject
{
    Q_OBJECT
  public:
    explicit Tracker( QgsVectorLayer *layer, bool visible );

    RubberbandModel *model() const;
    void setModel( RubberbandModel *model );

    //! \copydoc timeInterval
    int timeInterval() const;
    //! \copydoc timeInterval
    void setTimeInterval( const int timeInterval );

    //! \copydoc minimumDistance
    int minimumDistance() const;
    //! \copydoc timeInterval
    void setMinimumDistance( const int minimumDistance );

    //! \copydoc conjunction
    bool conjunction() const;
    //! \copydoc conjunction
    void setConjunction( const bool conjunction );

    QgsVectorLayer *layer() const { return mLayer; }
    void setLayer( QgsVectorLayer *layer ) { mLayer = layer; }
    QgsFeature feature() const { return mFeature; }
    void setFeature( const QgsFeature feature ) { mFeature = feature; }
    bool visible() const { return mVisible; }
    void setVisible( const bool visible ) { mVisible = visible; }

    void start();
    void stop();

  private slots:
    void positionReceived();
    void timeReceived();

  private:
    RubberbandModel *mRubberbandModel = nullptr;

    QTimer mTimer;
    int mTimeInterval = 0;
    int mMinimumDistance = 0;
    bool mConjunction = true;
    bool mTimeIntervalFulfilled = false;
    bool mMinimumDistanceFulfilled = false;

    QgsVectorLayer *mLayer = nullptr;
    QgsFeature mFeature;

    bool mVisible = true;

    QDateTime mStartPositionTimestamp;

    void trackPosition();

};

#endif // TRACKER_H

