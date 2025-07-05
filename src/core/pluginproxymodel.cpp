/***************************************************************************
 pluginproxymodel.cpp - PluginProxyModel

 ---------------------
 begin                : June 2025
 copyright            : (C) 2025 by Mohsen Dehghanzadeh
 email                : mohsen@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "pluginmodel.h"
#include "pluginproxymodel.h"

PluginProxyModel::PluginProxyModel( QObject *parent )
  : QSortFilterProxyModel( parent )
{
  setFilterCaseSensitivity( Qt::CaseInsensitive );
  setFilterRole( PluginModel::PluginRoles::NameRole );
}

QString PluginProxyModel::searchTerm() const
{
  return mSearchTerm;
}

void PluginProxyModel::setSearchTerm( const QString &searchTerm )
{
  if ( mSearchTerm != searchTerm )
  {
    mSearchTerm = searchTerm;
    emit searchTermChanged();
    invalidateFilter();
  }
}

bool PluginProxyModel::filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
{
  bool matchesPluginType = false;
  const QModelIndex currentRowIndex = sourceModel()->index( sourceRow, 0, sourceParent );

  switch ( mFilter )
  {
    case LocalPlugin:
      matchesPluginType = sourceModel()->data( currentRowIndex, PluginModel::InstalledLocallyRole ).toBool();
      break;
    case RemotePlugin:
      matchesPluginType = sourceModel()->data( currentRowIndex, PluginModel::AvailableRemotelyRole ).toBool();
      break;
  }

  const QModelIndex index = sourceModel()->index( sourceRow, 0, sourceParent );
  const QVariant data = sourceModel()->data( index, PluginModel::PluginRoles::NameRole );
  const bool matchesTextFilter = mSearchTerm.isEmpty() || data.toString().contains( mSearchTerm, Qt::CaseInsensitive );

  return matchesTextFilter && matchesPluginType;
}


void PluginProxyModel::setFilter( PluginFilter filter )
{
  if ( mFilter == filter )
    return;

  mFilter = filter;
  invalidateFilter();

  emit filterChanged();
}

PluginProxyModel::PluginFilter PluginProxyModel::filter() const
{
  return mFilter;
}
