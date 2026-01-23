/***************************************************************************
  qgsquick3dterrainprovider.h - QgsQuick3DTerrainProvider

 ---------------------
 begin                : 5.1.2026
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
#ifndef QGSQUICK3DTERRAINPROVIDER_H
#define QGSQUICK3DTERRAINPROVIDER_H

#include <QObject>
#include <QPointer>
#include <QVariantList>
#include <qgsrectangle.h>

class QgsProject;
class QgsRasterLayer;

class QgsQuick3DTerrainProvider : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )
    Q_PROPERTY( int resolution READ resolution WRITE setResolution NOTIFY resolutionChanged )
    Q_PROPERTY( QgsRectangle extent READ extent WRITE setExtent NOTIFY extentChanged )
    Q_PROPERTY( QVariantList normalizedData READ normalizedData NOTIFY normalizedDataChanged )
    Q_PROPERTY( int terrainBaseSize READ terrainBaseSize NOTIFY terrainBaseSizeChanged )

  public:
    explicit QgsQuick3DTerrainProvider( QObject *parent = nullptr );
    ~QgsQuick3DTerrainProvider() override;

    QgsProject *project() const;
    void setProject( QgsProject *project );

    int resolution() const;
    void setResolution( int resolution );

    QgsRectangle extent() const;
    void setExtent( const QgsRectangle &extent );

    QVariantList normalizedData() const;
    int terrainBaseSize() const;

    Q_INVOKABLE double heightAt( double x, double y ) const;
    Q_INVOKABLE double normalizedHeightAt( double x, double y ) const;
    Q_INVOKABLE QVariantMap terrainStats() const;
    Q_INVOKABLE double calculateVisualExaggeration() const;

  signals:
    void projectChanged();
    void resolutionChanged();
    void extentChanged();
    void terrainDataReady();
    void normalizedDataChanged();
    void terrainBaseSizeChanged();

  private:
    void updateTerrainProvider();
    void calcData();
    double sampleHeightFromRaster( QgsRasterLayer *layer, double x, double y ) const;

  private:
    QgsProject *mProject = nullptr;
    QPointer<QgsRasterLayer> mDemLayer;
    int mResolution = 64;
    double mTerrainBaseSize = 2000.0;
    QgsRectangle mExtent;
    QVariantList mNormalizedData;
    double mMinRealHeight;
    double mMaxRealHeight;
};

#endif // QGSQUICK3DTERRAINPROVIDER_H
