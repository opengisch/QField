

# File qfcogooperationsmodel.cpp

[**File List**](files.md) **>** [**cogo**](dir_bee6354f604373d3fe8d8d947aaacc65.md) **>** [**qfcogooperationsmodel.cpp**](qfcogooperationsmodel_8cpp.md)

[Go to the documentation of this file](qfcogooperationsmodel_8cpp.md)


```C++
/***************************************************************************
                              qfcogooperationsmodel.cpp
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

#include "qfcogooperationsmodel.h"
#include "qfcogoregistry.h"


QfCogoOperationsModel::QfCogoOperationsModel( QObject *parent )
  : QAbstractListModel( parent )
{
  connect( QfCogoRegistry::instance(), &QfCogoRegistry::operationAdded, this, [this]() { buildModel(); } );

  buildModel();
}

void QfCogoOperationsModel::buildModel()
{
  beginResetModel();
  mOperationNames = QfCogoRegistry::instance()->availableOperations();
  endResetModel();
}

QVariantMap QfCogoOperationsModel::get( int row ) const
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

QVariantMap QfCogoOperationsModel::get( const QString &name ) const
{
  return get( mOperationNames.indexOf( name ) );
}

int QfCogoOperationsModel::rowCount( const QModelIndex &parent ) const
{
  return !parent.isValid() ? static_cast<int>( mOperationNames.size() ) : 0;
}

QVariant QfCogoOperationsModel::data( const QModelIndex &index, int role ) const
{
  if ( !index.isValid() || index.row() >= mOperationNames.count() )
  {
    return QVariant();
  }

  const QfCogoOperation *operation = QfCogoRegistry::instance()->operation( mOperationNames[index.row()] );
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

QHash<int, QByteArray> QfCogoOperationsModel::roleNames() const
{
  return {
    { NameRole, "Name" },
    { DisplayNameRole, "DisplayName" },
    { IconRole, "Icon" },
  };
}
```


