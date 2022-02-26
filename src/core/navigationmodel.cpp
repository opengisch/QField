/***************************************************************************
 navigationmodel.cpp - NavigationModel

 ---------------------
 begin                : 22.02.2022
 copyright            : (C) 2022 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "navigationmodel.h"
#include "navigation.h"

NavigationModel::NavigationModel()
  : QAbstractListModel()
{
}

int NavigationModel::rowCount( const QModelIndex &parent ) const
{
  return mPoints.size();
}

QVariant NavigationModel::data( const QModelIndex &index, int role ) const
{
  const int row = index.row();
  if ( row < 0 || row >= mPoints.size() )
    return QVariant();

  switch ( role )
  {
    case NavigationModel::Point:
    {
      QgsGeometry geom( new QgsPoint( mPoints.at( row ) ) );
      return geom;
    }

    case NavigationModel::PointType:
    {
      return row == mPoints.size() -1 ? NavigationModel::Destination : NavigationModel::Intermediate;
    }
  }

  return QVariant();
}

QgsPoint NavigationModel::destination() const
{
  return !mPoints.isEmpty() ? mPoints.last() : QgsPoint();
}

void NavigationModel::setDestination( const QgsPoint &point )
{
  if ( !mPoints.isEmpty() )
  {
    if ( mPoints.last() == point )
      return;

    mPoints.removeLast();
    mPoints << point;
    QModelIndex changedIndex = index( mPoints.size() - 1, 0 );
    emit dataChanged( changedIndex, changedIndex );
  }
  else
  {
    beginInsertRows( QModelIndex(), 0, 0 );
    mPoints << point;
    endInsertRows();
  }

  emit destinationChanged();
  emit pointsChanged();
}

void NavigationModel::setCrs( QgsCoordinateReferenceSystem crs )
{
  if ( mCrs == crs )
    return;

  //TODO: transform points
  mCrs = crs;

  emit destinationChanged();
}

QHash<int, QByteArray> NavigationModel::roleNames() const
{
  QHash<int, QByteArray> roleNames = QAbstractListModel::roleNames();
  roleNames[NavigationModel::Point] = "Point";
  roleNames[NavigationModel::PointType] = "PointType";
  return roleNames;
}
