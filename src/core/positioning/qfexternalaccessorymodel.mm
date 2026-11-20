/***************************************************************************
  qfexternalaccessorymodel.mm - QfExternalAccessoryModel

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

#include "qfexternalaccessorymodel.h"

#include <ExternalAccessory/ExternalAccessory.h>

QfExternalAccessoryModel::QfExternalAccessoryModel(QObject *parent)
    : QAbstractListModel(parent) {}

void QfExternalAccessoryModel::refresh() {
  beginResetModel();
  mAccessories.clear();
  for (EAAccessory *connectedAccessory in
       [[EAAccessoryManager sharedAccessoryManager] connectedAccessories]) {
    Accessory accessory;
    accessory.name = QString::fromNSString(connectedAccessory.name);
    accessory.manufacturer =
        QString::fromNSString(connectedAccessory.manufacturer);
    accessory.modelNumber =
        QString::fromNSString(connectedAccessory.modelNumber);
    accessory.serialNumber =
        QString::fromNSString(connectedAccessory.serialNumber);
    for (NSString *protocol in connectedAccessory.protocolStrings) {
      accessory.protocols << QString::fromNSString(protocol);
    }
    mAccessories << accessory;
  }
  endResetModel();
}

int QfExternalAccessoryModel::findIndexFromSerialNumber(
    const QString &serialNumber) const {
  for (int i = 0; i < mAccessories.size(); i++) {
    if (mAccessories.at(i).serialNumber == serialNumber) {
      return i;
    }
  }

  return -1;
}

int QfExternalAccessoryModel::rowCount(const QModelIndex &parent) const {
  return !parent.isValid() ? static_cast<int>(mAccessories.size()) : 0;
}

QVariant QfExternalAccessoryModel::data(const QModelIndex &index,
                                        int role) const {
  if (!index.isValid() || index.row() >= mAccessories.size()) {
    return QVariant();
  }

  const Accessory &accessory = mAccessories.at(index.row());
  switch (role) {
  case Qt::DisplayRole:
    return !accessory.serialNumber.isEmpty()
               ? QStringLiteral("%1 (%2)").arg(accessory.name,
                                               accessory.serialNumber)
               : accessory.name;

  case NameRole:
    return accessory.name;

  case ManufacturerRole:
    return accessory.manufacturer;

  case ModelNumberRole:
    return accessory.modelNumber;

  case SerialNumberRole:
    return accessory.serialNumber;

  case ProtocolsRole:
    return accessory.protocols;
  }

  return QVariant();
}

QHash<int, QByteArray> QfExternalAccessoryModel::roleNames() const {
  QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();

  roles[NameRole] = "Name";
  roles[ManufacturerRole] = "Manufacturer";
  roles[ModelNumberRole] = "ModelNumber";
  roles[SerialNumberRole] = "SerialNumber";
  roles[ProtocolsRole] = "Protocols";

  return roles;
}
