

# File trackingmodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**trackingmodel.h**](trackingmodel_8h.md)

[Go to the documentation of this file](trackingmodel_8h.md)


```C++
/***************************************************************************
 trackingmodel.h - TrackingModel

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
#ifndef TRACKINGMODEL_H
#define TRACKINGMODEL_H

#include "tracker.h"

#include <QAbstractItemModel>

class QgsQuickCoordinateTransformer;
class RubberbandModel;
class Track;

class TrackingModel : public QAbstractItemModel
{
    Q_OBJECT

  public:
    explicit TrackingModel( QObject *parent = nullptr );
    ~TrackingModel() override;

    enum TrackingRoles
    {
      DisplayString = Qt::UserRole,
      TrackerPointer,
    };

    QHash<int, QByteArray> roleNames() const override;
    QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const override;
    QModelIndex parent( const QModelIndex &index ) const override;
    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const override;
    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;
    virtual bool setData( const QModelIndex &index, const QVariant &value, int role ) override;

    Q_INVOKABLE void createProjectTrackers( QgsProject *project );
    Q_INVOKABLE QModelIndex createTracker( QgsVectorLayer *layer );
    Q_INVOKABLE void startTracker( QgsVectorLayer *layer, const GnssPositionInformation &positionInformation = GnssPositionInformation(), const QgsPoint &projectedPosition = QgsPoint() );
    Q_INVOKABLE void stopTracker( QgsVectorLayer *layer );
    Q_INVOKABLE void stopTrackers();
    Q_INVOKABLE void setTrackerVisibility( QgsVectorLayer *layer, bool visible );
    Q_INVOKABLE bool featureInTracking( QgsVectorLayer *layer, QgsFeatureId featureId );
    Q_INVOKABLE bool featuresInTracking( QgsVectorLayer *layer, const QList<QgsFeature> &features );
    Q_INVOKABLE bool layerInTracking( QgsVectorLayer *layer ) const;
    Q_INVOKABLE bool layerInActiveTracking( QgsVectorLayer *layer ) const;
    Q_INVOKABLE Tracker *trackerForLayer( QgsVectorLayer *layer ) const;

    Q_INVOKABLE void replayPositionInformationList( const QList<GnssPositionInformation> &positionInformationList, QgsQuickCoordinateTransformer *coordinateTransformer = nullptr );

    Q_INVOKABLE void suspendUntilReplay();

    void reset();

    Q_INVOKABLE QList<QgsVectorLayer *> availableLayers( QgsProject *project ) const;

    Q_INVOKABLE QgsVectorLayer *bestAvailableLayer( QgsProject *project ) const;

    Q_INVOKABLE void requestTrackingSetup( QgsVectorLayer *layer, bool skipSettings = false );

    Q_INVOKABLE void trackingSetupDone();

  signals:

    void layerInTrackingChanged( QgsVectorLayer *layer, bool tracking );
    void trackingSetupRequested( QModelIndex trackerIndex, bool skipSettings );

  private:
    struct TrackerRequest
    {
        TrackerRequest()
        {}

        TrackerRequest( QgsVectorLayer *layer, bool skipSettings )
          : layer( layer )
          , skipSettings( skipSettings )
        {
        }

        QPointer<QgsVectorLayer> layer = nullptr;
        bool skipSettings = false;
    };

    QList<Tracker *> mTrackers;
    QList<TrackerRequest> mRequestedTrackers;

    QList<Tracker *>::const_iterator trackerIterator( QgsVectorLayer *layer ) const
    {
      return std::find_if( mTrackers.constBegin(), mTrackers.constEnd(), [layer]( const Tracker *tracker ) { return tracker->vectorLayer() == layer; } );
    }
};


#endif // TRACKINGMODEL_H
```


