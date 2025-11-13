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

/**
 * \ingroup core
 */
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

    //! Creates tracking sessions defined in a project being opened
    Q_INVOKABLE void createProjectTrackers( QgsProject *project );
    //! Creates a tracking session for the provided vector \a layer.
    Q_INVOKABLE QModelIndex createTracker( QgsVectorLayer *layer );
    //! Starts tracking for the provided vector \a layer provided it has a tracking session created.
    Q_INVOKABLE void startTracker( QgsVectorLayer *layer, const GnssPositionInformation &positionInformation = GnssPositionInformation(), const QgsPoint &projectedPosition = QgsPoint() );
    //! Stops the tracking session of the provided vector \a layer.
    Q_INVOKABLE void stopTracker( QgsVectorLayer *layer );
    //! Stops all tracking sessions.
    Q_INVOKABLE void stopTrackers();
    //! Sets whether the tracking session rubber band is \a visible.
    Q_INVOKABLE void setTrackerVisibility( QgsVectorLayer *layer, bool visible );
    //! Returns TRUE if the \a featureId is attached to a vector \a layer tracking session.
    Q_INVOKABLE bool featureInTracking( QgsVectorLayer *layer, QgsFeatureId featureId );
    //! Returns TRUE if the list of \a features is attached to a vector \a layer tracking session.
    Q_INVOKABLE bool featuresInTracking( QgsVectorLayer *layer, const QList<QgsFeature> &features );
    //! Returns TRUE if the vector \a layer has a tracking session.
    Q_INVOKABLE bool layerInTracking( QgsVectorLayer *layer ) const;
    //! Returns TRUE if the vector \a layer has an active tracking session.
    Q_INVOKABLE bool layerInActiveTracking( QgsVectorLayer *layer ) const;
    //! Returns the tracker for the vector \a layer if a tracking session is present, otherwise returns NULLPTR.
    Q_INVOKABLE Tracker *trackerForLayer( QgsVectorLayer *layer ) const;

    //! Replays a list of position information for all active trackers
    Q_INVOKABLE void replayPositionInformationList( const QList<GnssPositionInformation> &positionInformationList, QgsQuickCoordinateTransformer *coordinateTransformer = nullptr );

    Q_INVOKABLE void suspendUntilReplay();

    void reset();

    /**
     * Returns a list of available \a project vector layers with which a tracking session can be started.
     */
    Q_INVOKABLE QList<QgsVectorLayer *> availableLayers( QgsProject *project ) const;

    /**
     * Returns the best available \a project vector layer with which a tracking session can be started.
     */
    Q_INVOKABLE QgsVectorLayer *bestAvailableLayer( QgsProject *project ) const;

    /**
     * Forwards a tracking setup request to the user interface consisting of a settings panel followed by
     * a feature form (unless suppressed by the project configuration).
     * \a layer the vector layer associated with the tracking
     * \a skipSettings set to TRUE if the settings panel should be omitted and only show the feature form
     */
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
