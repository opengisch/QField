/***************************************************************************
  identifytool.h - IdentifyTool

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
#ifndef IDENTIFYTOOL_H
#define IDENTIFYTOOL_H

#include <QObject>

#include <qgsfeature.h>
#include <qgspoint.h>
#include <qgsmapsettings.h>
#include <qgsrendercontext.h>

class QgsMapLayer;
class QgsQuickMapSettings;
class QgsVectorLayer;
class MultiFeatureListModel;

class IdentifyTool : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )
    Q_PROPERTY( double searchRadiusMm READ searchRadiusMm WRITE setSearchRadiusMm NOTIFY searchRadiusMmChanged )
    Q_PROPERTY( MultiFeatureListModel *model READ model WRITE setModel NOTIFY modelChanged )
    Q_PROPERTY( bool deactivated READ deactivated WRITE setDeactivated NOTIFY deactivatedChanged )

  public:
    struct IdentifyResult
    {
      IdentifyResult( QgsMapLayer *layer, const QgsFeature &feature )
        : layer( layer )
        , feature( feature )
      {}

      QgsMapLayer *layer;
      QgsFeature feature;
    };

  public:
    explicit IdentifyTool( QObject *parent = nullptr );

    QgsQuickMapSettings *mapSettings() const;
    void setMapSettings( QgsQuickMapSettings *mapSettings );

    double searchRadiusMm() const;
    void setSearchRadiusMm( double searchRadiusMm );

    MultiFeatureListModel *model() const;
    void setModel( MultiFeatureListModel *model );

    bool deactivated() const { return mDeactivated; }
    void setDeactivated( bool deactivated );

  signals:
    void mapSettingsChanged();
    void searchRadiusMmChanged();
    void modelChanged();
    void deactivatedChanged();

  public slots:
    void identify( const QPointF &point ) const;

    QList<IdentifyResult> identifyVectorLayer( QgsVectorLayer *layer, const QgsPointXY &point ) const;

  private:
    QgsQuickMapSettings *mMapSettings = nullptr;
    MultiFeatureListModel *mModel = nullptr;

    double searchRadiusMU( const QgsRenderContext &context ) const;
    double searchRadiusMU() const;

    QgsRectangle toLayerCoordinates( QgsMapLayer *layer, const QgsRectangle &rect ) const;

    double mSearchRadiusMm;

    bool mDeactivated = false;
};

#endif // IDENTIFYTOOL_H
