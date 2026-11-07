

# File qflayerobserver.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldcloud**](dir_d27ac98b23548f5810403c734ce9817b.md) **>** [**qflayerobserver.h**](qflayerobserver_8h.md)

[Go to the documentation of this file](qflayerobserver_8h.md)


```C++
/***************************************************************************
                        qflayerobserver.h
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

#ifndef QFLAYEROBSERVER_H
#define QFLAYEROBSERVER_H


#include "qfdeltafilewrapper.h"

#include <QList>
#include <QPointer>
#include <qgsfeature.h>
#include <qgsmaplayer.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>


typedef QMap<QgsFeatureId, QgsFeature> QgsChangedFeatures;

class QfLayerObserver : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QfDeltaFileWrapper *deltaFileWrapper READ deltaFileWrapper WRITE setDeltaFileWrapper NOTIFY deltaFileWrapperChanged )

  public:
    explicit QfLayerObserver( const QgsProject *project );


    QfDeltaFileWrapper *deltaFileWrapper() const;

    void setDeltaFileWrapper( QfDeltaFileWrapper *wrapper );

    void addLayerListeners();


  signals:
    void layerEdited( const QString &layerId );
    void deltaFileWrapperChanged();


  private slots:
    void onReadProject();


    void onBeforeCommitChanges();


    void onCommittedFeaturesAdded( const QString &localLayerId, const QgsFeatureList &addedFeatures );


    void onCommittedFeaturesRemoved( const QString &localLayerId, const QgsFeatureIds &deletedFeatureIds );


    void onCommittedAttributeValuesChanges( const QString &localLayerId, const QgsChangedAttributesMap &changedAttributesValues );


    void onCommittedGeometriesChanges( const QString &localLayerId, const QgsGeometryMap &changedGeometries );


    void onEditingStopped();


  private:
    QPointer<QfDeltaFileWrapper> mDeltaFileWrapper;


    const QgsProject *mProject = nullptr;


    QString mDeltaFileName = nullptr;


    QMap<QString, QgsChangedFeatures> mChangedFeatures;


    QMap<QString, QgsFeatureIds> mPatchedFids;


    QSet<QString> mObservedLayerIds;


    bool mLocalAndSourcePkAttrAreEqual = false;
};

#endif // QFLAYEROBSERVER_H
```


