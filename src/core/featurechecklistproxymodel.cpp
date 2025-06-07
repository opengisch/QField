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
}

QString FeatureCheckListProxyModel::filterString() const
{
  return mTextFilter;
}

void FeatureCheckListProxyModel::setFilterString( const QString &textFilter )
{
  if ( mTextFilter != textFilter )
  {
    mTextFilter = textFilter;
    emit filterStringChanged();
    invalidateFilter();
  }
}

bool FeatureCheckListProxyModel::filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
{
  if ( mTextFilter.isEmpty() )
    return true;

  const QModelIndex idx = sourceModel()->index( sourceRow, 0, sourceParent );
  const QVariant displayData = sourceModel()->data( idx, Qt::DisplayRole );

  return displayData.toString().contains( mTextFilter, Qt::CaseInsensitive );
}
