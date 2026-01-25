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

class Quick3DTerrainProvider : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )
    Q_PROPERTY( int resolution READ resolution WRITE setResolution NOTIFY resolutionChanged )
    Q_PROPERTY( QgsRectangle extent READ extent WRITE setExtent NOTIFY extentChanged )
    Q_PROPERTY( QVariantList normalizedData READ normalizedData NOTIFY normalizedDataChanged )
    Q_PROPERTY( int terrainBaseSize READ terrainBaseSize CONSTANT )
    Q_PROPERTY( bool hasDemLayer READ hasDemLayer NOTIFY hasDemLayerChanged )
    Q_PROPERTY( bool isLoading READ isLoading NOTIFY isLoadingChanged )
    Q_PROPERTY( int loadingProgress READ loadingProgress NOTIFY loadingProgressChanged )

  public:
    explicit Quick3DTerrainProvider( QObject *parent = nullptr );
    ~Quick3DTerrainProvider() override;

    QgsProject *project() const;
    void setProject( QgsProject *project );

    int resolution() const;
    void setResolution( int resolution );

    QgsRectangle extent() const;
    void setExtent( const QgsRectangle &extent );

    QVariantList normalizedData() const;
    int terrainBaseSize() const;

    bool hasDemLayer() const;
    bool isLoading() const;
    int loadingProgress() const;

    Q_INVOKABLE double heightAt( double x, double y ) const;
    Q_INVOKABLE double normalizedHeightAt( double x, double y ) const;
    Q_INVOKABLE double calculateVisualExaggeration() const;

  signals:
    void projectChanged();
    void resolutionChanged();
    void extentChanged();
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
