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
  QModelIndex sourceIndex = mModel->index( row, column, parent.isValid() ? mapToSource( parent ) : static_cast<QModelIndex>( mRootIndex ) );
  return mapFromSource( sourceIndex );
}

QModelIndex SubModel::parent( const QModelIndex &child ) const
{
  QModelIndex idx = mModel->parent( child );
  if ( idx == mRootIndex )
    return QModelIndex();
  else
    return mapFromSource( idx );
}

int SubModel::rowCount( const QModelIndex &parent ) const
{
  return mModel->rowCount( parent.isValid() ? mapToSource( parent ) : static_cast<QModelIndex>( mRootIndex ) );
}

int SubModel::columnCount( const QModelIndex &parent ) const
{
  return mModel->columnCount( parent.isValid() ? mapToSource( parent ) : static_cast<QModelIndex>( mRootIndex ) );
}

QVariant SubModel::data( const QModelIndex &index, int role ) const
{
  return mModel->data( mapToSource( index ), role );
}

bool SubModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
  return mModel->setData( mapToSource( index ), value, role );
}

QHash<int, QByteArray> SubModel::roleNames() const
{
  return mModel->roleNames();
}

QModelIndex SubModel::rootIndex() const
{
  return mRootIndex;
}

void SubModel::setRootIndex( const QModelIndex &rootIndex )
{
  beginResetModel();
  mRootIndex = rootIndex;
  mMappings.clear();
  endResetModel();
  emit rootIndexChanged();
}

QAbstractItemModel *SubModel::model() const
{
  return mModel;
}

void SubModel::setModel( QAbstractItemModel *model )
{
  if ( model == mModel )
    return;

  connect( model, &QAbstractItemModel::rowsInserted, this, &SubModel::onRowsInserted );
  connect( model, &QAbstractItemModel::rowsAboutToBeRemoved, this, &SubModel::onRowsAboutToBeRemoved );
  connect( model, &QAbstractItemModel::modelAboutToBeReset, this, &SubModel::onModelAboutToBeReset );
  connect( model, &QAbstractItemModel::modelReset, this, &QAbstractItemModel::modelReset );
  connect( model, &QAbstractItemModel::dataChanged, this, &SubModel::onDataChanged );

  mModel = model;
  mMappings.clear();
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
  if ( !isInSubModel( sourceIndex ) )
    return QModelIndex();

  if ( !mMappings.contains( sourceIndex.internalId() ) )
  {
    mMappings.insert( sourceIndex.internalId(), sourceIndex.parent() );
  }

  return createIndex( sourceIndex.row(), sourceIndex.column(), sourceIndex.internalId() );
}

QModelIndex SubModel::mapToSource( const QModelIndex &index ) const
{
  if ( !index.isValid() )
    return mRootIndex;

  return mModel->index( index.row(), index.column(), mMappings.find( index.internalId() ).value() );
}
