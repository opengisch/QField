

# File qf3dmaptexturedata.h

[**File List**](files.md) **>** [**3d**](dir_4b0af5720da35ca08cf9fbb2fd056889.md) **>** [**qf3dmaptexturedata.h**](qf3dmaptexturedata_8h.md)

[Go to the documentation of this file](qf3dmaptexturedata_8h.md)


```C++
/***************************************************************************
  qf3dmaptexturedata.h - Qf3DMapTextureData

 ---------------------
 begin                : 30.1.2026
 copyright            : (C) 2026 by Mohsen Dehghanzadeh
 email                : mohsen@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QF3DMAPTEXTUREDATA_H
#define QF3DMAPTEXTUREDATA_H

#include "qgsquickmapsettings.h"

#include <QImage>
#include <QTimer>
#include <QtQuick3D/QQuick3DTextureData>
#include <qgsrectangle.h>
#include <qobjectuniqueptr.h>

class QgsMapRendererParallelJob;

class Qf3DMapTextureData : public QQuick3DTextureData
{
    Q_OBJECT

    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )

    
    Q_PROPERTY( QgsRectangle extent READ extent WRITE setExtent NOTIFY extentChanged )

    Q_PROPERTY( bool isRendering READ isRendering NOTIFY isRenderingChanged )

    Q_PROPERTY( bool isReady READ isReady NOTIFY isReadyChanged )

    Q_PROPERTY( bool incrementalRendering READ incrementalRendering WRITE setIncrementalRendering NOTIFY incrementalRenderingChanged )

    Q_PROPERTY( bool forceDeferredLayersRepaint READ forceDeferredLayersRepaint WRITE setForceDeferredLayersRepaint NOTIFY forceDeferredLayersRepaintChanged )

  public:
    explicit Qf3DMapTextureData( QQuick3DObject *parent = nullptr );
    ~Qf3DMapTextureData() override;

    QgsQuickMapSettings *mapSettings() const;

    void setMapSettings( QgsQuickMapSettings *mapSettings );

    QgsRectangle extent() const;

    void setExtent( const QgsRectangle &extent );

    bool isRendering() const;

    bool isReady() const;

    bool incrementalRendering() const;

    void setIncrementalRendering( bool incrementalRendering );

    bool forceDeferredLayersRepaint() const;

    void setForceDeferredLayersRepaint( bool deferred );

    Q_INVOKABLE void render();

  signals:
    void mapSettingsChanged();

    void extentChanged();

    void isRenderingChanged();

    void isReadyChanged();

    void textureUpdated();

    void incrementalRenderingChanged();

    void forceDeferredLayersRepaintChanged();

  private slots:
    void onRenderJobUpdated();
    void onRenderFinished();
    void layerRepaintRequested();

  private:
    void updateTextureData( const QImage &image );
    void refresh();

    QgsQuickMapSettings *mMapSettings = nullptr;
    QgsRectangle mExtent;
    QObjectUniquePtr<QgsMapRendererParallelJob> mRenderJob;
    QTimer mMapUpdateTimer;
    QTimer mRefreshTimer;
    bool mIncrementalRendering = false;
    bool mForceDeferredLayersRepaint = false;
    bool mDeferredRefreshPending = false;
    bool mIsReady = false;
    QVector<QMetaObject::Connection> mLayerConnections;
};

#endif // QF3DMAPTEXTUREDATA_H
```


