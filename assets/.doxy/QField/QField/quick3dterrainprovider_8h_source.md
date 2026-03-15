

# File quick3dterrainprovider.h

[**File List**](files.md) **>** [**3d**](dir_8623093fea4462c874263d69f8dca7e9.md) **>** [**quick3dterrainprovider.h**](quick3dterrainprovider_8h.md)

[Go to the documentation of this file](quick3dterrainprovider_8h.md)


```C++
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
#include <qgsproject.h>
#include <qgsrectangle.h>

#include <memory>

class QgsRasterLayer;
class QgsAbstractTerrainProvider;

class Quick3DTerrainProvider : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )

    
    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )

    Q_PROPERTY( QSize gridSize READ gridSize NOTIFY gridSizeChanged )

    Q_PROPERTY( QgsRectangle extent READ extent NOTIFY extentChanged )

    Q_PROPERTY( QSizeF size READ size NOTIFY extentChanged )

    Q_PROPERTY( bool forceSquareSize READ forceSquareSize WRITE setForceSquareSize NOTIFY forceSquareSizeChanged );

    Q_PROPERTY( QVariantList normalizedData READ normalizedData NOTIFY normalizedDataChanged )

    Q_PROPERTY( QgsRectangle normalizedDataExtent READ normalizedDataExtent NOTIFY normalizedDataChanged )

    Q_PROPERTY( QVector3D offsetVector READ offsetVector NOTIFY offsetVectorChanged )

    Q_PROPERTY( double offsetScale READ offsetScale NOTIFY offsetScaleChanged )

    Q_PROPERTY( bool isTransitioning READ isTransitioning NOTIFY isTransitioningChanged )

    Q_PROPERTY( bool isLoading READ isLoading NOTIFY isLoadingChanged )

  public:
    explicit Quick3DTerrainProvider( QObject *parent = nullptr );
    ~Quick3DTerrainProvider() override;

    QgsProject *project() const;

    void setProject( QgsProject *project );

    QgsQuickMapSettings *mapSettings() const;

    void setMapSettings( QgsQuickMapSettings *mapSettings );

    bool forceSquareSize() const;

    void setForceSquareSize( bool forceSquareSize );

    QSize gridSize() const { return mGridSize; }

    QgsRectangle extent() const { return mExtent; }

    QgsRectangle normalizedDataExtent() const { return mNormalizedDataExtent; }

    QSizeF size() const { return mSize; }

    QVariantList normalizedData() const;

    QVector3D offsetVector() const { return mOffsetVector; }

    double offsetScale() const { return mOffsetScale; }

    bool isTransitioning() const { return mIsTransitioning; }

    bool isLoading() const;

    Q_INVOKABLE double heightAt( double x, double y ) const;

    Q_INVOKABLE double normalizedHeightAt( double x, double y ) const;

    Q_INVOKABLE QVector3D geoTo3D( double geoX, double geoY, float heightOffset = 0.0f ) const;

    Q_INVOKABLE double calculateVisualExaggeration() const;

    Q_INVOKABLE void beginTransition();

    Q_INVOKABLE void endTransition();

    Q_INVOKABLE void pan( double x, double z );

    Q_INVOKABLE void zoom( double factor );

  signals:
    void projectChanged();
    void mapSettingsChanged();
    void forceSquareSizeChanged();
    void gridSizeChanged();
    void extentChanged();

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

    bool mIsTransitioning = false;
    QVector3D mOffsetVector = QVector3D( 0, 0, 0 );
    double mOffsetScale = 1.0;

    double mMinRealHeight = 0.0;
    double mMaxRealHeight = 0.0;

    bool mIsLoading = false;
    bool mRecalcPending = false;
    QFutureWatcher<QVector<double>> *mFutureWatcher = nullptr;
};

#endif // QUICK3DTERRAINPROVIDER_H
```


