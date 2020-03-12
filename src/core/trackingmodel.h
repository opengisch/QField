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

#include <QAbstractItemModel>

#include "rubberbandmodel.h"
#include "tracker.h"

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
      VectorLayer,        //! the layer in the current tracking session
      RubberModel,        //! the rubberbandmodel used in the current tracking session
      TimeInterval,       //! the (minimum) time interval between setting trackpoints
      MinimumDistance,    //! the minimum distance between setting trackpoints
      Conjunction,        //! if both, the minimum distance and the time interval, needs to be fulfilled before setting trackpoints
      Visible,            //! if the layer and so the tracking components like rubberband is visible
      Feature,            //! the feature in the current tracking session
      StartPositionTimestamp             //!
    };

    QHash<int, QByteArray> roleNames() const override;
    QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const override;
    QModelIndex parent( const QModelIndex &index ) const override;
    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const override;
    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;
    virtual bool setData( const QModelIndex &index, const QVariant &value, int role ) override;

    //! creates a tracking session for this \a layer and the information if it's \a visible , so it's ready to receive properties
    Q_INVOKABLE void createTracker( QgsVectorLayer *layer, bool visible );
    //! starts the tracking for the current tracking session of this \a layer, collects first vertex
    Q_INVOKABLE void startTracker( QgsVectorLayer *layer );
    //! stops the tracking session of this \a layer
    Q_INVOKABLE void stopTracker( QgsVectorLayer *layer );
    //! sets the information that this \a layer is \a visible or not, means the tracking component should be as well
    Q_INVOKABLE void setLayerVisible( QgsVectorLayer *layer, bool visible );
    //! if the \a feature is in a tracking session
    Q_INVOKABLE bool featureInTracking( QgsVectorLayer *layer,  const QgsFeatureId featureId );
    //! if the \a layer is in a tracking session
    Q_INVOKABLE bool layerInTracking( QgsVectorLayer *layer );

    void reset();

  signals:
    void layerInTrackingChanged( QgsVectorLayer *layer, bool tracking );

  private:
    QList<Tracker *> mTrackers;
    QList<Tracker *>::const_iterator trackerIterator( QgsVectorLayer *layer )
    {
      return std::find_if( mTrackers.constBegin(), mTrackers.constEnd(), [layer]( const Tracker * tracker ) { return tracker->layer() == layer;} );
    }
};



#endif // TRACKINGMODEL_H
