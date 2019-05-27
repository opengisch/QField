/***************************************************************************
  referencingfeaturelistmodel.cpp - ReferencingFeatureListModel

 ---------------------
 begin                : 1.3.2019
 copyright            : (C) 2019 by David Signer
 email                : david@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "referencingfeaturelistmodel.h"

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

void ReferencingFeatureListModel::setNmRelation( const QgsRelation &relation )
{
  mNmRelation = relation;
}

QgsRelation ReferencingFeatureListModel::nmRelation() const
{
  return mNmRelation;
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

  endResetModel();
}

void ReferencingFeatureListModel::gathererThreadFinished()
{
  delete mGatherer;
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

void ReferencingFeatureListModel::deleteFeature( QgsFeatureId referencingFeatureId )
{
  mRelation.referencingLayer()->startEditing();
  mRelation.referencingLayer()->deleteFeature( referencingFeatureId );
  mRelation.referencingLayer()->commitChanges();
  reload();
}

bool ReferencingFeatureListModel::isLoading() const
{
  return mGatherer;
}

bool ReferencingFeatureListModel::checkParentPrimaries()
{
  if ( !mRelation.isValid() || !mFeature.isValid() )
    return false;

  const auto fieldPairs = mRelation.fieldPairs();
  for ( QgsRelation::FieldPair fieldPair : fieldPairs )
  {
    if ( mFeature.attribute( fieldPair.second ).isNull() )
      return false;
  }
  return true;
}
