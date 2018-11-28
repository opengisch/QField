
#include <QStandardItem>
#include <QDebug>

#include <qgslocatormodel.h>

#include "locatormodelsuperbridge.h"

LocatorModelSuperBridge::LocatorModelSuperBridge( QObject *parent )
  : QgsLocatorModelBridge( parent )
{
}

LocatorActionsModel *LocatorModelSuperBridge::contextMenuActionsModel( const int row )
{
  const QModelIndex index = proxyModel()->index( row, 0 );
  if ( !index.isValid() )
    return nullptr;

  const QMap<int, QAction *> actionsMap = proxyModel()->data( index, QgsLocatorModel::ResultContextMenuActionsRole ).value<QMap<int, QAction *>>();
  int r = 0;
  LocatorActionsModel *model = new LocatorActionsModel( actionsMap.count(), 1 );
  QMap<int, QAction *>::const_iterator it = actionsMap.constBegin();
  for ( ; it != actionsMap.constEnd(); ++it )
  {
    QStandardItem *item = new QStandardItem( it.value()->text() );
    item->setData( it.value()->data().toString(), LocatorActionsModel::IconRole );
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
  return roles;
}
