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

#include <QAbstractListModel>

class QgsQuickMapSettings;

#include "qfield_core_export.h"
#include "qgscoordinatetransform.h"
#include "qgsgeometry.h"
#include "qgspoint.h"

// Copied from gtest/gtest_prod.h
/**
 * The VertexModel class is a model to highlight and edit vertices.
 * The model is used in map coordinates.
 * There are different modes: no editing, edit (move/remove) nodes, add nodes (to be implemented)
 *
 * The model holds all vertices and the candidates for new vertices. If you need the existing nodes, use flatVertices().
 * \ingroup core
 */
class QFIELD_CORE_EXPORT VertexModel : public QAbstractListModel
{
    Q_OBJECT
    //! The current mode
    Q_PROPERTY( EditingMode editingMode READ editingMode WRITE setEditingMode NOTIFY editingModeChanged )
    //! The current point being edited \see editingMode. The expected CRS to read/write is the map canvas CRS
    Q_PROPERTY( QgsPoint currentPoint READ currentPoint WRITE setCurrentPoint NOTIFY currentPointChanged )
    //! Map settings is used to define the map canvas CRS and detect any extent change
    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )
    //! number of vertices in the model
    Q_PROPERTY( int vertexCount READ vertexCount NOTIFY vertexCountChanged )
    //! number of rings in the model
    Q_PROPERTY( int ringCount READ ringCount NOTIFY ringCountChanged )
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
    //! determines if the map is currently being hovered (then when moving the map, it will not move directly a vertex if the mode is AddVertex)
    Q_PROPERTY( bool isHovering MEMBER mIsHovering )

    //! returns TRUE if an undo operation is available
    Q_PROPERTY( bool canUndo READ canUndo NOTIFY historyChanged )

    /**
     * The index of the currently active vertex. If no vertex is selected, this is -1.
     */
    Q_PROPERTY( int currentVertexIndex READ currentVertexIndex WRITE setCurrentVertexIndex NOTIFY currentVertexIndexChanged )

    /**
     * The geometry in layer coordinates
     */
    Q_PROPERTY( QgsGeometry geometry READ geometry WRITE setGeometry NOTIFY geometryChanged )

    /**
     * The coordinate reference system in which the geometry is
     */
    Q_PROPERTY( QgsCoordinateReferenceSystem crs READ crs WRITE setCrs NOTIFY crsChanged )

  public:
    enum PointType
    {
      ExistingVertex,
      NewVertexSegment,
      NewVertexExtending,
    };
    Q_ENUM( PointType )

    enum ColumnRole
    {
      PointRole = Qt::UserRole + 1,
      CurrentVertexRole,
      OriginalPointRole,
      ExistingVertexRole,
      RingIdRole,
    };
    Q_ENUM( ColumnRole )

    enum EditingMode
    {
      NoEditing,
      EditVertex,
      AddVertex
    };
    Q_ENUM( EditingMode )

    struct Vertex
    {
        QgsPoint point;
        QgsPoint originalPoint;
        bool currentVertex;
        PointType type;
        int ring;
    };

    enum VertexChangeType
    {
      NoChange,
      VertexMove,
      VertexAddition,
      VertexDeletion
    };
    Q_ENUM( VertexChangeType )

    struct VertexChange
    {
        VertexChange( VertexChangeType type, qsizetype &index, const Vertex &vertex )
          : type( type )
          , index( index )
          , vertex( vertex )
        {}

        VertexChangeType type = NoChange;
        qsizetype index = -1;
        Vertex vertex;
    };

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

    QModelIndex index( int row, int column, const QModelIndex &parent ) const override;
    QModelIndex parent( const QModelIndex &child ) const override;
    int rowCount( const QModelIndex &parent ) const override;
    int columnCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;

    //! This will clear the data
    Q_INVOKABLE void clear();

    //! Reset the model to its original geometry
    Q_INVOKABLE void reset();

    //! previous vertex or segment
    Q_INVOKABLE void previous();

    //! next vertex or segment
    Q_INVOKABLE void next();

    //! Selects the vertex at the given screen \a point within a given \a threshold
    Q_INVOKABLE void selectVertexAtPosition( const QPointF &point, double threshold, bool autoInsert = true );

    //! Selects the vertex at the given \a mapPoint within a given \a threshold
    void selectVertexAtPosition( const QgsPoint &mapPoint, double threshold, bool autoInsert = true );

    //! Adds a new vertex on the segment having its mid-point nearest to the \a mapPoint
    Q_INVOKABLE void addVertexNearestToPosition( const QgsPoint &mapPoint );

    Q_INVOKABLE void removeCurrentVertex();


    /**
     * sets the geometry to the given \a geometry but preserves the index of the current vertex
     * this is used to update the original geometry while still editing the model
     * \see `geometry` property
     */
    Q_INVOKABLE void updateGeometry( const QgsGeometry &geometry );

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
    //! \copydoc ringCount
    int ringCount() const;

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
    Qgis::GeometryType geometryType() const;

    //! Returns a list of point (segment vertex, if any, will be skipped)
    //! For a polygon, if ringId is not given the current ring will be returned
    QVector<QgsPoint> flatVertices( int ringId = -1 ) const;

    //! Returns a list of added vertices not found in linked geometry
    QVector<QgsPoint> verticesAdded() const;

    //! Returns a list of moved vertices found in linked geometry
    QVector<QPair<QgsPoint, QgsPoint>> verticesMoved() const;

    //! Returns a list of added vertices not found in linked geometry
    QVector<QgsPoint> verticesDeleted() const;

    //! Returns a list of vertices
    QList<Vertex> vertices() const;

    QHash<int, QByteArray> roleNames() const override;

    int currentVertexIndex() const;

    void setCurrentVertexIndex( qsizetype currentIndex );

    Vertex vertex( int row ) const;

    void clearHistory();

    void addToHistory( VertexChangeType type );

    Q_INVOKABLE void undoHistory();

    bool canUndo();

  signals:
    //! \copydoc editingMode
    void editingModeChanged();
    //! \copydoc currentPoint
    void currentPointChanged();
    //! \copydoc mapSettings
    void mapSettingsChanged();
    //! \copydoc vertexCount
    void vertexCountChanged();
    //! \copydoc ringCount
    void ringCountChanged();
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

    void currentVertexIndexChanged();

    //!Emitted when the coordinate reference system has changed
    void crsChanged();

    //! Emitted when the geometry has changed
    void geometryChanged();

    //! \copydoc geometryType
    void geometryTypeChanged();

    //! Emitted when the history has been modified
    void historyChanged();

  private:
    void refreshGeometry();
    //! Add the candidates of new vertices (extending or segment)
    //! This will not emit the reset signals, it's up to the caller to do so
    void createCandidates();
    void setDirty( bool dirty );
    void updateCanRemoveVertex();
    void updateCanAddVertex();
    void updateCanPreviousNextVertex();
    void setGeometryType( const Qgis::GeometryType &geometryType );

    QList<Vertex> mVertices;

    //! copy of the initial geometry, in destination (layer) CRS
    QgsGeometry mOriginalGeometry;
    QgsCoordinateReferenceSystem mCrs;

    //! CRS of the geometry, will be used to transform to map canvas coordinates
    QgsCoordinateTransform mTransform = QgsCoordinateTransform();
    bool mIsMulti = false;
    bool mDirty = false;

    QVector<QgsPoint> mVerticesDeleted;

    /**
     * @brief setCurrentVertex set the current vertex viewed/edited in the model
     * @param newVertex the new vertex index
     * @param forceUpdate if true, it will force to update all vertices and emit signal
     */
    void setCurrentVertex( qsizetype newVertex, bool forceUpdate = false );

    EditingMode mMode = NoEditing;
    //!
    qsizetype mCurrentIndex = -1;
    int mRingCount = 0;
    Qgis::GeometryType mGeometryType = Qgis::GeometryType::Line;
    Qgis::WkbType mGeometryWkbType = Qgis::WkbType::Unknown;
    QgsQuickMapSettings *mMapSettings = nullptr;
    bool mCanRemoveVertex = false;
    bool mCanAddVertex = false;
    bool mCanPreviousVertex = false;
    bool mCanNextVertex = false;
    bool mIsHovering = false;

    QList<VertexChange> mHistory;
    qsizetype mHistoryIndex = -1;
    bool mHistoryTraversing = false;

    friend class VertexModelTest;
};

Q_DECLARE_METATYPE( VertexModel::Vertex );


#endif // VERTEXMODEL_H
