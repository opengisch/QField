/***************************************************************************
                            layerresolver.cpp

                              -------------------
              begin                : January 2021
              copyright            : (C) 2011 by Matthias Kuhn
              email                : matthias@opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "layerresolver.h"
#include <qgsvectorlayerref.h>

LayerResolver::LayerResolver( QObject *parent )
  : QObject( parent )
{

}

void LayerResolver::resolve()
{
  if ( !mProject )
  {
    setLayer( nullptr );
    return;
  }

  QgsVectorLayerRef ref { mLayerId,
                          mLayerName,
                          mLayerSource,
                          mLayerProviderName };
  setLayer( ref.resolveByIdOrNameOnly( mProject ) );
}

QString LayerResolver::layerId() const
{
  return mLayerId;
}

void LayerResolver::setLayerId( const QString &layerId )
{
  if ( mLayerId == layerId )
    return;

  mLayerId = layerId;
  resolve();
  emit layerIdChanged();
}

QString LayerResolver::layerName() const
{
  return mLayerName;
}

void LayerResolver::setLayerName( const QString &layerName )
{
  if ( mLayerName == layerName )
    return;

  mLayerName = layerName;
  resolve();
  emit layerNameChanged();
}

QString LayerResolver::layerSource() const
{
  return mLayerSource;
}

void LayerResolver::setLayerSource( const QString &layerSource )
{
  if ( mLayerSource == layerSource )
    return;

  mLayerSource = layerSource;
  emit layerSourceChanged();
}

QString LayerResolver::layerProviderName() const
{
  return mLayerProviderName;
}

void LayerResolver::setLayerProviderName( const QString &layerProviderName )
{
  if ( mLayerProviderName == layerProviderName )
    return;

  mLayerProviderName = layerProviderName;
  resolve();
  emit layerProviderNameChanged();
}

QgsVectorLayer *LayerResolver::currentLayer() const
{
  return mLayer;
}

void LayerResolver::setLayer( QgsVectorLayer *layer )
{
  if ( mLayer == layer )
    return;

  mLayer = layer;
  emit currentLayerChanged();
}

QgsProject *LayerResolver::project() const
{
  return mProject;
}

void LayerResolver::setProject( QgsProject *project )
{
  if ( project == mProject )
    return;

  mProject = project;
  resolve();
  emit projectChanged();
}
