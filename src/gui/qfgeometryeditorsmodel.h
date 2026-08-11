/***************************************************************************
  qfgeometryeditorsmodel.h

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

#ifndef QFGEOMETRYEDITORSMODEL_H
#define QFGEOMETRYEDITORSMODEL_H


#include "qfvertexmodel.h"

#include <QQuickItem>
#include <QStandardItemModel>

/**
 * \ingroup core
 */
class QfGeometryEditorsModel : public QStandardItemModel
{
    Q_OBJECT

    Q_PROPERTY( QfVertexModel *vertexModel READ vertexModel WRITE setVertexModel NOTIFY vertexModelChanged )

  public:
    enum ActionRoles
    {
      NameRole = Qt::UserRole + 1,
      IconPathRole,
      ToolbarRole,
      SupportedGeometriesRole
    };
    Q_ENUM( ActionRoles )

    enum GeometryType
    {
      Point = 1,
      Line = 2,
      Polygon = 4,
      AllGeometries = Point | Line | Polygon
    };

    Q_DECLARE_FLAGS( SupportedGeometries, GeometryType )
    Q_FLAG( SupportedGeometries )


    explicit QfGeometryEditorsModel( QObject *parent = nullptr );
    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE void addEditor( const QString &name, const QString &icon, const QString &toolbar, SupportedGeometries supportedGeometries = AllGeometries );

    Q_INVOKABLE static bool supportsGeometry( const QgsGeometry &geometry, const SupportedGeometries &supportedGeometries );

    QfVertexModel *vertexModel() const { return mVertexModel; }
    void setVertexModel( QfVertexModel *vertexModel );

  signals:
    void vertexModelChanged();

  private:
    QfVertexModel *mVertexModel = nullptr;
};


#endif // QFGEOMETRYEDITORSMODEL_H
