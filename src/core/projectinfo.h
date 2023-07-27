/***************************************************************************
  projectinfo.h - ProjectInfo

 ---------------------
 begin                : 14.2.2021
 copyright            : (C) 2021 by Mathieu Pellerin
 email                : nirvn dot asia at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef PROJECTINFO_H
#define PROJECTINFO_H

#include "layertreemodel.h"
#include "qgsquickmapsettings.h"

#include <QObject>
#include <QSettings>
#include <QTimer>

/**
 * This class stores current projection information such as
 * extent and map theme in order for those to be saved and
 * afterwards restored when the project is re-opened
 */
class ProjectInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged )
    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )
    Q_PROPERTY( FlatLayerTreeModel *layerTree READ layerTree WRITE setLayerTree NOTIFY layerTreeChanged )
    Q_PROPERTY( QString stateMode READ stateMode WRITE setStateMode NOTIFY stateModeChanged )
    Q_PROPERTY( QgsMapLayer *activeLayer READ activeLayer WRITE setActiveLayer NOTIFY activeLayerChanged )

  public:
    explicit ProjectInfo( QObject *parent = nullptr );

    virtual ~ProjectInfo() = default;

    void setFilePath( const QString &filePath );

    QString filePath() const;

    void setMapSettings( QgsQuickMapSettings *mapSettings );

    QgsQuickMapSettings *mapSettings() const;

    void setLayerTree( FlatLayerTreeModel *layerTree );

    FlatLayerTreeModel *layerTree() const;

    /**
     * Saves the \a layer style to the current project information settings
     */
    Q_INVOKABLE void saveLayerStyle( QgsMapLayer *layer );

    /**
     * Saves the current state (visibility and collapse status) of the layer tree
     */
    Q_INVOKABLE void saveLayerTreeState();

    /**
     * Saves the current snapping configuration settings
     */
    Q_INVOKABLE void saveSnappingConfiguration();

    /**
     * Saves the state \a mode for the current project
     */
    void setStateMode( const QString &mode );

    /**
     * Returns the saved state mode for the current project
     */
    QString stateMode() const;

    /**
     * Saves the active \a layer within the current project
     */
    void setActiveLayer( QgsMapLayer *layer );

    /**
     * Returns the saved active layer for the current project
     * or nullptr if active layer was not saved or isn't present
     */
    QgsMapLayer *activeLayer() const;

  signals:

    void filePathChanged();
    void mapSettingsChanged();
    void layerTreeChanged();
    void stateModeChanged();
    void activeLayerChanged();

  private slots:

    void extentChanged();
    void rotationChanged();
    void temporalStateChanged();
    void mapThemeChanged();

  private:
    void saveExtent();
    void saveRotation();
    void saveTemporalState();

    QSettings mSettings;
    QString mFilePath;

    QgsQuickMapSettings *mMapSettings = nullptr;
    QTimer mSaveExtentTimer;
    QTimer mSaveRotationTimer;
    QTimer mSaveTemporalStateTimer;

    FlatLayerTreeModel *mLayerTree = nullptr;

    bool mIsTemporal = false;
};

#endif // PROJECTINFO_H
