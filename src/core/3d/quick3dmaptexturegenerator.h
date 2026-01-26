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

    //! The project from which to render map layers
    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )

    //! Geographic extent to render for texture generation
    Q_PROPERTY( QgsRectangle extent READ extent WRITE setExtent NOTIFY extentChanged )

    //! File path to the generated texture image (readonly)
    Q_PROPERTY( QString textureFilePath READ textureFilePath NOTIFY textureFilePathChanged )

  public:
    //! Creates a new map texture generator
    explicit Quick3DMapTextureGenerator( QObject *parent = nullptr );
    ~Quick3DMapTextureGenerator() override;

    //! Returns the project from which to render map layers.
    QgsProject *project() const;

    //! Sets the project.
    void setProject( QgsProject *project );

    //! Returns the geographic extent to render.
    QgsRectangle extent() const;

    //! Sets the extent.
    void setExtent( const QgsRectangle &extent );

    //! Returns the file path to the generated texture image.
    QString textureFilePath() const;

    /**
     * Starts the asynchronous map rendering process.
     * The ready() signal is emitted when rendering completes.
     */
    Q_INVOKABLE void render();

  signals:
    void projectChanged();
    void extentChanged();

    //! Emitted when texture size changes
    void textureSizeChanged();

    //! Emitted when texture rendering is complete and file is ready
    void ready();

    void textureFilePathChanged();

  private slots:
    void onRenderFinished();

  private:
    QgsProject *mProject = nullptr;
    QgsRectangle mExtent;
    int mTextureSize = 1024;
    QImage mRenderedImage;
    QString mTextureFilePath;
    std::unique_ptr<QgsMapRendererSequentialJob> mRenderJob;

    static int sInstanceCounter;
};

#endif // QUICK3DMAPTEXTUREGENERATOR_H
