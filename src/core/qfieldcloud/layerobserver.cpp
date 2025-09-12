/***************************************************************************
                        layerobserver.cpp
                        -----------------
  begin                : Apr 2020
  copyright            : (C) 2020 by Ivan Ivanov
  email                : ivan@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                     *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or   *
 *   (at your option) any later version.                   *
 *                                     *
 ***************************************************************************/

#include "layerobserver.h"
#include "qfieldcloudutils.h"

#include <QDebug>
#include <QDir>
#include <qgsfeature.h>
#include <qgsfeatureiterator.h>
#include <qgsfeaturerequest.h>
#include <qgsmessagelog.h>
#include <qgsvectorlayereditbuffer.h>

#include <appinterface.h>


LayerObserver::LayerObserver( const QgsProject *project )
  : mProject( project )
{
  QString dirPath = QFileInfo( mProject->absoluteFilePath() ).path();
  mDeltaFileWrapper = std::make_unique<DeltaFileWrapper>( mProject, QStringLiteral( "%1/deltafile.json" ).arg( dirPath ) );

  connect( mProject, &QgsProject::homePathChanged, this, &LayerObserver::onHomePathChanged );
  connect( mProject, &QgsProject::layersAdded, this, &LayerObserver::onLayersAdded );
}


void LayerObserver::reset( bool isHardReset ) const
{
  if ( isHardReset )
    mDeltaFileWrapper->resetId();

  return mDeltaFileWrapper->reset();
}


DeltaFileWrapper *LayerObserver::deltaFileWrapper() const
{
  return mDeltaFileWrapper.get();
}

void LayerObserver::onHomePathChanged()
{
  if ( mProject->homePath().isNull() )
    return;

  Q_ASSERT( mDeltaFileWrapper->hasError() || !mDeltaFileWrapper->isDirty() );

  QString dirPath = QFileInfo( mProject->absoluteFilePath() ).path();
  mDeltaFileWrapper = std::unique_ptr<DeltaFileWrapper>( new DeltaFileWrapper( mProject, QStringLiteral( "%1/deltafile.json" ).arg( dirPath ) ) );
  emit deltaFileWrapperChanged();

  mObservedLayerIds.clear();

  if ( !QFieldCloudUtils::getProjectId( mProject->fileName() ).isEmpty() )
  {
    if ( mDeltaFileWrapper->hasError() )
      QgsMessageLog::logMessage( QStringLiteral( "The current delta file wrapper experienced an error: %1" ).arg( mDeltaFileWrapper->errorString() ) );

    addLayerListeners();
  }
}

void LayerObserver::onLayersAdded( const QList<QgsMapLayer *> &layers )
{
  Q_UNUSED( layers );
  addLayerListeners();
}


void LayerObserver::onBeforeCommitChanges()
{
  QgsVectorLayer *vl = qobject_cast<QgsVectorLayer *>( sender() );
  QgsVectorLayerEditBuffer *eb = vl->editBuffer();

  if ( !eb )
    return;

  const QgsFeatureIds deletedFids = eb->deletedFeatureIds();
  const QgsFeatureIds changedGeometriesFids = qgis::listToSet( eb->changedGeometries().keys() );
  const QgsFeatureIds changedAttributesFids = qgis::listToSet( eb->changedAttributeValues().keys() );
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
  QgsChangedFeatures changedFeatures;
  QgsFeature f;

  // ? is it possible to use the iterator in a less ugly way? something like normal `for ( QgsFeature &f : it ) {}`
  while ( featuresIt.nextFeature( f ) )
    changedFeatures.insert( f.id(), f );

  qInfo() << "LayerObserver::onBeforeCommitChanges: vl->id()=" << vl->id() << "sourcePkAttrPair=" << changedFids;

  // NOTE no need to keep track of added features, as they are always present in the layer after commit
  mChangedFeatures.insert( vl->id(), changedFeatures );
  mPatchedFids.insert( vl->id(), QgsFeatureIds() );
}


