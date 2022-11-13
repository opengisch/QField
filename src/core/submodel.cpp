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

#include "attributeformmodel.h"
#include "submodel.h"

SubModel::SubModel( QObject *parent )
  : QAbstractItemModel( parent )
{
}

QModelIndex SubModel::index( int row, int column, const QModelIndex &parent ) const
{
  if ( !mEnabled || !mModel || parent.isValid() )
    return QModelIndex();

  QModelIndex sourceIndex = mModel->index( row, column, parent.isValid() ? mapToSource( parent ) : QModelIndex( mRootIndex ) );
  return mapFromSource( sourceIndex );
}

QModelIndex SubModel::parent( const QModelIndex &child ) const
{
  return QModelIndex();
}

int SubModel::rowCount( const QModelIndex &parent ) const
{
  if ( !mEnabled || !mModel || parent.isValid() )
    return 0;

  return mModel->rowCount( QModelIndex( mRootIndex ) );
}

int SubModel::columnCount( const QModelIndex &parent ) const
{
  if ( !mEnabled || !mModel || parent.isValid() )
    return 0;

  return mModel->columnCount( QModelIndex( mRootIndex ) );
}

QVariant SubModel::data( const QModelIndex &index, int role ) const
{
  if ( !mEnabled || !mModel )
    return QVariant();

  return mModel->data( mapToSource( index ), role );
}

bool SubModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
  if ( !mEnabled || !mModel )
    return false;

  return mModel->setData( mapToSource( index ), value, role );
}

QHash<int, QByteArray> SubModel::roleNames() const
{
  if ( !mEnabled || !mModel )
    return QHash<int, QByteArray>();

  return mModel->roleNames();
}

QModelIndex SubModel::rootIndex() const
{
  return mRootIndex;
}

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
    disconnect( mModel, &QAbstractItemModel::modelReset, this, &SubModel::onModelReset );
    disconnect( mModel, &QAbstractItemModel::dataChanged, this, &SubModel::onDataChanged );
  }
  else if ( mEnabled )
  {
    connect( mModel, &QAbstractItemModel::rowsInserted, this, &SubModel::onRowsInserted );
    connect( mModel, &QAbstractItemModel::rowsAboutToBeRemoved, this, &SubModel::onRowsAboutToBeRemoved );
    connect( mModel, &QAbstractItemModel::modelReset, this, &SubModel::onModelReset );
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

void SubModel::onModelReset()
{
  beginResetModel();
  mMappings.clear();
  endResetModel();
}

void SubModel::onDataChanged( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles )
{
  if ( isInSubModel( topLeft ) )
    emit dataChanged( mapFromSource( topLeft ), mapFromSource( bottomRight ), roles );
}

bool SubModel::isInSubModel( const QModelIndex &sourceIndex ) const
{
  if ( !mRootIndex.isValid() || !sourceIndex.isValid() || sourceIndex == mRootIndex )
    return false;

  return sourceIndex.parent() == mRootIndex;
}

QModelIndex SubModel::mapFromSource( const QModelIndex &sourceIndex ) const
{
  if ( !mEnabled || !isInSubModel( sourceIndex ) )
    return QModelIndex();

  return createIndex( sourceIndex.row(), sourceIndex.column(), sourceIndex.internalId() );
}

QModelIndex SubModel::mapToSource( const QModelIndex &index ) const
{
  if ( !mEnabled || !mModel )
    return QModelIndex();

  if ( !index.isValid() )
    return QModelIndex();

  return mModel->index( index.row(), index.column(), QModelIndex( mRootIndex ) );
}
