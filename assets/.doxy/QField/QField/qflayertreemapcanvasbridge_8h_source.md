

# File qflayertreemapcanvasbridge.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qflayertreemapcanvasbridge.h**](qflayertreemapcanvasbridge_8h.md)

[Go to the documentation of this file](qflayertreemapcanvasbridge_8h.md)


```C++
/***************************************************************************
  qflayertreemapcanvasbridge.h - QfLayerTreeMapCanvasBridge

 ---------------------
 begin                : 26.8.2016
 copyright            : (C) 2016 by Matthias Kuhn, OPENGIS.ch
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QFLAYERTREEMAPCANVASBRIDGE_H
#define QFLAYERTREEMAPCANVASBRIDGE_H

#include "qflayertreemodel.h"
#include "qftrackingmodel.h"

#include <QObject>
#include <QStringList>
#include <qgscoordinatereferencesystem.h>
#include <qgsmapthemecollection.h>

class QgsLayerTreeGroup;
class QgsLayerTreeNode;
class QgsQuickMapSettings;
class QgsMapCanvasLayer;
class QgsMapLayer;

class QfLayerTreeMapCanvasBridge : public QObject
{
    Q_OBJECT
  public:
    QfLayerTreeMapCanvasBridge( QfFlatLayerTreeModel *model, QgsQuickMapSettings *mapSettings, QfTrackingModel *trackingModel, QObject *parent = nullptr );

    QgsLayerTree *rootGroup() const { return mRoot; }
    QgsQuickMapSettings *mapSettings() const { return mMapSettings; }

    void setAutoSetupOnFirstLayer( bool enabled ) { mAutoSetupOnFirstLayer = enabled; }
    bool autoSetupOnFirstLayer() const { return mAutoSetupOnFirstLayer; }

    void setAutoEnableCrsTransform( bool enabled )
    {
      mAutoEnableCrsTransform = enabled;
    }
    bool autoEnableCrsTransform() const
    {
      return mAutoEnableCrsTransform;
    }

    Q_INVOKABLE void setCanvasLayers();

  private slots:
    void extentChanged();
    void nodeVisibilityChanged();
    void mapThemeChanged();
    void layersChanged();
    void layerInTrackingChanged( QgsVectorLayer *layer, bool tracking );

  private:
    void setCanvasLayers( QgsLayerTreeNode *node, QList<QgsMapLayer *> &canvasLayers, QList<QgsMapLayer *> &allLayers );

    void deferredSetCanvasLayers();

    QgsLayerTree *mRoot = nullptr;
    QfFlatLayerTreeModel *mModel = nullptr;
    QgsQuickMapSettings *mMapSettings = nullptr;
    QfTrackingModel *mTrackingModel = nullptr;

    bool mPendingCanvasUpdate;

    bool mHasCustomLayerOrder;
    QStringList mCustomLayerOrder;

    bool mAutoSetupOnFirstLayer;
    bool mAutoEnableCrsTransform;

    bool mHasFirstLayer;
    bool mHasLayersLoaded;

    QgsCoordinateReferenceSystem mFirstCRS;
};

#endif // QFLAYERTREEMAPCANVASBRIDGE_H
```


