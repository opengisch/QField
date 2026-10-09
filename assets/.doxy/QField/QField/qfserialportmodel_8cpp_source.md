

# File qfserialportmodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**qfserialportmodel.cpp**](qfserialportmodel_8cpp.md)

[Go to the documentation of this file](qfserialportmodel_8cpp.md)


```C++
/***************************************************************************
  qfserialportmodel.cpp - QfSerialPortModel

 ---------------------
 begin                : 15.01.2023
 copyright            : (C) 2023 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfserialportmodel.h"

QfSerialPortModel::QfSerialPortModel( QObject *parent )
  : QAbstractListModel( parent )
{
}

void QfSerialPortModel::refresh()
{
  beginResetModel();
  mAvailablePorts = QSerialPortInfo::availablePorts();
  endResetModel();
}

int QfSerialPortModel::findIndexFromName( const QString &name ) const
{
  for ( int i = 0; i < mAvailablePorts.size(); i++ )
  {
    if ( mAvailablePorts.at( i ).portName() == name )
    {
      return i;
    }
  }

  return -1;
}

int QfSerialPortModel::rowCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent )
  return static_cast<int>( mAvailablePorts.size() );
}

QVariant QfSerialPortModel::data( const QModelIndex &index, int role ) const
{
  if ( index.row() >= mAvailablePorts.size() )
  {
    return QVariant();
  }

  switch ( role )
  {
    case Qt::DisplayRole:
    {
      const QString portName = mAvailablePorts.at( index.row() ).portName();
      const QString description = mAvailablePorts.at( index.row() ).description();

      return !description.isEmpty() ? QStringLiteral( "%1 (%2)" ).arg( description, portName ) : portName;
    }

    case PortNameRole:
      return mAvailablePorts.at( index.row() ).portName();
      break;

    case DescriptionRole:
      return mAvailablePorts.at( index.row() ).description();
      break;
  }

  return QVariant();
}

QHash<int, QByteArray> QfSerialPortModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();

  roles[PortNameRole] = "PortName";
  roles[DescriptionRole] = "Description";

  return roles;
}
```


