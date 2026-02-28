

# File layerutils.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**layerutils.h**](layerutils_8h.md)

[Go to the documentation of this file](layerutils_8h.md)


```C++
/***************************************************************************
  layerutils.h - LayerUtils

 ---------------------
 begin                : 01.03.2021
 copyright            : (C) 2020 by Mathieu Pellerin
 email                : mathieu@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef LAYERUTILS_H
#define LAYERUTILS_H

#include <QObject>
#include <qgis.h>
#include <qgstextformat.h>
#include <qgsvectorlayer.h>

class FeatureModel;
class QgsVectorLayer;
class QgsRasterLayer;
class QgsSymbol;

#define OPENSTREETMAP_URL QStringLiteral( "type=xyz&tilePixelRatio=1&url=https://tile.openstreetmap.org/%7Bz%7D/%7Bx%7D/%7By%7D.png&zmax=19&zmin=0&crs=EPSG3857" )

class FeatureIterator
{
    Q_GADGET

  public:
    FeatureIterator( QgsVectorLayer *layer = nullptr, const QgsFeatureRequest &request = QgsFeatureRequest() )
    {
      if ( layer )
      {
        mFeatureIterator = layer->getFeatures( request );
      }
    }

    Q_INVOKABLE bool hasNext()
    {
      if ( !mHasNextChecked )
      {
        mHasNext = mFeatureIterator.nextFeature( mCurrentFeature );
        mHasNextChecked = true;
      }
      return mHasNext;
    }

    Q_INVOKABLE QgsFeature next()
    {
      if ( !mHasNextChecked )
      {
        mFeatureIterator.nextFeature( mCurrentFeature );
      }
      else
      {
        mHasNextChecked = false;
      }
      return mCurrentFeature;
    }

    Q_INVOKABLE void close()
    {
      mFeatureIterator.close();
    }

  private:
    QgsFeatureIterator mFeatureIterator;
    QgsFeature mCurrentFeature;

    bool mHasNext = false;
    bool mHasNextChecked = false;
};

class LayerUtils : public QObject
{
    Q_OBJECT

  public:
    explicit LayerUtils( QObject *parent = nullptr );

    static QgsSymbol *defaultSymbol( QgsVectorLayer *layer, const QString &attachmentField = QString(), const QString &colorField = QString() );

    static void setDefaultRenderer( QgsVectorLayer *layer, QgsProject *project = nullptr, const QString &attachmentField = QString(), const QString &colorField = QString() );

    static QgsAbstractVectorLayerLabeling *defaultLabeling( QgsVectorLayer *layer, QgsTextFormat textFormat = QgsTextFormat() );

    static void setDefaultLabeling( QgsVectorLayer *layer, QgsProject *project = nullptr );

    static QgsRasterLayer *createOnlineElevationLayer();

    static QgsMapLayer *createBasemap( const QString &style = QString() );

    static Q_INVOKABLE bool isAtlasCoverageLayer( QgsVectorLayer *layer );

    static Q_INVOKABLE bool isFeatureAdditionLocked( QgsMapLayer *layer );

    static Q_INVOKABLE void selectFeaturesInLayer( QgsVectorLayer *layer, const QList<int> &fids, Qgis::SelectBehavior behavior = Qgis::SelectBehavior::SetSelection );

    static Q_INVOKABLE bool deleteFeature( QgsProject *project, QgsVectorLayer *layer, const QgsFeatureId fid, bool flushBuffer = true );

    static Q_INVOKABLE QgsFeature duplicateFeature( QgsVectorLayer *layer, QgsFeature feature );

    Q_INVOKABLE static bool addFeature( QgsVectorLayer *layer, QgsFeature feature );

    Q_INVOKABLE static QString fieldType( const QgsField &field );

    Q_INVOKABLE static bool hasMValue( QgsVectorLayer *layer );

    Q_INVOKABLE static QgsVectorLayer *loadVectorLayer( const QString &uri, const QString &name = QString(), const QString &provider = QStringLiteral( "ogr" ) );

    Q_INVOKABLE static QgsRasterLayer *loadRasterLayer( const QString &uri, const QString &name = QString(), const QString &provider = QStringLiteral( "gdal" ) );

    Q_INVOKABLE static QgsVectorLayer *memoryLayerFromJsonString( const QString &name, const QString &string, const QgsCoordinateReferenceSystem &crs = QgsCoordinateReferenceSystem() );

    Q_INVOKABLE static QgsVectorLayer *createMemoryLayer( const QString &name,
                                                          const QgsFields &fields = QgsFields(),
                                                          Qgis::WkbType geometryType = Qgis::WkbType::NoGeometry,
                                                          const QgsCoordinateReferenceSystem &crs = QgsCoordinateReferenceSystem() );

    Q_INVOKABLE static FeatureIterator createFeatureIteratorFromExpression( QgsVectorLayer *layer, const QString &expression );

    Q_INVOKABLE static FeatureIterator createFeatureIteratorFromRectangle( QgsVectorLayer *layer, const QgsRectangle &rectangle );
};

#endif // LAYERUTILS_H
```


