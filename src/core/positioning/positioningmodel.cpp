#include "positioningmodel.h"

#include <QVariant>

PositioningModel::PositioningModel( QObject *parent )
  : QStandardItemModel( parent )
{
  connect( this, &QStandardItemModel::dataChanged, this, &PositioningModel::onDataChanged );
}

void PositioningModel::setupConnections()
{
  connect( mPositioningSource, &Positioning::positionInformationChanged, this, &PositioningModel::refreshData );
}

void PositioningModel::refreshData()
{
  if ( !mPositioningSource )
  {
    return;
  }

  QList<QPair<QString, QVariant>> details = mPositioningSource->device()->details();

  for ( int i = 0; i < details.size(); ++i )
  {
    const QString key = details[i].first;
    const QVariant value = details[i].second;

    updateInfo( key, value );
  }
}

void PositioningModel::updateInfo( const QString &name, const QVariant &value )
{
  // Check if the item already exists
  for ( int row = 0; row < rowCount(); ++row )
  {
    QStandardItem *rowItem = item( row );
    if ( rowItem->data( VariableNameRole ).toString() == name )
    {
      // Item exists, update its value
      rowItem->setData( value.toString(), VariableValueRole );
      return;
    }
  }

  QStandardItem *nameItem = new QStandardItem( name );
  nameItem->setData( name, VariableNameRole );
  nameItem->setData( value.toString(), VariableValueRole );
  insertRow( rowCount(), QList<QStandardItem *>() << nameItem );
}


bool PositioningModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
  QStandardItem *rowItem = item( index.row() );
  if ( !rowItem )
  {
    return false;
  }

  switch ( role )
  {
    case VariableNameRole:
      if ( rowItem->data( VariableNameRole ) == value )
      {
        return false;
      }

      rowItem->setData( value, VariableNameRole );
      return true;

    case VariableValueRole:

      if ( rowItem->data( VariableValueRole ) == value )
      {
        return false;
      }

      rowItem->setData( value, VariableValueRole );
      return true;

    default:
      break;
  }

  return false;
}

QHash<int, QByteArray> PositioningModel::roleNames() const
{
  QHash<int, QByteArray> names = QStandardItemModel::roleNames();
  names[VariableNameRole] = "VariableName";
  names[VariableValueRole] = "VariableValue";
  return names;
}

void PositioningModel::onDataChanged( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles )
{
  Q_UNUSED( bottomRight )
  Q_UNUSED( roles )
}

Positioning *PositioningModel::positioningSource() const
{
  return mPositioningSource;
}

void PositioningModel::setPositioningSource( Positioning *newPositioningSource )
{
  if ( mPositioningSource == newPositioningSource )
    return;
  mPositioningSource = newPositioningSource;
  emit positioningSourceChanged();
  refreshData();
}
