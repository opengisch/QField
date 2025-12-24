/***************************************************************************
                              cogooperationsmodel.h
                              -------------------
              begin                : 2025.12.21
              copyright            : (C) 2025 by Mathieu Pellerin
              email                : mathieu (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef COGOOPERATIONSMODEL_H
#define COGOOPERATIONSMODEL_H

#include "cogooperation.h"

#include <QAbstractListModel>

/**
 * \ingroup core
 */
class CogoOperationsModel : public QAbstractListModel
{
    Q_OBJECT

  public:
    enum OperationRoles
    {
      NameRole = Qt::UserRole + 1,
      DisplayNameRole,
      IconRole,
      ParametersRole,
    };
    Q_ENUM( OperationRoles )

    explicit CogoOperationsModel( QObject *parent = nullptr );

    void buildModel();

    Q_INVOKABLE QVariantMap get( int row ) const;
    Q_INVOKABLE QVariantMap get( const QString &name ) const;

    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;
    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;
    QHash<int, QByteArray> roleNames() const override;

  private:
    QStringList mOperationNames;
};

#endif // COGOOPERATIONSMODEL_H
