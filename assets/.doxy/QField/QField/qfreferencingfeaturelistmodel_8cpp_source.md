

# File qfreferencingfeaturelistmodel.cpp

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qfreferencingfeaturelistmodel.cpp**](qfreferencingfeaturelistmodel_8cpp.md)

[Go to the documentation of this file](qfreferencingfeaturelistmodel_8cpp.md)


```C++
/***************************************************************************
  qfreferencingfeaturelistmodel.cpp - QfReferencingFeatureListModel

 ---------------------
 begin                : 1.3.2019
 copyright            : (C) 2019 by David Signer
 email                : david (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qflayerutils.h"
#include "qfreferencingfeaturelistmodel.h"

#include <QTimer>
#include <qgsmessagelog.h>
#include <qgsproject.h>

QfReferencingFeatureListModelBase::QfReferencingFeatureListModelBase( QObject *parent )
  : QAbstractItemModel( parent )
{
}

QHash<int, QByteArray> QfReferencingFeatureListModelBase::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();

  roles[DisplayString] = "displayString";
  roles[ReferencingFeature] = "referencingFeature";
  roles[NmReferencedFeature] = "nmReferencedFeature";
  roles[NmDisplayString] = "nmDisplayString";
  roles[AttachmentPath] = "attachmentPath";

  return roles;
}

QModelIndex QfReferencingFeatureListModelBase::index( int row, int column, const QModelIndex &parent ) const
{
  Q_UNUSED( column )
  Q_UNUSED( parent )

  return createIndex( row, column, 1000 );
}

QModelIndex QfReferencingFeatureListModelBase::parent( const QModelIndex &index ) const
{
  Q_UNUSED( index )

  return QModelIndex();
}

int QfReferencingFeatureListModelBase::rowCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent )
  return static_cast<int>( mEntries.size() );
}

int QfReferencingFeatureListModelBase::columnCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent )
  return 1;
}

QVariant QfReferencingFeatureListModelBase::data( const QModelIndex &index, int role ) const
{
  if ( role == DisplayString )
  {
    return mEntries.value( index.row() ).displayString;
  }
  if ( role == ReferencingFeature )
  {
    return mEntries.value( index.row() ).referencingFeature;
  }
  if ( role == NmReferencedFeature )
  {
    return mEntries.value( index.row() ).nmReferencedFeature;
  }
  if ( role == NmDisplayString )
  {
    return mEntries.value( index.row() ).nmDisplayString;
  }
  if ( role == AttachmentPath )
  {
    if ( !mAttachmentFieldName.isEmpty() )
    {
      return mEntries.value( index.row() ).referencingFeature.attribute( mAttachmentFieldIndex ).toString();
    }
    return QString();
  }
  return QVariant();
}

void QfReferencingFeatureListModelBase::setFeature( const QgsFeature &feature )
{
  if ( mFeature == feature )
  {
    return;
  }

  mFeature = feature;
  emit featureChanged();

  if ( mRelation.isValid() && mFeature.isValid() && !mLastGathererFeaturesFilter.isEmpty() )
  {
    // The updated feature did not change the related features filter, skip reload
    if ( mLastGathererFeaturesFilter == mRelation.getRelatedFeaturesFilter( mFeature ) )
    {
      return;
    }
  }

  reload();
}

QgsFeature QfReferencingFeatureListModelBase::feature() const
{
  return mFeature;
}

void QfReferencingFeatureListModelBase::setRelation( const QgsRelation &relation )
{
  if ( mRelation.isValid() )
  {
    // Parent layer
    disconnect( mRelation.referencedLayer(), &QgsVectorLayer::committedAttributeValuesChanges, this, &QfReferencingFeatureListModelBase::layerCommittedAttributeValuesChanges );
    disconnect( mRelation.referencedLayer(), &QgsVectorLayer::committedGeometriesChanges, this, &QfReferencingFeatureListModelBase::layerCommittedGeometriesChanges );

    // Child layer
    disconnect( mRelation.referencingLayer(), &QgsVectorLayer::committedAttributeValuesChanges, this, &QfReferencingFeatureListModelBase::layerCommittedAttributeValuesChanges );
    disconnect( mRelation.referencingLayer(), &QgsVectorLayer::committedGeometriesChanges, this, &QfReferencingFeatureListModelBase::layerCommittedGeometriesChanges );
    disconnect( mRelation.referencingLayer(), &QgsVectorLayer::committedFeaturesAdded, this, &QfReferencingFeatureListModelBase::layerCommittedFeaturesAdded );
    disconnect( mRelation.referencingLayer(), &QgsVectorLayer::committedFeaturesRemoved, this, &QfReferencingFeatureListModelBase::layerCommittedFeaturesRemoved );
  }

  mRelation = relation;
  emit relationChanged();

  if ( mRelation.isValid() )
  {
    // Parent layer
    connect( mRelation.referencedLayer(), &QgsVectorLayer::committedAttributeValuesChanges, this, &QfReferencingFeatureListModelBase::layerCommittedAttributeValuesChanges );
    connect( mRelation.referencedLayer(), &QgsVectorLayer::committedGeometriesChanges, this, &QfReferencingFeatureListModelBase::layerCommittedGeometriesChanges );

    // Child layer
    connect( mRelation.referencingLayer(), &QgsVectorLayer::committedAttributeValuesChanges, this, &QfReferencingFeatureListModelBase::layerCommittedAttributeValuesChanges );
    connect( mRelation.referencingLayer(), &QgsVectorLayer::committedGeometriesChanges, this, &QfReferencingFeatureListModelBase::layerCommittedGeometriesChanges );
    connect( mRelation.referencingLayer(), &QgsVectorLayer::committedFeaturesAdded, this, &QfReferencingFeatureListModelBase::layerCommittedFeaturesAdded );
    connect( mRelation.referencingLayer(), &QgsVectorLayer::committedFeaturesRemoved, this, &QfReferencingFeatureListModelBase::layerCommittedFeaturesRemoved );
  }

  mLastGathererFeaturesFilter.clear();
  updateAttachmentFieldInfo();

  reload();
}

QgsRelation QfReferencingFeatureListModelBase::relation() const
{
  return mRelation;
}

QString QfReferencingFeatureListModelBase::currentRelationId() const
{
  return mRelation.isValid() ? mRelation.id() : QString();
}

void QfReferencingFeatureListModelBase::setCurrentRelationId( const QString &relationId )
{
  if ( relationId == currentRelationId() )
  {
    return;
  }

  setRelation( QgsProject::instance()->relationManager()->relation( relationId ) );
}

void QfReferencingFeatureListModelBase::setNmRelation( const QgsRelation &relation )
{
  mNmRelation = relation;
  emit nmRelationChanged();

  mLastGathererFeaturesFilter.clear();
  updateAttachmentFieldInfo();

  reload();
}

QgsRelation QfReferencingFeatureListModelBase::nmRelation() const
{
  return mNmRelation;
}

QString QfReferencingFeatureListModelBase::currentNmRelationId() const
{
  return mNmRelation.isValid() ? mNmRelation.id() : QString();
}

void QfReferencingFeatureListModelBase::setCurrentNmRelationId( const QString &nmRelationId )
{
  if ( nmRelationId == currentNmRelationId() )
  {
    return;
  }

  setNmRelation( QgsProject::instance()->relationManager()->relation( nmRelationId ) );
}

void QfReferencingFeatureListModelBase::setParentPrimariesAvailable( const bool parentPrimariesAvailable )
{
  mParentPrimariesAvailable = parentPrimariesAvailable;
}

bool QfReferencingFeatureListModelBase::parentPrimariesAvailable() const
{
  return mParentPrimariesAvailable;
}

void QfReferencingFeatureListModelBase::updateModel()
{
  beginResetModel();

  if ( mGatherer )
  {
    mEntries = mGatherer->entries();
  }

  emit beforeModelUpdated();
  endResetModel();
  emit modelUpdated();
}

void QfReferencingFeatureListModelBase::gathererThreadFinished()
{
  //ignore spooky signals from ancestor threads
  if ( sender() != mGatherer )
  {
    return;
  }

  mGatherer->deleteLater();
  mGatherer = nullptr;
  emit isLoadingChanged();
}

void QfReferencingFeatureListModelBase::layerCommittedAttributeValuesChanges( const QString &layerId, const QgsChangedAttributesMap &changedAttributesValues )
{
  const QList<QgsFeatureId> featureIds = changedAttributesValues.keys();
  if ( mRelation.referencingLayerId() == layerId )
  {
    if ( std::any_of( mEntries.begin(), mEntries.end(), [&featureIds]( const Entry &entry ) { return featureIds.contains( entry.referencingFeature.id() ); } ) )
    {
      QTimer::singleShot( 50, this, &QfReferencingFeatureListModelBase::reload );
    }
  }
  else if ( mRelation.referencedLayerId() == layerId )
  {
    if ( featureIds.contains( mFeature.id() ) )
    {
      QTimer::singleShot( 50, this, &QfReferencingFeatureListModelBase::reload );
    }
  }
}

void QfReferencingFeatureListModelBase::layerCommittedGeometriesChanges( const QString &layerId, const QgsGeometryMap &changedGeometries )
{
  const QList<QgsFeatureId> featureIds = changedGeometries.keys();
  if ( mRelation.referencingLayerId() == layerId )
  {
    if ( std::any_of( mEntries.begin(), mEntries.end(), [&featureIds]( const Entry &entry ) { return featureIds.contains( entry.referencingFeature.id() ); } ) )
    {
      QTimer::singleShot( 50, this, &QfReferencingFeatureListModelBase::reload );
    }
  }
  else if ( mRelation.referencedLayerId() == layerId )
  {
    if ( featureIds.contains( mFeature.id() ) )
    {
      QTimer::singleShot( 50, this, &QfReferencingFeatureListModelBase::reload );
    }
  }
}

void QfReferencingFeatureListModelBase::layerCommittedFeaturesAdded( const QString &layerId, const QgsFeatureList &addedFeatures )
{
  if ( mRelation.referencingLayerId() == layerId )
  {
    QTimer::singleShot( 50, this, &QfReferencingFeatureListModelBase::reload );
  }
}

void QfReferencingFeatureListModelBase::layerCommittedFeaturesRemoved( const QString &layerId, const QgsFeatureIds &deletedFeatureIds )
{
  if ( std::any_of( mEntries.begin(), mEntries.end(), [&deletedFeatureIds]( const Entry &entry ) { return deletedFeatureIds.contains( entry.referencingFeature.id() ); } ) )
  {
    QTimer::singleShot( 50, this, &QfReferencingFeatureListModelBase::reload );
  }
}

void QfReferencingFeatureListModelBase::reload()
{
  if ( !mRelation.isValid() || !mFeature.isValid() )
  {
    return;
  }

  if ( checkParentPrimaries() )
  {
    bool wasLoading = false;

    if ( mGatherer )
    {
      // Send the gatherer thread to the graveyard:
      //   forget about it, tell it to stop and delete when finished
      disconnect( mGatherer, &QfFeatureGatherer::collectedValues, this, &QfReferencingFeatureListModelBase::updateModel );
      disconnect( mGatherer, &QfFeatureGatherer::finished, this, &QfReferencingFeatureListModelBase::gathererThreadFinished );
      connect( mGatherer, &QfFeatureGatherer::finished, mGatherer, &QfFeatureGatherer::deleteLater );
      mGatherer->stop();
      wasLoading = true;
    }

    mGatherer = new QfFeatureGatherer( mFeature, mRelation, mNmRelation );
    mLastGathererFeaturesFilter = mRelation.getRelatedFeaturesFilter( mFeature );

    connect( mGatherer, &QfFeatureGatherer::collectedValues, this, &QfReferencingFeatureListModelBase::updateModel );
    connect( mGatherer, &QfFeatureGatherer::finished, this, &QfReferencingFeatureListModelBase::gathererThreadFinished );

    mGatherer->start();
    if ( !wasLoading )
    {
      emit isLoadingChanged();
    }
  }
  else
  {
    //clear model entries
    beginResetModel();
    mEntries.clear();
    endResetModel();
  }

  //set the property for parent primaries available status
  setParentPrimariesAvailable( checkParentPrimaries() );
}

bool QfReferencingFeatureListModelBase::deleteFeature( QgsFeatureId referencingFeatureId )
{
  QgsVectorLayer *referencingLayer = mRelation.referencingLayer();

  if ( !referencingLayer || !referencingLayer->isValid() )
  {
    QgsMessageLog::logMessage( tr( "Invalid referencing layer" ), "QField", Qgis::Critical );
    return false;
  }

  if ( !beforeDeleteFeature( referencingLayer, referencingFeatureId ) )
  {
    return false;
  }

  if ( !QfLayerUtils::deleteFeature( QgsProject::instance(), referencingLayer, referencingFeatureId, true ) )
  {
    QgsMessageLog::logMessage( tr( "Cannot delete feature" ), "QField", Qgis::Critical );

    return false;
  }

  return true;
}

int QfReferencingFeatureListModelBase::getFeatureIdRow( QgsFeatureId featureId )
{
  int row = 0;
  for ( const Entry &entry : mEntries )
  {
    if ( entry.referencingFeature.id() == featureId )
    {
      break;
    }
    row++;
  }

  return row < mEntries.size() ? row : -1;
}

bool QfReferencingFeatureListModelBase::isLoading() const
{
  return mGatherer;
}

QString QfReferencingFeatureListModelBase::attachmentFieldName() const
{
  return mAttachmentFieldName;
}

int QfReferencingFeatureListModelBase::attachmentDocumentViewer() const
{
  return mAttachmentDocumentViewer;
}

QString QfReferencingFeatureListModelBase::attachmentStorageType() const
{
  return mAttachmentStorageType;
}

QString QfReferencingFeatureListModelBase::attachmentStorageAuthConfigId() const
{
  return mAttachmentStorageAuthConfigId;
}

QString QfReferencingFeatureListModelBase::attachmentStorageUrl() const
{
  return mAttachmentStorageUrl;
}

void QfReferencingFeatureListModelBase::updateAttachmentFieldInfo()
{
  mAttachmentFieldName.clear();
  mAttachmentFieldIndex = -1;
  mAttachmentDocumentViewer = 0;
  mAttachmentStorageType.clear();
  mAttachmentStorageAuthConfigId.clear();
  mAttachmentStorageUrl.clear();

  QgsVectorLayer *layer = mRelation.referencingLayer();
  if ( layer )
  {
    for ( int i = 0; i < layer->fields().count(); i++ )
    {
      if ( layer->editorWidgetSetup( i ).type() == QLatin1String( "ExternalResource" ) )
      {
        const QVariantMap config = layer->editorWidgetSetup( i ).config();
        mAttachmentFieldName = layer->fields().at( i ).name();
        mAttachmentFieldIndex = i;
        mAttachmentDocumentViewer = config.value( QStringLiteral( "DocumentViewer" ), 0 ).toInt();
        mAttachmentStorageType = config.value( QStringLiteral( "StorageType" ) ).toString();
        mAttachmentStorageAuthConfigId = config.value( QStringLiteral( "StorageAuthConfigId" ) ).toString();
        mAttachmentStorageUrl = config.value( QStringLiteral( "StorageUrl" ) ).toString();
        if ( !mAttachmentStorageUrl.isEmpty() && !mAttachmentStorageUrl.endsWith( QLatin1Char( '/' ) ) )
        {
          mAttachmentStorageUrl.append( QLatin1Char( '/' ) );
        }
        break;
      }
    }
  }

  emit attachmentDetailsChanged();
}

bool QfReferencingFeatureListModelBase::checkParentPrimaries()
{
  if ( !mRelation.isValid() || !mFeature.isValid() )
  {
    return false;
  }

  const bool featureIsNew = std::numeric_limits<QgsFeatureId>::min() == mFeature.id();
  const auto fieldPairs = mRelation.fieldPairs();
  for ( QgsRelation::FieldPair fieldPair : fieldPairs )
  {
    if ( featureIsNew )
    {
      if ( mRelation.referencedLayer() && mRelation.referencedLayer()->dataProvider() )
      {
        if ( mFeature.attribute( fieldPair.second ) == mRelation.referencedLayer()->dataProvider()->defaultValueClause( mFeature.fieldNameIndex( fieldPair.second ) ) )
        {
          // Insure that the child feature layer's linked attribute supports NULL values,
          // used until we can replace with the provider-set value upon parent feature creation
          if ( mRelation.referencingLayer()->fields().field( fieldPair.first ).constraints().constraints() & QgsFieldConstraints::ConstraintNotNull )
          {
            return false;
          }
        }
      }
    }
    if ( mFeature.attribute( fieldPair.second ).isNull() )
    {
      return false;
    }
  }
  return true;
}

bool QfReferencingFeatureListModelBase::beforeDeleteFeature( QgsVectorLayer *referencingLayer, QgsFeatureId referencingFeatureId )
{
  Q_UNUSED( referencingLayer );
  Q_UNUSED( referencingFeatureId );
  return true;
}

QfReferencingFeatureListModel::QfReferencingFeatureListModel( QObject *parent )
  : QSortFilterProxyModel( parent )
  , mSourceModel( new QfReferencingFeatureListModelBase( parent ) )
{
  connect( mSourceModel, &QfReferencingFeatureListModelBase::isLoadingChanged, this, &QfReferencingFeatureListModel::isLoadingChanged );
  connect( mSourceModel, &QfReferencingFeatureListModelBase::featureChanged, this, &QfReferencingFeatureListModel::featureChanged );
  connect( mSourceModel, &QfReferencingFeatureListModelBase::relationChanged, this, &QfReferencingFeatureListModel::relationChanged );
  connect( mSourceModel, &QfReferencingFeatureListModelBase::nmRelationChanged, this, &QfReferencingFeatureListModel::nmRelationChanged );
  connect( mSourceModel, &QfReferencingFeatureListModelBase::parentPrimariesAvailableChanged, this, &QfReferencingFeatureListModel::parentPrimariesAvailableChanged );
  connect( mSourceModel, &QfReferencingFeatureListModelBase::beforeModelUpdated, this, &QfReferencingFeatureListModel::beforeModelUpdated );
  connect( mSourceModel, &QfReferencingFeatureListModelBase::modelUpdated, this, &QfReferencingFeatureListModel::modelUpdated );
  connect( mSourceModel, &QfReferencingFeatureListModelBase::attributeFormModelChanged, this, &QfReferencingFeatureListModel::attributeFormModelChanged );
  connect( mSourceModel, &QfReferencingFeatureListModelBase::attachmentDetailsChanged, this, &QfReferencingFeatureListModel::attachmentDetailsChanged );

  setSourceModel( mSourceModel );
  setSortRole( QfReferencingFeatureListModelBase::DisplayString );
  setDynamicSortFilter( true );
  sort( 0, mSortOrder );
}

QString QfReferencingFeatureListModel::currentRelationId() const
{
  return mSourceModel->currentRelationId();
}

void QfReferencingFeatureListModel::setCurrentRelationId( const QString &relationId )
{
  mSourceModel->setCurrentRelationId( relationId );
}

QString QfReferencingFeatureListModel::currentNmRelationId() const
{
  return mSourceModel->currentNmRelationId();
}

void QfReferencingFeatureListModel::setCurrentNmRelationId( const QString &nmRelationId )
{
  mSourceModel->setCurrentNmRelationId( nmRelationId );
}

void QfReferencingFeatureListModel::setFeature( const QgsFeature &feature )
{
  mSourceModel->setFeature( feature );
}

QgsFeature QfReferencingFeatureListModel::feature() const
{
  return mSourceModel->feature();
}

void QfReferencingFeatureListModel::setRelation( const QgsRelation &relation )
{
  mSourceModel->setRelation( relation );
}

QgsRelation QfReferencingFeatureListModel::relation() const
{
  return mSourceModel->relation();
}

void QfReferencingFeatureListModel::setNmRelation( const QgsRelation &relation )
{
  mSourceModel->setNmRelation( relation );
}

QgsRelation QfReferencingFeatureListModel::nmRelation() const
{
  return mSourceModel->nmRelation();
}

void QfReferencingFeatureListModel::setParentPrimariesAvailable( const bool parentPrimariesAvailable )
{
  mSourceModel->setParentPrimariesAvailable( parentPrimariesAvailable );
}

bool QfReferencingFeatureListModel::parentPrimariesAvailable() const
{
  return mSourceModel->parentPrimariesAvailable();
}

void QfReferencingFeatureListModel::reload()
{
  mSourceModel->reload();
}

bool QfReferencingFeatureListModel::deleteFeature( QgsFeatureId referencingFeatureId )
{
  return mSourceModel->deleteFeature( referencingFeatureId );
}

int QfReferencingFeatureListModel::getFeatureIdRow( QgsFeatureId featureId )
{
  const QModelIndex sourceIndex = mSourceModel->index( mSourceModel->getFeatureIdRow( featureId ), 0 );
  return mapFromSource( sourceIndex ).row();
}

bool QfReferencingFeatureListModel::isLoading() const
{
  return mSourceModel->isLoading();
}

QString QfReferencingFeatureListModel::attachmentFieldName() const
{
  return mSourceModel->attachmentFieldName();
}

int QfReferencingFeatureListModel::attachmentDocumentViewer() const
{
  return mSourceModel->attachmentDocumentViewer();
}

QString QfReferencingFeatureListModel::attachmentStorageType() const
{
  return mSourceModel->attachmentStorageType();
}

QString QfReferencingFeatureListModel::attachmentStorageAuthConfigId() const
{
  return mSourceModel->attachmentStorageAuthConfigId();
}

QString QfReferencingFeatureListModel::attachmentStorageUrl() const
{
  return mSourceModel->attachmentStorageUrl();
}

Qt::SortOrder QfReferencingFeatureListModel::sortOrder() const
{
  return mSortOrder;
}

void QfReferencingFeatureListModel::setSortOrder( Qt::SortOrder sortOrder )
{
  if ( mSortOrder == sortOrder )
  {
    return;
  }
  mSortOrder = sortOrder;
  emit sortOrderChanged();

  sort( 0, mSortOrder );
}

bool QfReferencingFeatureListModel::lessThan( const QModelIndex &left, const QModelIndex &right ) const
{
  return Qt::AscendingOrder ? left.row() > right.row() : left.row() < right.row();
}
```


