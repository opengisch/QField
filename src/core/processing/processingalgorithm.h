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

/**
 * \brief A processing algorithm item capable of runnning a given algorithm.
 * \ingroup core
 */
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

    /**
     * Returns the current algorithm ID from which parameters are taken from.
     */
    QString id() const { return mAlgorithmId; }

    /**
     * Sets the current algorithm \a ID from which parameters are taken from.
     */
    void setId( const QString &id );

    /**
     * Returns whether the current model refers to a valid algorithm.
     */
    bool isValid() const { return mAlgorithm; }

    /**
     * Returns the display name of the algorithm.
     */
    QString displayName() const;

    /**
     * Returns a short description of the algorithm.
     */
    QString shortHelp() const;

    /**
     * Returns the vector \a layer for in-place algorithm filter.
     */
    QgsVectorLayer *inPlaceLayer() const { return mInPlaceLayer.data(); }

    /**
     * Sets the vector \a layer for in-place algorithm filter.
     */
    void setInPlaceLayer( QgsVectorLayer *layer );

    /**
     * Returns the vector \a layer for in-place algorithm filter.
     */
    QList<QgsFeature> inPlaceFeatures() const { return mInPlaceFeatures; }

    /**
     * Sets the vector \a layer for in-place algorithm filter.
     */
    void setInPlaceFeatures( const QList<QgsFeature> &features );

    /**
     * Returns the algorithm parameters as a map of parameter names as keys and values.
     */
    QVariantMap parameters() const { return mAlgorithmParameters; }

    /**
     * Sets the algorithm parameters using a map with parameter names as keys and values.
     */
    void setParameters( const QVariantMap &parameters );

    /**
     * Returns whether the algorithm will provide preview outputs such as modified geometries.
     * from provided parameters.
     */
    bool preview() const { return mPreview; }

    /**
     * Sets whether the algorithm will provide preview outputs such as modified geometries.
     * from provided parameters.
     */
    void setPreview( bool preview );

    /**
     * Returns a list of geometries previewing the algorithm result using current parameters.
     */
    QList<QgsGeometry> previewGeometries() const { return mPreviewGeometries; }

    /**
     * Executes the algorithm.
     */
    Q_INVOKABLE bool run( bool previewMode = false );

  signals:
    /**
     * Emitted when the algorithm ID has changed
     */
    void idChanged( const QString &id );

    /**
    * Emitted when the parameter model has changed
    */
    void parametersChanged();

    /**
     * Emitted when the in place vector layer has changed
     */
    void inPlaceLayerChanged();

    /**
     * Emitted when the in place feature IDs list has changed
     */
    void inPlaceFeaturesChanged();

    /**
     * Emitted when the preview setting has changed
     */
    void previewChanged();

    /**
     * Emitted when the list of preview geometries has changed
     */
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
