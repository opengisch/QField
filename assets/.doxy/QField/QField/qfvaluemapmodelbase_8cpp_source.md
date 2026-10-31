

# File qfvaluemapmodelbase.cpp

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qfvaluemapmodelbase.cpp**](qfvaluemapmodelbase_8cpp.md)

[Go to the documentation of this file](qfvaluemapmodelbase_8cpp.md)


```C++
/***************************************************************************
                            qfvaluemapmodelbase.cpp

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
#include "qgsvaluemapfieldformatter.h"

QfValueMapModelBase::QfValueMapModelBase( QObject *parent )
  : QAbstractListModel( parent )
{
}

QVariant QfValueMapModelBase::map() const
{
  return mConfiguredMap;
}

void QfValueMapModelBase::setMap( const QVariant &map )
{
  mMap.clear();

  const QVariantList list = map.toList();
  if ( !list.empty() ) // QGIS 3
  {
    beginInsertRows( QModelIndex(), 0, static_cast<int>( list.size() ) - 1 );

    for ( const QVariant &item : list )
    {
      const QVariantMap mapItem = item.toMap();

      const QString &key = mapItem.firstKey();
      const QVariant value = mapItem.value( key );

      mMap.append( qMakePair( value, key ) );
    }
    endInsertRows();
  }
  else // QGIS 2 compat
  {
    const QVariantMap valueMap = map.toMap();
    if ( !valueMap.empty() )
    {
      beginInsertRows( QModelIndex(), 0, static_cast<int>( valueMap.size() ) - 1 );

      QMapIterator<QString, QVariant> i( valueMap );
      while ( i.hasNext() )
      {
        i.next();
        const QString key = i.key();
        const QVariant value = i.value();

        mMap.append( qMakePair( value, key ) );
      }
      endInsertRows();
    }
  }

  mConfiguredMap = map;
  emit mapChanged();
}

int QfValueMapModelBase::rowCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent )
  return static_cast<int>( mMap.size() );
}

QVariant QfValueMapModelBase::data( const QModelIndex &index, int role ) const
{
  if ( index.isValid() == false )
    return QVariant();

  if ( role == QfValueMapModel::KeyRole )
    return mMap.at( index.row() ).first;

  return mMap.at( index.row() ).second;
}

QHash<int, QByteArray> QfValueMapModelBase::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();

  roles[QfValueMapModel::KeyRole] = "key";
  roles[QfValueMapModel::ValueRole] = "value";

  return roles;
}

int QfValueMapModelBase::keyToIndex( const QVariant &key ) const
{
  int i = 0;
  for ( const auto &item : mMap )
  {
    if ( item.first.toString() == key.toString() )
    {
      return i;
    }
    ++i;
  }
  return -1;
}

QVariant QfValueMapModelBase::keyForValue( const QString &value ) const
{
  QVariant result;

  auto match = std::find_if( mMap.begin(), mMap.end(), [&value]( const QPair<QVariant, QString> &x ) { return x.second == value; } );

  if ( match != mMap.end() )
    result = match->first;

  if ( result == QgsValueMapFieldFormatter::NULL_VALUE )
    result = QVariant();

  return result;
}
```


