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

#include <QObject>
#include <qgsfeature.h>
#include <qgsmapsettings.h>
#include <qgspoint.h>
#include <qgsrendercontext.h>

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
    Q_PROPERTY( double searchRadiusMm READ searchRadiusMm WRITE setSearchRadiusMm NOTIFY searchRadiusMmChanged )

  public:
    struct IdentifyResult
    {
        IdentifyResult( QgsMapLayer *layer, const QgsFeature &feature, const QString &representationalLayerName = QString() )
          : layer( layer )
          , feature( feature )
          , representationalLayerName( representationalLayerName )
        {}

        QgsMapLayer *layer;
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

    Q_INVOKABLE void identify( const QPointF &point ) const;

    QList<IdentifyResult> identifyVectorLayer( QgsVectorLayer *layer, const QgsPointXY &point ) const;
    QList<IdentifyResult> identifyRasterLayer( QgsRasterLayer *layer, const QgsPointXY &point ) const;
    QList<IdentifyResult> identifyVectorTileLayer( QgsVectorTileLayer *layer, const QgsPointXY &point ) const;

  signals:
    void mapSettingsChanged();
    void markupManagerChanged();
    void modelChanged();
    void enabledChanged();
    void searchRadiusMmChanged();

    void identifyFinished() const;

  private:
    double searchRadiusMU( const QgsRenderContext &context ) const;
    double searchRadiusMU() const;

    QgsRectangle toLayerCoordinates( QgsMapLayer *layer, QgsRectangle rect ) const;
    QgsPointXY toLayerCoordinates( QgsMapLayer *layer, QgsPointXY point ) const;

    QgsQuickMapSettings *mMapSettings = nullptr;
    QfMarkupManager *mMarkupManager = nullptr;
    QfMultiFeatureListModel *mModel = nullptr;

    bool mEnabled = true;
    double mSearchRadiusMm;

    friend class IdentifyTool;
};

#endif // QFIDENTIFYTOOL_H
