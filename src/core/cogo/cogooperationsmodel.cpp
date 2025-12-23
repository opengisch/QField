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

#include "cogooperation.h"
#include "cogooperationsmodel.h"
#include "cogoregistry.h"


CogoOperationsModel::CogoOperationsModel( QObject *parent )
  : QAbstractListModel( parent )
{
  connect( CogoRegistry::instance(), &CogoRegistry::operationAdded, this, [=]() { buildModel(); } );
}

void CogoOperationsModel::buildModel()
{
  beginResetModel();
  mOperationNames = CogoRegistry::instance()->availableOperations();
  endResetModel();
}

int CogoOperationsModel::rowCount( const QModelIndex &parent ) const
{
  return !parent.isValid() ? static_cast<int>( mOperationNames.count() ) : 0;
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
  }

  return QVariant();
}

QHash<int, QByteArray> CogoOperationsModel::roleNames() const
{
  return {
    { NameRole, "Name" },
    { DisplayNameRole, "DisplayName" },
    { IconRole, "Icon" },
    { ParametersRole, "Parameters" },
  };
}
