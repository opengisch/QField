/***************************************************************************
                            featurelistmodelbase.cpp
                              -------------------
              begin                : 10.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qgsvectorlayer.h>
#include <qgsvectordataprovider.h>
#include <qgsproject.h>
#include <qgsgeometry.h>
#include <qgscoordinatereferencesystem.h>
#include <qgsrelationmanager.h>
#include <qgsmessagelog.h>

#include "multifeaturelistmodel.h"
#include "multifeaturelistmodelbase.h"
#include "featureutils.h"

#include <QDebug>

MultiFeatureListModelBase::MultiFeatureListModelBase( QObject *parent )
  :  QAbstractItemModel( parent )
{
  connect( this, &MultiFeatureListModelBase::modelReset, this, &MultiFeatureListModelBase::countChanged );
}

void MultiFeatureListModelBase::setFeatures( const QMap< QgsVectorLayer *, QgsFeatureRequest> requests )
{
  beginResetModel();

  mFeatures.clear();

  QMap<QgsVectorLayer *, QgsFeatureRequest>::ConstIterator it;
  for ( it = requests.constBegin(); it != requests.constEnd(); it++ )
  {
    QgsVectorLayer *vl = it.key();

    if ( ! vl || ! vl->isValid() )
      continue;

    QgsFeature feat;
    QgsFeatureIterator fit = vl->getFeatures( it.value() );
    while ( fit.nextFeature( feat ) )
    {
      mFeatures.append( QPair< QgsVectorLayer *, QgsFeature >( vl, feat ) );
      connect( vl, &QgsVectorLayer::destroyed, this, &MultiFeatureListModelBase::layerDeleted, Qt::UniqueConnection );
      connect( vl, &QgsVectorLayer::featureDeleted, this, &MultiFeatureListModelBase::featureDeleted, Qt::UniqueConnection );
      connect( vl, &QgsVectorLayer::attributeValueChanged, this, &MultiFeatureListModelBase::attributeValueChanged, Qt::UniqueConnection );
      connect( vl, &QgsVectorLayer::geometryChanged, this, &MultiFeatureListModelBase::geometryChanged, Qt::UniqueConnection );
    }
  }

  endResetModel();
}

void MultiFeatureListModelBase::appendFeatures( const QList<IdentifyTool::IdentifyResult> &results )
{
  beginInsertRows( QModelIndex(), mFeatures.count(), mFeatures.count() + results.count() - 1 );

  for ( const IdentifyTool::IdentifyResult &result : results )
  {
    QgsVectorLayer *layer = qobject_cast<QgsVectorLayer *>( result.layer );
    QPair<QgsVectorLayer *, QgsFeature> item( layer, result.feature );
    if ( !mFeatures.contains( item ) )
    {
      mFeatures.append( item );
      connect( layer, &QObject::destroyed, this, &MultiFeatureListModelBase::layerDeleted, Qt::UniqueConnection );
      connect( layer, &QgsVectorLayer::featureDeleted, this, &MultiFeatureListModelBase::featureDeleted, Qt::UniqueConnection );
      connect( layer, &QgsVectorLayer::attributeValueChanged, this, &MultiFeatureListModelBase::attributeValueChanged, Qt::UniqueConnection );
      connect( layer, &QgsVectorLayer::geometryChanged, this, &MultiFeatureListModelBase::geometryChanged, Qt::UniqueConnection );

      if ( !mSelectedFeatures.isEmpty() )
      {
        mSelectedFeatures.append( item );
      }
    }
    else if ( mSelectedFeatures.size() > 1 && mSelectedFeatures.contains( item ) )
    {
      int row = mFeatures.indexOf( item );
      mSelectedFeatures.removeAll( item );

      QModelIndex index = createIndex( row, 0 );
      emit dataChanged( index, index, QVector<int>() << MultiFeatureListModel::FeatureSelectedRole );
    }
  }
  endInsertRows();

  if ( !mSelectedFeatures.isEmpty() )
  {
    emit selectedCountChanged();
  }
}

void MultiFeatureListModelBase::clear( const bool keepSelected )
{
  // the model is already empty, no need to trigger "resetModel"
  if ( mFeatures.isEmpty() )
    return;

  beginResetModel();
  mFeatures.clear();
  if ( keepSelected )
  {
    mFeatures = mSelectedFeatures;
  }
  else
  {
    mSelectedFeatures.clear();
  }
  endResetModel();
}

void MultiFeatureListModelBase::clearSelection()
{
  if ( mSelectedFeatures.isEmpty() )
  {
    return;
  }

  mSelectedFeatures.clear();
  emit dataChanged( index( 0, 0 ), index( rowCount( QModelIndex() ) - 1, 0 ), QVector<int>() << MultiFeatureListModel::FeatureSelectedRole );
  emit selectedCountChanged();
}

void MultiFeatureListModelBase::toggleSelectedItem( int item )
{
  if ( !mSelectedFeatures.contains( mFeatures.at( item ) ) )
  {
    mSelectedFeatures << mFeatures.at( item );
  }
  else
  {
    mSelectedFeatures.removeAll( mFeatures.at( item ) );
  }

  QModelIndex modifiedIndex = index( item, 0 );
  emit dataChanged( modifiedIndex, modifiedIndex, QVector<int>() << MultiFeatureListModel::FeatureSelectedRole );
  emit selectedCountChanged();
}

QList<QgsFeature> MultiFeatureListModelBase::selectedFeatures()
{
  QList<QgsFeature> features;
  for ( const QPair< QgsVectorLayer *, QgsFeature > &pair : mSelectedFeatures )
  {
    features << pair.second;
  }
  return features;
}

QHash<int, QByteArray> MultiFeatureListModelBase::roleNames() const
{
  QHash<int, QByteArray> roleNames;

  roleNames[Qt::DisplayRole] = "display";
  roleNames[MultiFeatureListModel::FeatureIdRole] = "featureId";
  roleNames[MultiFeatureListModel::FeatureSelectedRole] = "featureSelected";
  roleNames[MultiFeatureListModel::FeatureRole] = "feature";
  roleNames[MultiFeatureListModel::LayerNameRole] = "layerName";
  roleNames[MultiFeatureListModel::LayerRole] = "currentLayer";
  roleNames[MultiFeatureListModel::GeometryRole] = "geometry";
  roleNames[MultiFeatureListModel::CrsRole] = "crs";
  roleNames[MultiFeatureListModel::DeleteFeatureRole] = "deleteFeatureCapability";
  roleNames[MultiFeatureListModel::EditGeometryRole] = "editGeometryCapability";

  return roleNames;
}

QModelIndex MultiFeatureListModelBase::index( int row, int column, const QModelIndex &parent ) const
{
  Q_UNUSED( parent )

  if ( row < 0 || row >= mFeatures.size() || column != 0 )
    return QModelIndex();

  return createIndex( row, column, const_cast<QPair< QgsVectorLayer *, QgsFeature >*>( &mFeatures.at( row ) ) );
}

QModelIndex MultiFeatureListModelBase::parent( const QModelIndex &child ) const
{
  Q_UNUSED( child )
  return QModelIndex();
}

int MultiFeatureListModelBase::rowCount( const QModelIndex &parent ) const
{
  if ( parent.isValid() )
    return 0;
  else
    return mFeatures.count();
}

int MultiFeatureListModelBase::columnCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent )
  return 1;
}

QVariant MultiFeatureListModelBase::data( const QModelIndex &index, int role ) const
{
  QPair< QgsVectorLayer *, QgsFeature > *feature = toFeature( index );
  if ( !feature )
    return QVariant();

  switch ( role )
  {
    case MultiFeatureListModel::FeatureIdRole:
      return feature->second.id();

    case MultiFeatureListModel::FeatureSelectedRole:
      return mSelectedFeatures.contains( mFeatures.at( index.row() ) );

    case MultiFeatureListModel::FeatureRole:
      return feature->second;

    case Qt::DisplayRole:
    {
      return FeatureUtils::displayName( feature->first, feature->second );
    }

    case MultiFeatureListModel::LayerNameRole:
      return feature->first->name();

    case MultiFeatureListModel::LayerRole:
      return QVariant::fromValue<QgsVectorLayer *>( feature->first );

    case MultiFeatureListModel::GeometryRole:
      return QVariant::fromValue<QgsGeometry>( feature->second.geometry() );

    case MultiFeatureListModel::CrsRole:
      return QVariant::fromValue<QgsCoordinateReferenceSystem>( feature->first->crs() );

    case MultiFeatureListModel::DeleteFeatureRole:
      return ! feature->first->readOnly()
             && ( feature->first->dataProvider()->capabilities() & QgsVectorDataProvider::DeleteFeatures )
             && ! feature->first->customProperty( QStringLiteral( "QFieldSync/is_geometry_locked" ), false ).toBool();

    case MultiFeatureListModel::EditGeometryRole:
      return ! feature->first->readOnly()
             && ( feature->first->dataProvider()->capabilities() & QgsVectorDataProvider::ChangeGeometries )
             && ! feature->first->customProperty( QStringLiteral( "QFieldSync/is_geometry_locked" ), false ).toBool();
  }

  return QVariant();
}

bool MultiFeatureListModelBase::removeRows( int row, int count, const QModelIndex &parent = QModelIndex() )
{
  if ( !count )
    return true;

  int i = 0;
  QMutableListIterator<QPair< QgsVectorLayer *, QgsFeature >> it( mFeatures );
  while ( i < row )
  {
    it.next();
    i++;
  }

  int last = row + count - 1;

  beginRemoveRows( parent, row, last );
  while ( i <= last )
  {
    it.next();
    it.remove();
    i++;
  }
  endRemoveRows();
  emit countChanged();

  return true;
}

int MultiFeatureListModelBase::count() const
{
  return mFeatures.size();
}

int MultiFeatureListModelBase::selectedCount() const
{
  return mSelectedFeatures.size();
}

bool MultiFeatureListModelBase::canEditAttributesSelection()
{
  if ( mSelectedFeatures.isEmpty() )
    return false;

  QgsVectorLayer *vlayer = mSelectedFeatures[0].first;
  return !vlayer->readOnly() &&
         ( vlayer->dataProvider()->capabilities() & QgsVectorDataProvider::ChangeAttributeValues );
}

bool MultiFeatureListModelBase::canMergeSelection()
{
  if ( mSelectedFeatures.isEmpty()  )
    return false;

  QgsVectorLayer *vlayer = mSelectedFeatures[0].first;
  return !vlayer->readOnly() && QgsWkbTypes::isMultiType( vlayer->wkbType() ) &&
         ( vlayer->dataProvider()->capabilities() & QgsVectorDataProvider::DeleteFeatures ) &&
         ( vlayer->dataProvider()->capabilities() & QgsVectorDataProvider::ChangeGeometries ) &&
         !vlayer->customProperty( QStringLiteral( "QFieldSync/is_geometry_locked" ), false ).toBool();
}

bool MultiFeatureListModelBase::canDeleteSelection()
{
  if ( mSelectedFeatures.isEmpty() )
    return false;

  QgsVectorLayer *vlayer = mSelectedFeatures[0].first;
  return !vlayer->readOnly() &&
         ( vlayer->dataProvider()->capabilities() & QgsVectorDataProvider::DeleteFeatures ) &&
         !vlayer->customProperty( QStringLiteral( "QFieldSync/is_geometry_locked" ), false ).toBool();
}

bool MultiFeatureListModelBase::mergeSelection()
{
  if ( !canMergeSelection() )
    return false;

  QList< QPair< QgsVectorLayer *, QgsFeature > > selectedFeatures = mSelectedFeatures;
  QgsVectorLayer *vlayer = selectedFeatures[0].first;
  bool isSuccess = true;
  QgsGeometry combinedGeometry;
  for ( const auto &pair : selectedFeatures )
  {
    if ( combinedGeometry.isEmpty() )
    {
      combinedGeometry = pair.second.geometry();
      continue;
    }

    QgsGeometry geom = combinedGeometry.combine( pair.second.geometry() );
    if ( combinedGeometry.lastError().isEmpty() )
    {
      combinedGeometry = geom;
    }
    else
    {
      isSuccess = false;
      break;
    }
  }

  if ( isSuccess )
  {
    if ( ! vlayer->startEditing() )
    {
      QgsMessageLog::logMessage( tr( "Cannot start editing" ), "QField", Qgis::Warning );
      return false;
    }

    QgsFeature mergedFeature = selectedFeatures[0].second;
    mergedFeature.setGeometry( combinedGeometry );
    isSuccess = vlayer->updateFeature( mergedFeature );

    if ( isSuccess )
    {
      selectedFeatures.removeFirst();
      for ( const auto &pair : qgis::as_const( selectedFeatures ) )
      {
        isSuccess = deleteFeature( pair.first, pair.second.id(), true );
        if ( !isSuccess )
        {
          break;
        }
      }
    }

    if ( isSuccess )
    {
      // commit changes
      isSuccess = vlayer->commitChanges();
    }

    if ( !isSuccess )
    {
      if ( !vlayer->rollBack() )
        QgsMessageLog::logMessage( tr( "Cannot rollback layer changes in layer %1" ).arg( vlayer->name() ), "QField", Qgis::Critical );
    }
  }

  mSelectedFeatures.clear();
  emit selectedCountChanged();

  return isSuccess;
}

bool MultiFeatureListModelBase::deleteFeature( QgsVectorLayer *layer, QgsFeatureId fid, bool selectionAction )
{
  if ( !layer )
  {
    QgsMessageLog::logMessage( tr( "Cannot start editing, no layer" ), "QField", Qgis::Warning );
    return false;
  }

  if ( !selectionAction )
  {
    if ( ! layer->startEditing() )
    {
      QgsMessageLog::logMessage( tr( "Cannot start editing" ), "QField", Qgis::Warning );
      return false;
    }
  }

  //delete child features in case of compositions
  const QList<QgsRelation> referencingRelations = QgsProject::instance()->relationManager()->referencedRelations( layer );
  QList<QgsVectorLayer *> childLayersEdited;
  bool isSuccess = true;
  for ( const QgsRelation &referencingRelation : referencingRelations )
  {
    if ( referencingRelation.strength() == QgsRelation::Composition )
    {
      QgsVectorLayer *childLayer = referencingRelation.referencingLayer();

      if ( childLayer->startEditing() )
      {
        QgsFeatureIterator relatedFeaturesIt = referencingRelation.getRelatedFeatures( layer->getFeature( fid ) );
        QgsFeature childFeature;
        while ( relatedFeaturesIt.nextFeature( childFeature ) )
        {
          if ( ! childLayer->deleteFeature( childFeature.id() ) )
          {
            QgsMessageLog::logMessage( tr( "Cannot delete feature from child layer" ), "QField", Qgis::Critical );
            isSuccess = false;
          }
        }
      }
      else
      {
        QgsMessageLog::logMessage( tr( "Cannot start editing child layer" ), "QField", Qgis::Critical );
        isSuccess = false;
        break;
      }

      if ( isSuccess )
        childLayersEdited.append( childLayer );
      else
        break;
    }
  }

  // we need to either commit or rollback the child layers that have experienced any modification
  for ( QgsVectorLayer *childLayer : qgis::as_const( childLayersEdited ) )
  {
    // if everything went well so far, we try to commit
    if ( isSuccess )
    {
      if ( ! childLayer->commitChanges() )
      {
        QgsMessageLog::logMessage( tr( "Cannot commit layer changes in layer %1." ).arg( childLayer->name() ), "QField", Qgis::Critical );
        isSuccess = false;
      }
    }

    // if the commit failed, we try to rollback the changes and the rest of the modified layers (parent and children) will be rollbacked
    if ( ! isSuccess )
    {
      if ( ! childLayer->rollBack() )
        QgsMessageLog::logMessage( tr( "Cannot rollback layer changes in layer %1" ).arg( childLayer->name() ), "QField", Qgis::Critical );
    }
  }

  if ( isSuccess )
  {
    //delete parent
    if ( layer->deleteFeature( fid ) )
    {
      if ( !selectionAction )
      {
        // commit changes
        if ( ! layer->commitChanges() )
          isSuccess = false;
      }
    }
    else
    {
      QgsMessageLog::logMessage( tr( "Cannot delete feature %1" ).arg( fid ), "QField", Qgis::Warning );

      isSuccess = false;
    }
  }

  if ( !selectionAction )
  {
    if ( ! isSuccess )
    {
      if ( ! layer->rollBack() )
        QgsMessageLog::logMessage( tr( "Cannot rollback layer changes in layer %1" ).arg( layer->name() ), "QField", Qgis::Critical );
    }
  }

  return isSuccess;
}

bool MultiFeatureListModelBase::deleteSelection()
{
  if ( !canDeleteSelection() )
    return false;

  QgsVectorLayer *vlayer = mSelectedFeatures[0].first;
  if ( !vlayer->startEditing() )
  {
    QgsMessageLog::logMessage( tr( "Cannot start editing" ), "QField", Qgis::Warning );
    return false;
  }

  const QList< QPair< QgsVectorLayer *, QgsFeature > > selectedFeatures = mSelectedFeatures;
  bool isSuccess = false;
  for ( const auto &pair : selectedFeatures )
  {
    isSuccess = deleteFeature( pair.first, pair.second.id(), true );
    if ( !isSuccess )
      break;
  }

  if ( isSuccess )
  {
    // commit changes
    isSuccess = vlayer->commitChanges();
  }

  if ( !isSuccess )
  {
    if ( !vlayer->rollBack() )
      QgsMessageLog::logMessage( tr( "Cannot rollback layer changes in layer %1" ).arg( vlayer->name() ), "QField", Qgis::Critical );
  }

  return isSuccess;
}

void MultiFeatureListModelBase::layerDeleted( QObject *object )
{
  int firstRowToRemove = -1;
  int count = 0;
  int currentRow = 0;

  /*
   * Features on the same layer are always subsequent.
   * We therefore can search for the first feature and
   * count all subsequent ones.
   * Once there is a feature of a different layer found
   * we can stop searching.
   */
  for ( auto it = mFeatures.constBegin(); it != mFeatures.constEnd(); it++ )
  {
    if ( it->first == object )
    {
      if ( firstRowToRemove == -1 )
        firstRowToRemove = currentRow;

      count++;
    }
    else if ( firstRowToRemove != -1 )
    {
      break;
    }
    currentRow++;
  }

  removeRows( firstRowToRemove, count );

  mSelectedFeatures.clear();
  emit selectedCountChanged();
}

