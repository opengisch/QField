/***************************************************************************
  vertexmodel.h - VertexModel

 ---------------------
 begin                : 18.08.2018
 copyright            : (C) 2018 by Denis Rouzaud
 email                : denis@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef VERTEXMODEL_H
#define VERTEXMODEL_H

#include <QStandardItemModel>

class QgsGeometry;

#include "qgscoordinatereferencesystem.h"

class VertexModel : public QStandardItemModel
{
    Q_OBJECT

  public:
    enum ColumnRole
    {
      PointRole = Qt::UserRole + 1,
    };

    explicit VertexModel( QObject* parent = nullptr );
    ~VertexModel() = default;

    Q_INVOKABLE void setGeometry( const QgsGeometry &geometry, const QgsCoordinateReferenceSystem &crs );

    QHash<int, QByteArray> roleNames() const;
};

#endif // VERTEXMODEL_H
