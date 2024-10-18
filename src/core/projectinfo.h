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
#include "qfieldcloudutils.h"
#include "qgsquickmapcanvasmap.h"
#include "qgsquickmapsettings.h"
#include "trackingmodel.h"

#include <QObject>
#include <QSettings>
#include <QTimer>

class FlatLayerTreeModel;

/**
 * This class stores current projection information such as
 * extent and map theme in order for those to be saved and
 * afterwards restored when the project is re-opened.
 * \ingroup core
 */
class ProjectInfo : public QObject
{
    Q_OBJECT

    /**
     * The file path associated with the currently opened project.
     */
    Q_PROPERTY( QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged )

    /**
     * The map settings object, used to keep track of details such as the map extent.
     */
    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )

    /**
     * The layer tree model object, used to keep track of details such as layer visibility, selected map theme, etc.
     */
    Q_PROPERTY( FlatLayerTreeModel *layerTree READ layerTree WRITE setLayerTree NOTIFY layerTreeChanged )

    /**
     * The state mode (browse vs. digitizing) for the currently opened project.
     */
    Q_PROPERTY( QString stateMode READ stateMode WRITE setStateMode NOTIFY stateModeChanged )

    /**
     * The active layer for the currently opened project.
     */
    Q_PROPERTY( QgsMapLayer *activeLayer READ activeLayer WRITE setActiveLayer NOTIFY activeLayerChanged )

    /**
     * The tracking model object, used to save and restore tracking session for individual vector layers.
     */
    Q_PROPERTY( TrackingModel *trackingModel READ trackingModel WRITE setTrackingModel NOTIFY trackingModelChanged )

    /**
     * The snapping enabled state for the currently opened project.
     */
    Q_PROPERTY( bool snappingEnabled READ snappingEnabled WRITE setSnappingEnabled NOTIFY snappingEnabledChanged )

    /**
     * Set cloud user information for offline usage.
     */
    Q_PROPERTY( CloudUserInformation cloudUserInformation READ cloudUserInformation WRITE setCloudUserInformation NOTIFY cloudUserInformationChanged )

  public:
    explicit ProjectInfo( QObject *parent = nullptr );

    virtual ~ProjectInfo() = default;

    //! \copydoc ProjectInfo::filePath
    void setFilePath( const QString &filePath );

    //! \copydoc ProjectInfo::filePath
    QString filePath() const;

    //! \copydoc ProjectInfo::mapSettings
    void setMapSettings( QgsQuickMapSettings *mapSettings );

    //! \copydoc ProjectInfo::mapSettings
    QgsQuickMapSettings *mapSettings() const;

    //! \copydoc ProjectInfo::layerTree
    void setLayerTree( FlatLayerTreeModel *layerTree );

    //! \copydoc ProjectInfo::layerTree
    FlatLayerTreeModel *layerTree() const;

    //! \copydoc ProjectInfo::trackingModel
    void setTrackingModel( TrackingModel *trackingModel );

    //! \copydoc ProjectInfo::trackingModel
    TrackingModel *trackingModel() const;

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
    Q_INVOKABLE void saveLayerSnappingConfiguration( QgsMapLayer *layer );

    /**
     * Saves the vector \a layer fields that are remembered during feature additions
     */
    Q_INVOKABLE void saveLayerRememberedFields( QgsMapLayer *layer );

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

    /**
     * Returns the saved snapping enabed state for the current project
     */
    bool snappingEnabled() const;

    /**
     * Saves the snapping \a enabled state for the current project
     */
    void setSnappingEnabled( bool enabled );

    /**
     * Returns the saved cloud user infomation for offline usage
     */
    CloudUserInformation cloudUserInformation() const;

    /**
     * Saves the cloud user infomation for offline usage
     */
    void setCloudUserInformation( const CloudUserInformation cloudUserInformation );

    /**
     * Restores last saved cloud user information details attached to the current project
     */
    Q_INVOKABLE void restoreCloudUserInformation();

    //! Save an ongoing vector \a layer tracking session details
    Q_INVOKABLE void saveTracker( QgsVectorLayer *layer );

    //! Restore the last tracking session that occured within a vector \a layer.
    Q_INVOKABLE QModelIndex restoreTracker( QgsVectorLayer *layer );

    Q_INVOKABLE void saveVariable( const QString &name, const QString &value );

    //! Restore various project settings
    static void restoreSettings( QString &projectFilePath, QgsProject *project, QgsQuickMapCanvasMap *mapCanvas, FlatLayerTreeModel *layerTree );

    //! Retrieves configuration of the title decoration
    Q_INVOKABLE QVariantMap getTitleDecorationConfiguration();

    //! Retrieves configuration of the copyright decoration
    Q_INVOKABLE QVariantMap getCopyrightDecorationConfiguration();

    //! Retrieves configuration of the image decoration
    Q_INVOKABLE QVariantMap getImageDecorationConfiguration();

    //! Retrieves configuration of the grid decoration
    Q_INVOKABLE QVariantMap getGridDecorationConfiguration();

    //! Retrieves the default active layer for a given map theme
    Q_INVOKABLE QgsMapLayer *getDefaultActiveLayerForMapTheme( const QString &mapTheme );

  signals:

    void filePathChanged();
    void mapSettingsChanged();
    void layerTreeChanged();
    void stateModeChanged();
    void activeLayerChanged();
    void trackingModelChanged();
    void snappingEnabledChanged();
    void cloudUserInformationChanged();

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
    TrackingModel *mTrackingModel = nullptr;

    bool mIsTemporal = false;
};

#endif // PROJECTINFO_H
