

# File cogooperationsmodel.cpp

[**File List**](files.md) **>** [**cogo**](dir_bee6354f604373d3fe8d8d947aaacc65.md) **>** [**cogooperationsmodel.cpp**](cogooperationsmodel_8cpp.md)

[Go to the documentation of this file](cogooperationsmodel_8cpp.md)


```C++
/***************************************************************************
                              cogooperationsmodel.cpp
                              -------------------
              begin                : 2025.12.21
              copyright            : (C) 2025 by Mathieu Pellerin
              email                : mathieu (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "cogooperationsmodel.h"
#include "cogoregistry.h"


CogoOperationsModel::CogoOperationsModel( QObject *parent )
  : QAbstractListModel( parent )
{
  connect( CogoRegistry::instance(), &CogoRegistry::operationAdded, this, [=]() { buildModel(); } );

  buildModel();
}

void CogoOperationsModel::buildModel()
{
  beginResetModel();
  mOperationNames = CogoRegistry::instance()->availableOperations();
  endResetModel();
}

QVariantMap CogoOperationsModel::get( int row ) const
{
  QVariantMap data;
  const QModelIndex idx = index( row, 0 );
  if ( !idx.isValid() )
  {
    return data;
  }

  const QHash<int, QByteArray> roles = roleNames();
  QHashIterator<int, QByteArray> it( roles );
  while ( it.hasNext() )
  {
    it.next();
    data[it.value()] = idx.data( it.key() );
  }

  return data;
}

QVariantMap CogoOperationsModel::get( const QString &name ) const
{
  return get( mOperationNames.indexOf( name ) );
}

int CogoOperationsModel::rowCount( const QModelIndex &parent ) const
{
  return !parent.isValid() ? static_cast<int>( mOperationNames.size() ) : 0;
}

QVariant CogoOperationsModel::data( const QModelIndex &index, int role ) const
{
  if ( !index.isValid() || index.row() >= mOperationNames.count() )
  {
    return QVariant();
  }

  const CogoOperation *operation = CogoRegistry::instance()->operation( mOperationNames[index.row()] );
  if ( !operation )
  {
    return QVariant();
  }

  switch ( role )
  {
    case NameRole:
      return operation->name();

    case DisplayNameRole:
      return operation->displayName();

    case IconRole:
      return operation->icon();

    default:
      return QVariant();
  }

  return QVariant();
}

QHash<int, QByteArray> CogoOperationsModel::roleNames() const
{
  return {
    { NameRole, "Name" },
    { DisplayNameRole, "DisplayName" },
    { IconRole, "Icon" },
  };
}
```


