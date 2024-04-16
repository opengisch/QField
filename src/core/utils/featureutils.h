/***************************************************************************
  featureutils.h - FeatureUtils

 ---------------------
 begin                : 05.03.2020
 copyright            : (C) 2020 by Denis Rouzaud
 email                : denis@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef FEATUREUTILS_H
#define FEATUREUTILS_H

#include "qfield_core_export.h"

#include <QObject>
#include <qgsfeature.h>
#include <qgsgeometry.h>

class QgsVectorLayer;
class QgsQuickMapSettings;

class QFIELD_CORE_EXPORT FeatureUtils : public QObject
{
    Q_OBJECT
  public:
    explicit FeatureUtils( QObject *parent = nullptr );

    static Q_INVOKABLE QgsFeature createFeature( QgsVectorLayer *layer = nullptr, QgsGeometry geometry = QgsGeometry() );

    /**
    * Returns the display name of a given feature.
    * \param layer the vector layer containing the feature
    * \param feature the feature to be named
    */
    static Q_INVOKABLE QString displayName( QgsVectorLayer *layer, const QgsFeature &feature );

    /**
     * Returns the map extent encompassig a given feature
     * \param mapSettings the map settings used to determine the CRS
     * \param layer the vector layer containing the feature
     * \param feature the feature from which the geometry will be used
     * \param righeEdge provide a right edge beyond which the canvas is hidden and extent shouldn't overlap with
     * \param bottomEdge provide a bottom edge beyond which the canvas is hidden and extent shouldn't overlap with
     * \returns a QgsRectangle extent
     */
    static Q_INVOKABLE QgsRectangle extent( QgsQuickMapSettings *mapSettings, QgsVectorLayer *layer, const QgsFeature &feature, const double &rightEdge = 0.0, const double &bottomEdge = 0.0 );
};

#endif // FEATUREUTILS_H
