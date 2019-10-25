/***************************************************************************
  geometryeditorsmodel.h

 ---------------------
 begin                : 24.09.2019
 copyright            : (C) 2019 by Denis Rouzaud
 email                : denis@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef GEOMETRYEDITORSMODEL_H
#define GEOMETRYEDITORSMODEL_H



#include <QStandardItemModel>
#include <QQuickItem>

#include "vertexmodel.h"


class GeometryEditorsModel : public QStandardItemModel
{
    Q_OBJECT

    Q_PROPERTY( VertexModel *vertexModel READ vertexModel WRITE setVertexModel NOTIFY vertexModelChanged )

  public:
    enum ActionRoles
    {
      NameRole = Qt::UserRole + 1,
      IconPathRole,
      ToolbarRole,
      SupportedGeometriesRole
    };

    enum GeometryType
    {
      Point = 1,
      Line = 2,
      Polygon = 4,
      AllGeometries = Point | Line | Polygon
    };

    Q_DECLARE_FLAGS( SupportedGeometries, GeometryType )
    Q_FLAG( SupportedGeometries )


    explicit GeometryEditorsModel( QObject *parent = nullptr );
    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE void addEditor( const QString &name, const QString &icon, const QString &toolbar, SupportedGeometries supportedGeometries = AllGeometries );

    Q_INVOKABLE static bool supportsGeometry( const QgsGeometry &geometry, const SupportedGeometries &supportedGeometries );

    VertexModel *vertexModel() const {return mVertexModel;}
    void setVertexModel( VertexModel *vertexModel );

  signals:
    void vertexModelChanged();
  private:
    VertexModel *mVertexModel = nullptr;

    // QAbstractItemModel interface
  public:
    QVariant data( const QModelIndex &index, int role ) const override;
};


#endif // GEOMETRYEDITORSMODEL_H
