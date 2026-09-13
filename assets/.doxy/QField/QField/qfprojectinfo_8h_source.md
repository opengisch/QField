

# File qfprojectinfo.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfprojectinfo.h**](qfprojectinfo_8h.md)

[Go to the documentation of this file](qfprojectinfo_8h.md)


```C++
/***************************************************************************
  qfprojectinfo.h - QfProjectInfo

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


#ifndef QFPROJECTINFO_H
#define QFPROJECTINFO_H

#include "qfcloudutils.h"
#include "qflayertreemodel.h"
#include "qftrackingmodel.h"
#include "qgsquickmapcanvasmap.h"
#include "qgsquickmapsettings.h"

#include <QObject>
#include <QSettings>
#include <QTimer>

class QfFlatLayerTreeModel;

class QfProjectInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString filePath READ filePath WRITE setFilePath NOTIFY filePathChanged )

    
    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )

    Q_PROPERTY( QfFlatLayerTreeModel *layerTree READ layerTree WRITE setLayerTree NOTIFY layerTreeChanged )

    Q_PROPERTY( QString stateMode READ stateMode WRITE setStateMode NOTIFY stateModeChanged )

    Q_PROPERTY( QgsMapLayer *activeLayer READ activeLayer WRITE setActiveLayer NOTIFY activeLayerChanged )

    Q_PROPERTY( QfTrackingModel *trackingModel READ trackingModel WRITE setTrackingModel NOTIFY trackingModelChanged )

    Q_PROPERTY( bool snappingEnabled READ snappingEnabled WRITE setSnappingEnabled NOTIFY snappingEnabledChanged )

    Q_PROPERTY( QfCloudUserInformation cloudUserInformation READ cloudUserInformation WRITE setCloudUserInformation NOTIFY cloudUserInformationChanged )

  public:
    explicit QfProjectInfo( QObject *parent = nullptr );

    virtual ~QfProjectInfo() = default;

    void setFilePath( const QString &filePath );

    QString filePath() const;

    void setMapSettings( QgsQuickMapSettings *mapSettings );

    QgsQuickMapSettings *mapSettings() const;

    void setLayerTree( QfFlatLayerTreeModel *layerTree );

    QfFlatLayerTreeModel *layerTree() const;

    void setTrackingModel( QfTrackingModel *trackingModel );

    QfTrackingModel *trackingModel() const;

    Q_INVOKABLE void saveLayerStyle( QgsMapLayer *layer );

    Q_INVOKABLE void saveLayerTreeState();

    Q_INVOKABLE void saveLayerSnappingConfiguration( QgsMapLayer *layer );

    Q_INVOKABLE void saveLayerRememberedFields( QgsMapLayer *layer );

    void setStateMode( const QString &mode );

    QString stateMode() const;

    void setActiveLayer( QgsMapLayer *layer );

    QgsMapLayer *activeLayer() const;

    bool snappingEnabled() const;

    void setSnappingEnabled( bool enabled );

    QfCloudUserInformation cloudUserInformation() const;

    void setCloudUserInformation( const QfCloudUserInformation &cloudUserInformation );

    Q_INVOKABLE void restoreCloudUserInformation();

    Q_INVOKABLE void saveTracker( QgsVectorLayer *layer );

    Q_INVOKABLE QModelIndex restoreTracker( QgsVectorLayer *layer );

    Q_INVOKABLE void saveVariable( const QString &name, const QString &value );

    static void restoreSettings( QString &projectFilePath, QgsProject *project, QgsQuickMapCanvasMap *mapCanvas, QfFlatLayerTreeModel *layerTree );

    Q_INVOKABLE QVariantMap getTitleDecorationConfiguration();

    Q_INVOKABLE QVariantMap getCopyrightDecorationConfiguration();

    Q_INVOKABLE QVariantMap getImageDecorationConfiguration();

    Q_INVOKABLE QVariantMap getGridDecorationConfiguration();

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

    QfFlatLayerTreeModel *mLayerTree = nullptr;
    QfTrackingModel *mTrackingModel = nullptr;

    bool mIsTemporal = false;
};

#endif // QFPROJECTINFO_H
```


