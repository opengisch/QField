

# File processingalgorithm.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**processing**](dir_14e4815d6f2d5bd986fc95a1af67243d.md) **>** [**processingalgorithm.h**](processingalgorithm_8h.md)

[Go to the documentation of this file](processingalgorithm_8h.md)


```C++
/***************************************************************************
  processingalgorithm.h - ProcessingAlgorithm

 ---------------------
 begin                : 22.06.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef PROCESSINGALGORITHM
#define PROCESSINGALGORITHM

#include "processingalgorithmparametersmodel.h"

#include <QAbstractListModel>
#include <QPointer>
#include <QSortFilterProxyModel>
#include <qgsfeature.h>

class QgsProcessingProvider;
class QgsProcessingAlgorithm;
class QgsVectorLayer;

class ProcessingAlgorithm : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString id READ id WRITE setId NOTIFY idChanged )
    Q_PROPERTY( bool isValid READ isValid NOTIFY idChanged )

    Q_PROPERTY( QString displayName READ displayName NOTIFY idChanged )
    Q_PROPERTY( QString shortHelp READ shortHelp NOTIFY idChanged )

    Q_PROPERTY( QVariantMap parameters READ parameters WRITE setParameters NOTIFY parametersChanged )
    Q_PROPERTY( QgsVectorLayer *inPlaceLayer READ inPlaceLayer WRITE setInPlaceLayer NOTIFY inPlaceLayerChanged )
    Q_PROPERTY( QList<QgsFeature> inPlaceFeatures READ inPlaceFeatures WRITE setInPlaceFeatures NOTIFY inPlaceFeaturesChanged )

    Q_PROPERTY( bool preview READ preview WRITE setPreview NOTIFY previewChanged )
    Q_PROPERTY( QList<QgsGeometry> previewGeometries READ previewGeometries NOTIFY previewGeometriesChanged )

  public:
    explicit ProcessingAlgorithm( QObject *parent = nullptr );

    QString id() const { return mAlgorithmId; }

    void setId( const QString &id );

    bool isValid() const { return mAlgorithm; }

    QString displayName() const;

    QString shortHelp() const;

    QgsVectorLayer *inPlaceLayer() const { return mInPlaceLayer.data(); }

    void setInPlaceLayer( QgsVectorLayer *layer );

    QList<QgsFeature> inPlaceFeatures() const { return mInPlaceFeatures; }

    void setInPlaceFeatures( const QList<QgsFeature> &features );

    QVariantMap parameters() const { return mAlgorithmParameters; }

    void setParameters( const QVariantMap &parameters );

    bool preview() const { return mPreview; }

    void setPreview( bool preview );

    QList<QgsGeometry> previewGeometries() const { return mPreviewGeometries; }

    Q_INVOKABLE bool run( bool previewMode = false );

  signals:
    void idChanged( const QString &id );

    void parametersChanged();

    void inPlaceLayerChanged();

    void inPlaceFeaturesChanged();

    void previewChanged();

    void previewGeometriesChanged();

  private:
    QString mAlgorithmId;
    const QgsProcessingAlgorithm *mAlgorithm = nullptr;
    QVariantMap mAlgorithmParameters;

    QPointer<QgsVectorLayer> mInPlaceLayer;
    QList<QgsFeature> mInPlaceFeatures;

    bool mPreview = false;
    QList<QgsGeometry> mPreviewGeometries;
};

#endif // PROCESSINGALGORITHM
```


