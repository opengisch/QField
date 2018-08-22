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
      CurrentVertexRole,
    };

    explicit VertexModel( QObject* parent = nullptr );
    ~VertexModel() override = default;

    Q_INVOKABLE void setGeometry( const QgsGeometry &geometry, const QgsCoordinateReferenceSystem &crs );

    Q_INVOKABLE void clear();

    Q_INVOKABLE bool isEmtpy();

    Q_INVOKABLE void previousVertex();
    Q_INVOKABLE void nextVertex();

    QHash<int, QByteArray> roleNames() const override;

  private:
    void updateCurrentVertex();
    int mCurrentVertex = -1;


};

#endif // VERTEXMODEL_H
