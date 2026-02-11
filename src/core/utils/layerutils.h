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

/**
 * A class providing a feature iterator interface to be used within QML/javascript environment.
 *
 * Users of this class must manually call its close() once feature iteration is finished.
 *
 * \ingroup core
 */
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

/**
 * \ingroup core
 */
class LayerUtils : public QObject
{
    Q_OBJECT

  public:
    explicit LayerUtils( QObject *parent = nullptr );

    /**
    * Returns the default symbol for a given layer.
    * \param layer the vector layer used to create the default symbol
    */
    static QgsSymbol *defaultSymbol( QgsVectorLayer *layer, const QString &attachmentField = QString(), const QString &colorField = QString() );

    /**
     * Sets the default symbology render for a given \a layer.
     */
    static void setDefaultRenderer( QgsVectorLayer *layer, QgsProject *project = nullptr, const QString &attachmentField = QString(), const QString &colorField = QString() );

    /**
     * Returns the default vector layer labeling for a given \a layer and \a textFormat.
     */
    static QgsAbstractVectorLayerLabeling *defaultLabeling( QgsVectorLayer *layer, QgsTextFormat textFormat = QgsTextFormat() );

    /**
     * Sets the default labeling for a given \a layer.
     */
    static void setDefaultLabeling( QgsVectorLayer *layer, QgsProject *project = nullptr );

    /**
     * Creats an online raster elevation layer.
     */
    static QgsRasterLayer *createOnlineElevationLayer();

    /**
     * Creates an online basemap layer.
     */
    static QgsMapLayer *createBasemap( const QString &style = QString() );

    /**
    * Returns TRUE if the vector layer is used as an atlas coverage layer in
    * any of the print layouts of the currently opened project.
    * \param layer the vector layer to check against print layouts
    */
    static Q_INVOKABLE bool isAtlasCoverageLayer( QgsVectorLayer *layer );

    /**
     * Returns TRUE if the \a layer permission state prevents feature addition.
     */
    static Q_INVOKABLE bool isFeatureAdditionLocked( QgsMapLayer *layer );

    /**
     * Selects features in a layer
     * This method is required since QML cannot perform the conversion of a feature ID to a QgsFeatureId
     * \param layer the vector layer
     * \param fids the list of feature IDs
     * \param behavior the selection behavior
     */
    static Q_INVOKABLE void selectFeaturesInLayer( QgsVectorLayer *layer, const QList<int> &fids, Qgis::SelectBehavior behavior = Qgis::SelectBehavior::SetSelection );

    /**
     * Deletes a vector layer feature, including related features tied to relationships.
     * \param project the project holding information on relationships
     * \param layer the layer from which the feature will be deleted
     * \param fid the feature ID to be deleted
     * \param flushBuffer set to TRUE to immediately save the edit buffer
     */
    static Q_INVOKABLE bool deleteFeature( QgsProject *project, QgsVectorLayer *layer, const QgsFeatureId fid, bool flushBuffer = true );

    /**
     * Duplicates a given \a feature within the provided vector \a layer. If successful, the function will
     * return the duplicated feature with attribute values saved updated to match what was saved
     * into the layer dataset.
     */
    static Q_INVOKABLE QgsFeature duplicateFeature( QgsVectorLayer *layer, QgsFeature feature );

    /**
     * Adds a \a feature into the \a layer.
     * \note The function will not call startEditing() and commitChanges()
     */
    Q_INVOKABLE static bool addFeature( QgsVectorLayer *layer, QgsFeature feature );

    /**
     * Returns the QVariant typeName of a \a field.
     * This is a stable identifier (compared to the provider field name).
     */
    Q_INVOKABLE static QString fieldType( const QgsField &field );

    /**
     * Returns TRUE if the vector \a layer geometry has an M value.
     */
    Q_INVOKABLE static bool hasMValue( QgsVectorLayer *layer );

    /**
     * Loads a vector layer.
     * \param uri the data source uri
     * \param name the layer name
     * \param provider the data provider name
     */
    Q_INVOKABLE static QgsVectorLayer *loadVectorLayer( const QString &uri, const QString &name = QString(), const QString &provider = QStringLiteral( "ogr" ) );

    /**
     * Loads a raster layer.
     * \param uri the data source uri
     * \param name the layer name
     * \param provider the data provider name
     */
    Q_INVOKABLE static QgsRasterLayer *loadRasterLayer( const QString &uri, const QString &name = QString(), const QString &provider = QStringLiteral( "gdal" ) );

    /**
     * Attempts to parse a GeoJSON string to a memory vector layer containing the collection of
     * features. The geometry type will be taken from the first parsed feature.
     * \param name layer name
     * \param string the GeoJSON string
     * \param crs optional layer CRS for layers with geometry
     */
    Q_INVOKABLE static QgsVectorLayer *memoryLayerFromJsonString( const QString &name, const QString &string, const QgsCoordinateReferenceSystem &crs = QgsCoordinateReferenceSystem() );

    /**
     * Creates a new memory layer using the specified parameters.
     * \param name layer name
     * \param fields fields for layer
     * \param geometryType optional layer geometry type
     * \param crs optional layer CRS for layers with geometry
     */
    Q_INVOKABLE static QgsVectorLayer *createMemoryLayer( const QString &name,
                                                          const QgsFields &fields = QgsFields(),
                                                          Qgis::WkbType geometryType = Qgis::WkbType::NoGeometry,
                                                          const QgsCoordinateReferenceSystem &crs = QgsCoordinateReferenceSystem() );

    /**
     * Returns a feature iterator to get features matching a given \a expression within the provided \a layer.
     */
    Q_INVOKABLE static FeatureIterator createFeatureIteratorFromExpression( QgsVectorLayer *layer, const QString &expression );

    /**
     * Returns a feature iterator to get features overlapping a given \a rectangle within the provided \a layer.
     */
    Q_INVOKABLE static FeatureIterator createFeatureIteratorFromRectangle( QgsVectorLayer *layer, const QgsRectangle &rectangle );
};

#endif // LAYERUTILS_H
