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

#include <QDebug>

#include <qgsgeometry.h>
#include <qgswkbtypes.h>
#include <qgslinestring.h>
#include <qgspolygon.h>
#include <qgsmessagelog.h>

#include "vertexmodel.h"
#include "qgsquickmapsettings.h"


VertexModel::VertexModel( QObject *parent )
  : QAbstractListModel( parent )
{
  connect( this, &VertexModel::editingModeChanged, this, &VertexModel::updateCanRemoveVertex );
  connect( this, &VertexModel::vertexCountChanged, this, &VertexModel::updateCanRemoveVertex );
  connect( this, &VertexModel::vertexCountChanged, this, &VertexModel::updateCanPreviousNextVertex );
  connect( this, &VertexModel::currentVertexIndexChanged, this, &VertexModel::updateCanPreviousNextVertex );
  connect( this, &VertexModel::currentVertexIndexChanged, this, &VertexModel::updateCanRemoveVertex );
}

void VertexModel::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
    return;

  mMapSettings = mapSettings;

  emit mapSettingsChanged();
}

QgsQuickMapSettings *VertexModel::mapSettings()
{
  return mMapSettings;
}

bool VertexModel::editingAllowed() const
{
  // at the moment we do not support editing on multi geometry
  // (not necessarly geometry layer TBC)
  return !mIsMulti;
}

void VertexModel::setCrs( const QgsCoordinateReferenceSystem &crs )
{
  if ( crs == mCrs )
    return;

  mCrs = crs;
  refreshGeometry();
  emit crsChanged();
}

QgsCoordinateReferenceSystem VertexModel::crs() const
{
  return mCrs;
}

void VertexModel::setGeometry( const QgsGeometry &geometry )
{
  beginResetModel();
  mVerticesDeleted.clear();
  mOriginalGeometry = geometry;
  mGeometryType = geometry.type();
  mRingCount = 0;
  refreshGeometry();
  setCurrentVertex( -1 );
  endResetModel();
  emit geometryChanged();
}

void VertexModel::refreshGeometry()
{
  mCurrentIndex = -1;
  QgsGeometry geom = mOriginalGeometry;

  if ( mMapSettings )
  {
    try
    {
      mTransform = QgsCoordinateTransform( mCrs, mMapSettings->destinationCrs(), mMapSettings->transformContext() );
      mTransform.setAllowFallbackTransforms( true );
      if ( mTransform.isValid() )
        geom.transform( mTransform );
    }
    catch ( QgsCsException &cs )
    {
      QgsMessageLog::logMessage( QStringLiteral( "Transformation error occurred: %1" ).arg( cs.what() ) );
    }
  }

  const QgsAbstractGeometry *abstractGeom = geom.constGet();
  if ( !abstractGeom )
    return;

  mIsMulti = abstractGeom->partCount() > 1;

  QgsVertexId vertexId;
  QgsPoint pt;

  mVertices.clear();

  while ( abstractGeom->nextVertex( vertexId, pt ) )
  {
    if ( vertexId.part > 1 )
      break;

    // skip first vertex on polygon, as it's duplicate of the last one
    // TODO: for multi, it might not be 0
    if ( mGeometryType == QgsWkbTypes::PolygonGeometry && vertexId.vertex == 0 )
      continue;

    Vertex vertex;
    vertex.point = pt;
    vertex.originalPoint = pt;
    vertex.currentVertex = false;
    vertex.type = ExistingVertex;
    vertex.ring = vertexId.ring;

    mVertices << vertex;

    mRingCount = vertexId.ring;
  }

  createCandidates();

  setDirty( false );

  emit ringCountChanged();
  emit currentVertexIndexChanged();
  emit vertexCountChanged();

  // for points, enable the editing mode directly
  if ( mGeometryType == QgsWkbTypes::PointGeometry )
    setCurrentVertex( 0 );

  updateCanAddVertex();
  updateCanRemoveVertex();
  updateCanPreviousNextVertex();
}

