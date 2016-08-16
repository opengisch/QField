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
#include "feature.h"
#include "geometry.h"

class FeatureModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY( Feature feature READ feature WRITE setFeature NOTIFY featureChanged )
    Q_PROPERTY( Geometry* geometry MEMBER mGeometry NOTIFY geometryChanged )
    Q_PROPERTY( QgsVectorLayer* currentLayer READ layer WRITE setLayer NOTIFY layerChanged )
    Q_ENUMS( FeatureRoles )

  public:
    enum FeatureRoles
    {
      AttributeName = Qt::UserRole + 1,
      AttributeValue,
      AttributeEditable,
      EditorWidget,
      EditorWidgetConfig,
      RememberValue,
      Field
    };

    explicit FeatureModel( QObject *parent = 0 );
    explicit FeatureModel( const QgsFeature& feat, QObject *parent = 0 );

    void setFeature( const QVariant& feature );
    void setFeature( const Feature& feature , bool force = false );

    void setLayer( QgsVectorLayer* layer );
    QgsVectorLayer* layer() const;


    /**
     * Return the feature wrapped in a QVariant for passing it around in QML
     */
    Feature feature() const;

    QHash<int, QByteArray> roleNames() const override;
    int rowCount( const QModelIndex& parent ) const override;
    QVariant data( const QModelIndex& index, int role ) const override;

    virtual bool setData( const QModelIndex &index, const QVariant &value, int role ) override;

    /**
     * Will change an attribute to a given value in the edit buffer.
     * At the moment only allows to AttributeValue as role.
     * May change in the future to commit changes to a local feature instead of the layer edit buffer.
     *
     * @param fieldindex The field to change
     * @param value Value to set
     * @param null Force NULL value. At the moment null QVariants are not pushed from QML.
     * @return Success of the operation
     */
    Q_INVOKABLE bool setAttribute( int fieldIndex, const QVariant& value, bool null = false );

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

    Q_INVOKABLE bool suppressFeatureForm() const;

    Q_INVOKABLE void resetAttributes( bool skipRemembered = true );

  public slots:
    void applyGeometry();

    void create();

  signals:
    void featureChanged();
    void geometryChanged();
    void layerChanged();

    void warning( const QString& text );

  private:
    bool commit();
    bool startEditing();

    Feature mFeature;
    Geometry* mGeometry;
    QVector<bool> mRememberedAttributes;
};

#endif // FEATUREMODEL_H
