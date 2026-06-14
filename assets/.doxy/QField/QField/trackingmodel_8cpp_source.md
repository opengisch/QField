

# File trackingmodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**trackingmodel.cpp**](trackingmodel_8cpp.md)

[Go to the documentation of this file](trackingmodel_8cpp.md)


```C++
/***************************************************************************
 trackingmodel.cpp - TrackingModel

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

#include "trackingmodel.h"

#include <qgsproject.h>
#include <qgsvectorlayerutils.h>

TrackingModel::TrackingModel( QObject *parent )
  : QAbstractItemModel( parent )
{
}

TrackingModel::~TrackingModel()
{
  qDeleteAll( mTrackers );
}

QHash<int, QByteArray> TrackingModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();

  roles[DisplayString] = "displayString";
  roles[TrackerPointer] = "tracker";

  return roles;
}

QModelIndex TrackingModel::index( int row, int column, const QModelIndex &parent ) const
{
  Q_UNUSED( column )
  Q_UNUSED( parent )

  return createIndex( row, 0, 1000 );
}

QModelIndex TrackingModel::parent( const QModelIndex &index ) const
{
  Q_UNUSED( index )

  return QModelIndex();
}

int TrackingModel::rowCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent )
  return static_cast<int>( mTrackers.size() );
}

int TrackingModel::columnCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent )
  return 1;
}

QVariant TrackingModel::data( const QModelIndex &index, int role ) const
{
  if ( index.row() < 0 || index.row() >= mTrackers.size() )
    return QVariant();

  Tracker *tracker = mTrackers[index.row()];
  switch ( role )
  {
    case DisplayString:
      return QString( "Tracker on layer %1" ).arg( tracker->vectorLayer()->name() );
    case TrackerPointer:
      return QVariant::fromValue<Tracker *>( tracker );
    default:
      return QVariant();
  }
}

bool TrackingModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
  return false;
}

bool TrackingModel::featureInTracking( QgsVectorLayer *layer, const QgsFeatureId featureId )
{
  auto it = trackerIterator( layer );
  if ( it != mTrackers.constEnd() )
  {
    const qsizetype idx = it - mTrackers.constBegin();
    if ( mTrackers[idx]->feature().id() == featureId )
    {
      return true;
    }
  }
  return false;
}

bool TrackingModel::featuresInTracking( QgsVectorLayer *layer, const QList<QgsFeature> &features )
{
  auto it = trackerIterator( layer );
  if ( it != mTrackers.constEnd() )
  {
    const qsizetype idx = it - mTrackers.constBegin();
    QgsFeatureId fid = mTrackers[idx]->feature().id();
    if ( std::any_of( features.begin(), features.end(), [fid]( const QgsFeature &f ) { return f.id() == fid; } ) )
    {
      return true;
    }
  }
  return false;
}

bool TrackingModel::layerInTracking( QgsVectorLayer *layer ) const
{
  return trackerIterator( layer ) != mTrackers.constEnd();
}

bool TrackingModel::layerInActiveTracking( QgsVectorLayer *layer ) const
{
  auto it = trackerIterator( layer );
  if ( it != mTrackers.constEnd() )
  {
    const qsizetype idx = it - mTrackers.constBegin();
    return mTrackers[idx]->isActive();
  }
  return false;
}

Tracker *TrackingModel::trackerForLayer( QgsVectorLayer *layer ) const
{
  auto it = trackerIterator( layer );
  if ( it != mTrackers.constEnd() )
  {
    const qsizetype idx = it - mTrackers.constBegin();
    return mTrackers[idx];
  }
  return nullptr;
}

void TrackingModel::reset()
{
  beginResetModel();
  qDeleteAll( mTrackers );
  mTrackers.clear();
  endResetModel();
}

QModelIndex TrackingModel::createTracker( QgsVectorLayer *layer )
{
  const int trackersSize = static_cast<int>( mTrackers.size() );
  beginInsertRows( QModelIndex(), trackersSize, trackersSize );
  mTrackers.append( new Tracker( layer ) );
  endInsertRows();
  return index( trackersSize, 0 );
}

void TrackingModel::startTracker( QgsVectorLayer *layer, const GnssPositionInformation &positionInformation, const QgsPoint &projectedPosition )
{
  auto it = trackerIterator( layer );
  if ( it != mTrackers.constEnd() )
  {
    const qsizetype idx = it - mTrackers.constBegin();
    mTrackers[idx]->start( positionInformation, projectedPosition );
    emit layerInTrackingChanged( layer, true );
  }
}

void TrackingModel::stopTracker( QgsVectorLayer *layer )
{
  auto it = trackerIterator( layer );
  if ( it != mTrackers.constEnd() )
  {
    const qsizetype idx = it - mTrackers.constBegin();
    mTrackers[idx]->stop();
    beginRemoveRows( QModelIndex(), static_cast<int>( idx ), static_cast<int>( idx ) );
    Tracker *tracker = mTrackers.takeAt( idx );
    endRemoveRows();
    delete tracker;
    emit layerInTrackingChanged( layer, false );
  }
}

void TrackingModel::stopTrackers()
{
  while ( !mTrackers.isEmpty() )
  {
    QgsVectorLayer *layer = mTrackers[0]->vectorLayer();
    mTrackers[0]->stop();
    beginRemoveRows( QModelIndex(), 0, 0 );
    Tracker *tracker = mTrackers.takeAt( 0 );
    endRemoveRows();
    delete tracker;
    emit layerInTrackingChanged( layer, false );
  }
}

void TrackingModel::replayPositionInformationList( const QList<GnssPositionInformation> &positionInformationList, QgsQuickCoordinateTransformer *coordinateTransformer )
{
  for ( int i = 0; i < mTrackers.size(); i++ )
  {
    Tracker *tracker = mTrackers[i];
    if ( tracker->isSuspended() )
    {
      tracker->replayPositionInformationList( positionInformationList, coordinateTransformer );
    }
  }
}

void TrackingModel::suspendUntilReplay()
{
  for ( int i = 0; i < mTrackers.size(); i++ )
  {
    Tracker *tracker = mTrackers[i];
    if ( tracker->isActive() )
    {
      tracker->suspendUntilReplay();
    }
  }
}

void TrackingModel::setTrackerVisibility( QgsVectorLayer *layer, bool visible )
{
  auto it = trackerIterator( layer );
  if ( it != mTrackers.constEnd() )
  {
    const qsizetype idx = it - mTrackers.constBegin();
    mTrackers[idx]->setVisible( visible );
  }
}

void TrackingModel::createProjectTrackers( QgsProject *project )
{
  if ( !project )
    return;

  const QList<QgsMapLayer *> layers = project->mapLayers().values();
  for ( QgsMapLayer *layer : layers )
  {
    if ( QgsVectorLayer *vl = qobject_cast<QgsVectorLayer *>( layer ) )
    {
      const bool trackingSessionActive = layer->customProperty( "QFieldSync/tracking_session_active", false ).toBool();
      if ( trackingSessionActive )
      {
        const bool timeRequirementActive = layer->customProperty( "QFieldSync/tracking_time_requirement_active", false ).toBool();
        const int timeRequirementIntervalSeconds = layer->customProperty( "QFieldSync/tracking_time_requirement_interval_seconds", 30 ).toInt();
        const bool distanceRequirementActive = layer->customProperty( "QFieldSync/tracking_distance_requirement_active", false ).toBool();
        const int distanceRequirementMinimumMeters = layer->customProperty( "QFieldSync/tracking_distance_requirement_minimum_meters", 30 ).toInt();
        const bool sensorDataRequirementActive = layer->customProperty( "QFieldSync/tracking_sensor_data_requirement_active", false ).toBool();
        const bool allRequirementsActive = layer->customProperty( "QFieldSync/tracking_all_requirements_active", false ).toBool();
        const bool erroneousDistanceSafeguardActive = layer->customProperty( "QFieldSync/tracking_erroneous_distance_safeguard_active", false ).toBool();
        const int erroneousDistanceSafeguardMaximumMeters = layer->customProperty( "QFieldSync/tracking_erroneous_distance_safeguard_maximum_meters", 250 ).toInt();
        const int measurementType = layer->customProperty( "QFieldSync/tracking_measurement_type", false ).toInt();

        Tracker *tracker = new Tracker( vl );
        tracker->setTimeInterval( timeRequirementActive ? timeRequirementIntervalSeconds : 0 );
        tracker->setMinimumDistance( distanceRequirementActive ? distanceRequirementMinimumMeters : 0 );
        tracker->setSensorCapture( sensorDataRequirementActive );
        tracker->setConjunction( allRequirementsActive );
        tracker->setMaximumDistance( erroneousDistanceSafeguardActive ? erroneousDistanceSafeguardMaximumMeters : 0 );
        tracker->setMeasureType( static_cast<Tracker::MeasureType>( measurementType ) );

        QgsExpressionContext context = vl->createExpressionContext();
        QgsFeature feature = QgsVectorLayerUtils::createFeature( vl, QgsGeometry(), QgsAttributeMap(), &context );
        tracker->setFeature( feature );

        const int trackersSize = static_cast<int>( mTrackers.size() );
        beginInsertRows( QModelIndex(), trackersSize, trackersSize );
        mTrackers.append( tracker );
        endInsertRows();

        requestTrackingSetup( vl, true );
      }
    }
  }
}

QList<QgsVectorLayer *> TrackingModel::availableLayers( QgsProject *project ) const
{
  QList<QgsVectorLayer *> layers;
  if ( project )
  {
    const QVector<QgsVectorLayer *> projectLayers = project->layers<QgsVectorLayer *>();
    for ( QgsVectorLayer *projectLayer : projectLayers )
    {
      if ( layerInActiveTracking( projectLayer ) || projectLayer->readOnly() )
      {
        continue;
      }

      if ( projectLayer->geometryType() == Qgis::GeometryType::Unknown || projectLayer->geometryType() == Qgis::GeometryType::Null )
      {
        continue;
      }

      layers << projectLayer;
    }
  }
  std::sort( layers.begin(), layers.end(), []( const QgsVectorLayer *l1, const QgsVectorLayer *l2 ) { return l1->name() < l2->name(); } );
  return layers;
}

QgsVectorLayer *TrackingModel::bestAvailableLayer( QgsProject *project ) const
{
  QList<QgsVectorLayer *> layers = availableLayers( project );
  if ( !layers.isEmpty() )
  {
    static QStringList sCandidates { QStringLiteral( "track" ),
                                     QStringLiteral( "suivi" ) };

    QgsVectorLayer *firstLineLayer = nullptr;
    QgsVectorLayer *firstMatchingNameLayer = nullptr;
    for ( QgsVectorLayer *layer : layers )
    {
      const QString name = layer->name();
      if ( layer->geometryType() == Qgis::GeometryType::Line )
      {
        if ( std::any_of( sCandidates.begin(), sCandidates.end(), [&name]( const QString &candidate ) { return name.contains( candidate ); } ) )
        {
          return layer;
        }

        if ( !firstLineLayer )
        {
          firstLineLayer = layer;
        }
      }
      else
      {
        if ( !firstMatchingNameLayer )
        {
          if ( std::any_of( sCandidates.begin(), sCandidates.end(), [&name]( const QString &candidate ) { return name.contains( candidate ); } ) )
          {
            firstMatchingNameLayer = layer;
          }
        }
      }
    }
    return firstMatchingNameLayer ? firstMatchingNameLayer : firstLineLayer ? firstLineLayer
                                                                            : layers.first();
  }
  return nullptr;
}

void TrackingModel::requestTrackingSetup( QgsVectorLayer *layer, bool skipSettings )
{
  mRequestedTrackers << TrackerRequest( layer, skipSettings );
  if ( mRequestedTrackers.size() == 1 )
  {
    const Tracker *tracker = trackerForLayer( mRequestedTrackers.first().layer );
    if ( tracker )
    {
      emit trackingSetupRequested( index( static_cast<int>( mTrackers.indexOf( tracker ) ), 0 ), mRequestedTrackers.first().skipSettings );
    }
  }
}

void TrackingModel::trackingSetupDone()
{
  if ( !mRequestedTrackers.isEmpty() )
  {
    mRequestedTrackers.removeFirst();
    if ( !mRequestedTrackers.isEmpty() )
    {
      const Tracker *tracker = trackerForLayer( mRequestedTrackers.first().layer );
      if ( tracker )
      {
        emit trackingSetupRequested( index( static_cast<int>( mTrackers.indexOf( tracker ) ), 0 ), mRequestedTrackers.first().skipSettings );
      }
    }
  }
}
```


