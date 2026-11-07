

# File qfpositioningdevicemodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**qfpositioningdevicemodel.h**](qfpositioningdevicemodel_8h.md)

[Go to the documentation of this file](qfpositioningdevicemodel_8h.md)


```C++
/***************************************************************************
  qfpositioningdevicemodel.h

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
#ifndef QFPOSITIONINGDEVICEMODEL_H
#define QFPOSITIONINGDEVICEMODEL_H

#include <QAbstractListModel>

class QfPositioningDeviceModel : public QAbstractListModel
{
    Q_OBJECT

  public:
    enum Type
    {
      InternalDevice,
      BluetoothDevice,
      TcpDevice,
      UdpDevice,
      EgenioussDevice,
      SerialPortDevice,
      FileDevice,
    };
    Q_ENUM( Type )

    struct Device
    {
        Device() = default;

        Device( Type type, const QString &name, const QVariantMap &settings )
          : type( type )
          , name( name )
          , settings( settings )
        {}

        Type type = Type::InternalDevice;
        QString name;
        QVariantMap settings;
    };

    enum Role
    {
      DeviceType = Qt::UserRole,
      DeviceId,
      DeviceName,
      DeviceSettings,
    };
    Q_ENUM( Role )


    explicit QfPositioningDeviceModel( QObject *parent = nullptr );

    QHash<int, QByteArray> roleNames() const override;

    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;

    Q_INVOKABLE void reloadModel();

    Q_INVOKABLE int addDevice( const Type &type, const QString &name, const QVariantMap &deviceSettings );

    Q_INVOKABLE void removeDevice( const QString &name );

    Q_INVOKABLE const QString deviceId( const Device &device ) const;

    Q_INVOKABLE int findIndexFromDeviceId( const QString &id );

  private:
    QList<Device> mDevices;
};

#endif // QFPOSITIONINGDEVICEMODEL_H
```


