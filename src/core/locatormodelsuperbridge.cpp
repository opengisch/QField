
#include <QStandardItem>

#include <qgslocatormodel.h>

#include "locatormodelsuperbridge.h"

LocatorModelSuperBridge::LocatorModelSuperBridge( QObject *parent )
  : QgsLocatorModelBridge( parent )
{
}

QStandardItemModel *LocatorModelSuperBridge::contextMenuActionsModel( const int row )
{
  const QModelIndex index = proxyModel()->index( row, 0 );
  if ( !index.isValid() )
    return nullptr;


  const QMap<int, QAction *> actionsMap = proxyModel()->data( index, QgsLocatorModel::ResultContextMenuActionsRole ).value<QMap<int, QAction *>>();
  int r = 0;
  QStandardItemModel *model = new QStandardItemModel( actionsMap.count(), 1 );
  QMap<int, QAction *>::const_iterator it = actionsMap.constBegin();
  for ( ; it != actionsMap.constEnd(); ++it )
  {
    QStandardItem *item = new QStandardItem( it.value()->text() );
    model->setItem( r, 0, item );
    r++;
  }

  return model;
}

void LocatorModelSuperBridge::triggerResultAtRow( const  int row )
{
  const QModelIndex index = proxyModel()->index( row, 0 );
  if ( index.isValid() )
    triggerResult( index );
}
