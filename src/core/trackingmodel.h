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

    Q_PROPERTY( bool activated  WRITE setActivated READ activated NOTIFY activatedChanged )

  public:
    explicit TrackingModel( QObject *parent = nullptr );
    ~TrackingModel() override;

    enum TrackingRoles
    {
      DisplayString = Qt::UserRole,
      VectorLayer,
      RubberModel,
      TimeInterval,
      MinimumDistance,
      Conjunction,
      Visible,
      Feature
    };

    QHash<int, QByteArray> roleNames() const override;
    QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const override;
    QModelIndex parent( const QModelIndex &index ) const override;
    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const override;
    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;
    virtual bool setData( const QModelIndex &index, const QVariant &value, int role ) override;

    void setActivated( const bool activated ) { mActivated = activated; }
    bool activated() const { return mActivated; }

    Q_INVOKABLE void startTracker( QgsVectorLayer *layer );

    //function used external from the model dependen objects
    Q_INVOKABLE void createTracker( QgsVectorLayer *layer );
    Q_INVOKABLE void stopTracker( QgsVectorLayer *layer );

    Q_INVOKABLE void setLayerVisible( QgsVectorLayer *layer, bool visible );
    Q_INVOKABLE bool featureOnTrack( QgsVectorLayer *layer,  QgsFeatureId featureId );
    Q_INVOKABLE bool layerOnTrack( QgsVectorLayer *layer );


  signals:
    void activatedChanged();
    void trackerStarted( QgsVectorLayer *layer );
    void trackerStopped( QgsVectorLayer *layer );

  private:
    QList<Tracker *> mTrackers;
    QList<Tracker *>::const_iterator trackerIterator( QgsVectorLayer *layer )
    {
      return std::find_if( mTrackers.constBegin(), mTrackers.constEnd(), [layer]( const Tracker * tracker ) { return tracker->layer() == layer;} );
    }

    bool mActivated = false;
};



#endif // TRACKINGMODEL_H
