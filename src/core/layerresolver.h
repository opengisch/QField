/***************************************************************************
                            layerresolver.h

                              -------------------
              begin                : January 2021
              copyright            : (C) 2011 by Matthias Kuhn
              email                : matthias@opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef LAYERRESOLVER_H
#define LAYERRESOLVER_H

#include <QObject>

class QgsVectorLayer;
class QgsProject;

/**
 * Helper class to resolve layers by id, name, source and provider.
 */
class LayerResolver : public QObject
{
    Q_OBJECT

    /**
     * The layer id to resolve
     */
    Q_PROPERTY( QString layerId READ layerId WRITE setLayerId NOTIFY layerIdChanged )

    /**
     * The layer name to resolve
     */
    Q_PROPERTY( QString layerName READ layerName WRITE setLayerName NOTIFY layerNameChanged )

    /**
     * The layer source to resolve
     */
    Q_PROPERTY( QString layerSource READ layerSource WRITE setLayerSource NOTIFY layerSourceChanged )

    /**
     * The layer provider name to resolve
     */
    Q_PROPERTY( QString layerProviderName READ layerProviderName WRITE setLayerProviderName NOTIFY layerProviderNameChanged )

    /**
     * The qgis project from which layers will be acquired
     */
    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )

    /**
     * Contains the layer after a successful resolving process
     */
    Q_PROPERTY( QgsVectorLayer *currentLayer READ currentLayer NOTIFY currentLayerChanged )

  public:
    explicit LayerResolver( QObject *parent = nullptr );

    void resolve();

    QString layerId() const;
    void setLayerId( const QString &layerId );

    QString layerName() const;
    void setLayerName( const QString &layerName );

    QString layerSource() const;
    void setLayerSource( const QString &layerSource );

    QString layerProviderName() const;
    void setLayerProviderName( const QString &layerProviderName );

    QgsVectorLayer *currentLayer() const;

    QgsProject *project() const;
    void setProject( QgsProject *project );

  signals:
    void layerIdChanged();
    void layerNameChanged();
    void layerSourceChanged();
    void layerProviderNameChanged();
    void currentLayerChanged();
    void projectChanged();

  private:
    void setLayer( QgsVectorLayer *layer );

    QString mLayerId;
    QString mLayerName;
    QString mLayerSource;
    QString mLayerProviderName;
    QgsProject *mProject = nullptr;
    QgsVectorLayer *mLayer = nullptr;
};

#endif // LAYERRESOLVER_H
