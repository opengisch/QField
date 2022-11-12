/***************************************************************************
  submodel.cpp - SubModel

 ---------------------
 begin                : 16.9.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "submodel.h"

SubModel::SubModel( QObject *parent )
  : QAbstractItemModel( parent )
{
}

QModelIndex SubModel::index( int row, int column, const QModelIndex &parent ) const
{
  if ( !mEnabled || !mModel )
    return QModelIndex();

  QModelIndex sourceIndex = mModel->index( row, column, parent.isValid() ? mapToSource( parent ) : static_cast<QModelIndex>( mRootIndex ) );
  return mapFromSource( sourceIndex );
}

QModelIndex SubModel::parent( const QModelIndex &child ) const
{
  if ( !mEnabled || !mModel )
    return QModelIndex();

  QModelIndex idx = mModel->parent( child );
  if ( idx == mRootIndex )
    return QModelIndex();
  else
    return mapFromSource( idx );
}

int SubModel::rowCount( const QModelIndex &parent ) const
{
  return mEnabled && mModel ? mModel->rowCount( parent.isValid() ? mapToSource( parent ) : static_cast<QModelIndex>( mRootIndex ) ) : 0;
}

int SubModel::columnCount( const QModelIndex &parent ) const
{
  return mEnabled && mModel ? mModel->columnCount( parent.isValid() ? mapToSource( parent ) : static_cast<QModelIndex>( mRootIndex ) ) : 0;
}

QVariant SubModel::data( const QModelIndex &index, int role ) const
{
  return mEnabled && mModel ? mModel->data( mapToSource( index ), role ) : QVariant();
}

bool SubModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
  return mEnabled && mModel ? mModel->setData( mapToSource( index ), value, role ) : false;
}

QHash<int, QByteArray> SubModel::roleNames() const
{
  return mEnabled && mModel ? mModel->roleNames() : QHash<int, QByteArray>();
}

QModelIndex SubModel::rootIndex() const
{
  return mRootIndex;
}
#include <QDebug>
void SubModel::setRootIndex( const QModelIndex &rootIndex )
{
  if ( rootIndex == mRootIndex )
    return;

  beginResetModel();
  mRootIndex = rootIndex;
  mMappings.clear();
  endResetModel();

  emit rootIndexChanged();
}

QAbstractItemModel *SubModel::model() const
{
  return mModel.data();
}

void SubModel::handleModelConnection( bool disconnecting ) const
{
  if ( !mModel )
    return;

  if ( disconnecting || !mEnabled )
  {
    disconnect( mModel, &QAbstractItemModel::rowsInserted, this, &SubModel::onRowsInserted );
    disconnect( mModel, &QAbstractItemModel::rowsAboutToBeRemoved, this, &SubModel::onRowsAboutToBeRemoved );
    disconnect( mModel, &QAbstractItemModel::modelAboutToBeReset, this, &SubModel::onModelAboutToBeReset );
    disconnect( mModel, &QAbstractItemModel::modelReset, this, &QAbstractItemModel::modelReset );
    disconnect( mModel, &QAbstractItemModel::dataChanged, this, &SubModel::onDataChanged );
  }
  else if ( mEnabled )
  {
    connect( mModel, &QAbstractItemModel::rowsInserted, this, &SubModel::onRowsInserted );
    connect( mModel, &QAbstractItemModel::rowsAboutToBeRemoved, this, &SubModel::onRowsAboutToBeRemoved );
    connect( mModel, &QAbstractItemModel::modelAboutToBeReset, this, &SubModel::onModelAboutToBeReset );
    connect( mModel, &QAbstractItemModel::modelReset, this, &QAbstractItemModel::modelReset );
    connect( mModel, &QAbstractItemModel::dataChanged, this, &SubModel::onDataChanged );
  }
}

void SubModel::setModel( QAbstractItemModel *model )
{
  if ( model == mModel )
    return;

  // Disconnect previous model connections
  handleModelConnection( true );

  beginResetModel();
  mModel = model;
  mMappings.clear();
  handleModelConnection();
  endResetModel();

  emit modelChanged();
}

void SubModel::setEnabled( bool enabled )
{
  if ( enabled == mEnabled )
    return;

  beginResetModel();
  mEnabled = enabled;
  mMappings.clear();
  handleModelConnection();
  endResetModel();

  emit enabledChanged();
}

void SubModel::onRowsInserted( const QModelIndex &parent, int first, int last )
{
  Q_UNUSED( last )
  if ( isInSubModel( mModel->index( first, 0, parent ) ) )
  {
    emit beginInsertRows( mapFromSource( parent ), first, last );
    emit endInsertRows();
  }
}

void SubModel::onRowsAboutToBeRemoved( const QModelIndex &parent, int first, int last )
{
  Q_UNUSED( last )
  if ( isInSubModel( mModel->index( first, 0, parent ) ) )
  {
    emit beginRemoveRows( mapFromSource( parent ), first, last );
    emit endRemoveRows();
  }
}

void SubModel::onModelAboutToBeReset()
{
  mMappings.clear();
}

void SubModel::onDataChanged( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles )
{
  if ( isInSubModel( topLeft ) )
    emit dataChanged( mapFromSource( topLeft ), mapFromSource( bottomRight ), roles );
}

bool SubModel::isInSubModel( const QModelIndex &sourceIndex ) const
{
  if ( !mRootIndex.isValid() )
    return true;

  if ( sourceIndex == mRootIndex || !sourceIndex.isValid() )
    return false;

  QModelIndex idx = sourceIndex;
  bool foundRootIndex = false;
  while ( idx.isValid() )
  {
    if ( mModel->parent( idx ) == mRootIndex )
    {
      foundRootIndex = true;
      break;
    }
    idx = mModel->parent( idx );
  }
  return foundRootIndex;
}

QModelIndex SubModel::mapFromSource( const QModelIndex &sourceIndex ) const
{
  if ( !mEnabled || !isInSubModel( sourceIndex ) )
    return QModelIndex();

  if ( !mMappings.contains( sourceIndex.internalId() ) )
  {
    mMappings.insert( sourceIndex.internalId(), sourceIndex.parent() );
  }

  return createIndex( sourceIndex.row(), sourceIndex.column(), sourceIndex.internalId() );
}

QModelIndex SubModel::mapToSource( const QModelIndex &index ) const
{
  if ( !mEnabled || !mModel )
    return QModelIndex();

  if ( !index.isValid() )
    return QModelIndex();

  return mModel->index( index.row(), index.column(), mMappings.find( index.internalId() ).value() );
}
