/***************************************************************************
                            valuemapmodel.cpp

                              -------------------
              begin                : March 2019
              copyright            : (C) 2019 by Matthias Kuhn
              email                : matthias@opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "valuemapmodel.h"
#include "valuemapmodelbase.h"

ValueMapModel::ValueMapModel( QObject *parent )
  : QSortFilterProxyModel( parent )
  , mSourceModel( new ValueMapModelBase( this ) )
{
  setSourceModel( mSourceModel );

  setFilterRole( ValueRole );

  connect( mSourceModel, &ValueMapModelBase::mapChanged, this, &ValueMapModel::mapChanged );
}

QVariant ValueMapModel::map() const
{
  return mSourceModel->map();
}

void ValueMapModel::setMap( const QVariant &map )
{
  mSourceModel->setMap( map );
}

int ValueMapModel::keyToIndex( const QVariant &key ) const
{
  return mSourceModel->keyToIndex( key );
}

QVariant ValueMapModel::keyForValue( const QString &value ) const
{
  return mSourceModel->keyForValue( value );
}

bool ValueMapModel::filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
{
  QModelIndex index = sourceModel()->index( sourceRow, 0, sourceParent );

  QVariant data = sourceModel()->data( index, ValueRole );
  return data.toString().contains( filterRegularExpression() );
}
