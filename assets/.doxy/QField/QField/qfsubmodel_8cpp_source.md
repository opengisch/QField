

# File qfsubmodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfsubmodel.cpp**](qfsubmodel_8cpp.md)

[Go to the documentation of this file](qfsubmodel_8cpp.md)


```C++
/***************************************************************************
  qfsubmodel.cpp - QfSubModel

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

#include "qfsubmodel.h"

QfSubModel::QfSubModel( QObject *parent )
  : QAbstractItemModel( parent )
{
}

QModelIndex QfSubModel::index( int row, int column, const QModelIndex &parent ) const
{
  if ( !mEnabled || !mModel || parent.isValid() )
    return QModelIndex();

  QModelIndex sourceIndex = mModel->index( row, column, QModelIndex( mRootIndex ) );
  return mapFromSource( sourceIndex );
}

QModelIndex QfSubModel::parent( const QModelIndex &child ) const
{
  return QModelIndex();
}

int QfSubModel::rowCount( const QModelIndex &parent ) const
{
  if ( !mEnabled || !mModel || parent.isValid() )
    return 0;

  return mModel->rowCount( QModelIndex( mRootIndex ) );
}

int QfSubModel::columnCount( const QModelIndex &parent ) const
{
  if ( !mEnabled || !mModel || parent.isValid() )
    return 0;

  return mModel->columnCount( QModelIndex( mRootIndex ) );
}

QVariant QfSubModel::data( const QModelIndex &index, int role ) const
{
  if ( !mEnabled || !mModel )
    return QVariant();

  return mModel->data( mapToSource( index ), role );
}

bool QfSubModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
  if ( !mEnabled || !mModel )
    return false;

  return mModel->setData( mapToSource( index ), value, role );
}

QHash<int, QByteArray> QfSubModel::roleNames() const
{
  if ( !mEnabled || !mModel )
    return QHash<int, QByteArray>();

  return mModel->roleNames();
}

QModelIndex QfSubModel::rootIndex() const
{
  return mRootIndex;
}

void QfSubModel::setRootIndex( const QModelIndex &rootIndex )
{
  if ( rootIndex == mRootIndex )
    return;

  beginResetModel();
  mRootIndex = rootIndex;
  mMappings.clear();
  endResetModel();

  emit rootIndexChanged();
}

QAbstractItemModel *QfSubModel::model() const
{
  return mModel.data();
}

void QfSubModel::handleModelConnection( bool disconnecting ) const
{
  if ( !mModel )
    return;

  if ( disconnecting || !mEnabled )
  {
    disconnect( mModel, &QAbstractItemModel::rowsInserted, this, &QfSubModel::onRowsInserted );
    disconnect( mModel, &QAbstractItemModel::rowsAboutToBeRemoved, this, &QfSubModel::onRowsAboutToBeRemoved );
    disconnect( mModel, &QAbstractItemModel::modelReset, this, &QfSubModel::onModelReset );
    disconnect( mModel, &QAbstractItemModel::dataChanged, this, &QfSubModel::onDataChanged );
  }
  else
  {
    connect( mModel, &QAbstractItemModel::rowsInserted, this, &QfSubModel::onRowsInserted );
    connect( mModel, &QAbstractItemModel::rowsAboutToBeRemoved, this, &QfSubModel::onRowsAboutToBeRemoved );
    connect( mModel, &QAbstractItemModel::modelReset, this, &QfSubModel::onModelReset );
    connect( mModel, &QAbstractItemModel::dataChanged, this, &QfSubModel::onDataChanged );
  }
}

void QfSubModel::setModel( QAbstractItemModel *model )
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

void QfSubModel::setEnabled( bool enabled )
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

void QfSubModel::onRowsInserted( const QModelIndex &parent, int first, int last )
{
  Q_UNUSED( last )
  if ( isInSubModel( mModel->index( first, 0, parent ) ) )
  {
    emit beginInsertRows( mapFromSource( parent ), first, last );
    emit endInsertRows();
  }
}

void QfSubModel::onRowsAboutToBeRemoved( const QModelIndex &parent, int first, int last )
{
  Q_UNUSED( last )
  if ( isInSubModel( mModel->index( first, 0, parent ) ) )
  {
    emit beginRemoveRows( mapFromSource( parent ), first, last );
    emit endRemoveRows();
  }
}

void QfSubModel::onModelReset()
{
  beginResetModel();
  mMappings.clear();
  endResetModel();
}

void QfSubModel::onDataChanged( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles )
{
  if ( isInSubModel( topLeft ) )
    emit dataChanged( mapFromSource( topLeft ), mapFromSource( bottomRight ), roles );
}

bool QfSubModel::isInSubModel( const QModelIndex &sourceIndex ) const
{
  if ( !mRootIndex.isValid() || !sourceIndex.isValid() || sourceIndex == mRootIndex )
    return false;

  return sourceIndex.parent() == mRootIndex;
}

QModelIndex QfSubModel::mapFromSource( const QModelIndex &sourceIndex ) const
{
  if ( !mEnabled || !isInSubModel( sourceIndex ) )
    return QModelIndex();

  return createIndex( sourceIndex.row(), sourceIndex.column(), sourceIndex.internalId() );
}

QModelIndex QfSubModel::mapToSource( const QModelIndex &index ) const
{
  if ( !mEnabled || !mModel )
    return QModelIndex();

  if ( !index.isValid() )
    return QModelIndex();

  return mModel->index( index.row(), index.column(), QModelIndex( mRootIndex ) );
}
```


