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

#include "rubberbandmodel.h"
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
  roles[VectorLayer] = "vectorLayer";
  roles[TimeInterval] = "timeInterval";
  roles[MinimumDistance] = "minimumDistance";
  roles[MaximumDistance] = "maximumDistance";
  roles[Conjunction] = "conjunction";
  roles[Feature] = "feature";
  roles[RubberModel] = "rubberModel";
  roles[Visible] = "visible";
  roles[StartPositionTimestamp] = "startPositionTimestamp";
  roles[MeasureType] = "measureType";
  roles[SensorCapture] = "sensorCapture";
  roles[IsActive] = "isActive";

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
  return mTrackers.size();
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
      return QString( "Tracker on layer %1" ).arg( tracker->layer()->name() );
    case VectorLayer:
      return QVariant::fromValue<QgsVectorLayer *>( tracker->layer() );
    case Feature:
      return QVariant::fromValue<QgsFeature>( tracker->feature() );
    case Visible:
      return tracker->visible();
    case StartPositionTimestamp:
      return tracker->startPositionTimestamp();
    case TimeInterval:
      return tracker->timeInterval();
    case MinimumDistance:
      return tracker->minimumDistance();
    case Conjunction:
      return tracker->conjunction();
    case RubberModel:
      return QVariant::fromValue<RubberbandModel *>( tracker->model() );
    case MeasureType:
      return tracker->measureType();
    case SensorCapture:
      return tracker->sensorCapture();
    case MaximumDistance:
      return tracker->maximumDistance();
    case IsActive:
      return tracker->isActive();
    default:
      return QVariant();
  }
}

bool TrackingModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
  if ( index.row() < 0 || index.row() >= mTrackers.size() )
    return false;

  Tracker *tracker = mTrackers[index.row()];
  switch ( role )
  {
    case Feature:
      tracker->setFeature( value.value<QgsFeature>() );
      break;
    case Visible:
      tracker->setVisible( value.toBool() );
      break;
    case TimeInterval:
      tracker->setTimeInterval( value.toDouble() );
      break;
    case MinimumDistance:
      tracker->setMinimumDistance( value.toDouble() );
      break;
    case Conjunction:
      tracker->setConjunction( value.toBool() );
      break;
    case RubberModel:
      tracker->setModel( value.value<RubberbandModel *>() );
      break;
    case MeasureType:
      tracker->setMeasureType( static_cast<Tracker::MeasureType>( value.toInt() ) );
      break;
    case SensorCapture:
      tracker->setSensorCapture( value.toBool() );
      break;
    case MaximumDistance:
      tracker->setMaximumDistance( value.toDouble() );
      break;
    default:
      return false;
  }
  emit dataChanged( index, index, QVector<int>() << role );
  return true;
}

bool TrackingModel::featureInTracking( QgsVectorLayer *layer, const QgsFeatureId featureId )
{
  if ( trackerIterator( layer ) != mTrackers.constEnd() )
  {
    int listIndex = trackerIterator( layer ) - mTrackers.constBegin();
    if ( mTrackers[listIndex]->feature().id() == featureId )
      return true;
  }
  return false;
}

bool TrackingModel::featuresInTracking( QgsVectorLayer *layer, const QList<QgsFeature> &features )
{
  if ( trackerIterator( layer ) != mTrackers.constEnd() )
  {
    int listIndex = trackerIterator( layer ) - mTrackers.constBegin();
    QgsFeatureId fid = mTrackers[listIndex]->feature().id();
    if ( std::any_of( features.begin(), features.end(), [fid]( const QgsFeature &f ) { return f.id() == fid; } ) )
    {
      return true;
    }
  }
  return false;
}

bool TrackingModel::layerInTracking( QgsVectorLayer *layer )
{
  return trackerIterator( layer ) != mTrackers.constEnd();
}

Tracker *TrackingModel::trackerForLayer( QgsVectorLayer *layer )
{
  return *trackerIterator( layer );
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
  beginInsertRows( QModelIndex(), mTrackers.count(), mTrackers.count() );
  mTrackers.append( new Tracker( layer ) );
  endInsertRows();
  return index( mTrackers.size() - 1, 0 );
}

void TrackingModel::startTracker( QgsVectorLayer *layer )
{
  int listIndex = trackerIterator( layer ) - mTrackers.constBegin();
  mTrackers[listIndex]->start();

  QModelIndex idx = index( listIndex, 0 );
  emit dataChanged( idx, idx, QVector<int>() << TrackingModel::IsActive );
  emit layerInTrackingChanged( layer, true );
}

void TrackingModel::stopTracker( QgsVectorLayer *layer )
{
  int listIndex = trackerIterator( layer ) - mTrackers.constBegin();
  mTrackers[listIndex]->stop();

  QModelIndex idx = index( listIndex, 0 );
  emit dataChanged( idx, idx, QVector<int>() << TrackingModel::IsActive );

  beginRemoveRows( QModelIndex(), listIndex, listIndex );
  delete mTrackers.takeAt( listIndex );
  endRemoveRows();

  emit layerInTrackingChanged( layer, false );
}

void TrackingModel::setTrackerVisibility( QgsVectorLayer *layer, bool visible )
{
  if ( trackerIterator( layer ) != mTrackers.constEnd() )
  {
    int listIndex = trackerIterator( layer ) - mTrackers.constBegin();
    setData( index( listIndex, 0, QModelIndex() ), visible, Visible );
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
        const bool erroneousDistanceSafeguardMaximumMeters = layer->customProperty( "QFieldSync/tracking_erroneous_distance_safeguard_maximum_meters", 250 ).toInt();
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

        beginInsertRows( QModelIndex(), mTrackers.count(), mTrackers.count() );
        mTrackers.append( tracker );
        endInsertRows();

        startTracker( vl );
      }
    }
  }
}
