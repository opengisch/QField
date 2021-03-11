/***************************************************************************
                        layerobserver.h
                        ---------------
  begin                : Apr 2020
  copyright            : (C) 2020 by Ivan Ivanov
  email                : ivan@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef LAYEROBSERVER_H
#define LAYEROBSERVER_H


#include "deltafilewrapper.h"

#include <QList>
#include <qgsfeature.h>
#include <qgsmaplayer.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>

typedef QMap<QgsFeatureId, QgsFeature> QgsChangedFeatures;

/**
 * Monitors all layers for changes and writes those changes to a delta file
 */
class LayerObserver : public QObject
{
    Q_OBJECT

    Q_PROPERTY( DeltaFileWrapper *deltaFileWrapper READ deltaFileWrapper NOTIFY deltaFileWrapperChanged )

  public:
    /**
     * Construct a new Layer Observer object
     *
     * @param project
     */
    explicit LayerObserver( const QgsProject *project );


    /**
     * Clears the current delta file changes
     */
    Q_INVOKABLE void reset( bool isHardReset = false ) const;


    /**
     * Gets the current delta file
     *
     * @return current delta file
     */
    DeltaFileWrapper *deltaFileWrapper() const;


  signals:
    void layerEdited( const QString &layerId );
    void deltaFileWrapperChanged();


  private slots:
    /**
     * Monitors the current project for new layers.
     *
     * @param layers layers added
     */
    void onLayersAdded( const QList<QgsMapLayer *> &layers );


    /**
     * Commit the changes of the current delta file and
     *
     */
    void onHomePathChanged();


    /**
     * Extracts the old values of the modified features
     */
    void onBeforeCommitChanges();


    /**
     * Writes the "create" deltas
     *
     * @param layerId layer ID
     * @param addedFeatures new features
     */
    void onCommittedFeaturesAdded( const QString &localLayerId, const QgsFeatureList &addedFeatures );


    /**
     * Writes the "delete" deltas
     *
     * @param layerId layer ID
     * @param deletedFeatureIds old feature IDs
     */
    void onCommittedFeaturesRemoved( const QString &localLayerId, const QgsFeatureIds &deletedFeatureIds );


    /**
     * Writes the "patch" deltas
     *
     * @param layerId
     * @param changedAttributesValues
     */
    void onCommittedAttributeValuesChanges( const QString &localLayerId, const QgsChangedAttributesMap &changedAttributesValues );


    /**
     * Writes the "patch" deltas.
     *
     * @param layerId
     * @param changedGeometries
     */
    void onCommittedGeometriesChanges( const QString &localLayerId, const QgsGeometryMap &changedGeometries );


    /**
     * Writes the deltas to the delta file
     */
    void onEditingStopped();


  private:
    /**
     * The current Deltas File Wrapper object
     */
    std::unique_ptr<DeltaFileWrapper> mDeltaFileWrapper;


    /**
     * The current project instance
     */
    const QgsProject *mProject = nullptr;


    /**
     * The current project delta file name
     */
    QString mDeltaFileName = nullptr;


    /**
     * Store the old version of changed (patch or delete) features per layer.
     * key    - layer ID
     * value  - changed features for that layer
     */
    QMap<QString, QgsChangedFeatures> mChangedFeatures;


    /**
     * Store the old version of patched features per layer.
     * key    - layer ID
     * value  - patched feature IDs for that layer
     */
    QMap<QString, QgsFeatureIds> mPatchedFids;


    /**
     * Layer ids being observed for changes. Should reset when the project is changed.
     */
    QSet<QString> mObservedLayerIds;


    /**
     * Add the needed event listeners to monitor for changes.
     * Assigns listeners only for layer actions of `cloud` and `offline`.
     */
    void addLayerListeners();

};

#endif // LAYEROBSERVER_H
