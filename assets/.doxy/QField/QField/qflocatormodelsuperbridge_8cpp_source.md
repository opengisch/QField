

# File qflocatormodelsuperbridge.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**locator**](dir_952e15d753357bc11da55d627a96f3ed.md) **>** [**qflocatormodelsuperbridge.cpp**](qflocatormodelsuperbridge_8cpp.md)

[Go to the documentation of this file](qflocatormodelsuperbridge_8cpp.md)


```C++
/***************************************************************************
  qflocatormodelsuperbridge.cpp

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


#include "qfactivelayerfeatureslocatorfilter.h"
#include "qfbookmarklocatorfilter.h"
#include "qfexpressioncalculatorlocatorfilter.h"
#include "qffeaturelistextentcontroller.h"
#include "qffeatureslocatorfilter.h"
#include "qffinlandlocatorfilter.h"
#include "qfgnsspositioninformation.h"
#include "qfgotolocatorfilter.h"
#include "qfhelplocatorfilter.h"
#include "qflocatorfilter.h"
#include "qflocatormodelsuperbridge.h"
#include "qfpeliasgeocoder.h"
#include "qgsquickmapsettings.h"

#include <QStandardItem>
#include <qgslocator.h>
#include <qgslocatormodel.h>
#include <qgssettings.h>


QfLocatorModelSuperBridge::QfLocatorModelSuperBridge( QObject *parent )
  : QgsLocatorModelBridge( parent )
{
  locator()->registerFilter( new QfActiveLayerFeaturesLocatorFilter( this ) );
  locator()->registerFilter( new QfFeaturesLocatorFilter( this ) );
  locator()->registerFilter( new QfGotoLocatorFilter( this ) );
  locator()->registerFilter( new QfBookmarkLocatorFilter( this ) );
  locator()->registerFilter( new QfExpressionCalculatorLocatorFilter( this ) );
  locator()->registerFilter( new QfHelpLocatorFilter( this ) );

  // Finnish's Digitransit geocoder (disabled until API access can be sorted)
  //mFinlandGeocoder = new QfPeliasGeocoder( QStringLiteral( "https://api.digitransit.fi/geocoding/v1/search" ) );
  //locator()->registerFilter( new QfFinlandLocatorFilter( mFinlandGeocoder, this ) );
}

void QfLocatorModelSuperBridge::registerQFieldLocatorFilter( QfLocatorFilter *filter )
{
  locator()->registerFilter( filter );
  emit locatorFiltersChanged();
}

void QfLocatorModelSuperBridge::deregisterQFieldLocatorFilter( QfLocatorFilter *filter )
{
  locator()->deregisterFilter( filter );
  emit locatorFiltersChanged();
}

QfNavigation *QfLocatorModelSuperBridge::navigation() const
{
  return mNavigation;
}

void QfLocatorModelSuperBridge::setNavigation( QfNavigation *navigation )
{
  if ( navigation == mNavigation )
    return;

  mNavigation = navigation;

  emit navigationChanged();
}

QfBookmarkModel *QfLocatorModelSuperBridge::bookmarks() const
{
  return mBookmarks;
}

void QfLocatorModelSuperBridge::setBookmarks( QfBookmarkModel *bookmarks )
{
  if ( bookmarks == mBookmarks )
    return;

  mBookmarks = bookmarks;

  emit bookmarksChanged();
}

QgsQuickMapSettings *QfLocatorModelSuperBridge::mapSettings() const
{
  return mMapSettings;
}

void QfLocatorModelSuperBridge::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mapSettings == mMapSettings )
    return;

  mMapSettings = mapSettings;

  updateCanvasExtent( mMapSettings->extent() );
  updateCanvasCrs( mMapSettings->destinationCrs() );

  connect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, [this]() { updateCanvasExtent( mMapSettings->visibleExtent() ); } );
  connect( mMapSettings, &QgsQuickMapSettings::destinationCrsChanged, this, [this]() { updateCanvasCrs( mMapSettings->destinationCrs() ); } );

  emit mapSettingsChanged();
}

QObject *QfLocatorModelSuperBridge::geometryHighlighter() const
{
  return mGeometryHighlighter;
}

void QfLocatorModelSuperBridge::setGeometryHighlighter( QObject *geometryHighlighter )
{
  if ( mGeometryHighlighter == geometryHighlighter )
    return;

  mGeometryHighlighter = geometryHighlighter;
  emit geometryHighlighterChanged();
}

QfFeatureListExtentController *QfLocatorModelSuperBridge::featureListController() const
{
  return mFeatureListController;
}

void QfLocatorModelSuperBridge::setFeatureListController( QfFeatureListExtentController *featureListController )
{
  if ( featureListController == mFeatureListController )
    return;

  mFeatureListController = featureListController;
  emit featureListControllerChanged();
}

QgsMapLayer *QfLocatorModelSuperBridge::activeLayer() const
{
  return mActiveLayer.data();
}

void QfLocatorModelSuperBridge::setActiveLayer( QgsMapLayer *layer )
{
  if ( mActiveLayer == layer )
    return;

  mActiveLayer = layer;
  emit activeLayerChanged();
}

bool QfLocatorModelSuperBridge::keepScale() const
{
  return mKeepScale;
}

void QfLocatorModelSuperBridge::setKeepScale( bool keepScale )
{
  if ( keepScale == mKeepScale )
    return;

  mKeepScale = keepScale;
  emit keepScaleChanged();
}

void QfLocatorModelSuperBridge::requestSearch( const QString &text )
{
  emit searchRequested( text );
}

void QfLocatorModelSuperBridge::requestSearchTextChange( const QString &text )
{
  emit searchTextChangeRequested( text );
}

QfLocatorActionsModel *QfLocatorModelSuperBridge::contextMenuActionsModel( const int row )
{
  const QModelIndex index = proxyModel()->index( row, 0 );
  if ( !index.isValid() )
  {
    return nullptr;
  }

  const QList<QgsLocatorResult::ResultAction> actions = proxyModel()->data( index, static_cast<int>( QgsLocatorModel::CustomRole::ResultActions ) ).value<QList<QgsLocatorResult::ResultAction>>();
  QfLocatorActionsModel *model = new QfLocatorActionsModel( static_cast<int>( actions.count() ), 1 );
  int r = 0;
  for ( auto action : actions )
  {
    QStandardItem *item = new QStandardItem( action.text );
    item->setData( action.id, QfLocatorActionsModel::IdRole );
    item->setData( action.iconPath, QfLocatorActionsModel::IconPathRole );
    model->setItem( r, 0, item );
    r++;
  }

  return model;
}

QString QfLocatorModelSuperBridge::getLocatorModelDescription( const int row )
{
  const QModelIndex index = proxyModel()->index( row, 1 );
  if ( !index.isValid() )
    return nullptr;

  return proxyModel()->data( index, Qt::DisplayRole ).toString();
}

QString QfLocatorModelSuperBridge::getPrefixFromSearchString( const QString &string )
{
  QRegularExpression separatorRx( QStringLiteral( "^([^\\s]+)(?:\\s|$)" ) );
  QRegularExpressionMatch match = separatorRx.match( string.trimmed() );
  if ( match.hasMatch() )
  {
    if ( !locator()->filters( match.captured( 1 ) ).isEmpty() )
    {
      return match.captured( 1 );
    }
  }

  return QString();
}

void QfLocatorModelSuperBridge::emitMessage( const QString &text )
{
  emit messageEmitted( text );
}

void QfLocatorModelSuperBridge::triggerResultAtRow( const int row, const int id )
{
  const QModelIndex index = proxyModel()->index( row, 0 );
  if ( index.isValid() )
    triggerResult( index, id );
}

//
// QfLocatorActionsModel
//

QfLocatorActionsModel::QfLocatorActionsModel( QObject *parent )
  : QStandardItemModel( parent )
{
}

QfLocatorActionsModel::QfLocatorActionsModel( int rows, int columns, QObject *parent )
  : QStandardItemModel( rows, columns, parent )
{
}

QHash<int, QByteArray> QfLocatorActionsModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[IconPathRole] = "IconPath";
  roles[IdRole] = "Id";
  return roles;
}

//
// QfLocatorFiltersModel
//

QfLocatorFiltersModel::QfLocatorFiltersModel()
  : QAbstractListModel()
{
}

int QfLocatorFiltersModel::rowCount( const QModelIndex &parent ) const
{
  if ( !mLocatorModelSuperBridge || !mLocatorModelSuperBridge->locator() || parent.isValid() )
    return 0;

  return static_cast<int>( mLocatorModelSuperBridge->locator()->filters().count() );
}

QHash<int, QByteArray> QfLocatorFiltersModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();

  roles[NameRole] = "Name";
  roles[DescriptionRole] = "Description";
  roles[PrefixRole] = "Prefix";
  roles[ActiveRole] = "Active";
  roles[DefaultRole] = "Default";

  return roles;
}

QVariant QfLocatorFiltersModel::data( const QModelIndex &index, int role ) const
{
  if ( !mLocatorModelSuperBridge->locator() || !index.isValid() || index.parent().isValid() || index.row() < 0 || index.row() >= rowCount( QModelIndex() ) )
    return QVariant();

  switch ( role )
  {
    case Qt::DisplayRole:
    case NameRole:
      return filterForIndex( index )->displayName();

    case DescriptionRole:
      return filterForIndex( index )->description();

    case PrefixRole:
      return filterForIndex( index )->activePrefix();

    case ActiveRole:
      return QVariant( filterForIndex( index )->enabled() );

    case DefaultRole:
      return filterForIndex( index )->useWithoutPrefix();
  }

  return QVariant();
}

bool QfLocatorFiltersModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
  if ( !mLocatorModelSuperBridge || !mLocatorModelSuperBridge->locator() || !index.isValid() || index.parent().isValid() || index.row() < 0 || index.row() >= rowCount( QModelIndex() ) )
    return false;

  switch ( role )
  {
    case NameRole:
    case PrefixRole:
      return false;

    case ActiveRole:
    {
      QgsLocatorFilter *filter = filterForIndex( index );
      const bool newValue = value.toBool();
      if ( filter->enabled() != newValue )
      {
        QgsSettings settings;
        filter->setEnabled( newValue );
        settings.setValue( QStringLiteral( "locator_filters/enabled_%1" ).arg( filter->name() ), newValue, QgsSettings::Section::Gui );
        emit dataChanged( index, index, QVector<int>() << ActiveRole );
        return true;
      }
      break;
    }

    case DefaultRole:
    {
      QgsLocatorFilter *filter = filterForIndex( index );
      const bool newValue = value.toBool();
      if ( filter->useWithoutPrefix() != newValue )
      {
        QgsSettings settings;
        filter->setUseWithoutPrefix( value.toBool() );
        settings.setValue( QStringLiteral( "locator_filters/default_%1" ).arg( filter->name() ), newValue, QgsSettings::Section::Gui );
        settings.setValue( QStringLiteral( "locator_filters/default_touched_%1" ).arg( filter->name() ), true, QgsSettings::Section::Gui );
        emit dataChanged( index, index, QVector<int>() << DefaultRole );
        return true;
      }
      break;
    }
  }

  return false;
}

void QfLocatorFiltersModel::setGeocoderLocatorFiltersDefaulByPosition( const QfGnssPositionInformation &position )
{
  if ( !mLocatorModelSuperBridge || !mLocatorModelSuperBridge->locator() )
    return;

  QgsPointXY point( position.longitude(), position.latitude() );
  int i = 0;
  for ( QgsLocatorFilter *filter : mLocatorModelSuperBridge->locator()->filters() )
  {
    QfFinlandLocatorFilter *f = dynamic_cast<QfFinlandLocatorFilter *>( filter );
    if ( f )
    {
      if ( f->boundingBox().contains( point ) )
      {
        QgsSettings settings;
        bool filterTouched = settings.value( QStringLiteral( "locator_filters/default_touched_%1" ).arg( f->name() ), false, QgsSettings::Section::Gui ).toBool();
        bool filterDefault = settings.value( QStringLiteral( "locator_filters/default_%1" ).arg( f->name() ), false, QgsSettings::Section::Gui ).toBool();
        if ( !filterTouched && !filterDefault )
        {
          f->setUseWithoutPrefix( true );
          settings.setValue( QStringLiteral( "locator_filters/default_%1" ).arg( filter->name() ), true, QgsSettings::Section::Gui );
          QModelIndex modifiedIndex = index( i, 0 );
          emit dataChanged( modifiedIndex, modifiedIndex, QVector<int>() << DefaultRole );
          emit mLocatorModelSuperBridge->emitMessage( tr( "Search filters for your locations have been activated, customize results in the settings panel" ) );
        }
      }
    }
    i++;
  }
}

QgsLocatorFilter *QfLocatorFiltersModel::filterForIndex( const QModelIndex &index ) const
{
  if ( !mLocatorModelSuperBridge || !mLocatorModelSuperBridge->locator() )
    return nullptr;

  return mLocatorModelSuperBridge->locator()->filters().at( index.row() );
}

QfLocatorModelSuperBridge *QfLocatorFiltersModel::locatorModelSuperBridge() const
{
  return mLocatorModelSuperBridge;
}

void QfLocatorFiltersModel::locatorFiltersChanged()
{
  emit beginResetModel();
  emit endResetModel();
}

void QfLocatorFiltersModel::setLocatorModelSuperBridge( QfLocatorModelSuperBridge *locatorModelSuperBridge )
{
  if ( mLocatorModelSuperBridge == locatorModelSuperBridge )
    return;

  if ( mLocatorModelSuperBridge )
  {
    disconnect( mLocatorModelSuperBridge, &QfLocatorModelSuperBridge::locatorFiltersChanged, this, &QfLocatorFiltersModel::locatorFiltersChanged );
  }
  emit beginResetModel();
  mLocatorModelSuperBridge = locatorModelSuperBridge;
  emit locatorModelSuperBridgeChanged();
  emit endResetModel();

  connect( mLocatorModelSuperBridge, &QfLocatorModelSuperBridge::locatorFiltersChanged, this, &QfLocatorFiltersModel::locatorFiltersChanged );
}
```


