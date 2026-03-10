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

#include "appexpressioncontextscopesgenerator.h"
#include "geometry.h"
#include "snappingresult.h"
#include "vertexmodel.h"

#include <QAbstractListModel>
#include <qgsfeature.h>
#include <qgsrelationmanager.h>

/**
 * \ingroup core
 */
class FeatureModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY( FeatureModel::ModelModes modelMode READ modelMode WRITE setModelMode NOTIFY modelModeChanged )
    Q_PROPERTY( QgsFeature feature READ feature WRITE setFeature NOTIFY featureChanged )
    Q_PROPERTY( QList<QgsFeature> features READ features WRITE setFeatures NOTIFY featuresChanged )
    Q_PROPERTY( QgsFeature linkedParentFeature READ linkedParentFeature WRITE setLinkedParentFeature NOTIFY linkedParentFeatureChanged )
    Q_PROPERTY( QgsRelation linkedRelation READ linkedRelation WRITE setLinkedRelation NOTIFY linkedRelationChanged )
    Q_PROPERTY( QString linkedRelationOrderingField READ linkedRelationOrderingField WRITE setLinkedRelationOrderingField NOTIFY linkedRelationOrderingFieldChanged )
    Q_PROPERTY( VertexModel *vertexModel READ vertexModel WRITE setVertexModel NOTIFY vertexModelChanged )
    Q_PROPERTY( Geometry *geometry READ geometry WRITE setGeometry NOTIFY geometryChanged )
    Q_PROPERTY( bool featureAdditionLocked READ featureAdditionLocked NOTIFY featureAdditionLockedChanged )
    Q_PROPERTY( bool attributeEditingLocked READ attributeEditingLocked NOTIFY attributeEditingLockedChanged )
    Q_PROPERTY( bool geometryEditingLocked READ geometryEditingLocked NOTIFY geometryEditingLockedChanged )
    Q_PROPERTY( bool featureDeletionLocked READ featureDeletionLocked NOTIFY featureDeletionLockedChanged )
    Q_PROPERTY( QgsVectorLayer *currentLayer READ layer WRITE setCurrentLayer NOTIFY currentLayerChanged )
    Q_PROPERTY( AppExpressionContextScopesGenerator *appExpressionContextScopesGenerator READ appExpressionContextScopesGenerator WRITE setAppExpressionContextScopesGenerator NOTIFY appExpressionContextScopesGeneratorChanged )
    Q_PROPERTY( SnappingResult topSnappingResult READ topSnappingResult WRITE setTopSnappingResult NOTIFY topSnappingResultChanged )
    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )
    Q_PROPERTY( bool batchMode READ batchMode WRITE setBatchMode NOTIFY batchModeChanged )

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

    /**
     * Returns the geometry object that will drive the feature geometry.
     */
    Geometry *geometry();

    /**
     * Sets the geometry object that will drive the feature geometry.
     * \note This is not the QgsGeometry of the feature. To change that,
     * use the changeGeometry function.
     */
    void setGeometry( Geometry *geometry );

    //! Returns the vertex model is used to highlight vertices on the map.
    VertexModel *vertexModel();
    //! Sets the vertex \a model is used to highlight vertices on the map.
    void setVertexModel( VertexModel *model );

    bool featureAdditionLocked() const { return mFeatureAdditionLocked; }
    bool attributeEditingLocked() const { return mAttributeEditingLocked; }
    bool geometryEditingLocked() const { return mGeometryEditingLocked; }
    bool featureDeletionLocked() const { return mFeatureDeletionLocked; }

    QHash<int, QByteArray> roleNames() const override;
    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;
    bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole ) override;

    /**
     * Sets the \a geometry of the feature.
     */
    Q_INVOKABLE bool changeGeometry( const QgsGeometry &geometry );

    /**
     * Will commit the edit buffer of this layer.
     * May change in the future to only commit the changes buffered in this model.
     *
     * By setting \a flushBuffer to FALSE, the edits made to the feature will remain
     * in the edit buffer provided the vector layer was already in editing mode.
     *
     * \return TRUE if a feature was successfully saved
     */
    Q_INVOKABLE bool save( bool flushBuffer = true );

    /**
     * Will reset the feature to the original values and dismiss any buffered edits.
     */
    Q_INVOKABLE void reset();

    /**
     * Will refresh the feature values and geometry from the data source.
     */
    Q_INVOKABLE void refresh();

    /**
     * Will create this feature as a new feature on the data source.
     *
     * By setting \a flushBuffer to FALSE, the created feature will remain
     * in the edit buffer provided the vector layer was already in editing mode.
     * The flushBuffer parameter will be ignored for layers containing
     * relationships.
     *
     * \return TRUE if a feature was successfully created
     */
    Q_INVOKABLE bool create( bool flushBuffer = true );

    /**
     * Deletes the current feature from the data source.
     */
    Q_INVOKABLE bool deleteFeature();

    /**
     * Returns whether the feature form should be suppressed when adding new features.
     */
    Q_INVOKABLE bool suppressFeatureForm() const;

    /**
     * Resets the attribute values of the current feature
     * \param partialReset when set to TRUE, only attributes with default or remembered values will be reset
     */
    Q_INVOKABLE void resetAttributes( bool partialReset = false );

    Q_INVOKABLE void resetFeature();

    Q_INVOKABLE void resetFeatureId();

    /**
     * Update attributes to match that of a given feature
     * \param feature feature from which attributes will be taken
     * \return TRUE if one or more attribute was updated
     */
    Q_INVOKABLE bool updateAttributesFromFeature( const QgsFeature &feature );

    QVector<bool> rememberedAttributes() const;

    /**
     * Returns the application expression context scopes generator object
     */
    AppExpressionContextScopesGenerator *appExpressionContextScopesGenerator() const;

    /**
     * Sets the application expression context scopes \a generator object
     */
    void setAppExpressionContextScopesGenerator( AppExpressionContextScopesGenerator *generator );

    /**
     * Returns the top snapping result of the coordinate locator
     */
    SnappingResult topSnappingResult() const;

    /**
     * Sets the top snapping result of the coordinate locator
     * \param topSnappingResult the top snapping result object
     */
    void setTopSnappingResult( const SnappingResult &topSnappingResult );

    /**
     * Apply the geometry object or vertex model object's geometry to the feature geometry.
     * \param fromVertexModel set to TRUE to use the vertex model
     */
    Q_INVOKABLE void applyGeometry( bool fromVertexModel = false );

    //! Apply the feature geometry to a vertex model if present.
    Q_INVOKABLE void applyGeometryToVertexModel();

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

    //! Returns the current project from which the digitizing logs will be sought
    QgsProject *project() const { return mProject; }

    //! Sets the \a project used to find the digitizing logs layer
    void setProject( QgsProject *project );

    QgsExpressionContext createExpressionContext() const;

    /**
     * Returns TRUE if the feature model is in batch mode. When enabled, the vector layer
     * will remain in editing mode until batch mode is disabled.
     */
    bool batchMode() const { return mBatchMode; }

    /**
     * Toggles the feature model batch mode. When enabled, the vector layer
     * will remain in editing mode until batch mode is disabled.
     */
    void setBatchMode( bool batchMode );

  public slots:
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
    void featureAdditionLockedChanged();
    void attributeEditingLockedChanged();
    void geometryEditingLockedChanged();
    void featureDeletionLockedChanged();
    void currentLayerChanged();
    void appExpressionContextScopesGeneratorChanged();
    void topSnappingResultChanged();
    void projectChanged();
    void batchModeChanged();

    void warning( const QString &text );

  private:
    QgsFeatureIds applyVertexModelTopography();
    void applyGeometryTopography( const QgsGeometry &geometry );

    bool commit( bool stopEditing = true );
    bool startEditing();
    bool isEditing() const;

    void setLinkedFeatureValues();
    void updateDefaultValues();
    void updatePermissions();

    // The current feature locked states
    bool mFeatureAdditionLocked = false;
    bool mAttributeEditingLocked = false;
    bool mGeometryEditingLocked = false;
    bool mFeatureDeletionLocked = false;

    bool mFeatureAdditionLockedByDefault = false;
    QString mFeatureAdditionLockedExpression;
    bool mAttributeEditingLockedByDefault = false;
    QString mAttributeEditingLockedExpression;
    bool mGeometryEditingLockedByDefault = false;
    QString mGeometryEditingLockedExpression;
    bool mFeatureDeletionLockedByDefault = false;
    QString mFeatureDeletionLockedExpression;

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
    QPointer<AppExpressionContextScopesGenerator> mAppExpressionContextScopesGenerator;
    SnappingResult mTopSnappingResult;
    QgsProject *mProject = nullptr;
    QString mTempName;

    bool mBatchMode = false;
    bool mBatchModeWasEditing = false;
};

#endif // FEATUREMODEL_H
