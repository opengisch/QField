/***************************************************************************
  rubberband.cpp - Rubberband

 ---------------------
 begin                : 11.6.2016
 copyright            : (C) 2016 by Matthias Kuhn (OPENGIS.ch)
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include <QTimer>
#include <QDebug>

#include "vertexmodel.h"
#include "rubberband.h"

#include "rubberbandmodel.h"
#include "sgrubberband.h"

#include "qgsproject.h"

Rubberband::Rubberband( QQuickItem *parent )
  : QQuickItem( parent )
{
  setFlags( QQuickItem::ItemHasContents );
  setAntialiasing( true );
}

RubberbandModel *Rubberband::model() const
{
  return mRubberbandModel;
}

void Rubberband::setModel( RubberbandModel *model )
{
  if ( model )
    setVertexModel( nullptr );

  if ( mRubberbandModel == model )
    return;

  if ( mRubberbandModel )
  {
    disconnect( mRubberbandModel, &RubberbandModel::vertexChanged, this, &Rubberband::markDirty );
    disconnect( mRubberbandModel, &RubberbandModel::verticesRemoved, this, &Rubberband::markDirty );
    disconnect( mRubberbandModel, &RubberbandModel::verticesInserted, this, &Rubberband::markDirty );
  }


  mRubberbandModel = model;

  if ( mRubberbandModel )
  {
    connect( mRubberbandModel, &RubberbandModel::vertexChanged, this, &Rubberband::markDirty );
    connect( mRubberbandModel, &RubberbandModel::verticesRemoved, this, &Rubberband::markDirty );
    connect( mRubberbandModel, &RubberbandModel::verticesInserted, this, &Rubberband::markDirty );
  }

  markDirty();

  emit modelChanged();
}

VertexModel *Rubberband::vertexModel() const
{
  return mVertexModel;
}

void Rubberband::setVertexModel( VertexModel *model )
{
  if ( model )
    setModel( nullptr );

  if ( mVertexModel == model )
    return;

  if ( mVertexModel )
  {
    disconnect( mVertexModel, &VertexModel::dataChanged, this, &Rubberband::markDirty );
  }

  mVertexModel = model;

  if ( mVertexModel )
  {
    connect( mVertexModel, &VertexModel::dataChanged, this, &Rubberband::markDirty );
    connect( mVertexModel, &VertexModel::vertexCountChanged, this, &Rubberband::markDirty );
  }

  markDirty();

  emit vertexModelChanged();
}


QgsQuickMapSettings *Rubberband::mapSettings() const
{
  return mMapSettings;
}

void Rubberband::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
    return;

  mMapSettings = mapSettings;
  markDirty();

  emit mapSettingsChanged();
}

void Rubberband::markDirty()
{
  mDirty = true;
  update();
}

QSGNode *Rubberband::updatePaintNode( QSGNode *n, QQuickItem::UpdatePaintNodeData * )
{
  if ( mDirty )
  {
    delete n;
    n = new QSGNode;

    bool frozen = mRubberbandModel && mRubberbandModel->frozen();

    QVector<QgsPoint> allVertices = QVector<QgsPoint>();
    QVector<QgsPoint> allButCurrentVertices = QVector<QgsPoint>();
    QgsWkbTypes::GeometryType geomType = QgsWkbTypes::LineGeometry;

    if ( mRubberbandModel && !mRubberbandModel->isEmpty() )
    {
      allVertices = mRubberbandModel->flatVertices();
      geomType = mRubberbandModel->geometryType();
      if ( frozen )
        allButCurrentVertices = mRubberbandModel->flatVertices( true );
    }
    else if ( mVertexModel && mVertexModel->vertexCount() > 0 )
    {
      allVertices = mVertexModel->flatVertices();
      geomType = mVertexModel->geometryType();
    }

    if ( !allVertices.isEmpty() )
    {
      SGRubberband *rb = new SGRubberband( allVertices, geomType, mColor, mWidth );
      rb->setFlag( QSGNode::OwnedByParent );
      n->appendChildNode( rb );

      if ( frozen )
      {
        SGRubberband *rbCurrentPoint = new SGRubberband( allButCurrentVertices, geomType, mColorCurrentPoint, mWidthCurrentPoint );
        rbCurrentPoint->setFlag( QSGNode::OwnedByParent );
        n->appendChildNode( rbCurrentPoint );
      }
    }
  }

  mDirty = false;
  return n;
}

qreal Rubberband::width() const
{
  return mWidth;
}

void Rubberband::setWidth( qreal width )
{
  if ( mWidth == width )
    return;

  mWidth = width;

  emit widthChanged();
}

QColor Rubberband::color() const
{
  return mColor;
}

void Rubberband::setColor( const QColor &color )
{
  if ( mColor == color )
    return;

  mColor = color;

  emit colorChanged();
}

qreal Rubberband::widthCurrentPoint() const
{
  return mWidthCurrentPoint;
}

void Rubberband::setWidthCurrentPoint( qreal width )
{
  if ( mWidthCurrentPoint == width )
    return;

  mWidthCurrentPoint = width;

  emit widthCurrentPointChanged();
}

int Rubberband::trackTimeInterval() const
{
  return mTrackTimeInterval;
}

void Rubberband::setTrackTimeInterval( int trackTimeInterval )
{
  mTrackTimeInterval = trackTimeInterval;
}

int Rubberband::trackMinimumDistance() const
{
  return mTrackMinimumDistance;
}

void Rubberband::setTrackConjunction( bool trackConjunction )
{
  mTrackConjunction = trackConjunction;
}

bool Rubberband::trackConjunction() const
{
  return mTrackConjunction;
}

void Rubberband::setTrackMinimumDistance( int trackMinimumDistance )
{
  mTrackMinimumDistance = trackMinimumDistance;
}

void Rubberband::trackPosition()
{
  if ( std::isnan( model()->currentCoordinate().x() ) || std::isnan( model()->currentCoordinate().y() ) )
  {
    qDebug() << QString( "Coordinates not available " ) << " x:" << model()->currentCoordinate().x() << " y:" << model()->currentCoordinate().y();
    return;
  }
  qDebug() << QString( "Collect " ) << model()->vectorLayer() << " x:" << model()->currentCoordinate().x() << " y:" << model()->currentCoordinate().y() << " z:" << model()->currentCoordinate().z();
  model()->addVertex();
  mTrackTimeIntervalFulfilled = false;
  mTrackMinimumDistanceFulfilled = false;
}

void Rubberband::trackPositionReceived()
{

  QVector<QgsPointXY> points = mRubberbandModel->flatPointSequence( QgsProject::instance()->crs() );

  auto pointIt = points.constEnd() - 1;

  QVector<QgsPointXY> flatPoints;

  flatPoints << *pointIt;
  pointIt--;
  flatPoints << *pointIt;

  QgsDistanceArea distanceArea;
  distanceArea.setEllipsoid( QgsProject::instance()->ellipsoid() );
  distanceArea.setSourceCrs( QgsProject::instance()->crs(), QgsProject::instance()->transformContext() );

  qDebug() << QString( "distance is: " ) << distanceArea.measureLine( flatPoints ) << QString( " and the minimum is " ) << mTrackMinimumDistance;

  if ( distanceArea.measureLine( flatPoints ) > mTrackMinimumDistance )
  {
    mTrackMinimumDistanceFulfilled = true;
    if ( !mTrackConjunction || mTrackTimeIntervalFulfilled )
      trackPosition();
  }
}

void Rubberband::trackTimeReceived()
{
  mTrackTimeIntervalFulfilled = true;
  if ( !mTrackConjunction || mTrackMinimumDistanceFulfilled )
    trackPosition();
}

void Rubberband::trackStart()
{
  if ( mTrackTimeInterval > 0 )
  {
    connect( &mTrackTimer, &QTimer::timeout, this, &Rubberband::trackTimeReceived );
    mTrackTimer.start( mTrackTimeInterval * 1000 );
  }
  if ( mTrackMinimumDistance > 0 )
  {
    connect( mRubberbandModel, &RubberbandModel::currentCoordinateChanged, this, &Rubberband::trackPositionReceived );
  }

  qDebug() << QString( "Tracos startos with time" ) << mTrackTimeInterval << " and distance " << mTrackMinimumDistance;

  //track first position
  trackPosition();
}

void Rubberband::trackStop()
{
  if ( mTrackTimeInterval > 0 )
  {
    mTrackTimer.stop();
    disconnect( &mTrackTimer, &QTimer::timeout, this, &Rubberband::trackPosition );
  }
  if ( mTrackMinimumDistance > 0 )
  {
    disconnect( mRubberbandModel,  &RubberbandModel::currentCoordinateChanged, this, &Rubberband::trackPositionReceived );
  }

  qDebug() << QString( "Tracos stoppos" );
}

QColor Rubberband::colorCurrentPoint() const
{
  return mColorCurrentPoint;
}

void Rubberband::setColorCurrentPoint( const QColor &color )
{
  if ( mColorCurrentPoint == color )
    return;

  mColorCurrentPoint = color;

  emit colorCurrentPointChanged();
}

