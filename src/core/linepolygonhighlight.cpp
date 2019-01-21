/***************************************************************************
              locatorhighlight.cpp
               ----------------------------------------------------
              date                 : 9.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qgsgeometry.h>
#include <qgscoordinatereferencesystem.h>
#include <qgsproject.h>

#include "linepolygonhighlight.h"

#include "qgsgeometrywrapper.h"
#include "qgssggeometry.h"


LinePolygonHighlight::LinePolygonHighlight( QQuickItem *parent )
  : QQuickItem( parent )
  , mDirty( false )
  , mMapSettings( nullptr )
{
  setFlags( QQuickItem::ItemHasContents );
  setAntialiasing( true );
}

QSGNode *LinePolygonHighlight::updatePaintNode( QSGNode *n, QQuickItem::UpdatePaintNodeData * )
{
  if ( mDirty && mMapSettings )
  {
    delete n;
    n = new QSGNode;

    QgsGeometry geometry;
    if ( mGeometry )
    {
      geometry = QgsGeometry( mGeometry->qgsGeometry() );

      if ( mMapSettings )
      {
        QgsCoordinateTransform ct( mGeometry->crs(), mMapSettings->destinationCrs(), QgsProject::instance()->transformContext() );
        geometry.transform( ct );
      }
    }

    QgsSGGeometry *gn = new QgsSGGeometry( geometry, mColor, mWidth );
    gn->setFlag( QSGNode::OwnedByParent );
    n->appendChildNode( gn );

    mDirty = false;

    emit updated();
  }

  return n;
}

QgsQuickMapSettings *LinePolygonHighlight::mapSettings() const
{
  return mMapSettings;
}

void LinePolygonHighlight::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
    return;

  if ( mMapSettings )
    disconnect( mMapSettings, &QgsQuickMapSettings::destinationCrsChanged, this, &LinePolygonHighlight::mapCrsChanged );


  mMapSettings = mapSettings;

  connect( mMapSettings, &QgsQuickMapSettings::destinationCrsChanged, this, &LinePolygonHighlight::mapCrsChanged );

  emit mapSettingsChanged();
}

void LinePolygonHighlight::mapCrsChanged()
{
  mDirty = true;
  update();
}

QgsGeometryWrapper *LinePolygonHighlight::geometry() const
{
  return mGeometry;
}

void LinePolygonHighlight::setGeometry( QgsGeometryWrapper *geometry )
{
  if ( mGeometry == geometry )
    return;

  Q_ASSERT( geometry->qgsGeometry().type() != QgsWkbTypes::PointGeometry );

  if ( mGeometry )
    mGeometry->deleteLater(); // crashes

  mGeometry = geometry;

  mDirty = true;
  emit qgsGeometryChanged();

  update();
}
