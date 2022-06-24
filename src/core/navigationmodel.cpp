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

#include "navigation.h"
#include "navigationmodel.h"

#include <QSettings>
#include <qgsproject.h>

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
      return row == mPoints.size() - 1 ? NavigationModel::Destination : NavigationModel::Intermediate;
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
}

void NavigationModel::setCrs( QgsCoordinateReferenceSystem crs )
{
  if ( mCrs == crs )
    return;

  if ( !mPoints.isEmpty() )
  {
    beginResetModel();
    QgsCoordinateTransform transform( mCrs, crs, QgsProject::instance()->transformContext() );
    for ( int i = 0; i < mPoints.size(); i++ )
    {
      QgsPointXY transformedPoint;
      bool transformed = true;
      try
      {
        transformedPoint = transform.transform( mPoints.at( i ).x(), mPoints.at( i ).y() );
      }
      catch ( const QgsException &e )
      {
        Q_UNUSED( e )
        transformed = false;
      }
      catch ( ... )
      {
        // catch any other errors
        transformed = false;
      }

      if ( transformed )
      {
        mPoints.replace( i, QgsWkbTypes::hasZ( mPoints.at( i ).wkbType() )
                              ? QgsPoint( transformedPoint.x(), transformedPoint.y(), mPoints.at( i ).z() )
                              : QgsPoint( transformedPoint ) );
      }
      else
      {
        // One or more points can't be transformed, clear
        mPoints.clear();
      }
    }
    endResetModel();
    emit destinationChanged();
  }

  mCrs = crs;
}

QHash<int, QByteArray> NavigationModel::roleNames() const
{
  QHash<int, QByteArray> roleNames = QAbstractListModel::roleNames();
  roleNames[NavigationModel::Point] = "Point";
  roleNames[NavigationModel::PointType] = "PointType";
  return roleNames;
}

void NavigationModel::save()
{
  QSettings settings;
  if ( !mPoints.isEmpty() )
  {
    QStringList pointWkts;
    for ( const QgsPoint &point : mPoints )
    {
      pointWkts << point.asWkt();
    }

    settings.beginGroup( QStringLiteral( "/QField/navigation" ) );
    settings.setValue( QStringLiteral( "crs" ), mCrs.toWkt() );
    settings.setValue( QStringLiteral( "points" ), pointWkts );
    settings.endGroup();
  }
  else
  {
    settings.remove( QStringLiteral( "/QField/navigation" ) );
  }
}

void NavigationModel::restore()
{
  QSettings settings;
  if ( settings.contains( QStringLiteral( "/QField/navigation/points" ) ) )
  {
    beginResetModel();
    settings.beginGroup( QStringLiteral( "/QField/navigation" ) );
    mCrs.createFromWkt( settings.value( QStringLiteral( "crs" ), QString() ).toString() );
    mPoints.clear();
    const QStringList pointWkts( settings.value( QStringLiteral( "points" ), QString() ).toStringList() );
    for ( const QString &pointWkt : pointWkts )
    {
      QgsPoint point;
      point.fromWkt( pointWkt );
      if ( !point.isEmpty() )
        mPoints << point;
    }
    settings.endGroup();
    endResetModel();
  }
}

void NavigationModel::clear()
{
  beginResetModel();
  mPoints.clear();
  endResetModel();
}
