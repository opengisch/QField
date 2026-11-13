/***************************************************************************
  qfidentifytool.h - QfIdentifyTool

 ---------------------
 begin                : 30.8.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QFIDENTIFYTOOL_H
#define QFIDENTIFYTOOL_H

#include "qfmarkupmanager.h"
#include "qgsquickmapsettings.h"

#include <QFutureWatcher>
#include <QObject>
#include <qgsfeature.h>
#include <qgsmapsettings.h>
#include <qgspoint.h>
#include <qgsrasterdataprovider.h>
#include <qgsrendercontext.h>
#include <qgsvectorlayerfeatureiterator.h>
#include <qgsvectortiledataprovider.h>

class QgsMapLayer;
class QgsRasterLayer;
class QgsVectorLayer;
class QgsVectorTileLayer;
class QfMultiFeatureListModel;

/**
 * \ingroup core
 */
class QfIdentifyTool : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )
    Q_PROPERTY( QfMarkupManager *markups READ markupManager WRITE setMarkupManager NOTIFY markupManagerChanged )
    Q_PROPERTY( QfMultiFeatureListModel *model READ model WRITE setModel NOTIFY modelChanged )

    Q_PROPERTY( bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged )
    Q_PROPERTY( bool isIdentifying READ isIdentifying NOTIFY isIdentifyingChanged )
    Q_PROPERTY( double searchRadiusMm READ searchRadiusMm WRITE setSearchRadiusMm NOTIFY searchRadiusMmChanged )

  public:
    struct IdentifyResult
    {
        IdentifyResult( QString &layerId, const QgsFeature &feature, const QString &representationalLayerName = QString() )
          : layerId( layerId )
          , feature( feature )
          , representationalLayerName( representationalLayerName )
        {}

        QString layerId;
        QgsMapLayer *layer = nullptr;
        QgsFeature feature;
        QString representationalLayerName;
    };

  public:
    explicit QfIdentifyTool( QObject *parent = nullptr );

    QgsQuickMapSettings *mapSettings() const;
    void setMapSettings( QgsQuickMapSettings *mapSettings );

    QfMarkupManager *markupManager() const;
    void setMarkupManager( QfMarkupManager *markupManager );

    double searchRadiusMm() const;
    void setSearchRadiusMm( double searchRadiusMm );

    QfMultiFeatureListModel *model() const;
    void setModel( QfMultiFeatureListModel *model );

    bool enabled() const { return mEnabled; }
    void setEnabled( bool enabled );

    bool isIdentifying() const;

    Q_INVOKABLE void identify( const QPointF &point );

    QList<IdentifyResult> identifyVectorLayer( QgsVectorLayer *layer, const QgsPointXY &point ) const;
    QList<IdentifyResult> identifyRasterLayer( QgsRasterLayer *layer, const QgsPointXY &point ) const;
    QList<IdentifyResult> identifyVectorTileLayer( QgsVectorTileLayer *layer, const QgsPointXY &point ) const;

  signals:
    void enabledChanged();
    void isIdentifyingChanged();

    void mapSettingsChanged();
    void markupManagerChanged();
    void modelChanged();
    void searchRadiusMmChanged();

    void identifyFinished();

  private slots:
    void onIdentifyFinished();

  private:
    struct PreparedLayer
    {
      public:
        bool isValid = false;

        Qgis::LayerType layerType;
        QString layerId;
        QString layerName;
        QgsRectangle extent;

        std::unique_ptr<QgsVectorLayerFeatureSource> vectorFeatureSource;
        std::unique_ptr<QgsFeatureRenderer> vectorRenderer;
        QgsFields fields;
        QgsFeatureRequest request;

        std::unique_ptr<QgsRasterDataProvider> rasterDataProvider;
        int rasterWidth = 1;
        int rasterHeight = 1;
        QgsPointXY rasterPoint;

        std::unique_ptr<QgsVectorTileDataProvider> vectorTileDataProvider;
        double vectorTileScale = 0.0;

        QgsRenderContext renderContext;
    };

    PreparedLayer *prepareVectorLayer( QgsVectorLayer *layer, const QgsPointXY &point ) const;
    QList<IdentifyResult> processVectorLayer( PreparedLayer *preparedLayer ) const;

    PreparedLayer *prepareRasterLayer( QgsRasterLayer *layer, const QgsPointXY &point ) const;
    QList<IdentifyResult> processRasterLayer( PreparedLayer *preparedLayer ) const;

    PreparedLayer *prepareVectorTileLayer( QgsVectorTileLayer *layer, const QgsPointXY &point ) const;
    QList<IdentifyResult> processVectorTileLayer( PreparedLayer *preparedLayer ) const;

    double searchRadiusMU( const QgsRenderContext &context ) const;
    double searchRadiusMU() const;

    QgsRectangle toLayerCoordinates( QgsMapLayer *layer, QgsRectangle rect ) const;
    QgsPointXY toLayerCoordinates( QgsMapLayer *layer, QgsPointXY point ) const;

    QgsQuickMapSettings *mMapSettings = nullptr;
    QfMarkupManager *mMarkupManager = nullptr;
    QfMultiFeatureListModel *mModel = nullptr;

    bool mEnabled = true;
    double mSearchRadiusMm;

    QList<std::shared_ptr<PreparedLayer>> mPreparedLayers;
    QMap<QString, QPointer<QgsMapLayer>> mIdentifiedLayers;

    QFutureWatcher<QList<IdentifyResult>> mIdentifyWatcher;

    friend class IdentifyTool;
};

#endif // QFIDENTIFYTOOL_H
