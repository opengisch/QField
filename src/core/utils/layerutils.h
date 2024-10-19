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

class QgsVectorLayer;
class QgsRasterLayer;
class QgsSymbol;

/**
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
    static QgsSymbol *defaultSymbol( QgsVectorLayer *layer );

    static QgsAbstractVectorLayerLabeling *defaultLabeling( QgsVectorLayer *layer, QgsTextFormat textFormat = QgsTextFormat() );

    static QgsRasterLayer *createOnlineElevationLayer();

    /**
    * Returns TRUE if the vector layer is used as an atlas coverage layer in
    * any of the print layouts of the currently opened project.
    * \param layer the vector layer to check against print layouts
    */
    static Q_INVOKABLE bool isAtlasCoverageLayer( QgsVectorLayer *layer );

    /**
     * Selects features in a layer
     * This method is required since QML cannot perform the conversion of a feature ID to a QgsFeatureId
     * \param layer the vector layer
     * \param fids the list of feature IDs
     * \param behavior the selection behavior
     */
    static Q_INVOKABLE void selectFeaturesInLayer( QgsVectorLayer *layer, const QList<int> &fids, Qgis::SelectBehavior behavior = Qgis::SelectBehavior::SetSelection );

    static bool deleteFeature( QgsProject *project, QgsVectorLayer *layer, const QgsFeatureId fid, bool shouldWriteChanges = true );

    /**
     * Duplicates a given \a feature within the provided vector \a layer. If successful, the function will
     * return the duplicated feature with attribute values saved updated to match what was saved
     * into the layer dataset.
     */
    static QgsFeature duplicateFeature( QgsVectorLayer *layer, const QgsFeature &feature );

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
     * Returns a feature request to get features.
     */
    Q_INVOKABLE static FeatureIterator createFeatureIteratorFromExpression( QgsVectorLayer *layer, const QString &expression );
};

#endif // LAYERUTILS_H
