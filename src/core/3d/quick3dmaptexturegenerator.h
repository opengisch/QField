/***************************************************************************
  quick3dmaptexturegenerator.h - Quick3DMapTextureGenerator

 ---------------------
 begin                : 26.1.2026
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
#ifndef QUICK3DMAPTEXTUREGENERATOR_H
#define QUICK3DMAPTEXTUREGENERATOR_H

#include "qgsquickmapsettings.h"

#include <QImage>
#include <QObject>
#include <QStandardPaths>
#include <qgsproject.h>
#include <qgsrectangle.h>

class QgsMapRendererSequentialJob;

/**
 * Generates a 2D map texture from project layers for use as terrain surface material.
 *
 * This class renders the map canvas view to an image file, which is then applied as
 * a texture on the 3D terrain mesh. The rendering respects the project's layer visibility,
 * symbology, and styling, providing a photorealistic texture for terrain visualization.
 *
 * \note QML Type: Quick3DMapTextureGenerator
 * \ingroup core
 *
 */
class Quick3DMapTextureGenerator : public QObject
{
    Q_OBJECT

    //! The map settings from which to get layers, extent, and output size for rendering
    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )

    //! Optional custom extent to render. If not set, uses mapSettings extent
    Q_PROPERTY( QgsRectangle extent READ extent WRITE setExtent NOTIFY extentChanged )

    //! File path to the generated texture image (readonly)
    Q_PROPERTY( QString textureFilePath READ textureFilePath NOTIFY textureFilePathChanged )

  public:
    //! Creates a new map texture generator
    explicit Quick3DMapTextureGenerator( QObject *parent = nullptr );
    ~Quick3DMapTextureGenerator() override;

    //! Returns the map settings from which to get layers.
    QgsQuickMapSettings *mapSettings() const;

    //! Sets the map settings.
    void setMapSettings( QgsQuickMapSettings *mapSettings );

    //! Returns the custom extent for rendering.
    QgsRectangle extent() const;

    //! Sets a custom extent for rendering. If empty, uses mapSettings extent.
    void setExtent( const QgsRectangle &extent );

    //! Returns the file path to the generated texture image.
    QString textureFilePath() const;

    /**
     * Starts the asynchronous map rendering process.
     * The ready() signal is emitted when rendering completes.
     */
    Q_INVOKABLE void render();

  signals:
    void mapSettingsChanged();
    void extentChanged();

    //! Emitted when texture rendering is complete and file is ready
    void ready();

    void textureFilePathChanged();

  private slots:
    void onRenderFinished();

  private:
    QgsQuickMapSettings *mMapSettings = nullptr;
    QgsRectangle mExtent;
    QImage mRenderedImage;
    QString mTextureFilePath;
    std::unique_ptr<QgsMapRendererSequentialJob> mRenderJob;

    static int sInstanceCounter;
};

#endif // QUICK3DMAPTEXTUREGENERATOR_H
