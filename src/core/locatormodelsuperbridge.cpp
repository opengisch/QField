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


#include <QStandardItem>

#include <qgslocatormodel.h>

#include "locatormodelsuperbridge.h"
#include "qgsquickmapsettings.h"
#include "featureslocatorfilter.h"
#include "qgslocator.h"
#include "featurelistextentcontroller.h"

LocatorModelSuperBridge::LocatorModelSuperBridge( QObject *parent )
  : QgsLocatorModelBridge( parent )
{
  FeaturesLocatorFilter *filter = new FeaturesLocatorFilter( this );
  locator()->registerFilter( filter );
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