void VertexModel::createCandidates()
{
  // remove non existing vertices
  mVertices.erase( std::remove_if( mVertices.begin(),
                                   mVertices.end(),
  []( const Vertex & vertex ) { return vertex.type != ExistingVertex; } ),
  mVertices.end() );

  int r = 0;

  while ( r < mVertices.count() )
  {
    Vertex vertex = mVertices.at( r );
    int verticesAdded = 0;
    Q_ASSERT( vertex.type == ExistingVertex );

    // adding new vertices
    if ( r < mVertices.count() - 1 && mVertices.at( r + 1 ).ring == vertex.ring && mGeometryType != QgsWkbTypes::PointGeometry )
    {
      QVector<QgsPoint> points = {mVertices.at( r + 1 ).point, vertex.point};
      QgsPoint centroid = QgsLineString( points ).centroid();

      Vertex newVertex;
      newVertex.point = centroid;
      newVertex.originalPoint = QgsPoint();
      newVertex.currentVertex = false;
      newVertex.type = NewVertexSegment;
      newVertex.ring = vertex.ring;
      mVertices.insert( r + 1, newVertex );
      verticesAdded++;
    }

    // if line, adding start extending point
    // TODO multipart: check that we are at the beginning of a part (replace r by indexInPart)
    if ( r == 0 && mGeometryType == QgsWkbTypes::LineGeometry )
    {
      // last point is an existing vertex, the next one is a candidate (created just before)
      QgsPoint extendingPoint = mVertices.at( r ).point - ( mVertices.at( 1 ).point - mVertices.at( r ).point ) / 2;

      Vertex newVertex;
      newVertex.point = extendingPoint;
      newVertex.originalPoint = QgsPoint();
      newVertex.currentVertex = false;
      newVertex.type = NewVertexExtending;
      newVertex.ring = vertex.ring;
      mVertices.insert( 0, newVertex );
      verticesAdded++;

      // last point of previous part
      // TODO when adding support for multi part
    }

    // if polygon, create candidate to the last vertex of the ring
    if ( ( r == 0 || mVertices.at( r - 1 ).ring != vertex.ring ) && mGeometryType == QgsWkbTypes::PolygonGeometry )
    {
      Vertex lastVertex;
      for ( int i = r + 1; i < mVertices.count(); i++ )
      {
        if ( mVertices.at( i ).ring != vertex.ring )
          break;
        // TODO multipart
        lastVertex = mVertices.at( i );
      }
      QVector<QgsPoint> points = {lastVertex.point, vertex.point};
      QgsPoint centroid = QgsLineString( points ).centroid();

      Vertex newVertex;
      newVertex.point = centroid;
      newVertex.originalPoint = QgsPoint();
      newVertex.currentVertex = false;
      newVertex.type = NewVertexSegment;
      newVertex.ring = vertex.ring;
      mVertices.insert( r, newVertex );
      verticesAdded++;
    }

    r += 1 + verticesAdded;
  }

  // if line, adding ending extending vertex
  r = mVertices.count() - 1;
  if ( mGeometryType == QgsWkbTypes::LineGeometry )
  {
    // last point is an existing vertex, the previous one is a candidate
    QgsPoint extendingPoint = mVertices.at( r ).point - ( mVertices.at( r - 1 ).point - mVertices.at( r ).point ) / 2;

    Vertex newVertex;
    newVertex.point = extendingPoint;
    newVertex.originalPoint = QgsPoint();
    newVertex.currentVertex = false;
    newVertex.type = NewVertexExtending;
    newVertex.ring = mVertices.at( r ).ring;
    mVertices.insert( r + 1, newVertex );
  }

  // re-calculate the current index
  for ( int i = 0; i < mVertices.count(); i++ )
  {
    if ( mVertices.at( i ).currentVertex )
    {
      setCurrentVertex( i, true );
      break;
    }
  }
}

