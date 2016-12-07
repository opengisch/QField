/***************************************************************************
  layertreemodel.h - LayerTree

 ---------------------
 begin                : 6.12.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef LAYERTREEMODEL_H
#define LAYERTREEMODEL_H

#include <QSortFilterProxyModel>

class QgsLayerTreeGroup;
class QgsLayerTreeModel;

class LayerTreeModel : public QSortFilterProxyModel
{
    Q_OBJECT
  public:
    enum Roles
    {
      VectorLayer = Qt::UserRole + 1
    };
    Q_ENUMS( Roles )

    explicit LayerTreeModel( QgsLayerTreeGroup* rootNode, QObject* parent = nullptr );

    QVariant data( const QModelIndex& index, int role ) const override;

    QHash<int, QByteArray> roleNames() const override;

  private:
    QgsLayerTreeModel* mLayerTreeModel;
};

#endif // LAYERTREEMODEL_H
