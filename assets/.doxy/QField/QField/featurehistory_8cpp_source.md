

# File featurehistory.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**featurehistory.cpp**](featurehistory_8cpp.md)

[Go to the documentation of this file](featurehistory_8cpp.md)


```C++
/***************************************************************************
                        featurehistory.cpp
                        ------------------
  begin                : Dec 2023
  copyright            : (C) 2023 by Ivan Ivanov
  email                : ivan@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "featurehistory.h"
#include "trackingmodel.h"

#include <qgsmessagelog.h>
#include <qgsvectorlayer.h>
#include <qgsvectorlayereditbuffer.h>
#include <qgsvectorlayerutils.h>

FeatureHistory::FeatureHistory( const QgsProject *project, TrackingModel *trackingModel )
  : mProject( project )
  , mTrackingModel( trackingModel )
{
  connect( mProject, &QgsProject::homePathChanged, this, &FeatureHistory::onHomePathChanged );
  connect( mProject, &QgsProject::layersAdded, this, &FeatureHistory::onLayersAdded );
  connect( &mTimer, &QTimer::timeout, this, &FeatureHistory::onTimerTimeout );

  if ( mTrackingModel )
  {
    connect( mTrackingModel, &TrackingModel::layerInTrackingChanged, this, &FeatureHistory::onLayerInTrackingChanged );
  }
}

void FeatureHistory::onHomePathChanged()
{
  if ( mProject->homePath().isNull() )
    return;

  mObservedLayerIds.clear();

  addLayerListeners();
}

void FeatureHistory::addLayerListeners()
{
  const QList<QgsMapLayer *> layers = mProject->mapLayers().values();

  for ( QgsMapLayer *layer : layers )
  {
    QgsVectorLayer *vl = qobject_cast<QgsVectorLayer *>( layer );
    if ( !vl )
    {
      continue;
    }

    if ( mObservedLayerIds.contains( vl->id() ) )
    {
      continue;
    }

    // make sure we disconnect vector layer signal slots for these events if they are already present
    disconnect( vl, &QgsVectorLayer::beforeCommitChanges, this, &FeatureHistory::onBeforeCommitChanges );
    disconnect( vl, &QgsVectorLayer::afterCommitChanges, this, &FeatureHistory::onAfterCommitChanges );
    disconnect( vl, &QgsVectorLayer::committedFeaturesAdded, this, &FeatureHistory::onCommittedFeaturesAdded );
    disconnect( vl, &QgsVectorLayer::committedFeaturesRemoved, this, &FeatureHistory::onCommittedFeaturesRemoved );

    connect( vl, &QgsVectorLayer::beforeCommitChanges, this, &FeatureHistory::onBeforeCommitChanges );
    connect( vl, &QgsVectorLayer::afterCommitChanges, this, &FeatureHistory::onAfterCommitChanges );
    connect( vl, &QgsVectorLayer::committedFeaturesAdded, this, &FeatureHistory::onCommittedFeaturesAdded );
    connect( vl, &QgsVectorLayer::committedFeaturesRemoved, this, &FeatureHistory::onCommittedFeaturesRemoved );

    mObservedLayerIds.insert( vl->id() );
  }
}


void FeatureHistory::onBeforeCommitChanges()
{
  if ( mIsApplyingModifications )
  {
    return;
  }

  QgsVectorLayer *vl = qobject_cast<QgsVectorLayer *>( sender() );
  if ( !vl )
  {
    return;
  }

  QgsVectorLayerEditBuffer *eb = vl->editBuffer();
  if ( !eb )
  {
    return;
  }

  const QgsFeatureIds deletedFids = eb->deletedFeatureIds();
  const QgsFeatureIds changedGeometriesFids = qgis::listToSet( eb->changedGeometries().keys() );
  const QgsFeatureIds changedAttributesFids = qgis::listToSet( eb->changedAttributeValues().keys() );
  // The feature FIDs will not be valid, we'll nevertheless use a basic layer ID check
  const QgsFeatureIds addedFids = qgis::listToSet( eb->addedFeatures().keys() );

  if ( deletedFids.isEmpty() && changedGeometriesFids.isEmpty() && changedAttributesFids.isEmpty() && addedFids.isEmpty() )
  {
    return;
  }

  // NOTE QgsFeatureIds underlying implementation is QSet, so no need to check if the QgsFeatureId already exists
  QgsFeatureIds changedFids;
  for ( const QgsFeatureId fid : deletedFids )
    changedFids.insert( fid );

  for ( const QgsFeatureId fid : changedGeometriesFids )
    changedFids.insert( fid );

  for ( const QgsFeatureId fid : changedAttributesFids )
    changedFids.insert( fid );

  // NOTE we read the features from the dataProvider directly as we want to access the old values.
  // If we use the layer, we get the values from the edit buffer.
  QgsFeatureIterator featuresIt = vl->dataProvider()->getFeatures( QgsFeatureRequest( changedFids ) );
  QMap<QgsFeatureId, QgsFeature> modifiedFeatures;
  QgsFeature f;

  while ( featuresIt.nextFeature( f ) )
  {
    if ( deletedFids.contains( f.id() ) )
    {
      // Insure that join-provided fields are added to avoid error restoring deleted feature
      // on vector layers containing joins
      QgsVectorLayerUtils::matchAttributesToFields( f, vl->fields() );
    }
    modifiedFeatures.insert( f.id(), f );
  }

  changedFids.unite( addedFids );

  qDebug() << "FeatureHistory::onBeforeCommitChanges: vl->id()=" << vl->id() << "changedFids=" << changedFids;

  mTempModifiedFeatureIdsByLayerId[vl->id()].unite( changedFids );
  mTempModifiedFeaturesByLayerId.insert( vl->id(), modifiedFeatures );
}


void FeatureHistory::onCommittedFeaturesAdded( const QString &localLayerId, const QgsFeatureList &addedFeatures )
{
  if ( mIsApplyingModifications )
  {
    return;
  }

  QgsVectorLayer *vl = qobject_cast<QgsVectorLayer *>( sender() );
  if ( !vl || !mTempModifiedFeatureIdsByLayerId.keys().contains( vl->id() ) )
  {
    return;
  }

  qDebug() << "FeatureHistory::onCommittedFeaturesAdded: adding committed features added";

  FeatureModifications modifications = mTempHistoryStep.take( vl->id() );

  for ( QgsFeature f : addedFeatures )
  {
    QgsVectorLayerUtils::matchAttributesToFields( f, vl->fields() );
    modifications.createdFeatures.append( OldNewFeaturePair( QgsFeature(), f ) );
  }

  mTempHistoryStep.insert( vl->id(), modifications );
}

void FeatureHistory::onCommittedFeaturesRemoved( const QString &layerId, const QgsFeatureIds &deletedFeatureIds )
{
  if ( mIsApplyingModifications )
  {
    return;
  }

  qDebug() << "FeatureHistory::onCommittedFeaturesRemoved: adding committed features removed";

  QgsFeatureIds fids;
  for ( const QgsFeatureId &fid : deletedFeatureIds )
  {
    if ( !mTempModifiedFeatureIdsByLayerId[layerId].contains( fid ) )
      continue;

    fids << fid;
  }

  if ( !fids.isEmpty() )
  {
    mTempDeletedFeatureIdsByLayerId.insert( layerId, deletedFeatureIds );
  }
}

void FeatureHistory::onAfterCommitChanges()
{
  if ( mIsApplyingModifications )
  {
    return;
  }

  QgsVectorLayer *vl = qobject_cast<QgsVectorLayer *>( sender() );
  if ( !vl )
  {
    return;
  }

  const QString layerId = vl->id();
  FeatureModifications modifications = mTempHistoryStep.take( layerId );
  QMap<QgsFeatureId, QgsFeature> modifiedFeaturesOld = mTempModifiedFeaturesByLayerId.take( layerId );
  const QgsFeatureIds deletedFids = mTempDeletedFeatureIdsByLayerId.take( layerId );

  for ( const QgsFeatureId &deletedFid : deletedFids )
  {
    OldNewFeaturePair oldNewFeaturePair( modifiedFeaturesOld.take( deletedFid ), QgsFeature() );
    modifications.deletedFeatures.append( oldNewFeaturePair );
  }

  const QgsFeatureIds modifiedFids = qgis::listToSet( modifiedFeaturesOld.keys() );
  QgsFeatureIterator featuresIt = vl->getFeatures( QgsFeatureRequest( modifiedFids ) );
  QgsFeature f;

  while ( featuresIt.nextFeature( f ) )
  {
    OldNewFeaturePair oldNewFeaturePair( modifiedFeaturesOld.take( f.id() ), f );
    modifications.updatedFeatures.append( oldNewFeaturePair );
  }

  if ( !modifications.createdFeatures.isEmpty() || !modifications.updatedFeatures.isEmpty() || !modifications.deletedFeatures.isEmpty() )
  {
    mTempHistoryStep.insert( vl->id(), modifications );
    mTimer.start( sTimeoutMs );
  }
}

void FeatureHistory::onLayersAdded( const QList<QgsMapLayer *> &layers )
{
  Q_UNUSED( layers );
  addLayerListeners();
}

void FeatureHistory::onLayerInTrackingChanged( QgsVectorLayer *vl, bool isTracking )
{
  if ( isTracking )
  {
    disconnect( vl, &QgsVectorLayer::beforeCommitChanges, this, &FeatureHistory::onBeforeCommitChanges );
    disconnect( vl, &QgsVectorLayer::afterCommitChanges, this, &FeatureHistory::onAfterCommitChanges );
    disconnect( vl, &QgsVectorLayer::committedFeaturesAdded, this, &FeatureHistory::onCommittedFeaturesAdded );
    disconnect( vl, &QgsVectorLayer::committedFeaturesRemoved, this, &FeatureHistory::onCommittedFeaturesRemoved );
  }
  else
  {
    connect( vl, &QgsVectorLayer::beforeCommitChanges, this, &FeatureHistory::onBeforeCommitChanges );
    connect( vl, &QgsVectorLayer::afterCommitChanges, this, &FeatureHistory::onAfterCommitChanges );
    connect( vl, &QgsVectorLayer::committedFeaturesAdded, this, &FeatureHistory::onCommittedFeaturesAdded );
    connect( vl, &QgsVectorLayer::committedFeaturesRemoved, this, &FeatureHistory::onCommittedFeaturesRemoved );
  }
}

void FeatureHistory::onTimerTimeout()
{
  mTimer.stop();
  mUndoHistory.append( mTempHistoryStep );
  mTempHistoryStep.clear();
  mTempModifiedFeatureIdsByLayerId.clear();
  mRedoHistory.clear();

  emit isUndoAvailableChanged();
  emit isRedoAvailableChanged();
}

QMap<QString, FeatureHistory::FeatureModifications> FeatureHistory::reverseModifications( QMap<QString, FeatureModifications> &modificationsByLayerId )
{
  QMap<QString, FeatureModifications> reversedModificationsByLayerId;

  const QStringList layerIds = modificationsByLayerId.keys();
  for ( const QString &layerId : layerIds )
  {
    const QgsVectorLayer *vl = qobject_cast<QgsVectorLayer *>( mProject->mapLayer( layerId ) );
    if ( !vl )
    {
      continue;
    }

    FeatureModifications modifications = modificationsByLayerId.value( layerId );
    FeatureModifications reversedModifications;

    for ( const OldNewFeaturePair &pair : modifications.deletedFeatures )
    {
      reversedModifications.createdFeatures.append( OldNewFeaturePair( pair.second, pair.first ) );
    }

    for ( const OldNewFeaturePair &pair : modifications.createdFeatures )
    {
      reversedModifications.deletedFeatures.append( OldNewFeaturePair( pair.second, pair.first ) );
    }

    for ( const OldNewFeaturePair &pair : modifications.updatedFeatures )
    {
      reversedModifications.updatedFeatures.append( OldNewFeaturePair( pair.second, pair.first ) );
    }

    reversedModificationsByLayerId.insert( layerId, reversedModifications );
  }

  return reversedModificationsByLayerId;
}


bool FeatureHistory::applyModifications( QMap<QString, FeatureModifications> &modificationsByLayerId )
{
  mIsApplyingModifications = true;

  const QStringList layerIds = modificationsByLayerId.keys();

  for ( const QString &layerId : layerIds )
  {
    QgsVectorLayer *vl = qobject_cast<QgsVectorLayer *>( mProject->mapLayer( layerId ) );
    if ( !vl )
    {
      continue;
    }

    FeatureModifications undoFeatureModifications = modificationsByLayerId.value( layerId );

    if ( !vl->startEditing() )
    {
      return false;
    }

    // created features
    QgsFeatureIds fidsToDelete;
    for ( const OldNewFeaturePair &pair : undoFeatureModifications.createdFeatures )
    {
      fidsToDelete << pair.second.id();
    }

    if ( !undoFeatureModifications.createdFeatures.isEmpty() && !vl->deleteFeatures( fidsToDelete ) )
    {
      QgsMessageLog::logMessage( tr( "Failed to undo created features in layer \"%1\"" ).arg( vl->name() ) );
      return false;
    }

    // deleted features
    QgsFeatureList featuresToAdd;
    for ( const OldNewFeaturePair &pair : undoFeatureModifications.deletedFeatures )
    {
      featuresToAdd.append( pair.first );
    }

    if ( !undoFeatureModifications.deletedFeatures.isEmpty() && !vl->addFeatures( featuresToAdd ) )
    {
      QgsMessageLog::logMessage( tr( "Failed to undo deleted features in layer \"%1\"" ).arg( vl->name() ) );
      return false;
    }

    // update features
    if ( !undoFeatureModifications.updatedFeatures.isEmpty() )
    {
      // go through the updated features steps in reverse order
      auto it = undoFeatureModifications.updatedFeatures.end();
      while ( it != undoFeatureModifications.updatedFeatures.begin() )
      {
        --it;
        if ( !vl->updateFeature( ( *it ).first, true ) )
        {
          QgsMessageLog::logMessage( tr( "Failed to undo update features in layer \"%1\"" ).arg( vl->name() ) );
          return false;
        }
      }
    }

    if ( !vl->commitChanges() )
    {
      QgsMessageLog::logMessage( tr( "Failed to commit undo feature modification in layer \"%1\"" ).arg( vl->name() ) );

      if ( !vl->rollBack() )
      {
        QgsMessageLog::logMessage( tr( "Failed to rollback undo feature modifications in layer \"%1\"" ).arg( vl->name() ) );
      }

      return false;
    }
  }

  mIsApplyingModifications = false;

  return true;
}

bool FeatureHistory::undo()
{
  if ( mUndoHistory.isEmpty() )
  {
    return false;
  }

  QMap<QString, FeatureModifications> modifications = mUndoHistory.takeLast();
  QMap<QString, FeatureModifications> reversedModifications = reverseModifications( modifications );

  if ( !applyModifications( modifications ) )
  {
    return false;
  }

  mRedoHistory.append( reversedModifications );

  emit isUndoAvailableChanged();
  emit isRedoAvailableChanged();

  return true;
}


bool FeatureHistory::redo()
{
  if ( mRedoHistory.isEmpty() )
  {
    return false;
  }

  QMap<QString, FeatureModifications> modifications = mRedoHistory.takeLast();
  QMap<QString, FeatureModifications> reversedModifications = reverseModifications( modifications );

  if ( !applyModifications( modifications ) )
  {
    return false;
  }

  mUndoHistory.append( reversedModifications );

  emit isUndoAvailableChanged();
  emit isRedoAvailableChanged();

  return true;
}


bool FeatureHistory::isUndoAvailable()
{
  return !mUndoHistory.isEmpty();
}


bool FeatureHistory::isRedoAvailable()
{
  return !mRedoHistory.isEmpty();
}

const QString FeatureHistory::undoMessage()
{
  if ( mUndoHistory.isEmpty() )
  {
    return QString();
  }

  int totalChanges = 0;
  QMap<QString, FeatureModifications> modifiedFeaturesByLayerId = mUndoHistory.last();
  QStringList layerIds;
  bool hasCreatedFeatures = false;
  bool hasUpdatedFeatures = false;
  bool hasDeletedFeatures = false;
  for ( const QString &layerId : modifiedFeaturesByLayerId.keys() )
  {
    if ( !layerIds.contains( layerId ) )
    {
      layerIds << layerId;
    }

    hasCreatedFeatures |= modifiedFeaturesByLayerId[layerId].createdFeatures.count() > 0;
    hasUpdatedFeatures |= modifiedFeaturesByLayerId[layerId].updatedFeatures.count() > 0;
    hasDeletedFeatures |= modifiedFeaturesByLayerId[layerId].deletedFeatures.count() > 0;
    // cppcheck-suppress useStlAlgorithm
    totalChanges += modifiedFeaturesByLayerId[layerId].createdFeatures.count()
                    + modifiedFeaturesByLayerId[layerId].updatedFeatures.count()
                    + modifiedFeaturesByLayerId[layerId].deletedFeatures.count();
  }

  if ( totalChanges > 0 )
  {
    QgsVectorLayer *vl = layerIds.size() == 1 ? qobject_cast<QgsVectorLayer *>( mProject->mapLayer( layerIds.first() ) ) : nullptr;
    if ( hasCreatedFeatures && !hasUpdatedFeatures && !hasDeletedFeatures )
    {
      return vl ? tr( "Undo creation of %n feature(s) on layer %1.", "", totalChanges ).arg( vl->name() ) : tr( "Undo creation of %n feature(s).", "", totalChanges );
    }
    else if ( !hasCreatedFeatures && !hasUpdatedFeatures && hasDeletedFeatures )
    {
      return vl ? tr( "Undo deletion of %n feature(s) on layer %1.", "", totalChanges ).arg( vl->name() ) : tr( "Undo deletion of %n feature(s).", "", totalChanges );
    }
    else
    {
      return vl ? tr( "Undo modifications on %n feature(s) on layer %1.", "", totalChanges ).arg( vl->name() ) : tr( "Undo modifications on %n feature(s).", "", totalChanges );
    }
  }

  return QString();
}


const QString FeatureHistory::redoMessage()
{
  if ( mRedoHistory.isEmpty() )
  {
    return QString();
  }

  int totalChanges = 0;
  QMap<QString, FeatureModifications> modifiedFeaturesByLayerId = mRedoHistory.last();
  QStringList layerIds;
  bool hasCreatedFeatures = false;
  bool hasUpdatedFeatures = false;
  bool hasDeletedFeatures = false;
  for ( const QString &layerId : modifiedFeaturesByLayerId.keys() )
  {
    if ( !layerIds.contains( layerId ) )
    {
      layerIds << layerId;
    }

    hasCreatedFeatures |= modifiedFeaturesByLayerId[layerId].deletedFeatures.count() > 0;
    hasUpdatedFeatures |= modifiedFeaturesByLayerId[layerId].updatedFeatures.count() > 0;
    hasDeletedFeatures |= modifiedFeaturesByLayerId[layerId].createdFeatures.count() > 0;
    // cppcheck-suppress useStlAlgorithm
    totalChanges += modifiedFeaturesByLayerId[layerId].deletedFeatures.count()
                    + modifiedFeaturesByLayerId[layerId].updatedFeatures.count()
                    + modifiedFeaturesByLayerId[layerId].createdFeatures.count();
  }

  if ( totalChanges > 0 )
  {
    QgsVectorLayer *vl = layerIds.size() == 1 ? qobject_cast<QgsVectorLayer *>( mProject->mapLayer( layerIds.first() ) ) : nullptr;
    if ( hasCreatedFeatures && !hasUpdatedFeatures && !hasDeletedFeatures )
    {
      return vl ? tr( "Redo creation of %n feature(s) on layer %1", "", totalChanges ).arg( vl->name() ) : tr( "Redo creation of %n feature(s)", "", totalChanges );
    }
    else if ( !hasCreatedFeatures && !hasUpdatedFeatures && hasDeletedFeatures )
    {
      return vl ? tr( "Redo deletion of %n feature(s) on layer %1", "", totalChanges ).arg( vl->name() ) : tr( "Redo deletion of %n feature(s)", "", totalChanges );
    }
    else
    {
      return vl ? tr( "Redo modifications on %n feature(s) on layer %1", "", totalChanges ).arg( vl->name() ) : tr( "Redo modifications on %n feature(s)", "", totalChanges );
    }
  }

  return QString();
}
```


