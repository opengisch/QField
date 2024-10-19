/***************************************************************************
  positioningdevicemodel.h

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
#ifndef POSITIONINGDEVICEMODEL_H
#define POSITIONINGDEVICEMODEL_H

#include <QAbstractListModel>

/**
 * \ingroup core
 */
class PositioningDeviceModel : public QAbstractListModel
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


    explicit PositioningDeviceModel( QObject *parent = nullptr );

    QHash<int, QByteArray> roleNames() const override;

    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;

    /**
     * Reloads the model from the user settings
     */
    Q_INVOKABLE void reloadModel();

    /**
     * Adds a positioning device to the user settings
     * \param type device type
     * \param name friendly device name used as identifier in the user interface
     * \param deviceSettings settings map (used to generate the positioning device ID, editing, etc.)
     * \returns returns index of the added device
     */
    Q_INVOKABLE int addDevice( const Type &type, const QString &name, const QVariantMap &deviceSettings );

    /**
     * Removes the positioning device \a name from the user settings
     */
    Q_INVOKABLE void removeDevice( const QString &name );

    /**
     * Returns the device ID string for a specific \a device
     * \note this is the string to be used with the Positioning deviceId property
     */
    Q_INVOKABLE const QString deviceId( const Device &device ) const;

    /**
     * Returns the row index for a given device ID
     */
    Q_INVOKABLE int findIndexFromDeviceId( const QString &id );

  private:
    QList<Device> mDevices;
};

#endif // POSITIONINGDEVICEMODEL_H
