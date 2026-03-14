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

/**
 * Provides in-memory texture data from rendered map layers for 3D terrain visualization.
 *
 * This class inherits from QQuick3DTextureData to provide texture data directly in memory,
 * avoiding the need for temporary files. The map is rendered using QGIS rendering engine
 * and the result is exposed as texture data for Qt Quick 3D.
 *
 * \note QML Type: Quick3DMapTextureData
 * \ingroup core
 */
class Quick3DMapTextureData : public QQuick3DTextureData
{
    Q_OBJECT
    QML_ELEMENT

    //! The map settings from which to get layers, extent, and output size for rendering
    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )

    //! Optional custom extent to render. If not set, uses mapSettings extent
    Q_PROPERTY( QgsRectangle extent READ extent WRITE setExtent NOTIFY extentChanged )

    //! Whether the texture data is being rendered
    Q_PROPERTY( bool isRendering READ isRendering NOTIFY isRenderingChanged )

    //! Whether the texture data is ready to use
    Q_PROPERTY( bool isReady READ isReady NOTIFY isReadyChanged )

    //! When the incrementalRendering property is set to true, the incremental refresh of the terrain data during rendering is allowed
    Q_PROPERTY( bool incrementalRendering READ incrementalRendering WRITE setIncrementalRendering NOTIFY incrementalRenderingChanged )

    //! Defers layer repaint requests while a render job is running
    Q_PROPERTY( bool forceDeferredLayersRepaint READ forceDeferredLayersRepaint WRITE setForceDeferredLayersRepaint NOTIFY forceDeferredLayersRepaintChanged )

  public:
    //! Creates a new map texture data provider
    explicit Quick3DMapTextureData( QQuick3DObject *parent = nullptr );
    ~Quick3DMapTextureData() override;

    //! Returns the map settings from which to get layers.
    QgsQuickMapSettings *mapSettings() const;

    //! Sets the map settings.
    void setMapSettings( QgsQuickMapSettings *mapSettings );

    //! Returns the custom extent for rendering.
    QgsRectangle extent() const;

    //! Sets a custom extent for rendering.
    void setExtent( const QgsRectangle &extent );

    //! Returns true while a rendering job is pending for this texture data.
    bool isRendering() const;

    //! Returns whether the texture data is ready to use.
    bool isReady() const;

    //! Returns whether incremental rendering is enabled.
    bool incrementalRendering() const;

    //! Sets whether incremental rendering is enabled.
    void setIncrementalRendering( bool incrementalRendering );

    //! Returns whether deferred layers repaint is forced.
    bool forceDeferredLayersRepaint() const;

    //! Sets whether deferred layers repaint is forced.
    void setForceDeferredLayersRepaint( bool deferred );

    /**
     * Starts the asynchronous map rendering process.
     */
    Q_INVOKABLE void render();

  signals:
    //! Emitted when map settings changes
    void mapSettingsChanged();

    //! Emitted when extent changes
    void extentChanged();

    //! Emitted when the texture data is being rendered
    void isRenderingChanged();

    //! Emitted when texture rendering is complete and data is ready
    void isReadyChanged();

    //! Emitted every time texture data has been updated
    void textureUpdated();

    //! Emitted when incremental rendering setting changes
    void incrementalRenderingChanged();

    //! Emitted when forceDeferredLayersRepaint changes
    void forceDeferredLayersRepaintChanged();

  private slots:
    void onRenderJobUpdated();
    void onRenderFinished();
    void layerRepaintRequested();

  private:
    /**
     * Create a 3x3 metagrid: the center block is the rendered map,
     * surrounding blocks are a neutral gray representing non-rendered areas.
     * This allows panning via Texture positionU/V to reveal gray edges
     */
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

#endif // QUICK3DMAPTEXTUREDATA_H
