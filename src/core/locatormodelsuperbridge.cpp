/***************************************************************************
  locatormodelsuperbridge.cpp

 ---------------------
 begin                : 01.12.2018
 copyright            : (C) 2018 by Denis Rouzaud
 email                : denis@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "locatormodelsuperbridge.h"
#include "qgsquickmapsettings.h"
#include "featurelistextentcontroller.h"
#include "featureslocatorfilter.h"
#include "gotolocatorfilter.h"
#include "peliasgeocoder.h"
#include "peliaslocatorfilter.h"

#include <QStandardItem>

#include <qgslocatormodel.h>
#include <qgslocator.h>
#include <qgssettings.h>


LocatorModelSuperBridge::LocatorModelSuperBridge( QObject *parent )
  : QgsLocatorModelBridge( parent )
{
  locator()->registerFilter( new GotoLocatorFilter( this ) );
  locator()->registerFilter( new FeaturesLocatorFilter( this ) );

  // Finish's digitransit.fi geocoder
  mFinlandGeocoder = new PeliasGeocoder( "https://api.digitransit.fi/geocoding/v1/search" );
  locator()->registerFilter( new PeliasLocatorFilter( mFinlandGeocoder, this ) );
}

QgsQuickMapSettings *LocatorModelSuperBridge::mapSettings() const
{
  return mMapSettings;
}

void LocatorModelSuperBridge::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mapSettings == mMapSettings )
    return;

  mMapSettings = mapSettings;

  updateCanvasExtent( mMapSettings->extent() );
  updateCanvasCrs( mMapSettings->destinationCrs() );

  connect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, [ = ]() {updateCanvasExtent( mMapSettings->visibleExtent() );} );
  connect( mMapSettings, &QgsQuickMapSettings::destinationCrsChanged, this, [ = ]() {updateCanvasCrs( mMapSettings->destinationCrs() );} ) ;

  emit mapSettingsChanged();
}

QObject *LocatorModelSuperBridge::locatorHighlightGeometry() const
{
  return mLocatorHighlightGeometry;
}

void LocatorModelSuperBridge::setLocatorHighlightGeometry( QObject *locatorHighlightGeometry )
{
  if ( locatorHighlightGeometry == mLocatorHighlightGeometry )
    return;

  mLocatorHighlightGeometry = locatorHighlightGeometry;
  emit locatorHighlightGeometryChanged();
}

FeatureListExtentController *LocatorModelSuperBridge::featureListController() const
{
  return mFeatureListController;
}

void LocatorModelSuperBridge::setFeatureListController( FeatureListExtentController *featureListController )
{
  if ( featureListController == mFeatureListController )
    return;

  mFeatureListController = featureListController;
  emit featureListControllerChanged();
}

bool LocatorModelSuperBridge::keepScale() const
{
  return mKeepScale;
}

void LocatorModelSuperBridge::setKeepScale( bool keepScale )
{
  if ( keepScale == mKeepScale )
    return;

  mKeepScale = keepScale;
  emit keepScaleChanged();
}

LocatorActionsModel *LocatorModelSuperBridge::contextMenuActionsModel( const int row )
{
  const QModelIndex index = proxyModel()->index( row, 0 );
  if ( !index.isValid() )
    return nullptr;

  const QList<QgsLocatorResult::ResultAction> actions = proxyModel()->data( index, QgsLocatorModel::ResultActionsRole ).value<QList<QgsLocatorResult::ResultAction>>();
  int r = 0;
  LocatorActionsModel *model = new LocatorActionsModel( actions.count(), 1 );
  for ( auto action : actions )
  {
    QStandardItem *item = new QStandardItem( action.text );
    item->setData( action.id, LocatorActionsModel::IdRole );
    item->setData( action.iconPath, LocatorActionsModel::IconPathRole );
    model->setItem( r, 0, item );
    r++;
  }

  return model;
}

void LocatorModelSuperBridge::emitMessage( const QString &text )
{
  emit messageEmitted( text );
}

void LocatorModelSuperBridge::triggerResultAtRow( const int row, const int id )
{
  const QModelIndex index = proxyModel()->index( row, 0 );
  if ( index.isValid() )
    triggerResult( index, id );
}

//
// LocatorActionsModel
//

LocatorActionsModel::LocatorActionsModel( QObject *parent )
  : QStandardItemModel( parent )
{
}

LocatorActionsModel::LocatorActionsModel( int rows, int columns, QObject *parent )
  : QStandardItemModel( rows, columns, parent )
{
}

QHash<int, QByteArray> LocatorActionsModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[IconPathRole] = "iconPath";
  roles[IdRole] = "id";
  return roles;
}

//
// LocatorFiltersModel
//

LocatorFiltersModel::LocatorFiltersModel() : QAbstractListModel()
{
}

int LocatorFiltersModel::rowCount( const QModelIndex &parent ) const
{
  if ( !mLocatorModelSuperBridge->locator() || parent.isValid() )
    return 0;

  return mLocatorModelSuperBridge->locator()->filters().count();
}

QHash<int, QByteArray> LocatorFiltersModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
  roles[NameRole] = "Name";
  roles[PrefixRole] = "Prefix";
  roles[ActiveRole] = "Active";
  roles[DefaultRole] = "Default";

  return roles;
}

QVariant LocatorFiltersModel::data( const QModelIndex &index, int role ) const
{
  if ( !mLocatorModelSuperBridge->locator() || !index.isValid() || index.parent().isValid() ||
       index.row() < 0 || index.row() >= rowCount( QModelIndex() ) )
    return QVariant();

  switch ( role )
  {
    case Qt::DisplayRole:
    case NameRole:
      return filterForIndex( index )->displayName();

    case PrefixRole:
      return filterForIndex( index )->activePrefix();

    case ActiveRole:
      return QVariant( filterForIndex( index )->enabled() );

    case DefaultRole:
      return filterForIndex( index )->useWithoutPrefix();
  }

  return QVariant();
}

bool LocatorFiltersModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
  if ( !mLocatorModelSuperBridge->locator() || !index.isValid() || index.parent().isValid() ||
       index.row() < 0 || index.row() >= rowCount( QModelIndex() ) )
    return false;

  switch ( role )
  {
    case NameRole:
    case PrefixRole:
      return false;

    case ActiveRole:
    {
      QgsSettings settings;
      QgsLocatorFilter *filter = filterForIndex( index );
      filter->setEnabled( value.toBool() );
      settings.setValue( QStringLiteral( "locator_filters/enabled_%1" ).arg( filter->name() ), filter->enabled(), QgsSettings::Section::Gui );
      emit dataChanged( index, index, QVector<int>() << ActiveRole );
      return true;
    }

    case DefaultRole:
    {
      QgsSettings settings;
      QgsLocatorFilter *filter = filterForIndex( index );
      filter->setUseWithoutPrefix( value.toBool() );
      settings.setValue( QStringLiteral( "locator_filters/default_%1" ).arg( filter->name() ), filter->useWithoutPrefix(), QgsSettings::Section::Gui );
      emit dataChanged( index, index, QVector<int>() << DefaultRole );
      return true;
    }
  }

  return false;
}

QgsLocatorFilter *LocatorFiltersModel::filterForIndex( const QModelIndex &index ) const
{
  if ( !mLocatorModelSuperBridge->locator() )
    return nullptr;

  return mLocatorModelSuperBridge->locator()->filters().at( index.row() );
}

LocatorModelSuperBridge *LocatorFiltersModel::locatorModelSuperBridge() const
{
  return mLocatorModelSuperBridge;
}

void LocatorFiltersModel::setLocatorModelSuperBridge( LocatorModelSuperBridge *locatorModelSuperBridge )
{
  if ( mLocatorModelSuperBridge == locatorModelSuperBridge )
    return;

  qDebug() << "XXXXX";
  emit beginResetModel();
  mLocatorModelSuperBridge = locatorModelSuperBridge;
  emit locatorModelSuperBridgeChanged();
  emit endResetModel();
}