void LayerObserver::onCommittedFeaturesAdded( const QString &localLayerId, const QgsFeatureList &addedFeatures )
{
  if ( mDeltaFileWrapper->isDeltaBeingApplied() )
    return;

  const QgsVectorLayer *vl = qobject_cast<QgsVectorLayer *>( sender() );
  const QString sourceLayerId = DeltaFileWrapper::getSourceLayerId( vl );
  const QPair<int, QString> localPkAttrPair = DeltaFileWrapper::getLocalPkAttribute( vl );
  const QPair<int, QString> sourcePkAttrPair = DeltaFileWrapper::getSourcePkAttribute( vl );

  qInfo() << "LayerObserver::onCommittedFeaturesAdded: sourcePkAttrPair=" << sourcePkAttrPair << " sourceLayerId=" << sourceLayerId;

  for ( const QgsFeature &newFeature : addedFeatures )
  {
    qInfo() << "  LayerObserver::onCommittedFeaturesAdded: adding create delta... FID=" << newFeature.id();

    mDeltaFileWrapper->addCreate( localLayerId, sourceLayerId, localPkAttrPair.second, sourcePkAttrPair.second, newFeature );
  }
}


void LayerObserver::onCommittedFeaturesRemoved( const QString &localLayerId, const QgsFeatureIds &deletedFeatureIds )
{
  if ( mDeltaFileWrapper->isDeltaBeingApplied() )
    return;

  QgsChangedFeatures changedFeatures = mChangedFeatures.value( localLayerId );
  const QgsVectorLayer *vl = qobject_cast<QgsVectorLayer *>( sender() );
  const QString sourceLayerId = DeltaFileWrapper::getSourceLayerId( vl );
  const QPair<int, QString> localPkAttrPair = DeltaFileWrapper::getLocalPkAttribute( vl );
  const QPair<int, QString> sourcePkAttrPair = DeltaFileWrapper::getSourcePkAttribute( vl );

  qInfo() << "LayerObserver::onCommittedFeaturesRemoved: sourcePkAttrPair=" << sourcePkAttrPair << " sourceLayerId=" << sourceLayerId << " deletedFeatureIds=" << deletedFeatureIds;

  for ( const QgsFeatureId &fid : deletedFeatureIds )
  {
    Q_ASSERT( changedFeatures.contains( fid ) );

    QgsFeature oldFeature = changedFeatures.take( fid );

    qInfo() << "  LayerObserver::onCommittedFeaturesRemoved: adding delete delta... FID=" << fid;

    mDeltaFileWrapper->addDelete( localLayerId, sourceLayerId, localPkAttrPair.second, sourcePkAttrPair.second, oldFeature );
  }

  mChangedFeatures.insert( localLayerId, changedFeatures );
}


