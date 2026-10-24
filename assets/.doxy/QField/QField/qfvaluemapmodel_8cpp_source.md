

# File qfvaluemapmodel.cpp

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qfvaluemapmodel.cpp**](qfvaluemapmodel_8cpp.md)

[Go to the documentation of this file](qfvaluemapmodel_8cpp.md)


```C++
/***************************************************************************
                            qfvaluemapmodel.cpp

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

#include "qfvaluemapmodel.h"
#include "qfvaluemapmodelbase.h"

QfValueMapModel::QfValueMapModel( QObject *parent )
  : QSortFilterProxyModel( parent )
  , mSourceModel( new QfValueMapModelBase( this ) )
{
  setSourceModel( mSourceModel );

  setFilterRole( ValueRole );

  connect( mSourceModel, &QfValueMapModelBase::mapChanged, this, &QfValueMapModel::mapChanged );
}

QVariant QfValueMapModel::map() const
{
  return mSourceModel->map();
}

void QfValueMapModel::setMap( const QVariant &map )
{
  mSourceModel->setMap( map );
}

int QfValueMapModel::keyToIndex( const QVariant &key ) const
{
  return mSourceModel->keyToIndex( key );
}

QVariant QfValueMapModel::keyForValue( const QString &value ) const
{
  return mSourceModel->keyForValue( value );
}

bool QfValueMapModel::filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
{
  QModelIndex index = sourceModel()->index( sourceRow, 0, sourceParent );

  QVariant data = sourceModel()->data( index, ValueRole );
  return data.toString().contains( filterRegularExpression() );
}
```


