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

class QgsQuickMapSettings;

#include "qgspoint.h"
#include "qgsgeometry.h"
#include "qgscoordinatetransform.h"
#include "qfieldcore_global.h"

/**
 * @brief The VertexModel class is a model to highlight and edit vertices.
 * The model is used in map coordinates.
 * There are different modes: no editing, edit (move/remove) nodes, add nodes (to be implemented)
 */
class QFIELDCORE_EXPORT VertexModel : public QStandardItemModel
{
    Q_OBJECT
    //! The current mode
    Q_PROPERTY( EditingMode editingMode READ editingMode WRITE setEditingMode NOTIFY editingModeChanged )
    //! The current point being edited \see editingMode. The expected CRS to read/write is the map canvas CRS
    Q_PROPERTY( QgsPoint currentPoint READ currentPoint WRITE setCurrentPoint NOTIFY currentPointChanged )
    //! Map settings is used to define the map canvas CRS and detect any extent change
    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )
    //! number of points in the model
    Q_PROPERTY( int vertexCount READ vertexCount NOTIFY vertexCountChanged )
    //! determines if the model has changes
    Q_PROPERTY( bool dirty READ dirty NOTIFY dirtyChanged )
    //! determines if the model allows editing the geometry
    Q_PROPERTY( bool editingAllowed READ editingAllowed )
    //! determines if one can remove current vertex
    Q_PROPERTY( bool canRemoveVertex READ canRemoveVertex NOTIFY canRemoveVertexChanged )
    //! determines if one can add vertex
    Q_PROPERTY( bool canAddVertex READ canAddVertex NOTIFY canAddVertexChanged )
    //! determines if one can go to previous vertex
    Q_PROPERTY( bool canPreviousVertex READ canPreviousVertex NOTIFY canPreviousVertexChanged )
    //! determines if one can go to next vertex
    Q_PROPERTY( bool canNextVertex READ canNextVertex NOTIFY canNextVertexChanged )

    /**
     * The geometry in layer coordinates
     */
    Q_PROPERTY( QgsGeometry geometry READ geometry WRITE setGeometry NOTIFY geometryChanged )

    /**
     * The coordinate reference system in which the geometry is
     */
    Q_PROPERTY( QgsCoordinateReferenceSystem crs READ crs WRITE setCrs NOTIFY crsChanged )

  public:
    enum ColumnRole
    {
      PointRole = Qt::UserRole + 1,
      CurrentVertexRole,
      SegmentVertexRole,
    };

    enum EditingMode
    {
      NoEditing,
      EditVertex,
      AddVertex
    };
    Q_ENUM( EditingMode )

    explicit VertexModel( QObject *parent = nullptr );
    ~VertexModel() override = default;

    //! \copydoc mapSettings
    void setMapSettings( QgsQuickMapSettings *mapSettings );
    //! \copydoc mapSettings
    QgsQuickMapSettings *mapSettings();

    //! \copydoc editingAllowed
    bool editingAllowed() const;

    /**
     * The coorinate reference system in which the geometry is
     */
    void setCrs( const QgsCoordinateReferenceSystem &crs );

    /**
     * The coordinate reference system in which the geometry is
     */
    QgsCoordinateReferenceSystem crs() const;

    /**
     * The geometry in layer coordinates
     */
    void setGeometry( const QgsGeometry &geometry );

    /**
     * The geometry in layer coordinates
     */
    QgsGeometry geometry() const;

    Q_INVOKABLE void clear();

    //! previous vertex or segment
    Q_INVOKABLE void previous();

    //! next vertex or segment
    Q_INVOKABLE void next();

    Q_INVOKABLE void removeCurrentVertex();

    //! \copydoc editingMode
    EditingMode editingMode() const;
    //! \copydoc editingMode
    void setEditingMode( EditingMode mode );

    //! \copydoc currentPoint
    QgsPoint currentPoint() const;
    //! \copydoc currentPoint
    void setCurrentPoint( const QgsPoint &point );

    //! \copydoc vertexCount
    int vertexCount() const;

    //! \copydoc dirty
    bool dirty() const;

    //! \copydoc canRemoveVertex
    bool canRemoveVertex();
    //! \copydoc canAddVertex
    bool canAddVertex();
    //! \copydoc canPreviousVertex
    bool canPreviousVertex();
    //! \copydoc canNextVertex
    bool canNextVertex();

    //! Returns the geometry type
    QgsWkbTypes::GeometryType geometryType() const;

    //! list of points. Segment vertex, if any, will be skipped.
    QVector<QgsPoint> flatVertices() const;

    QHash<int, QByteArray> roleNames() const override;

  signals:
    //! \copydoc editingMode
    void editingModeChanged();
    //! \copydoc currentPoint
    void currentPointChanged();
    //! \copydoc mapSettings
    void mapSettingsChanged();
    //! \copydoc vertexCount
    void vertexCountChanged();
    //! \copydoc dirty
    void dirtyChanged();
    //! \copydoc canRemoveVertex
    void canRemoveVertexChanged();
    //! \copydoc canAddVertex
    void canAddVertexChanged();
    //! \copydoc canPreviousVertex
    void canPreviousVertexChanged();
    //! \copydoc canNextVertex
    void canNextVertexChanged();

    /**
     * The coordinate reference system in which the geometry is
     */
    void crsChanged();

    /**
     * The geometry in layer coordinates
     */
    void geometryChanged();

  private:
    void refreshGeometry();
    void setDirty( bool dirty );
    void updateCanRemoveVertex();
    void updateCanAddVertex();
    void updateCanPreviousNextVertex();
    //! copy of the initial geometry, in destination (layer) CRS
    QgsGeometry mOriginalGeometry;
    QgsCoordinateReferenceSystem mCrs;

    //! CRS of the geometry, will be used to transform to map canvas coordinates
    QgsCoordinateTransform mTransform = QgsCoordinateTransform();
    bool mIsMulti = false;
    bool mDirty = false;
    /**
     * @brief setCurrentVertex set the current vertex viewed/edited in the model
     * @param newVertex the new vertex index
     * @param forceUpdate if true, it will force to update all vertices and emit signal
     */
    void setCurrentVertex( int newVertex, bool forceUpdate = false );

    struct Centroid
    {
      QgsPoint point;
      int index;
    };
    Centroid segmentCentroid( int leftIndex, int rightIndex,
                              bool isExtending = false );

    EditingMode mMode = NoEditing;
    //!
    int mCurrentIndex = -1;
    QgsWkbTypes::GeometryType mGeometryType = QgsWkbTypes::LineGeometry;
    QgsQuickMapSettings *mMapSettings = nullptr;
    bool mCanRemoveVertex = false;
    bool mCanAddVertex = false;
    bool mCanPreviousVertex = false;
    bool mCanNextVertex = false;

    friend class TestVertexModel;
};

#endif // VERTEXMODEL_H