void LayerObserver::onCommittedAttributeValuesChanges( const QString &localLayerId, const QgsChangedAttributesMap &changedAttributesValues )
{
  if ( mDeltaFileWrapper->isDeltaBeingApplied() )
    return;

  QgsVectorLayer *vl = qobject_cast<QgsVectorLayer *>( sender() );
  QgsFeatureIds patchedFids = mPatchedFids.value( localLayerId );
  QgsChangedFeatures changedFeatures = mChangedFeatures.value( localLayerId );
  const QgsFeatureIds changedAttributesValuesFids = qgis::listToSet( changedAttributesValues.keys() );
  const QString sourceLayerId = DeltaFileWrapper::getSourceLayerId( vl );
  const QPair<int, QString> localPkAttrPair = DeltaFileWrapper::getLocalPkAttribute( vl );
  const QPair<int, QString> sourcePkAttrPair = DeltaFileWrapper::getSourcePkAttribute( vl );

  qInfo() << "LayerObserver::onCommittedAttributeValuesChanges: sourcePkAttrPair=" << sourcePkAttrPair << " sourceLayerId=" << sourceLayerId << " changedAttributesValuesFids=" << changedAttributesValuesFids;

  for ( const QgsFeatureId fid : changedAttributesValuesFids )
  {
    if ( patchedFids.contains( fid ) )
      continue;

    Q_ASSERT( changedFeatures.contains( fid ) );

    patchedFids.insert( fid );

    QgsFeature oldFeature = changedFeatures.take( fid );
    QgsFeature newFeature = vl->getFeature( fid );

    qInfo() << "LayerObserver::onCommittedAttributeValuesChanges: adding patch delta... FID=" << fid;

    if ( vl->fields().indexOf( "fid_1" ) != -1 && localPkAttrPair.second == sourcePkAttrPair.second && newFeature.attribute( "fid" ) != newFeature.attribute( "fid_1" ) )
    {
      mLocalAndSourcePkAttrAreEqual = true;
    }
    mDeltaFileWrapper->addPatch( localLayerId, sourceLayerId, localPkAttrPair.second, sourcePkAttrPair.second, oldFeature, newFeature );
  }

  mPatchedFids.insert( localLayerId, patchedFids );
  mChangedFeatures.insert( localLayerId, changedFeatures );
}


void LayerObserver::onCommittedGeometriesChanges( const QString &localLayerId, const QgsGeometryMap &changedGeometries )
{
  if ( mDeltaFileWrapper->isDeltaBeingApplied() )
    return;

  QgsVectorLayer *vl = qobject_cast<QgsVectorLayer *>( sender() );
  QgsFeatureIds patchedFids = mPatchedFids.value( localLayerId );
  QgsChangedFeatures changedFeatures = mChangedFeatures.value( localLayerId );
  const QgsFeatureIds changedGeometriesFids = qgis::listToSet( changedGeometries.keys() );
  const QString sourceLayerId = DeltaFileWrapper::getSourceLayerId( vl );
  const QPair<int, QString> localPkAttrPair = DeltaFileWrapper::getLocalPkAttribute( vl );
  const QPair<int, QString> sourcePkAttrPair = DeltaFileWrapper::getSourcePkAttribute( vl );

  qInfo() << "LayerObserver::onCommittedGeometriesChanges: sourcePkAttrPair=" << sourcePkAttrPair << " sourceLayerId=" << sourceLayerId << " changedGeometriesFids=" << changedGeometriesFids;

  for ( const QgsFeatureId &fid : changedGeometriesFids )
  {
    if ( patchedFids.contains( fid ) )
      continue;

    Q_ASSERT( changedFeatures.contains( fid ) );

    patchedFids.insert( fid );

    QgsFeature oldFeature = changedFeatures.take( fid );
    QgsFeature newFeature = vl->getFeature( fid );

    qInfo() << "  LayerObserver::onCommittedGeometriesChanges: adding patch delta... FID=" << fid;

    if ( vl->fields().indexOf( "fid_1" ) != -1 && localPkAttrPair.second == sourcePkAttrPair.second && newFeature.attribute( "fid" ) != newFeature.attribute( "fid_1" ) )
    {
      mLocalAndSourcePkAttrAreEqual = true;
    }
    mDeltaFileWrapper->addPatch( localLayerId, sourceLayerId, localPkAttrPair.second, sourcePkAttrPair.second, oldFeature, newFeature );
  }

  mPatchedFids.insert( localLayerId, patchedFids );
  mChangedFeatures.insert( localLayerId, changedFeatures );
}


void LayerObserver::onEditingStopped()
{
  const QgsVectorLayer *vl = qobject_cast<QgsVectorLayer *>( sender() );
  const QString layerId = vl->id();

  mPatchedFids.take( layerId );
  mChangedFeatures.take( layerId );

  if ( !mDeltaFileWrapper->toFile() )
  {
    // TODO somehow indicate the user that writing failed
    QgsMessageLog::logMessage( QStringLiteral( "Failed writing JSON file" ) );
  }

  if ( vl->source().contains( QStringLiteral( "data.gpkg" ) ) && mLocalAndSourcePkAttrAreEqual )
  {
    AppInterface::instance()->sendLog( QStringLiteral( "Called LayerObserver::onEditingStopped!" ), QString() );
    mLocalAndSourcePkAttrAreEqual = false;
  }

  emit layerEdited( layerId );
}


