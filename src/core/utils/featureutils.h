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

#include "gnsspositioninformation.h"
#include "qfield_core_export.h"
#include "qfieldcloudutils.h"

#include <QObject>
#include <qgsfeature.h>
#include <qgsgeometry.h>

class QgsVectorLayer;
class QgsQuickMapSettings;

/**
 * \ingroup core
 */
class QFIELD_CORE_EXPORT FeatureUtils : public QObject
{
    Q_OBJECT
  public:
    enum FieldType
    {
      Bool = QMetaType::Type::Bool,
      Int = QMetaType::Type::Int,
      String = QMetaType::Type::QString,
      Double = QMetaType::Type::Double,
      Date = QMetaType::Type::QDate,
      DateTime = QMetaType::Type::QDateTime,
      Time = QMetaType::Type::QTime,
    };
    Q_ENUM( FieldType )

    explicit FeatureUtils( QObject *parent = nullptr );

    /**
     * Returns a new feature with its \a fields completely blank.
     */
    static Q_INVOKABLE QgsFeature createBlankFeature( const QgsFields &fields = QgsFields(), const QgsGeometry &geometry = QgsGeometry() );

    /**
     * Returns a new feature with its fields set to default values.
     */
    static Q_INVOKABLE QgsFeature createFeature( QgsVectorLayer *layer, const QgsGeometry &geometry = QgsGeometry(), const GnssPositionInformation &positionInformation = GnssPositionInformation(), const CloudUserInformation &cloudUserInformation = CloudUserInformation() );

    /**
    * Returns the display name of a given feature.
    * \param layer the vector layer containing the feature
    * \param feature the feature to be named
    */
    static Q_INVOKABLE QString displayName( QgsVectorLayer *layer, const QgsFeature &feature );

    /**
     * Returns the map extent encompassig a given feature.
     * \param mapSettings the map settings used to determine the CRS
     * \param layer the vector layer containing the feature
     * \param feature the feature from which the geometry will be used
     * \returns a QgsRectangle extent
     */
    static Q_INVOKABLE QgsRectangle extent( QgsQuickMapSettings *mapSettings, QgsVectorLayer *layer, const QgsFeature &feature );

    /**
     * Returns a list of features while attempting to parse a GeoJSON \a string. If the string could not
     * be parsed, an enmpty list will be returned.
     */
    static Q_INVOKABLE QList<QgsFeature> featuresFromJsonString( const QString &string );

    /**
     * Returns a new a field gadget.
     */
    static Q_INVOKABLE QgsField createField( const QString &name, FieldType type, const QString &typeName = QString(), int length = 0, int precision = 0, const QString &comment = QString() );

    /**
     * Returns a new a fields gadget.
     */
    static Q_INVOKABLE QgsFields createFields( const QList<QgsField> &fields );

    /**
     * Returns TRUE if an attribute value is null.
     */
    static Q_INVOKABLE bool attributeIsNull( const QVariant &value );
};

#endif // FEATUREUTILS_H
