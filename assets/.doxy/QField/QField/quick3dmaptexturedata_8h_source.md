

# File quick3dmaptexturedata.h

[**File List**](files.md) **>** [**3d**](dir_8623093fea4462c874263d69f8dca7e9.md) **>** [**quick3dmaptexturedata.h**](quick3dmaptexturedata_8h.md)

[Go to the documentation of this file](quick3dmaptexturedata_8h.md)


```C++
/***************************************************************************
  quick3dmaptexturedata.h - Quick3DMapTextureData

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
#ifndef QUICK3DMAPTEXTUREDATA_H
#define QUICK3DMAPTEXTUREDATA_H

#include "qgsquickmapsettings.h"

#include <QImage>
#include <QTimer>
#include <QtQuick3D/QQuick3DTextureData>
#include <qgsrectangle.h>
#include <qobjectuniqueptr.h>

class QgsMapRendererParallelJob;

class Quick3DMapTextureData : public QQuick3DTextureData
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )

    
    Q_PROPERTY( QgsRectangle extent READ extent WRITE setExtent NOTIFY extentChanged )

    Q_PROPERTY( bool ready READ isReady NOTIFY readyChanged )

    Q_PROPERTY( bool incrementalRendering READ incrementalRendering WRITE setIncrementalRendering NOTIFY incrementalRenderingChanged )

    Q_PROPERTY( bool forceDeferredLayersRepaint READ forceDeferredLayersRepaint WRITE setForceDeferredLayersRepaint NOTIFY forceDeferredLayersRepaintChanged )

  public:
    explicit Quick3DMapTextureData( QQuick3DObject *parent = nullptr );
    ~Quick3DMapTextureData() override;

    QgsQuickMapSettings *mapSettings() const;

    void setMapSettings( QgsQuickMapSettings *mapSettings );

    QgsRectangle extent() const;

    void setExtent( const QgsRectangle &extent );

    bool isReady() const;

    bool incrementalRendering() const;

    void setIncrementalRendering( bool incrementalRendering );

    bool forceDeferredLayersRepaint() const;

    void setForceDeferredLayersRepaint( bool deferred );

    Q_INVOKABLE void render();

  signals:
    void mapSettingsChanged();

    void extentChanged();

    void readyChanged();

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
    bool mReady = false;
    QVector<QMetaObject::Connection> mLayerConnections;
};

#endif // QUICK3DMAPTEXTUREDATA_H
```


