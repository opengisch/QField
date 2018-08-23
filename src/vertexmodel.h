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

class MapSettings;

#include "qgspoint.h"
#include "qgsgeometry.h"

/**
 * @brief The VertexModel class is a model to highlight and edit vertices.
 * The model is used in map coordinates.
 */
class VertexModel : public QStandardItemModel
{
    Q_OBJECT
    Q_PROPERTY( EditingMode editingMode READ editingMode NOTIFY editingModeChanged )
    Q_PROPERTY( QgsPoint currentPoint READ currentPoint WRITE setCurrentPoint NOTIFY currentPointChanged )
    //! Map settings is used to define the map canvas CRS and detect any extent change
    Q_PROPERTY( MapSettings* mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )

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

    //! \copydoc mapSettings
    void setMapSettings( MapSettings *mapSettings );
    //! \copydoc mapSettings
    MapSettings *mapSettings();

    Q_INVOKABLE void setGeometry( const QgsGeometry &geometry, const QgsCoordinateReferenceSystem &crs );
    Q_INVOKABLE QgsGeometry geometry() const;

    Q_INVOKABLE void clear();

    Q_INVOKABLE bool isEmtpy() const;

    Q_INVOKABLE void previousVertex();
    Q_INVOKABLE void nextVertex();

    EditingMode editingMode() const;
    QgsPoint currentPoint() const;
    void setCurrentPoint( const QgsPoint &point );

    QgsWkbTypes::GeometryType geometryType() const;

    QVector<QgsPoint> flatVertices() const;

    QHash<int, QByteArray> roleNames() const override;

  signals:
    void editingModeChanged();
    void currentPointChanged();
    //! \copydoc mapSettings
    void mapSettingsChanged();

  private:
    //! copy of the initial geometry, in destination (layer) CRS
    QgsGeometry mOriginalGeoemtry;

    //! CRS of the geometry, will be used to transform to map canvas coordinates
    QgsCoordinateReferenceSystem mCrs;
    bool mIsMulti = false;
    void setCurrentVertex( int newVertex );
    void setEditingMode( EditingMode mode );
    QgsPoint mCurrentPoint = QgsPoint();
    EditingMode mMode = NoEditing;
    int mCurrentVertex = -1;
    QgsWkbTypes::GeometryType mGeometryType = QgsWkbTypes::LineGeometry;
    MapSettings *mMapSettings = nullptr;
};

#endif // VERTEXMODEL_H
