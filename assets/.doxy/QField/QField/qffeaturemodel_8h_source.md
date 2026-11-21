

# File qffeaturemodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qffeaturemodel.h**](qffeaturemodel_8h.md)

[Go to the documentation of this file](qffeaturemodel_8h.md)


```C++
/***************************************************************************
                            qffeaturemodel.h
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

#ifndef QFFEATUREMODEL_H
#define QFFEATUREMODEL_H

#include "qfappexpressioncontextscopesgenerator.h"
#include "qfgeometry.h"
#include "qfsnappingresult.h"
#include "qfvertexmodel.h"

#include <QAbstractListModel>
#include <qgsfeature.h>
#include <qgsrelationmanager.h>

class QfFeatureModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY( QfFeatureModel::ModelModes modelMode READ modelMode WRITE setModelMode NOTIFY modelModeChanged )
    Q_PROPERTY( QgsFeature feature READ feature WRITE setFeature NOTIFY featureChanged )
    Q_PROPERTY( QList<QgsFeature> features READ features WRITE setFeatures NOTIFY featuresChanged )
    Q_PROPERTY( QgsFeature linkedParentFeature READ linkedParentFeature WRITE setLinkedParentFeature NOTIFY linkedParentFeatureChanged )
    Q_PROPERTY( QgsRelation linkedRelation READ linkedRelation WRITE setLinkedRelation NOTIFY linkedRelationChanged )
    Q_PROPERTY( QString linkedRelationOrderingField READ linkedRelationOrderingField WRITE setLinkedRelationOrderingField NOTIFY linkedRelationOrderingFieldChanged )
    Q_PROPERTY( QfVertexModel *vertexModel READ vertexModel WRITE setVertexModel NOTIFY vertexModelChanged )
    Q_PROPERTY( QfGeometry *geometry READ geometry WRITE setGeometry NOTIFY geometryChanged )
    Q_PROPERTY( bool featureAdditionLocked READ featureAdditionLocked NOTIFY featureAdditionLockedChanged )
    Q_PROPERTY( bool attributeEditingLocked READ attributeEditingLocked NOTIFY attributeEditingLockedChanged )
    Q_PROPERTY( bool geometryEditingLocked READ geometryEditingLocked NOTIFY geometryEditingLockedChanged )
    Q_PROPERTY( bool featureDeletionLocked READ featureDeletionLocked NOTIFY featureDeletionLockedChanged )
    Q_PROPERTY( QgsVectorLayer *currentLayer READ layer WRITE setCurrentLayer NOTIFY currentLayerChanged )
    Q_PROPERTY( QfAppExpressionContextScopesGenerator *appExpressionContextScopesGenerator READ appExpressionContextScopesGenerator WRITE setAppExpressionContextScopesGenerator NOTIFY appExpressionContextScopesGeneratorChanged )
    Q_PROPERTY( QfSnappingResult topSnappingResult READ topSnappingResult WRITE setTopSnappingResult NOTIFY topSnappingResultChanged )
    Q_PROPERTY( QgsProject *project READ project WRITE setProject NOTIFY projectChanged )
    Q_PROPERTY( bool batchMode READ batchMode WRITE setBatchMode NOTIFY batchModeChanged )

  public:
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
      LinkedAttribute,    
      AttributeAllowEdit, 
    };
    Q_ENUM( FeatureRoles )

    explicit QfFeatureModel( QObject *parent = nullptr );

    void setModelMode( const ModelModes mode );
    ModelModes modelMode() const;

    void setFeature( const QgsFeature &feature );

    QgsFeature feature() const;

    void setFeatures( const QList<QgsFeature> &features );

    QList<QgsFeature> features() const;

    void setLinkedParentFeature( const QgsFeature &feature );

    QgsFeature linkedParentFeature() const;

    void setLinkedRelation( const QgsRelation &relation );

    QgsRelation linkedRelation() const;

    QString linkedRelationOrderingField() const;

    void setLinkedRelationOrderingField( const QString &orderingField );

    void setCurrentLayer( QgsVectorLayer *layer );
    QgsVectorLayer *layer() const;

    QfGeometry *geometry();

    void setGeometry( QfGeometry *geometry );

    QfVertexModel *vertexModel();
    void setVertexModel( QfVertexModel *model );

    bool featureAdditionLocked() const { return mFeatureAdditionLocked; }
    bool attributeEditingLocked() const { return mAttributeEditingLocked; }
    bool geometryEditingLocked() const { return mGeometryEditingLocked; }
    bool featureDeletionLocked() const { return mFeatureDeletionLocked; }

    QHash<int, QByteArray> roleNames() const override;
    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;
    bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole ) override;

    Q_INVOKABLE bool changeGeometry( const QgsGeometry &geometry );

    Q_INVOKABLE bool save( bool flushBuffer = true );

    Q_INVOKABLE void reset();

    Q_INVOKABLE void refresh();

    Q_INVOKABLE bool create( bool flushBuffer = true );

    Q_INVOKABLE bool deleteFeature();

    Q_INVOKABLE bool suppressFeatureForm() const;

    Q_INVOKABLE void resetAttributes( bool partialReset = false );

    Q_INVOKABLE void resetFeature();

    Q_INVOKABLE void resetFeatureId();

    Q_INVOKABLE bool updateAttributesFromFeature( const QgsFeature &feature );

    QVector<bool> rememberedAttributes() const;

    QfAppExpressionContextScopesGenerator *appExpressionContextScopesGenerator() const;

    void setAppExpressionContextScopesGenerator( QfAppExpressionContextScopesGenerator *generator );

    QfSnappingResult topSnappingResult() const;

    void setTopSnappingResult( const QfSnappingResult &topSnappingResult );

    Q_INVOKABLE void applyGeometry( bool fromVertexModel = false, bool skipTopologicalEditing = false );

    Q_INVOKABLE void applyGeometryToVertexModel();

    Q_INVOKABLE void updateRubberband() const;

    bool positionLocked() const;

    void setPositionLocked( bool positionLocked );

    QgsProject *project() const { return mProject; }

    void setProject( QgsProject *project );

    QgsExpressionContext createExpressionContext() const;

    bool batchMode() const { return mBatchMode; }

    void setBatchMode( bool batchMode );

  public slots:
    void removeLayer( QObject *layer );

  signals:
    void modelModeChanged();

    void featureUpdated();
    void featureChanged();
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
    QgsFeature mSavedFeature;
    QgsFeature mFeature;
    QList<QgsFeature> mFeatures;
    QList<bool> mAttributesAllowEdit;
    QgsFeature mLinkedParentFeature;
    QgsRelation mLinkedRelation;
    QString mLinkedRelationOrderingField;
    QList<int> mLinkedAttributeIndexes;
    QfVertexModel *mVertexModel = nullptr;
    QfGeometry *mGeometry = nullptr;
    QPointer<QfAppExpressionContextScopesGenerator> mAppExpressionContextScopesGenerator;
    QfSnappingResult mTopSnappingResult;
    QgsProject *mProject = nullptr;
    QString mTempName;

    bool mBatchMode = false;
    bool mBatchModeWasEditing = false;
};

#endif // QFFEATUREMODEL_H
```