void LayerObserver::addLayerListeners()
{
  const QList<QgsMapLayer *> layers = mProject->mapLayers().values();

  // we should keep track only of the layers on cloud projects
  if ( QFieldCloudUtils::getProjectId( mProject->fileName() ).isEmpty() )
    return;

  for ( QgsMapLayer *layer : layers )
  {
    QgsVectorLayer *vl = qobject_cast<QgsVectorLayer *>( layer );

    if ( vl )
    {
      if ( mObservedLayerIds.contains( vl->id() ) )
        continue;

      if ( !vl->readOnly() && QFieldCloudUtils::isCloudAction( vl ) )
      {
        // Ignore all layers that cannot determine a primary key column
        if ( DeltaFileWrapper::getLocalPkAttribute( vl ).first == -1 )
        {
          QgsMessageLog::logMessage( tr( "Failed to find a local primary key column in layer \"%1\"" ).arg( layer->name() ) );
          continue;
        }

        if ( DeltaFileWrapper::getSourcePkAttribute( vl ).first == -1 )
        {
          QgsMessageLog::logMessage( tr( "Failed to find a source primary key column in layer \"%1\"" ).arg( layer->name() ) );
          continue;
        }

        qInfo() << "LayerObserver::addLayerListeners: vl->getLocalPkAttribute()=" << DeltaFileWrapper::getLocalPkAttribute( vl );
        qInfo() << "LayerObserver::addLayerListeners: vl->getSourcePkAttribute()=" << DeltaFileWrapper::getSourcePkAttribute( vl );
        qInfo() << "LayerObserver::addLayerListeners: vl->customProperties()=" << vl->customProperties().keys();

        disconnect( vl, &QgsVectorLayer::beforeCommitChanges, this, &LayerObserver::onBeforeCommitChanges );
        disconnect( vl, &QgsVectorLayer::committedFeaturesAdded, this, &LayerObserver::onCommittedFeaturesAdded );
        disconnect( vl, &QgsVectorLayer::committedFeaturesRemoved, this, &LayerObserver::onCommittedFeaturesRemoved );
        disconnect( vl, &QgsVectorLayer::committedAttributeValuesChanges, this, &LayerObserver::onCommittedAttributeValuesChanges );
        disconnect( vl, &QgsVectorLayer::committedGeometriesChanges, this, &LayerObserver::onCommittedGeometriesChanges );
        // TODO use the future "afterCommitChanges" signal
        disconnect( vl, &QgsVectorLayer::editingStopped, this, &LayerObserver::onEditingStopped );

        // for `cloud` projects, we keep track of any change that has occurred
        connect( vl, &QgsVectorLayer::beforeCommitChanges, this, &LayerObserver::onBeforeCommitChanges );
        connect( vl, &QgsVectorLayer::committedFeaturesAdded, this, &LayerObserver::onCommittedFeaturesAdded );
        connect( vl, &QgsVectorLayer::committedFeaturesRemoved, this, &LayerObserver::onCommittedFeaturesRemoved );
        connect( vl, &QgsVectorLayer::committedAttributeValuesChanges, this, &LayerObserver::onCommittedAttributeValuesChanges );
        connect( vl, &QgsVectorLayer::committedGeometriesChanges, this, &LayerObserver::onCommittedGeometriesChanges );
        // TODO use the future "afterCommitChanges" signal
        connect( vl, &QgsVectorLayer::editingStopped, this, &LayerObserver::onEditingStopped );

        mObservedLayerIds.insert( vl->id() );
      }
    }
  }
}
