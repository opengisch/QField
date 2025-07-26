/***************************************************************************
  referencingfeaturelistmodel.cpp - ReferencingFeatureListModel

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

#include "referencingfeaturelistmodel.h"

#include <qgsmessagelog.h>
#include <qgsproject.h>

ReferencingFeatureListModel::ReferencingFeatureListModel( QObject *parent )
  : QAbstractItemModel( parent )
{
}

QHash<int, QByteArray> ReferencingFeatureListModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();

  roles[DisplayString] = "displayString";
  roles[ReferencingFeature] = "referencingFeature";
  roles[NmReferencedFeature] = "nmReferencedFeature";
  roles[NmDisplayString] = "nmDisplayString";

  return roles;
}

QModelIndex ReferencingFeatureListModel::index( int row, int column, const QModelIndex &parent ) const
{
  Q_UNUSED( column )
  Q_UNUSED( parent )

  return createIndex( row, column, 1000 );
}

QModelIndex ReferencingFeatureListModel::parent( const QModelIndex &index ) const
{
  Q_UNUSED( index )

  return QModelIndex();
}

int ReferencingFeatureListModel::rowCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent )
  return mEntries.size();
}

int ReferencingFeatureListModel::columnCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent )
  return 1;
}

QVariant ReferencingFeatureListModel::data( const QModelIndex &index, int role ) const
{
  if ( role == DisplayString )
    return mEntries.value( index.row() ).displayString;
  if ( role == ReferencingFeature )
    return mEntries.value( index.row() ).referencingFeature;
  if ( role == NmReferencedFeature )
    return mEntries.value( index.row() ).nmReferencedFeature;
  if ( role == NmDisplayString )
    return mEntries.value( index.row() ).nmDisplayString;
  return QVariant();
}

void ReferencingFeatureListModel::setFeature( const QgsFeature &feature )
{
  if ( mFeature == feature )
    return;

  mFeature = feature;
  reload();
}

QgsFeature ReferencingFeatureListModel::feature() const
{
  return mFeature;
}

void ReferencingFeatureListModel::setRelation( const QgsRelation &relation )
{
  mRelation = relation;
  reload();
}

QgsRelation ReferencingFeatureListModel::relation() const
{
  return mRelation;
}

QString ReferencingFeatureListModel::currentRelationId() const
{
  return mRelation.isValid() ? mRelation.id() : QString();
}

void ReferencingFeatureListModel::setCurrentRelationId( const QString &relationId )
{
  if ( relationId == currentRelationId() )
    return;


  mRelation = QgsProject::instance()->relationManager()->relation( relationId );
  reload();
}

void ReferencingFeatureListModel::setNmRelation( const QgsRelation &relation )
{
  mNmRelation = relation;
  reload();
}

QgsRelation ReferencingFeatureListModel::nmRelation() const
{
  return mNmRelation;
}

QString ReferencingFeatureListModel::currentNmRelationId() const
{
  return mNmRelation.isValid() ? mNmRelation.id() : QString();
}

void ReferencingFeatureListModel::setCurrentNmRelationId( const QString &nmRelationId )
{
  if ( nmRelationId == currentNmRelationId() )
    return;

  mNmRelation = QgsProject::instance()->relationManager()->relation( nmRelationId );
  reload();
}

void ReferencingFeatureListModel::setParentPrimariesAvailable( const bool parentPrimariesAvailable )
{
  mParentPrimariesAvailable = parentPrimariesAvailable;
}

bool ReferencingFeatureListModel::parentPrimariesAvailable() const
{
  return mParentPrimariesAvailable;
}

void ReferencingFeatureListModel::updateModel()
{
  beginResetModel();

  if ( mGatherer )
    mEntries = mGatherer->entries();

  emit beforeModelUpdated();
  endResetModel();
  emit modelUpdated();
}

void ReferencingFeatureListModel::gathererThreadFinished()
{
  //ignore spooky signals from ancestor threads
  if ( sender() != mGatherer )
    return;

  mGatherer->deleteLater();
  mGatherer = nullptr;
  emit isLoadingChanged();
}

void ReferencingFeatureListModel::reload()
{
  if ( !mRelation.isValid() || !mFeature.isValid() )
    return;

  if ( checkParentPrimaries() )
  {
    bool wasLoading = false;

    if ( mGatherer )
    {
      // Send the gatherer thread to the graveyard:
      //   forget about it, tell it to stop and delete when finished
      disconnect( mGatherer, &FeatureGatherer::collectedValues, this, &ReferencingFeatureListModel::updateModel );
      disconnect( mGatherer, &FeatureGatherer::finished, this, &ReferencingFeatureListModel::gathererThreadFinished );
      connect( mGatherer, &FeatureGatherer::finished, mGatherer, &FeatureGatherer::deleteLater );
      mGatherer->stop();
      wasLoading = true;
    }

    mGatherer = new FeatureGatherer( mFeature, mRelation, mNmRelation );

    connect( mGatherer, &FeatureGatherer::collectedValues, this, &ReferencingFeatureListModel::updateModel );
    connect( mGatherer, &FeatureGatherer::finished, this, &ReferencingFeatureListModel::gathererThreadFinished );

    mGatherer->start();
    if ( !wasLoading )
      emit isLoadingChanged();
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

bool ReferencingFeatureListModel::deleteFeature( QgsFeatureId referencingFeatureId )
{
  QgsVectorLayer *referencingLayer = mRelation.referencingLayer();

  if ( !referencingLayer || !referencingLayer->isValid() )
  {
    QgsMessageLog::logMessage( tr( "Invalid referencing layer" ), "QField", Qgis::Critical );
    return false;
  }

  if ( !referencingLayer->startEditing() )
  {
    QgsMessageLog::logMessage( tr( "Cannot start editing" ), "QField", Qgis::Critical );
    return false;
  }

  if ( !beforeDeleteFeature( referencingLayer, referencingFeatureId ) )
    return false;

  if ( !referencingLayer->deleteFeature( referencingFeatureId ) )
  {
    QgsMessageLog::logMessage( tr( "Cannot delete feature" ), "QField", Qgis::Critical );

    if ( !referencingLayer->rollBack() )
      QgsMessageLog::logMessage( tr( "Cannot rollback layer changes in layer %1" ).arg( referencingLayer->name() ), "QField", Qgis::Critical );

    return false;
  }

  if ( !referencingLayer->commitChanges() )
  {
    QgsMessageLog::logMessage( tr( "Cannot commit layer changes in layer %1." ).arg( referencingLayer->name() ), "QField", Qgis::Critical );

    if ( !referencingLayer->rollBack() )
      QgsMessageLog::logMessage( tr( "Cannot rollback layer changes in layer %1" ).arg( referencingLayer->name() ), "QField", Qgis::Critical );

    return false;
  }

  reload();

  return true;
}

int ReferencingFeatureListModel::getFeatureIdRow( QgsFeatureId featureId )
{
  int row = 0;
  for ( const Entry &entry : mEntries )
  {
    if ( entry.referencingFeature.id() == featureId )
      break;
    row++;
  }

  return row < mEntries.size() ? row : -1;
}

bool ReferencingFeatureListModel::isLoading() const
{
  return mGatherer;
}

bool ReferencingFeatureListModel::checkParentPrimaries()
{
  if ( !mRelation.isValid() || !mFeature.isValid() )
    return false;

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
          // Insure that the child feature layer's lnked attribute supports NULL values,
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

bool ReferencingFeatureListModel::beforeDeleteFeature( QgsVectorLayer *referencingLayer, QgsFeatureId referencingFeatureId )
{
  Q_UNUSED( referencingLayer );
  Q_UNUSED( referencingFeatureId );
  return true;
}

ReferencingFeatureProxyModel::ReferencingFeatureProxyModel( QObject *parent )
  : QSortFilterProxyModel( parent )
{
  setSortRole( ReferencingFeatureListModel::DisplayString );
  setDynamicSortFilter( true );
  sort( 0, mSortOrder );
}

Qt::SortOrder ReferencingFeatureProxyModel::sortOrder() const
{
  return mSortOrder;
}

void ReferencingFeatureProxyModel::setSortOrder( Qt::SortOrder sortOrder )
{
  if ( mSortOrder == sortOrder )
    return;
  mSortOrder = sortOrder;
  emit sortOrderChanged();

  sort( 0, mSortOrder );
}