void MultiFeatureListModelBase::featureDeleted( QgsFeatureId fid )
{
  QgsVectorLayer *l = qobject_cast<QgsVectorLayer *>( sender() );
  Q_ASSERT( l );

  mSelectedFeatures.clear();
  emit selectedCountChanged();

  int i = 0;
  for ( auto it = mFeatures.constBegin(); it != mFeatures.constEnd(); it++ )
  {
    if ( it->first == l && it->second.id() == fid )
    {
      removeRows( i, 1 );
      break;
    }
    ++i;
  }
}

void MultiFeatureListModelBase::attributeValueChanged( QgsFeatureId fid, int idx, const QVariant &value )
{
  QgsVectorLayer *l = qobject_cast<QgsVectorLayer *>( sender() );
  Q_ASSERT( l );

  int i = 0;
  for ( auto &pair : mFeatures )
  {
    if ( pair.first == l && pair.second.id() == fid )
    {
      pair.second.setAttribute( idx, value );
      break;
    }
    i++;
  }

  QModelIndex indexChanged = createIndex( i, 1 );
  emit dataChanged( indexChanged, indexChanged );

  for ( auto &pair : mSelectedFeatures )
  {
    if ( pair.first == l && pair.second.id() == fid )
    {
      pair.second.setAttribute( idx, value );
      break;
    }
  }
}

void MultiFeatureListModelBase::geometryChanged( QgsFeatureId fid, const QgsGeometry &geometry )
{
  QgsVectorLayer *l = qobject_cast<QgsVectorLayer *>( sender() );
  Q_ASSERT( l );

  int i = 0;
  for ( auto &pair : mFeatures )
  {
    if ( pair.first == l && pair.second.id() == fid )
    {
      pair.second.setGeometry( geometry );
      break;
    }
    i++;
  }

  QModelIndex indexChanged = createIndex( i, 1 );
  emit dataChanged( indexChanged, indexChanged, QVector<int>() << MultiFeatureListModel::GeometryRole << MultiFeatureListModel::FeatureSelectedRole );

  for ( auto &pair : mSelectedFeatures )
  {
    if ( pair.first == l && pair.second.id() == fid )
    {
      pair.second.setGeometry( geometry );
      break;
    }
  }
}
