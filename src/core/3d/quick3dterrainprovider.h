/***************************************************************************
  quick3dterrainprovider.h - Quick3DTerrainProvider

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
#ifndef QUICK3DTERRAINPROVIDER_H
#define QUICK3DTERRAINPROVIDER_H

#include "qgsquickmapsettings.h"

#include <QFutureWatcher>
#include <QObject>
#include <QPointer>
#include <QSize>
#include <QVariantList>
#include <QVector3D>
#include <qgspoint.h>
#include <qgsproject.h>
#include <qgsrectangle.h>

#include <memory>

class QgsRasterLayer;
class QgsAbstractTerrainProvider;

/**
 * Provides terrain elevation data for 3D visualization from DEM raster layers
 * or online elevation services.
 *
 * This class handles elevation data retrieval from either a project's DEM raster layer
 * or falls back to QGIS terrain providers (e.g., online Terrarium tiles) when no DEM
 * is available. The data is normalized and provided as height samples for terrain mesh
 * generation in Qt Quick 3D.
 *
 * \note QML Type: Quick3DTerrainProvider
 * \ingroup core
 *
 */
class Quick3DTerrainProvider : public QObject
{
    Q_OBJECT

    //! The project from which to read the terrain configuration and DEM layer
    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )

    //! The map settings from which to get extent for terrain generation
    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )

    //! Grid dimensions (width x height) for terrain sampling
    Q_PROPERTY( QSize gridSize READ gridSize NOTIFY gridSizeChanged )

    //! Geographic extent used for terrain data
    Q_PROPERTY( QgsRectangle extent READ extent NOTIFY extentChanged )

    //! Size of the terrain
    Q_PROPERTY( QSizeF size READ size NOTIFY extentChanged )

    //! Whether the extent's shorter side will be expanded to form a square terrain
    Q_PROPERTY( bool forceSquareSize READ forceSquareSize WRITE setForceSquareSize NOTIFY forceSquareSizeChanged );

    //! Normalized height data array [0.0-1.0] for terrain mesh generation
    Q_PROPERTY( QVariantList normalizedData READ normalizedData NOTIFY normalizedDataChanged )

    //! Geographic extent used for terrain data
    Q_PROPERTY( QgsRectangle normalizedDataExtent READ normalizedDataExtent NOTIFY normalizedDataChanged )

    //! Offset vector from the last generated terrain data
    Q_PROPERTY( QVector3D offsetVector READ offsetVector NOTIFY offsetVectorChanged )

    //! Offset scale from the last generated terrain data
    Q_PROPERTY( double offsetScale READ offsetScale NOTIFY offsetScaleChanged )

    //! Whether a transition to apply pan/zoom offsets has begun
    Q_PROPERTY( bool isTransitioning READ isTransitioning NOTIFY isTransitioningChanged )

    //! Whether terrain data is currently being loaded
    Q_PROPERTY( bool isLoading READ isLoading NOTIFY isLoadingChanged )

  public:
    //! Creates a new terrain provider
    explicit Quick3DTerrainProvider( QObject *parent = nullptr );
    ~Quick3DTerrainProvider() override;

    //! Returns the project from which to read terrain configuration and DEM layer.
    QgsProject *project() const;

    //! Sets the project.
    void setProject( QgsProject *project );

    //! Returns the map settings.
    QgsQuickMapSettings *mapSettings() const;

    //! Sets the map settings.
    void setMapSettings( QgsQuickMapSettings *mapSettings );

    bool forceSquareSize() const;

    void setForceSquareSize( bool forceSquareSize );

    //! Returns the grid dimensions for terrain sampling.
    QSize gridSize() const { return mGridSize; }

    //! Returns the current extent taking into account offsets.
    QgsRectangle extent() const { return mExtent; }

    //! Returns the geographic extent for the retrieved terrain data.
    QgsRectangle normalizedDataExtent() const { return mNormalizedDataExtent; }

    //! Returns the size of the terrain.
    QSizeF size() const { return mSize; }

    //! Returns the normalized height data array [0.0-1.0].
    QVariantList normalizedData() const;

    //! Returns the offset scale from the last generated terrain data
    QVector3D offsetVector() const { return mOffsetVector; }

    //! Returns the offset scale from the last generated terrain data
    double offsetScale() const { return mOffsetScale; }

    bool isTransitioning() const { return mIsTransitioning; }

    //! Returns TRUE if terrain data is currently being loaded.
    bool isLoading() const;

    /**
     * Returns the real height value at the specified map coordinates.
     * \param x X coordinate in map CRS
     * \param y Y coordinate in map CRS
     * \returns Height in map units, or 0 if outside extent
     */
    Q_INVOKABLE double heightAt( double x, double y ) const;

    /**
     * Returns the normalized height value [0.0-1.0] at the specified map coordinates.
     * \param x X coordinate in map CRS
     * \param y Y coordinate in map CRS
     * \returns Normalized height between 0 and 1, or 0 if outside extent
     */
    Q_INVOKABLE double normalizedHeightAt( double x, double y ) const;

    /**
     * Converts geographic coordinates to a 3D scene position.
     * \param geoX X coordinate in map CRS
     * \param geoY Y coordinate in map CRS
     * \param heightOffset additional vertical offset (default 0)
     * \returns 3D position in scene space, or a null vector if the extent is invalid
     */
    Q_INVOKABLE QVector3D geoTo3D( double geoX, double geoY, float heightOffset = 0.0f ) const;

    /**
     * Converts a 3D scene position back to geographic coordinates in map CRS.
     * Inverse of geoTo3D(); returns an empty QgsPoint when the extent is invalid.
     */
    Q_INVOKABLE QgsPoint scene3DToGeo( double sceneX, double sceneZ ) const;

    //! Regenerates the extent and terrain data from current offset
    Q_INVOKABLE void beginTransition();

    //! Applies offsets and reset values to zeros
    Q_INVOKABLE void endTransition();

    //! Applies X/Z offsets to the stored terrain based on pan action
    Q_INVOKABLE void pan( double x, double z );

    //! Applies scale offset to the stored terrain based on zoom action
    Q_INVOKABLE void zoom( double factor );

  signals:
    void projectChanged();
    void mapSettingsChanged();
    void forceSquareSizeChanged();
    void gridSizeChanged();
    void extentChanged();

    //! Emitted when terrain data has been successfully loaded and is ready for use
    void terrainDataReady();

    void normalizedDataChanged();
    void offsetVectorChanged();
    void offsetScaleChanged();
    void isTransitioningChanged();

    void isLoadingChanged();

  private:
    void updateTerrainProvider();
    void updateFromMapSettings();
    void updateExtentFromOffsets();
    void calculateResolution();
    void generateData();

    //! Calculates terrain heights asynchronously in a worker thread and normalizes the data
    void calcNormalizedData();

    void onTerrainDataCalculated();
    double sampleHeightFromTerrainProvider( double x, double y ) const;

  private:
    QgsProject *mProject = nullptr;
    QgsQuickMapSettings *mMapSettings = nullptr;

    std::unique_ptr<QgsAbstractTerrainProvider> mTerrainProvider;

    QgsRectangle mExtent;
    QSizeF mSize;
    bool mForceSquareSize = false;
    double mBaseSize = 2000.0;

    QSize mGridSize = QSize( 64, 64 );
    QVariantList mNormalizedData;
    QgsRectangle mNormalizedDataExtent;

    bool mIsLoading = false;
    bool mIsTransitioning = false;

    QVector3D mOffsetVector = QVector3D( 0, 0, 0 );
    double mOffsetScale = 1.0;

    double mMinRealHeight = 0.0;
    double mMaxRealHeight = 0.0;

    QFutureWatcher<QVector<double>> *mFutureWatcher = nullptr;
    QgsRectangle mFutureExtent;
};

#endif // QUICK3DTERRAINPROVIDER_H
