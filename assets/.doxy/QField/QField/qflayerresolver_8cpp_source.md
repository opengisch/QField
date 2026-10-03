

# File qflayerresolver.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qflayerresolver.cpp**](qflayerresolver_8cpp.md)

[Go to the documentation of this file](qflayerresolver_8cpp.md)


```C++
/***************************************************************************
                            qflayerresolver.cpp

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
#include "qflayerresolver.h"

#include <qgsvectorlayerref.h>

QfLayerResolver::QfLayerResolver( QObject *parent )
  : QObject( parent )
{
}

void QfLayerResolver::resolve()
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

QString QfLayerResolver::layerId() const
{
  return mLayerId;
}

void QfLayerResolver::setLayerId( const QString &layerId )
{
  if ( mLayerId == layerId )
    return;

  mLayerId = layerId;
  resolve();
  emit layerIdChanged();
}

QString QfLayerResolver::layerName() const
{
  return mLayerName;
}

void QfLayerResolver::setLayerName( const QString &layerName )
{
  if ( mLayerName == layerName )
    return;

  mLayerName = layerName;
  resolve();
  emit layerNameChanged();
}

QString QfLayerResolver::layerSource() const
{
  return mLayerSource;
}

void QfLayerResolver::setLayerSource( const QString &layerSource )
{
  if ( mLayerSource == layerSource )
    return;

  mLayerSource = layerSource;
  emit layerSourceChanged();
}

QString QfLayerResolver::layerProviderName() const
{
  return mLayerProviderName;
}

void QfLayerResolver::setLayerProviderName( const QString &layerProviderName )
{
  if ( mLayerProviderName == layerProviderName )
    return;

  mLayerProviderName = layerProviderName;
  resolve();
  emit layerProviderNameChanged();
}

QgsVectorLayer *QfLayerResolver::currentLayer() const
{
  return mLayer.data();
}

void QfLayerResolver::setLayer( QgsVectorLayer *layer )
{
  if ( mLayer == layer )
    return;

  mLayer = layer;
  emit currentLayerChanged();
}

QgsProject *QfLayerResolver::project() const
{
  return mProject;
}

void QfLayerResolver::setProject( QgsProject *project )
{
  if ( project == mProject )
    return;

  mProject = project;
  resolve();
  emit projectChanged();
}
```