QModelIndex VertexModel::index( int row, int column, const QModelIndex &parent ) const
{
  if ( !hasIndex( row, column, parent ) )
    return QModelIndex();

  if ( !parent.isValid() )
  {
    return createIndex( row, column );
  }

  return QModelIndex();
}

QModelIndex VertexModel::parent( const QModelIndex &child ) const
{
  Q_UNUSED( child )
  return QModelIndex();
}

int VertexModel::rowCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent )
  return mVertices.count();
}

int VertexModel::columnCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent )
  return 1;
}

VertexModel::Vertex VertexModel::vertex( int row ) const
{
  return data( index( row, 0, QModelIndex() ), Qt::UserRole ).value<Vertex>();
}

QVariant VertexModel::data( const QModelIndex &index, int role ) const
{
  if ( index.row() < 0 || index.row() > vertexCount() )
    return QVariant();

  Vertex vertex = mVertices.at( index.row() );

  switch ( role )
  {
    case Qt::UserRole:
      return QVariant::fromValue( vertex );
    case PointRole:
      return QVariant::fromValue( vertex.point );

    case CurrentVertexRole:
      return vertex.currentVertex;

    case OriginalPointRole:
      return QVariant::fromValue( vertex.originalPoint );

    case ExistingVertexRole:
      return vertex.type == ExistingVertex;

    case RingIdRole:
      return QVariant::fromValue( vertex.ring );
  }

  return QVariant();
}


QgsGeometry VertexModel::geometry() const
{
  if ( !editingAllowed() )
  {
    // TODO: handle multi, for now return original to avoid any data destruction
    return mOriginalGeometry;
  }

  QVector<QgsPoint> vertices = flatVertices( 0 );
  QgsGeometry geometry;

  switch ( mGeometryType )
  {
    case QgsWkbTypes::PointGeometry:
    {
      geometry.set( new QgsPoint( vertices.first() ) );
      break;
    }
    case QgsWkbTypes::LineGeometry:
    {
      geometry = QgsGeometry::fromPolyline( vertices );
      break;
    }
    case QgsWkbTypes::PolygonGeometry:
    {
      std::unique_ptr<QgsLineString> ls( std::make_unique<QgsLineString>( vertices ) );
      std::unique_ptr<QgsPolygon> polygon( std::make_unique<QgsPolygon>() );
      polygon->setExteriorRing( ls.release() );
      for ( int r = 1; r <= ringCount(); r++ )
      {
        std::unique_ptr<QgsLineString> ring( std::make_unique<QgsLineString>( flatVertices( r ) ) );
        polygon->addInteriorRing( ring.release() );
      }
      geometry.set( polygon.release() );
      break;
    }
    case QgsWkbTypes::NullGeometry:
    case QgsWkbTypes::UnknownGeometry:
      break;
  }

  if ( mTransform.isValid() )
    geometry.transform( mTransform, QgsCoordinateTransform::ReverseTransform );

  return geometry;
}

void VertexModel::clear()
{
  beginResetModel();
  setEditingMode( NoEditing );
  mVertices.clear();
  mVerticesDeleted.clear();
  updateCanRemoveVertex();
  updateCanAddVertex();
  emit vertexCountChanged();
  setDirty( false );
  endResetModel();
}

void VertexModel::reset()
{
  refreshGeometry();
}

void VertexModel::previous()
{
  if ( !mCanPreviousVertex )
    return;

  if ( mCurrentIndex < 2 )
  {
    if ( mGeometryType == QgsWkbTypes::PointGeometry )
    {
      setCurrentVertex( mVertices.count() - 1 );
    }
    else if ( mGeometryType == QgsWkbTypes::LineGeometry )
    {
      if ( mMode == AddVertex )
      {
        setCurrentVertex( mVertices.count() - 1 );
      }
      else
      {
        setCurrentVertex( mVertices.count() - 2 );
      }
    }
    else if ( mGeometryType == QgsWkbTypes::PolygonGeometry )
    {
      if ( mMode == AddVertex )
      {
        setCurrentVertex( mVertices.count() - 2 );
      }
      else
      {
        setCurrentVertex( mVertices.count() - 1 );
      }
    }
  }
  else
  {
    setCurrentVertex( mCurrentIndex - 2 );
  }
}

