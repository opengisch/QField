

# File qfpositioningdevicemodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**qfpositioningdevicemodel.cpp**](qfpositioningdevicemodel_8cpp.md)

[Go to the documentation of this file](qfpositioningdevicemodel_8cpp.md)


```C++
/***************************************************************************
  qfpositioningdevicemodel.cpp

 ---------------------
 begin                : 24.12.2022
 copyright            : (C) 2022 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfegenioussreceiver.h"
#include "qffilereceiver.h"
#include "qfpositioningdevicemodel.h"
#include "qftcpreceiver.h"
#include "qfudpreceiver.h"
#ifdef WITH_SERIALPORT
#include "qfserialportreceiver.h"
#endif
#ifdef WITH_BLUETOOTH
#include "qfbluetoothlowenergyreceiver.h"
#endif

#include <QSettings>

QfPositioningDeviceModel::QfPositioningDeviceModel( QObject *parent )
  : QAbstractListModel( parent )
{
  reloadModel();
}

QHash<int, QByteArray> QfPositioningDeviceModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
  roles[DeviceType] = "DeviceType";
  roles[DeviceId] = "DeviceId";
  roles[DeviceName] = "DeviceName";
  roles[DeviceSettings] = "DeviceSettings";

  return roles;
}

void QfPositioningDeviceModel::reloadModel()
{
  beginResetModel();
  mDevices.clear();

  // Add internal device, assume it is always present
  mDevices << Device( InternalDevice, tr( "Internal device" ), QVariantMap() );

  QSettings settings;
  settings.beginGroup( "/qfield/positioningDevices" );
  const QStringList deviceKeys = settings.childGroups();
  for ( int i = 0; i < deviceKeys.count(); i++ )
  {
    settings.beginGroup( deviceKeys.at( i ) );
    mDevices << Device( static_cast<Type>( settings.value( QStringLiteral( "type" ), InternalDevice ).toInt() ),
                        QUrl::fromPercentEncoding( deviceKeys.at( i ).toLatin1() ),
                        settings.value( QStringLiteral( "settings" ), QVariantMap() ).toMap() );
    settings.endGroup();
  }
  settings.endGroup();

  endResetModel();
}

int QfPositioningDeviceModel::rowCount( const QModelIndex &parent ) const
{
  return !parent.isValid() ? static_cast<int>( mDevices.size() ) : 0;
}

QVariant QfPositioningDeviceModel::data( const QModelIndex &index, int role ) const
{
  if ( index.row() >= mDevices.size() || index.row() < 0 )
    return QVariant();

  switch ( role )
  {
    case Qt::DisplayRole:
      return mDevices.at( index.row() ).name;

    case DeviceType:
      return mDevices.at( index.row() ).type;

    case DeviceId:
      return deviceId( mDevices.at( index.row() ) );

    case DeviceName:
      return mDevices.at( index.row() ).name;

    case DeviceSettings:
      return mDevices.at( index.row() ).settings;
  }

  return QVariant();
}

int QfPositioningDeviceModel::addDevice( const Type &type, const QString &name, const QVariantMap &deviceSettings )
{
  if ( name.isEmpty() )
    return -1;

  QSettings settings;
  settings.beginGroup( "/qfield/positioningDevices" );
  const QStringList deviceKeys = settings.childGroups();

  QString uniqueName = name;
  int suffix = 0;
  while ( deviceKeys.contains( uniqueName ) )
  {
    uniqueName = QStringLiteral( "%1 %2" ).arg( name, QString::number( ++suffix ) );
  }

  settings.beginGroup( QUrl::toPercentEncoding( uniqueName ) );
  settings.setValue( "type", static_cast<int>( type ) );
  settings.setValue( "settings", deviceSettings );
  settings.endGroup();

  settings.endGroup();

  const int index = static_cast<int>( mDevices.size() );
  beginInsertRows( QModelIndex(), index, index );
  Device device( type, uniqueName, deviceSettings );
  mDevices << device;
  endInsertRows();

  return index;
}

void QfPositioningDeviceModel::removeDevice( const QString &name )
{
  if ( name.isEmpty() )
    return;

  QSettings settings;
  settings.beginGroup( "/qfield/positioningDevices" );
  const QStringList deviceKeys = settings.childGroups();
  const QString encodedName = QUrl::toPercentEncoding( name );
  if ( deviceKeys.contains( encodedName ) )
  {
    // Remove pre-existing positioning device details
    settings.beginGroup( encodedName );
    settings.remove( QString() );
    settings.endGroup();
  }
  settings.endGroup();

  for ( int i = 0; i < mDevices.size(); i++ )
  {
    if ( mDevices.at( i ).name == name )
    {
      beginRemoveRows( QModelIndex(), i, i );
      mDevices.removeAt( i );
      endRemoveRows();
    }
  }
}

const QString QfPositioningDeviceModel::deviceId( const Device &device ) const
{
  switch ( device.type )
  {
    case InternalDevice:
      return QString();

    case BluetoothDevice:
#ifdef WITH_BLUETOOTH
      if ( device.settings.value( QStringLiteral( "ble" ) ).toBool() )
      {
        return QStringLiteral( "%1:%2" ).arg( QfBluetoothLowEnergyReceiver::identifier, device.settings.value( QStringLiteral( "address" ) ).toString() );
      }
#endif
      return QStringLiteral( "%1" ).arg( device.settings.value( QStringLiteral( "address" ) ).toString() );

    case TcpDevice:
      return QStringLiteral( "%1:%2:%3" ).arg( QfTcpReceiver::identifier, device.settings.value( QStringLiteral( "address" ) ).toString(), QString::number( device.settings.value( QStringLiteral( "port" ) ).toInt() ) );

    case UdpDevice:
      return QStringLiteral( "%1:%2:%3" ).arg( QfUdpReceiver::identifier, device.settings.value( QStringLiteral( "address" ) ).toString(), QString::number( device.settings.value( QStringLiteral( "port" ) ).toInt() ) );

    case FileDevice:
      return QStringLiteral( "%1:%2:%3" ).arg( QfFileReceiver::identifier, device.settings.value( QStringLiteral( "filePath" ) ).toString(), QString::number( device.settings.value( QStringLiteral( "interval" ) ).toInt() ) );

#ifdef WITH_SERIALPORT
    case SerialPortDevice:
      return QStringLiteral( "%1:%2" ).arg( QfSerialPortReceiver::identifier, device.settings.value( QStringLiteral( "address" ) ).toString() );
#endif

    case EgenioussDevice:
      return QStringLiteral( "%1:%2:%3" ).arg( QfEgenioussReceiver::identifier, device.settings.value( QStringLiteral( "address" ) ).toString(), QString::number( device.settings.value( QStringLiteral( "port" ) ).toInt() ) );
  }

  return QString();
}

int QfPositioningDeviceModel::findIndexFromDeviceId( const QString &id )
{
  if ( id.isEmpty() )
  {
    // empty ID is the internal device, hard-coded to be the first row
    return 0;
  }

  for ( int i = 0; i < mDevices.size(); i++ )
  {
    if ( deviceId( mDevices.at( i ) ) == id )
    {
      return i;
    }
  }

  return -1;
}
```


