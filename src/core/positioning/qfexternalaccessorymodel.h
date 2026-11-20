/***************************************************************************
  qfexternalaccessorymodel.h - QfExternalAccessoryModel

 ---------------------
 begin                : November 2026
 copyright            : (C) 2026 by Mohsen Dehghanzadeh
 email                : mohsen@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFEXTERNALACCESSORYMODEL_H
#define QFEXTERNALACCESSORYMODEL_H

#include <QAbstractListModel>

/**
 * A model that provides the external accessories connected to an iOS device.
 * \ingroup core
 */
class QfExternalAccessoryModel : public QAbstractListModel
{
    Q_OBJECT

  public:
    //! The roles provided by this model
    enum Roles
    {
      NameRole = Qt::UserRole + 1,
      ManufacturerRole,
      ModelNumberRole,
      SerialNumberRole,
      ProtocolsRole,
    };
    Q_ENUM( Roles )

    explicit QfExternalAccessoryModel( QObject *parent = nullptr );

    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;

    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;

    QHash<int, QByteArray> roleNames() const override;

    /**
     * Refresh the list of connected accessories.
     */
    Q_INVOKABLE void refresh();

    /**
     * Returns the row index for a given accessory \a serialNumber.
     */
    Q_INVOKABLE int findIndexFromSerialNumber( const QString &serialNumber ) const;

  private:
    struct Accessory
    {
        QString name;
        QString manufacturer;
        QString modelNumber;
        QString serialNumber;
        QStringList protocols;
    };

    QList<Accessory> mAccessories;
};

#endif // QFEXTERNALACCESSORYMODEL_H
