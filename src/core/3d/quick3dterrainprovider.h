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

#include <QObject>
#include <QPointer>
#include <QVariantList>
#include <qgsrectangle.h>

#include <memory>

class QgsProject;
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

    //! Number of height samples per terrain axis, determines terrain mesh density
    Q_PROPERTY( int resolution READ resolution WRITE setResolution NOTIFY resolutionChanged )

    //! Geographic extent for which to retrieve terrain data
    Q_PROPERTY( QgsRectangle extent READ extent WRITE setExtent NOTIFY extentChanged )

    //! Normalized height data array [0.0-1.0] for terrain mesh generation
    Q_PROPERTY( QVariantList normalizedData READ normalizedData NOTIFY normalizedDataChanged )

    //! Base size of the terrain in map units, used for scaling calculations
    Q_PROPERTY( int terrainBaseSize READ terrainBaseSize CONSTANT )

    //! Whether a DEM layer is available in the project
    Q_PROPERTY( bool hasDemLayer READ hasDemLayer NOTIFY hasDemLayerChanged )

    //! Whether terrain data is currently being loaded
    Q_PROPERTY( bool isLoading READ isLoading NOTIFY isLoadingChanged )

    //! Loading progress percentage (0-100) for online terrain providers
    Q_PROPERTY( int loadingProgress READ loadingProgress NOTIFY loadingProgressChanged )

  public:
    //! Creates a new terrain provider
    explicit Quick3DTerrainProvider( QObject *parent = nullptr );
    ~Quick3DTerrainProvider() override;

    //! Returns the project from which to read terrain configuration and DEM layer.
    QgsProject *project() const;

    //! Sets the project.
    void setProject( QgsProject *project );

    //! Returns the number of height samples per terrain axis.
    int resolution() const;

    //! Sets the resolution.
    void setResolution( int resolution );

    //! Returns the geographic extent for terrain data retrieval.
    QgsRectangle extent() const;

    //! Sets the extent.
    void setExtent( const QgsRectangle &extent );

    //! Returns the normalized height data array [0.0-1.0].
    QVariantList normalizedData() const;

    //! Returns the base size of the terrain in map units.
    int terrainBaseSize() const;

    //! Returns TRUE if a DEM layer is available in the project.
    bool hasDemLayer() const;

    //! Returns TRUE if terrain data is currently being loaded.
    bool isLoading() const;

    //! Returns the loading progress percentage (0-100).
    int loadingProgress() const;

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
     * Calculates recommended vertical exaggeration factor based on terrain height range.
     * \returns Exaggeration multiplier to enhance terrain visibility
     */
    Q_INVOKABLE double calculateVisualExaggeration() const;

  signals:
    void projectChanged();
    void resolutionChanged();
    void extentChanged();

    //! Emitted when terrain data has been successfully loaded and is ready for use
    void terrainDataReady();

    void normalizedDataChanged();
    void hasDemLayerChanged();
    void isLoadingChanged();
    void loadingProgressChanged();

  private:
    void updateTerrainProvider();
    void calcNormalizedData();
    double sampleHeightFromRaster( QgsRasterLayer *layer, double x, double y ) const;
    double sampleHeightFromTerrainProvider( double x, double y ) const;

  private:
    QgsProject *mProject = nullptr;
    QPointer<QgsRasterLayer> mDemLayer;
    std::unique_ptr<QgsAbstractTerrainProvider> mQgisTerrainProvider;
    int mResolution = 32;
    double mTerrainBaseSize = 2000.0;
    QgsRectangle mExtent;
    QVariantList mNormalizedData;
    double mMinRealHeight = 0.0;
    double mMaxRealHeight = 0.0;
    bool mIsLoading = false;
    int mLoadingProgress = 0;
};

#endif // QUICK3DTERRAINPROVIDER_H