void VertexModel::next()
{
  if ( !mCanNextVertex )
    return;

  if ( mCurrentIndex == -1 || mCurrentIndex >= mVertices.count() - 2 )
  {
    if ( mMode == AddVertex )
    {
      setCurrentVertex( 0 );
    }
    else
    {
      setCurrentVertex( 1 );
    }
  }
  else
  {
    setCurrentVertex( mCurrentIndex + 2 );
  }
}

void VertexModel::selectVertexAtPosition( const QPointF &point, double threshold )
{
  QgsPoint mapPoint( mapSettings()->screenToCoordinate( point ) );
  selectVertexAtPosition( mapPoint, threshold );
}

void VertexModel::selectVertexAtPosition( const QgsPoint &mapPoint, double threshold )
{
  double closestDistance = std::numeric_limits<double>::max();

  int closestRow = -1;

  for ( int r = 0; r < mVertices.count(); r++ )
  {
    double dist = mVertices.at( r ).point.distance( mapPoint );
    if ( dist < closestDistance )
    {
      closestDistance = dist;
      closestRow = r;
    }
  }

  if ( closestRow >= 0 && closestDistance / mapSettings()->mapSettings().mapUnitsPerPixel() < threshold )
  {
    if ( mVertices.at( closestRow ).type != ExistingVertex )
    {
      // makes a new vertex as an existing vertex
      beginResetModel();
      mVertices[closestRow].type = ExistingVertex;
      setCurrentVertex( closestRow );
      createCandidates();
      setEditingMode( EditVertex );
      endResetModel();
    }
    else
    {
      setCurrentVertex( closestRow );
    }
  }
}

void VertexModel::removeCurrentVertex()
{
  if ( !mCanRemoveVertex )
    return;

  if ( mCurrentIndex < 0 || mCurrentIndex >= mVertices.count() )
    return;

  if ( mVertices.at( mCurrentIndex ).type != ExistingVertex )
    return;

  if ( !mVertices.at( mCurrentIndex ).originalPoint.isEmpty() )
    mVerticesDeleted << mVertices.at( mCurrentIndex ).originalPoint;

  beginResetModel();
  mVertices.removeAt( mCurrentIndex );
  createCandidates();
  endResetModel();

  setDirty( true );

  emit vertexCountChanged();

  setCurrentVertex( mCurrentIndex < mVertices.count() - 1 ? mCurrentIndex : mCurrentIndex - 2, true );
}

void VertexModel::updateGeometry( const QgsGeometry &geometry )
{
  int preservedIndex = mCurrentIndex;
  setGeometry( geometry );
  //since the index is shifted after reload, we decrement
  setCurrentVertex( preservedIndex - 1 );
}

VertexModel::EditingMode VertexModel::editingMode() const
{
  return mMode;
}

QgsPoint VertexModel::currentPoint() const
{
  return mVertices.value( mCurrentIndex ).point;
}

void VertexModel::setCurrentPoint( const QgsPoint &point )
{
  if ( mCurrentIndex < 0 || mCurrentIndex >= mVertices.count() )
    return;

  if ( mMode == NoEditing )
    return;

  Vertex &vertex = mVertices[mCurrentIndex];

  if ( mMapSettings && vertex.point.distance( point ) / mMapSettings->mapSettings().mapUnitsPerPixel() < 1 )
    return;

  setDirty( true );
  beginResetModel();

  vertex.point = point;

  if ( mMode == AddVertex )
  {
    // we move a candidate, make it an existing vertex
    Q_ASSERT( vertex.type != ExistingVertex );
    vertex.type = ExistingVertex;
    setEditingMode( EditVertex );
  }

  createCandidates();
  endResetModel();

  emit geometryChanged();
}

