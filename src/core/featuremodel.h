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

#include <QAbstractListModel>
#include <QtPositioning/QGeoPositionInfoSource>
#include <qgsrelationmanager.h>
#include <memory>
#include <qgsfeature.h>
#include "snappingresult.h"

#include "geometry.h"

class VertexModel;

class FeatureModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY( QgsFeature feature READ feature WRITE setFeature NOTIFY featureChanged )
    Q_PROPERTY( QgsFeature linkedParentFeature READ linkedParentFeature WRITE setLinkedParentFeature NOTIFY linkedParentFeatureChanged )
    Q_PROPERTY( QgsRelation linkedRelation READ linkedRelation WRITE setLinkedRelation NOTIFY linkedRelationChanged )
    //! the vertex model is used to highlight vertices on the map
    Q_PROPERTY( VertexModel *vertexModel READ vertexModel WRITE setVertexModel NOTIFY vertexModelChanged )
    Q_PROPERTY( Geometry *geometry MEMBER mGeometry NOTIFY geometryChanged )
    Q_PROPERTY( QgsVectorLayer *currentLayer READ layer WRITE setCurrentLayer NOTIFY currentLayerChanged )
    Q_PROPERTY( QString positionSourceName READ positionSourceName WRITE setPositionSourceName NOTIFY positionSourceChanged )
    Q_PROPERTY( SnappingResult topSnappingResult READ topSnappingResult WRITE setTopSnappingResult NOTIFY topSnappingResultChanged )

    //! keeping the information what attributes are remembered and the last edited feature
    struct RememberValues
    {
      QgsFeature rememberedFeature;
      QVector<bool> rememberedAttributes;
    };

  public:
    enum FeatureRoles
    {
      AttributeName = Qt::UserRole + 1,
      AttributeValue,
      Field,
      RememberAttribute,
      LinkedAttribute  //! value of this attribute is given by the parent feature and does not to be available for editing in the form
    };
    Q_ENUM( FeatureRoles )

    explicit FeatureModel( QObject *parent = nullptr );
    explicit FeatureModel( const QgsFeature &feat, QObject *parent = nullptr );

    void setFeature( const QgsFeature &feature );

    /**
     * Return the feature wrapped in a QVariant for passing it around in QML
     */
    QgsFeature feature() const;

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
    Q_INVOKABLE void create();

    /**
     * Deletes the current feature from the data source
     */
    Q_INVOKABLE void deleteFeature();

    Q_INVOKABLE bool suppressFeatureForm() const;

    Q_INVOKABLE void resetAttributes();

    QVector<bool> rememberedAttributes() const;

    /**
     * The name of the position source to use.
     * A QGeoPositionInfoSource is created internally based on this name.
     */
    QString positionSourceName() const;

    /**
     * The name of the position source to use.
     * A QGeoPositionInfoSource is created internally based on this name.
     */
    void setPositionSourceName( const QString &positionSourceName );

    /**
     * The snapping result of the coordinate locator
     */
    SnappingResult topSnappingResult() const;

    /**
     * The snapping result of the coordinate locator
     */
    void setTopSnappingResult( const SnappingResult &topSnappingResult );

    //! Apply the vertex model to the feature geometry.
    //! \note This shall be used if the feature model is used with the vertex model rather than the geometry and rubberband model
    Q_INVOKABLE void applyVertexModelToGeometry();

    //! Apply the vertex model changes to the layer topography.
    Q_INVOKABLE void applyVertexModelToLayerTopography();

  public slots:
    void applyGeometry();
    void removeLayer( QObject *layer );

  signals:
    void featureChanged();
    void linkedParentFeatureChanged();
    void linkedRelationChanged();
    void vertexModelChanged();
    void geometryChanged();
    void currentLayerChanged();
    void positionSourceChanged();
    void topSnappingResultChanged();

    void warning( const QString &text );

  private slots:
    void featureAdded( QgsFeatureId fid );

  private:
    bool commit();
    bool startEditing();
    void setLinkedFeatureValues();

    QgsVectorLayer *mLayer = nullptr;
    QgsFeature mFeature;
    QgsFeature mLinkedParentFeature;
    QgsRelation mLinkedRelation;
    QList<int> mLinkedAttributeIndexes;
    VertexModel *mVertexModel = nullptr;
    Geometry *mGeometry = nullptr;
    std::unique_ptr<QGeoPositionInfoSource> mPositionSource;
    SnappingResult mTopSnappingResult;
    QString mTempName;
    QMap<QgsVectorLayer *, RememberValues> mRememberings;
};

#endif // FEATUREMODEL_H
