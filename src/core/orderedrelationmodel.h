/***************************************************************************
  orderedrelationmodel.h - OrderedRelationModel

 ---------------------
 begin                : Jun 2021
 copyright            : (C) 2021 by Ivan Ivanov
 email                : ivan (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef ORDEREDRELATIONMODEL_H
#define ORDEREDRELATIONMODEL_H

#include "qgsfeature.h"
#include "qgsfeaturerequest.h"
#include "qgsrelation.h"
#include "qgsvectorlayer.h"
#include "referencingfeaturelistmodel.h"

#include <QSortFilterProxyModel>

class FeatureExpressionValuesGatherer;

/**
 * \ingroup core
 */
class OrderedRelationModel : public ReferencingFeatureListModel
{
    Q_OBJECT

    Q_PROPERTY( QString orderingField READ orderingField WRITE setOrderingField NOTIFY orderingFieldChanged )
    Q_PROPERTY( QString imagePath READ imagePath WRITE setImagePath NOTIFY imagePathChanged )
    Q_PROPERTY( QString description READ description WRITE setDescription NOTIFY descriptionChanged )

  public:
    explicit OrderedRelationModel( QObject *parent = nullptr );

    enum Roles
    {
      ImagePathRole = Qt::UserRole + 100,
      DescriptionRole,
      FeatureIdRole,
      OrderingValueRole,
    };

    QString orderingField() const;
    void setOrderingField( const QString &orderingField );
    QString imagePath() const;
    void setImagePath( const QString &imagePath );
    QString description() const;
    void setDescription( const QString &description );
    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;
    Q_INVOKABLE bool moveItems( const int fromIdx, const int toIdx );
    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE void triggerViewCurrentFeatureChange( int index );

  signals:
    void orderingFieldChanged();
    void imagePathChanged();
    void descriptionChanged();
    void currentFeatureChanged( QgsFeature feature );
    void failedReorder();

  private:
    bool beforeDeleteFeature( QgsVectorLayer *referencingLayer, QgsFeatureId referencingFeatureId ) override;

    QString mOrderingField;
    QString mImagePath;
    QString mDescription;
};

/**
 * \ingroup core
 */
class OrderedRelationProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

  public:
    explicit OrderedRelationProxyModel( QObject *parent = nullptr );

  protected:
    /**
     * Compare two model items for sorting by converting their data to integers.
     * Assumes both data values can be converted to integers without error.
     * Performs a simple numeric comparison on the converted values.
     */
    bool lessThan( const QModelIndex &left, const QModelIndex &right ) const override;
};

#endif // ORDEREDRELATIONMODEL_H
