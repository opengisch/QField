/***************************************************************************
  modelhelpers.h - ModelHelpers

 ---------------------
 begin                : 24.2.2016
 copyright            : (C) 2016 by Matthias Kuhn, OPENGIS.ch
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef MODELHELPERS_H
#define MODELHELPERS_H

#include <QObject>
#include <QAbstractItemModel>

class ModelHelpers : public QObject
{
    Q_OBJECT
  public:
    explicit ModelHelpers(QObject *parent = 0);

    Q_INVOKABLE QModelIndex index( QAbstractItemModel* model, int row, int column )
    {
      return model->index( row, column );
    }

    Q_INVOKABLE int role( QAbstractItemModel* model, QString roleName )
    {
      return model->roleNames().key( roleName.toLatin1() );
    }

    Q_INVOKABLE QVariant data( QAbstractItemModel* model, int row, int column, QString roleName )
    {
      return model->data( model->index( row, column ), model->roleNames().key( roleName.toLatin1() ) );
    }
};

#endif // MODELHELPERS_H
