

# File featureslocatorfilter.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**locator**](dir_952e15d753357bc11da55d627a96f3ed.md) **>** [**featureslocatorfilter.h**](featureslocatorfilter_8h.md)

[Go to the documentation of this file](featureslocatorfilter_8h.md)


```C++
/***************************************************************************
  featureslocatorfilter.h

 ---------------------
 begin                : 01.12.2018
 copyright            : (C) 2018 by Denis Rouzaud
 email                : denis@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef FEATURESLOCATORFILTER_H
#define FEATURESLOCATORFILTER_H

#include <QObject>
#include <qgsexpressioncontext.h>
#include <qgslocatorfilter.h>
#include <qgsvectorlayerfeatureiterator.h>


class LocatorModelSuperBridge;

class FeaturesLocatorFilter : public QgsLocatorFilter
{
    Q_OBJECT

  public:
    enum ActionOrigin
    {
      Normal,
      OpenForm,
      Navigation,
    };

    struct PreparedLayer
    {
      public:
        QgsExpression expression;
        QgsExpressionContext context;
        std::unique_ptr<QgsVectorLayerFeatureSource> featureSource;
        QgsFeatureRequest request;
        QString layerName;
        QString layerId;
        QIcon layerIcon;
        Qgis::GeometryType layerGeometryType;
    };

    explicit FeaturesLocatorFilter( LocatorModelSuperBridge *locatorBridge, QObject *parent = nullptr );
    FeaturesLocatorFilter *clone() const override;
    QString name() const override { return QStringLiteral( "allfeatures" ); }
    QString displayName() const override { return tr( "Features in all layers" ); }
    QString description() const override { return tr( "Returns a list of features accross all searchable layers with matching display name." ); }
    Priority priority() const override { return Medium; }
    QString prefix() const override { return QStringLiteral( "af" ); }

    QStringList prepare( const QString &string, const QgsLocatorContext &locatorContext ) override;
    void fetchResults( const QString &string, const QgsLocatorContext &context, QgsFeedback *feedback ) override;
    void triggerResult( const QgsLocatorResult &result ) override;
    void triggerResultFromAction( const QgsLocatorResult &result, const int actionId ) override;

  private:
    int mMaxResultsPerLayer = 12;
    int mMaxTotalResults = 16;
    QList<std::shared_ptr<PreparedLayer>> mPreparedLayers;
    LocatorModelSuperBridge *mLocatorBridge = nullptr;
};

#endif // FEATURESLOCATORFILTER_H
```


