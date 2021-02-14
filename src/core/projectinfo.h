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

#include "qgsquickmapsettings.h"
#include "layertreemodel.h"

#include <QObject>

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

  public:
  
    explicit ProjectInfo( QObject *parent = nullptr );

    virtual ~ProjectInfo() = default;

    void setFilePath( const QString &filePath );
    
    QString filePath() const;

    void setMapSettings( QgsQuickMapSettings *mapSettings );
    
    QgsQuickMapSettings *mapSettings() const;

    void setLayerTree( FlatLayerTreeModel *layerTree );

    FlatLayerTreeModel *layerTree() const;

  signals:
  
    void filePathChanged();
    
    void mapSettingsChanged();

    void layerTreeChanged();

  private slots:

    void extentChanged();

    void mapThemeChanged();

  private:
  
    QString mFilePath;

    QgsQuickMapSettings *mMapSettings = nullptr;
    QTimer mSaveExtentTimer;

    FlatLayerTreeModel *mLayerTree = nullptr;
};

#endif // PROJECTINFO_H
