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

    //! the (minimum) time interval between setting trackpoints
    int timeInterval() const;
    //! the (minimum) time interval between setting trackpoints
    void setTimeInterval( const int timeInterval );

    //! the minimum distance between setting trackpoints
    int minimumDistance() const;
    //! the minimum distance between setting trackpoints
    void setMinimumDistance( const int minimumDistance );

    //! if both, the minimum distance and the time interval, needs to be fulfilled before setting trackpoints
    bool conjunction() const;
    //! if both, the minimum distance and the time interval, needs to be fulfilled before setting trackpoints
    void setConjunction( const bool conjunction );

    //! the current layer
    QgsVectorLayer *layer() const { return mLayer; }
    //! the current layer
    void setLayer( QgsVectorLayer *layer ) { mLayer = layer; }
    //! the created feature
    QgsFeature feature() const { return mFeature; }
    //! the created feature
    void setFeature( const QgsFeature feature ) { mFeature = feature; }
    //! if the layer (and the rubberband ) is visible
    bool visible() const { return mVisible; }
    //! if the layer (and the rubberband ) is visible
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

