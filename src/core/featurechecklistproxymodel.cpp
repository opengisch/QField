/***************************************************************************
  featurechecklistproxymodel.h - FeatureCheckListProxyModel

 ---------------------
 begin                : Jun 2025
 copyright            : (C) 2025 by Mohsen Dehghanzadeh
 email                : mohsen (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.
 *                                                                         *
 ***************************************************************************/

#include "featurechecklistproxymodel.h"

#include <QAbstractItemModel>

FeatureCheckListProxyModel::FeatureCheckListProxyModel( QObject *parent )
  : QSortFilterProxyModel( parent )
{
  setFilterCaseSensitivity( Qt::CaseInsensitive );
  setFilterRole( Qt::DisplayRole );
  setSortRole( Qt::UserRole + 100 );
  setDynamicSortFilter( false );
}

QString FeatureCheckListProxyModel::searchTerm() const
{
  return mSearchTerm;
}

void FeatureCheckListProxyModel::setSearchTerm( const QString &searchTerm )
{
  if ( mSearchTerm != searchTerm )
  {
    mSearchTerm = searchTerm;
    emit searchTermChanged();
    invalidateFilter();
  }
}

bool FeatureCheckListProxyModel::filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
{
  if ( mSearchTerm.isEmpty() )
    return true;

  const QModelIndex idx = sourceModel()->index( sourceRow, 0, sourceParent );
  const QVariant displayData = sourceModel()->data( idx, Qt::DisplayRole );

  return displayData.toString().contains( mSearchTerm, Qt::CaseInsensitive );
}

void FeatureCheckListProxyModel::sortCheckedFirst( const bool enabled )
{
  if ( enabled )
  {
    sort( 0, Qt::DescendingOrder );
  }
  else
  {
    sort( -1 );
  }
}
