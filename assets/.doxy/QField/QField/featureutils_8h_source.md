

# File featureutils.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**featureutils.h**](featureutils_8h.md)

[Go to the documentation of this file](featureutils_8h.md)


```C++
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

    static Q_INVOKABLE QgsFeature createBlankFeature( const QgsFields &fields = QgsFields(), const QgsGeometry &geometry = QgsGeometry() );

    static Q_INVOKABLE QgsFeature createFeature( QgsVectorLayer *layer, const QgsGeometry &geometry = QgsGeometry(), const GnssPositionInformation &positionInformation = GnssPositionInformation(), const CloudUserInformation &cloudUserInformation = CloudUserInformation() );

    static Q_INVOKABLE QString displayName( QgsVectorLayer *layer, const QgsFeature &feature );

    static Q_INVOKABLE QgsRectangle extent( QgsQuickMapSettings *mapSettings, QgsVectorLayer *layer, const QgsFeature &feature );

    static Q_INVOKABLE QList<QgsFeature> featuresFromJsonString( const QString &string );

    static Q_INVOKABLE QgsField createField( const QString &name, FieldType type, const QString &typeName = QString(), int length = 0, int precision = 0, const QString &comment = QString() );

    static Q_INVOKABLE QgsFields createFields( const QList<QgsField> &fields );

    static Q_INVOKABLE bool attributeIsNull( const QVariant &value );
};

#endif // FEATUREUTILS_H
```


