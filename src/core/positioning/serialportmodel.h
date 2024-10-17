/***************************************************************************
  serialportmodel.h - SerialPortModel

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

#ifndef SERIALPORTMODEL_H
#define SERIALPORTMODEL_H

#include <QAbstractListModel>
#include <QSerialPortInfo>

/**
 * A model that provides serial ports available on the device.
 * \ingroup core
 */
class SerialPortModel : public QAbstractListModel
{
    Q_OBJECT

  public:
    //! The roles provided by this model
    enum Roles
    {
      PortNameRole = Qt::UserRole + 1,
      DescriptionRole,
    };
    Q_ENUM( Roles )

    explicit SerialPortModel( QObject *parent = nullptr );

    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;

    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;

    QHash<int, QByteArray> roleNames() const override;

    /**
     * Refresh the list of available serial ports.
     */
    Q_INVOKABLE void refresh();

    /**
     * Returns the row index for a given serial port \a name.
     */
    Q_INVOKABLE int findIndexFromName( const QString &name ) const;

  private:
    QList<QSerialPortInfo> mAvailablePorts;
};

#endif // SERIALPORTMODEL_H