void VertexModel::setCurrentVertex( int newVertex, bool forceUpdate )
{
  if ( mCurrentIndex >= 0 && mCurrentIndex < mVertices.count() )
  {
    mVertices[mCurrentIndex].currentVertex = false;
    emit dataChanged( index( mCurrentIndex, 0, QModelIndex() ), index( mCurrentIndex, 0, QModelIndex() ) );
  }

  if ( mVertices.count() == 0 )
  {
    setEditingMode( NoEditing );
    newVertex = -1;
  }
  else if ( newVertex < 0 )
  {
    setEditingMode( NoEditing );
  }
  else if ( mMode == NoEditing )
  {
    setEditingMode( EditVertex );
  }

  if ( !forceUpdate && mCurrentIndex == newVertex )
    return;

  mCurrentIndex = newVertex;

  if ( mCurrentIndex >= 0 && mCurrentIndex < mVertices.count() )
  {
    mVertices[mCurrentIndex].currentVertex = true;
    emit dataChanged( index( mCurrentIndex, 0, QModelIndex() ), index( mCurrentIndex, 0, QModelIndex() ) );
  }

  emit currentVertexIndexChanged();
}

void VertexModel::setCurrentVertexIndex( int currentIndex )
{
  if ( currentIndex == mCurrentIndex )
    return;

  if ( currentIndex < 0 || currentIndex >= mVertices.count() )
    currentIndex = -1;

  setCurrentVertex( currentIndex );
}

int VertexModel::currentVertexIndex() const
{
  return mCurrentIndex;
}

int VertexModel::vertexCount() const
{
  return mVertices.count();
}

int VertexModel::ringCount() const
{
  return mRingCount;
}

bool VertexModel::dirty() const
{
  return mDirty;
}

bool VertexModel::canRemoveVertex()
{
  return mCanRemoveVertex;
}

bool VertexModel::canAddVertex()
{
  return mCanAddVertex;
}

bool VertexModel::canPreviousVertex()
{
  return mCanPreviousVertex;
}

bool VertexModel::canNextVertex()
{
  return mCanNextVertex;
}

QVector<QgsPoint> VertexModel::flatVertices( int ringId ) const
{
  if ( ringId == -1 )
  {
    ringId = mVertices.value( mCurrentIndex ).ring;
  }

  QVector<QgsPoint> vertices = QVector<QgsPoint>();
  for ( const Vertex &vertex : qgis::as_const( mVertices ) )
  {
    if ( vertex.type != ExistingVertex || vertex.ring != ringId )
      continue;
    vertices << vertex.point;
  }
  // re-append vertex to close polygon
  if ( mGeometryType == QgsWkbTypes::PolygonGeometry )
  {
    vertices << vertices.constFirst();
  }
  return vertices;
}

QVector<QPair<QgsPoint, QgsPoint>> VertexModel::verticesMoved() const
{
  QVector<QPair<QgsPoint, QgsPoint>> vertices;
  for ( const Vertex &vertex : qgis::as_const( mVertices ) )
  {
    if ( vertex.type != ExistingVertex )
      continue;
    if ( vertex.originalPoint.isEmpty() )
      continue;
    if ( vertex.point != vertex.originalPoint )
      vertices << qMakePair( vertex.originalPoint, vertex.point );
  }
  return vertices;
}

QHash<int, QByteArray> VertexModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[PointRole] = "Point";
  roles[CurrentVertexRole] = "CurrentVertex";
  roles[OriginalPointRole] = "OriginalPoint";
  roles[ExistingVertexRole] = "ExistingVertex";
  roles[RingIdRole] = "RingId";
  return roles;
}

void VertexModel::setDirty( bool dirty )
{
  if ( mDirty == dirty )
    return;

  mDirty = dirty;
  emit dirtyChanged();
}

