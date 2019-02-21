#include "valuemapmodel.h"
#include <QDebug>

ValueMapModel::ValueMapModel( QObject *parent )
  : QAbstractListModel( parent )
{
}

QVariant ValueMapModel::map() const
{
  return mConfiguredMap;
}

void ValueMapModel::setMap( const QVariant &map )
{
  mMap.clear();
  // QGIS 3
//  if ( map.type() == QVariant::List )
  {
    const QVariantList list = map.toList();
    beginInsertRows( QModelIndex(), 0, list.size() );

    for ( const QVariant &item : list )
    {
      const QVariantMap mapItem = item.toMap();

      const QString key = mapItem.firstKey();
      const QVariant value = mapItem.value( key );

      mMap.append( qMakePair( value, key ) );
    }
    endInsertRows();
  }
  // QGIS 2
  {

  }

  mConfiguredMap = map;
  emit mapChanged();
}

int ValueMapModel::rowCount( const QModelIndex &parent ) const
{
  return mMap.size();
}

QVariant ValueMapModel::data( const QModelIndex &index, int role ) const
{
  if ( role == KeyRole )
    return mMap.at( index.row() ).first;
  else
    return mMap.at( index.row() ).second;
}

QHash<int, QByteArray> ValueMapModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();

  roles[KeyRole] = "key";
  roles[ValueRole] = "value";

  return roles;
}

int ValueMapModel::keyToIndex( const QVariant &value ) const
{
  qWarning() << "KEY: " << value;
  int i = 0;
  for ( const auto &item : mMap )
  {
    qWarning() << "FIST   " << item.first;
    if ( item.first.toString() == value.toString() )
    {
      qWarning() << "RETURNING I " << i;
      return i;
    }
    ++i;
  }
  return -1;
}

QVariant ValueMapModel::keyForValue( const QString &value ) const
{
  QVariant result;
  for ( const auto &item : mMap )
  {
    if ( item.second == value )
      result = item.first;
  }
  return result;
}

