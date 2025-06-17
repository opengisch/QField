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
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "featurechecklistmodel.h"
#include "featurechecklistproxymodel.h"

FeatureCheckListProxyModel::FeatureCheckListProxyModel( QObject *parent )
  : QSortFilterProxyModel( parent ), mSearchTerm( "" ), mSortCheckedFirst( false )
{
  setFilterCaseSensitivity( Qt::CaseInsensitive );
  setFilterRole( Qt::DisplayRole );
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

    sort( 0 );
  }
}

bool FeatureCheckListProxyModel::sortCheckedFirst() const
{
  return mSortCheckedFirst;
}

void FeatureCheckListProxyModel::setSortCheckedFirst( bool enabled )
{
  if ( mSortCheckedFirst != enabled )
  {
    mSortCheckedFirst = enabled;
    emit sortCheckedFirstChanged();

    sort( 0 );
  }
}

bool FeatureCheckListProxyModel::filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
{
  if ( mSearchTerm.isEmpty() )
    return true;

  const QModelIndex index = sourceModel()->index( sourceRow, 0, sourceParent );
  const QVariant data = sourceModel()->data( index, Qt::DisplayRole );
  return data.toString().contains( mSearchTerm, Qt::CaseInsensitive );
}

bool FeatureCheckListProxyModel::lessThan( const QModelIndex &left, const QModelIndex &right ) const
{
  if ( ( mSearchTerm.isEmpty() && mSortCheckedFirst ) )
  {
    const bool leftItemSelected = sourceModel()->data( left, FeatureCheckListModel::CheckedRole ).toBool();
    const bool rightItemSelected = sourceModel()->data( right, FeatureCheckListModel::CheckedRole ).toBool();

    if ( rightItemSelected && !leftItemSelected )
    {
      return false;
    }
    else if ( !rightItemSelected && leftItemSelected )
    {
      return true;
    }
  }

  const QString leftDisplay = sourceModel()->data( left, Qt::DisplayRole ).toString().toLower();
  const QString rightDisplay = sourceModel()->data( right, Qt::DisplayRole ).toString().toLower();

  if ( !mSearchTerm.isEmpty() )
  {
    const bool leftStartsWithSearchTerm = leftDisplay.startsWith( mSearchTerm.toLower() );
    const bool rightStartsWithSearchTerm = rightDisplay.startsWith( mSearchTerm.toLower() );

    if ( rightStartsWithSearchTerm && !leftStartsWithSearchTerm )
    {
      return false;
    }
    else if ( !rightStartsWithSearchTerm && leftStartsWithSearchTerm )
    {
      return true;
    }
    const double leftFuzzyScore = calcFuzzyScore( leftDisplay, mSearchTerm.toLower() );
    const double rightFuzzyScore = calcFuzzyScore( rightDisplay, mSearchTerm.toLower() );

    if ( leftFuzzyScore != rightFuzzyScore )
    {
      return leftFuzzyScore > rightFuzzyScore;
    }
  }

  // Alphabetically
  return leftDisplay < rightDisplay;
}

double FeatureCheckListProxyModel::calcFuzzyScore( const QString &displayString, const QString &searchTerm ) const
{
  double fuzzyScore = 0;
  fuzzyScore = StringUtils::fuzzyMatch( displayString, searchTerm ) ? 0.5 : 0;
  fuzzyScore += QgsStringUtils::fuzzyScore( displayString, searchTerm ) * 0.5;
  return fuzzyScore;
};
