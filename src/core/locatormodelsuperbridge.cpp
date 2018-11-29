
#include <QStandardItem>

#include <qgslocatormodel.h>

#include "locatormodelsuperbridge.h"
#include "qgsquickmapsettings.h"
#include "featureslocatorfilter.h"
#include "qgslocator.h"

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

LocatorHighlight *LocatorModelSuperBridge::locatorHighlight() const
{
  return mLocatorHighlight;
}

void LocatorModelSuperBridge::setLocatorHighlight( LocatorHighlight *locatorHighlight )
{
  if ( locatorHighlight == mLocatorHighlight )
    return;

  mLocatorHighlight = locatorHighlight;
  emit locatorHighlightChanged();
}

MultiFeatureListModel *LocatorModelSuperBridge::model() const
{
  return mModel;
}

void LocatorModelSuperBridge::setModel( MultiFeatureListModel *model )
{
  if ( model == mModel )
    return;

  mModel = model;
  emit modelChanged();
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
    item->setData( action.iconPath, LocatorActionsModel::IconRole );
    model->setItem( r, 0, item );
    r++;
  }

  return model;
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
  roles[IconRole] = "icon";
  roles[IdRole] = "id";
  return roles;
}
