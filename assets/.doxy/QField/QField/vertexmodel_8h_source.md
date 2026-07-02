

# File vertexmodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**vertexmodel.h**](vertexmodel_8h.md)

[Go to the documentation of this file](vertexmodel_8h.md)


```C++
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
class QFIELD_CORE_EXPORT VertexModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY( EditingMode editingMode READ editingMode WRITE setEditingMode NOTIFY editingModeChanged )
    Q_PROPERTY( QgsPoint currentPoint READ currentPoint WRITE setCurrentPoint NOTIFY currentPointChanged )
    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )
    Q_PROPERTY( int vertexCount READ vertexCount NOTIFY vertexCountChanged )
    Q_PROPERTY( int ringCount READ ringCount NOTIFY ringCountChanged )
    Q_PROPERTY( bool dirty READ dirty NOTIFY dirtyChanged )
    Q_PROPERTY( bool editingAllowed READ editingAllowed )
    Q_PROPERTY( bool canRemoveVertex READ canRemoveVertex NOTIFY canRemoveVertexChanged )
    Q_PROPERTY( bool canAddVertex READ canAddVertex NOTIFY canAddVertexChanged )
    Q_PROPERTY( bool canPreviousVertex READ canPreviousVertex NOTIFY canPreviousVertexChanged )
    Q_PROPERTY( bool canNextVertex READ canNextVertex NOTIFY canNextVertexChanged )
    Q_PROPERTY( bool isHovering MEMBER mIsHovering )

    Q_PROPERTY( bool canUndo READ canUndo NOTIFY historyChanged )

    Q_PROPERTY( int currentVertexIndex READ currentVertexIndex WRITE setCurrentVertexIndex NOTIFY currentVertexIndexChanged )

    Q_PROPERTY( QgsGeometry geometry READ geometry WRITE setGeometry NOTIFY geometryChanged )

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

    void setMapSettings( QgsQuickMapSettings *mapSettings );
    QgsQuickMapSettings *mapSettings();

    bool editingAllowed() const;

    void setCrs( const QgsCoordinateReferenceSystem &crs );

    QgsCoordinateReferenceSystem crs() const;

    void setGeometry( const QgsGeometry &geometry );

    QgsGeometry geometry() const;

    QModelIndex index( int row, int column, const QModelIndex &parent ) const override;
    QModelIndex parent( const QModelIndex &child ) const override;
    int rowCount( const QModelIndex &parent ) const override;
    int columnCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;

    Q_INVOKABLE void clear();

    Q_INVOKABLE void reset();

    Q_INVOKABLE void previous();

    Q_INVOKABLE void next();

    Q_INVOKABLE void selectVertexAtPosition( const QPointF &point, double threshold, bool autoInsert = true );

    void selectVertexAtPosition( const QgsPoint &mapPoint, double threshold, bool autoInsert = true );

    Q_INVOKABLE void addVertexNearestToPosition( const QgsPoint &mapPoint );

    Q_INVOKABLE void removeCurrentVertex();


    Q_INVOKABLE void updateGeometry( const QgsGeometry &geometry );

    EditingMode editingMode() const;
    void setEditingMode( EditingMode mode );

    QgsPoint currentPoint() const;
    void setCurrentPoint( const QgsPoint &point );

    int vertexCount() const;
    int ringCount() const;

    bool dirty() const;

    bool canRemoveVertex();
    bool canAddVertex();
    bool canPreviousVertex();
    bool canNextVertex();

    Qgis::GeometryType geometryType() const;

    QVector<QgsPoint> flatVertices( int ringId = -1 ) const;

    QVector<QgsPoint> verticesAdded() const;

    QVector<QPair<QgsPoint, QgsPoint>> verticesMoved() const;

    QVector<QgsPoint> verticesDeleted() const;

    QList<Vertex> vertices() const;

    QHash<int, QByteArray> roleNames() const override;

    int currentVertexIndex() const;

    void setCurrentVertexIndex( qsizetype currentIndex );

    Vertex vertex( int row ) const;

    QList<VertexChange> history( bool transformPoints = false, bool includeAvailableRedos = false ) const;

    void clearHistory();

    void addToHistory( VertexChangeType type );

    Q_INVOKABLE void undoHistory();

    bool canUndo();

  signals:
    void editingModeChanged();
    void currentPointChanged();
    void mapSettingsChanged();
    void vertexCountChanged();
    void ringCountChanged();
    void dirtyChanged();
    void canRemoveVertexChanged();
    void canAddVertexChanged();
    void canPreviousVertexChanged();
    void canNextVertexChanged();

    void currentVertexIndexChanged();

    void crsChanged();

    void geometryChanged();

    void geometryTypeChanged();

    void historyChanged();

  private:
    void refreshGeometry();
    void createCandidates();
    void setDirty( bool dirty );
    void updateCanRemoveVertex();
    void updateCanAddVertex();
    void updateCanPreviousNextVertex();
    void setGeometryType( const Qgis::GeometryType &geometryType );

    QList<Vertex> mVertices;

    QgsGeometry mOriginalGeometry;
    QgsCoordinateReferenceSystem mCrs;

    QgsCoordinateTransform mTransform = QgsCoordinateTransform();
    bool mIsMulti = false;
    bool mDirty = false;

    QVector<QgsPoint> mVerticesDeleted;

    void setCurrentVertex( qsizetype newVertex, bool forceUpdate = false );

    EditingMode mMode = NoEditing;
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
```