void VertexModel::updateCanRemoveVertex()
{
  bool canRemoveVertex = false;

  if ( mMode == EditVertex )
  {
    switch ( mGeometryType )
    {
      case QgsWkbTypes::PointGeometry:
        canRemoveVertex = false;
        break;
      case QgsWkbTypes::LineGeometry:
        canRemoveVertex =  flatVertices().count() > 2;
        break;
      case QgsWkbTypes::PolygonGeometry:
        canRemoveVertex =  flatVertices().count() > 4; // the polygon is returned closed
        break;
      case QgsWkbTypes::NullGeometry:
      case QgsWkbTypes::UnknownGeometry:
        canRemoveVertex =  false;
        break;
    }
  }

  if ( canRemoveVertex == mCanRemoveVertex )
    return;

  mCanRemoveVertex = canRemoveVertex;

  emit canRemoveVertexChanged();
}

void VertexModel::updateCanAddVertex()
{
  bool canAddVertex = vertexCount() > 0 && mGeometryType != QgsWkbTypes::PointGeometry;

  if ( canAddVertex == mCanAddVertex )
    return;

  mCanAddVertex = canAddVertex;
  emit canAddVertexChanged();
}

void VertexModel::updateCanPreviousNextVertex()
{
  bool canPrevious = false;
  bool canNext = false;

  switch ( mGeometryType )
  {
    case QgsWkbTypes::PointGeometry:
      // todo for multi
      break;
    case QgsWkbTypes::LineGeometry:
      switch ( mMode )
      {
        case NoEditing:
          canPrevious = true;
          canNext = true;
          break;
        case AddVertex:
          canPrevious = mCurrentIndex >= 1;
          canNext = mCurrentIndex < mVertices.count() - 2;
          break;
        case EditVertex:
          canPrevious = mCurrentIndex >= 2;
          canNext = mCurrentIndex < mVertices.count() - 3;
          break;
      }
      break;
    case QgsWkbTypes::PolygonGeometry:
      canPrevious = true;
      canNext = true;
      break;
    case QgsWkbTypes::UnknownGeometry:
    case QgsWkbTypes::NullGeometry:
      break;
  }

  if ( canPrevious != mCanPreviousVertex )
  {
    mCanPreviousVertex = canPrevious;
    emit canPreviousVertexChanged();
  }
  if ( canNext != mCanNextVertex )
  {
    mCanNextVertex = canNext;
    emit canNextVertexChanged();
  }
}

void VertexModel::setGeometryType( const QgsWkbTypes::GeometryType &geometryType )
{
  if ( mGeometryType == geometryType )
    return;

  mGeometryType = geometryType;
  emit geometryTypeChanged();
}

QgsWkbTypes::GeometryType VertexModel::geometryType() const
{
  return mGeometryType;
}

void VertexModel::setEditingMode( VertexModel::EditingMode mode )
{
  if ( !mVertices.count() )
    mode = NoEditing;

  // TODO multi
  if ( mGeometryType == QgsWkbTypes::PointGeometry && mode == AddVertex )
    mode = NoEditing;

  if ( mMode == mode )
    return;

  mMode = mode;

  if ( mode == AddVertex )
  {
    switch ( mGeometryType )
    {
      case QgsWkbTypes::PointGeometry:
      {
        // should not happen for now
        break;
      }
      case QgsWkbTypes::LineGeometry:
      case QgsWkbTypes::PolygonGeometry:
      {
        if ( mCurrentIndex == -1 )
        {
          setCurrentVertex( 0 );
        }
        else
        {
          bool vertexMatchesMode = mVertices.at( mCurrentIndex ).type != ExistingVertex;
          int direction = mCurrentIndex < vertexCount() - 2 ? 1 : -1;
          setCurrentVertex( mCurrentIndex + direction * ( vertexMatchesMode ? 0 : 1 ), true );
        }
        break;
      }
      case QgsWkbTypes::NullGeometry:
      case QgsWkbTypes::UnknownGeometry:
        break;
    }
  }

  emit editingModeChanged();
}
