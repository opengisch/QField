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

#include "qgspoint.h"

class VertexModel : public QStandardItemModel
{
    Q_OBJECT
    Q_PROPERTY( EditingMode editingMode READ editingMode NOTIFY editingModeChanged )
    Q_PROPERTY( QgsPoint currentPoint READ currentPoint WRITE setCurrentPoint NOTIFY currentPointChanged )

  public:
    enum ColumnRole
    {
      PointRole = Qt::UserRole + 1,
      CurrentVertexRole,
    };

    enum EditingMode
    {
      NoEditing,
      EditVertex,
      AddVertex
    };

    explicit VertexModel( QObject* parent = nullptr );
    ~VertexModel() override = default;

    Q_INVOKABLE void setGeometry( const QgsGeometry &geometry, const QgsCoordinateReferenceSystem &crs );

    Q_INVOKABLE void clear();

    Q_INVOKABLE bool isEmtpy();

    Q_INVOKABLE void previousVertex();
    Q_INVOKABLE void nextVertex();

    EditingMode editingMode();
    QgsPoint currentPoint();
    void setCurrentPoint( const QgsPoint &point );

    QgsWkbTypes::GeometryType geometryType();

    QVector<QgsPoint> flatVertices();

    QHash<int, QByteArray> roleNames() const override;

  signals:
    void editingModeChanged();
    void currentPointChanged();

  private:
    void setCurrentVertex( int newVertex );
    void setEditingMode( EditingMode mode );
    QgsPoint mCurrentPoint = QgsPoint();
    EditingMode mMode = NoEditing;
    int mCurrentVertex = -1;
    QgsWkbTypes::GeometryType mGeometryType = QgsWkbTypes::LineGeometry;

};

#endif // VERTEXMODEL_H
