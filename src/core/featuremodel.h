/***************************************************************************
                            featuremodel.h
                              -------------------
              begin                : 10.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias.kuhn (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef FEATUREMODEL_H
#define FEATUREMODEL_H

#include "geometry.h"
#include "gnsspositioninformation.h"
#include "qfieldcloudconnection.h"
#include "snappingresult.h"
#include "vertexmodel.h"

#include <QAbstractListModel>
#include <QtPositioning/QGeoPositionInfoSource>
#include <qgsfeature.h>
#include <qgsrelationmanager.h>

#include <memory>

class FeatureModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY( FeatureModel::ModelModes modelMode READ modelMode WRITE setModelMode NOTIFY modelModeChanged )
    Q_PROPERTY( QgsFeature feature READ feature WRITE setFeature NOTIFY featureChanged )
    Q_PROPERTY( QList<QgsFeature> features READ features WRITE setFeatures NOTIFY featuresChanged )
    Q_PROPERTY( QgsFeature linkedParentFeature READ linkedParentFeature WRITE setLinkedParentFeature NOTIFY linkedParentFeatureChanged )
    Q_PROPERTY( QgsRelation linkedRelation READ linkedRelation WRITE setLinkedRelation NOTIFY linkedRelationChanged )
    Q_PROPERTY( QString linkedRelationOrderingField READ linkedRelationOrderingField WRITE setLinkedRelationOrderingField NOTIFY linkedRelationOrderingFieldChanged )
    //! the vertex model is used to highlight vertices on the map
    Q_PROPERTY( VertexModel *vertexModel READ vertexModel WRITE setVertexModel NOTIFY vertexModelChanged )
    Q_PROPERTY( Geometry *geometry MEMBER mGeometry NOTIFY geometryChanged )
    Q_PROPERTY( QgsVectorLayer *currentLayer READ layer WRITE setCurrentLayer NOTIFY currentLayerChanged )
    Q_PROPERTY( GnssPositionInformation positionInformation READ positionInformation WRITE setPositionInformation NOTIFY positionInformationChanged )
    Q_PROPERTY( SnappingResult topSnappingResult READ topSnappingResult WRITE setTopSnappingResult NOTIFY topSnappingResultChanged )
    Q_PROPERTY( bool positionLocked READ positionLocked WRITE setPositionLocked NOTIFY positionLockedChanged )
    Q_PROPERTY( CloudUserInformation cloudUserInformation READ cloudUserInformation WRITE setCloudUserInformation NOTIFY cloudUserInformationChanged );
    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )

  public:
    //! keeping the information what attributes are remembered and the last edited feature
    struct RememberValues
    {
        QgsFeature rememberedFeature;
        QVector<bool> rememberedAttributes;
    };

    enum ModelModes
    {
      SingleFeatureModel = 1,
      MultiFeatureModel,
    };
    Q_ENUM( ModelModes )

    enum FeatureRoles
    {
      AttributeName = Qt::UserRole + 1,
      AttributeValue,
      Field,
      RememberAttribute,
      LinkedAttribute,    //! value of this attribute is given by the parent feature and does not to be available for editing in the form
      AttributeAllowEdit, //! value of this attribute is equal across features being edited
    };
    Q_ENUM( FeatureRoles )

    explicit FeatureModel( QObject *parent = nullptr );

    void setModelMode( const ModelModes mode );
    ModelModes modelMode() const;

    void setFeature( const QgsFeature &feature );

    /**
     * Return the feature wrapped in a QVariant for passing it around in QML
     */
    QgsFeature feature() const;

    void setFeatures( const QList<QgsFeature> &features );

    /**
     * Return the features list for passing it around in QML
     */
    QList<QgsFeature> features() const;

    /**
     * A linked feature is a parent feature of a relation passing it's pk(s) to the created child features fk(s)
     * The fk fields are evaluated over the linked relation.
     * \param feature
     * \see linkedParentFeature
     */
    void setLinkedParentFeature( const QgsFeature &feature );

    /**
     * A linked feature is a parent feature of a relation passing it's pk(s) to the created child features fk(s)
     * \return the parent feature linked to this feature
     * \see setLinkedParentFeature
     */
    QgsFeature linkedParentFeature() const;

    /**
     * The relation connecting this feature to the parent, over which this feature has been loaded (e.g. over relation editor widget)
     * The relation is userd to evaluate the parents pk(s) and the childs fk(s)
     * \param relation
     * \see linkedRelation
     */
    void setLinkedRelation( const QgsRelation &relation );

    /**
     * The relation connecting this feature to the parent, over which this feature has been loaded (e.g. over relation editor widget)
     * \return the relation connecting the parent
     * \see setLinkedRelation
     */
    QgsRelation linkedRelation() const;

    /**
     * Returns the field name used for ordering in ordered relations. Empty string if not available.
     * \see setLinkedRelationOrderingField
     */
    QString linkedRelationOrderingField() const;

    /**
     * Sets the field name used for ordering in ordered relations. Empty string if not available.
     * \see linkedRelationOrderingField
     */
    void setLinkedRelationOrderingField( const QString &orderingField );

    void setCurrentLayer( QgsVectorLayer *layer );
    QgsVectorLayer *layer() const;

    //! \copydoc vertexModel
    VertexModel *vertexModel();
    //! \copydoc vertexModel
    void setVertexModel( VertexModel *model );

    QHash<int, QByteArray> roleNames() const override;
    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;
    bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole ) override;

    /**
     * Will commit the edit buffer of this layer.
     * May change in the future to only commit the changes buffered in this model.
     *
     * @return Success of the operation
     */
    Q_INVOKABLE bool save();

    /**
     * Will reset the feature to the original values and dismiss any buffered edits.
     */
    Q_INVOKABLE void reset();

    /**
     * Will refresh the feature values and geometry from the data source.
     */
    Q_INVOKABLE void refresh();

    /**
     * Will create this feature as a new feature on the data source
     */
    Q_INVOKABLE bool create();

    /**
     * Deletes the current feature from the data source
     */
    Q_INVOKABLE bool deleteFeature();

    Q_INVOKABLE bool suppressFeatureForm() const;

    /**
     * Resets the attribute values of the current feature
     * \param partialReset when set to TRUE, only attributes with default or remembered values will be reset
     */
    Q_INVOKABLE void resetAttributes( bool partialReset = false );

    Q_INVOKABLE void resetFeature();

    Q_INVOKABLE void resetFeatureId();

    QVector<bool> rememberedAttributes() const;

    /**
     * Returns position information generated by the TransformedPositionSource according to its provider
     */
    GnssPositionInformation positionInformation() const;

    /**
     * Sets position information generated by the TransformedPositionSource according to its provider
     * \param positionInformation the position information
     */
    void setPositionInformation( const GnssPositionInformation &positionInformation );

    /**
     * Returns the top snapping result of the coordinate locator
     */
    SnappingResult topSnappingResult() const;

    /**
     * Sets the top snapping result of the coordinate locator
     * \param topSnappingResult the top snapping result object
     */
    void setTopSnappingResult( const SnappingResult &topSnappingResult );

    //! Apply the vertex model to the feature geometry.
    //! \note This shall be used if the feature model is used with the vertex model rather than the geometry and rubberband model
    Q_INVOKABLE void applyVertexModelToGeometry();

    //! Apply the feature geometry to a vertex model if present.
    Q_INVOKABLE void applyGeometryToVertexModel();

    //! Apply the vertex model changes to editablelayers' topography.
    Q_INVOKABLE void applyVertexModelTopography();

    //! Update the linked geometry rubber band to match the feature's geometry
    Q_INVOKABLE void updateRubberband() const;

    /**
     * Returns whether the position is locked to the GNSS
     */
    bool positionLocked() const;

    /**
     * Sets whether the position is locked to the GNSS
     */
    void setPositionLocked( bool positionLocked );

    /**
     * Returns the current cloud user information
     */
    CloudUserInformation cloudUserInformation() const { return mCloudUserInformation; }

    /**
     * Sets the current cloud user information
     * \param cloudUserInformation the cloud user information
     */
    void setCloudUserInformation( const CloudUserInformation &cloudUserInformation );

    //! Returns the current project from which the digitizing logs will be sought
    QgsProject *project() const { return mProject; }

    //! Sets the \a project used to find the digitizing logs layer
    void setProject( QgsProject *project );

  public slots:
    void applyGeometry();
    void removeLayer( QObject *layer );

  signals:
    void modelModeChanged();

    //! Emitted when the model's feature has been saved (i.e. updated) but not changed as a result
    void featureUpdated();
    //! Emitted when the model's single feature has been changed
    void featureChanged();
    //! Emitted when the model's multi features list has been changed
    void featuresChanged();

    void linkedParentFeatureChanged();
    void linkedRelationChanged();
    void linkedRelationOrderingFieldChanged();
    void vertexModelChanged();
    void geometryChanged();
    void currentLayerChanged();
    void positionInformationChanged();
    void topSnappingResultChanged();
    void positionLockedChanged();
    void projectChanged();
    void cloudUserInformationChanged();

    void warning( const QString &text );

  private:
    bool commit();
    bool startEditing();
    void setLinkedFeatureValues();
    void updateDefaultValues();

    ModelModes mModelMode = SingleFeatureModel;
    QPointer<QgsVectorLayer> mLayer;
    QgsFeature mFeature;
    QList<QgsFeature> mFeatures;
    QList<bool> mAttributesAllowEdit;
    QgsFeature mLinkedParentFeature;
    QgsRelation mLinkedRelation;
    QString mLinkedRelationOrderingField;
    QList<int> mLinkedAttributeIndexes;
    VertexModel *mVertexModel = nullptr;
    Geometry *mGeometry = nullptr;
    GnssPositionInformation mPositionInformation;
    SnappingResult mTopSnappingResult;
    CloudUserInformation mCloudUserInformation;
    QgsProject *mProject = nullptr;
    QString mTempName;
    bool mPositionLocked = false;
};

#endif // FEATUREMODEL_H
