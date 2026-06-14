

# File orderedrelationmodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**orderedrelationmodel.h**](orderedrelationmodel_8h.md)

[Go to the documentation of this file](orderedrelationmodel_8h.md)


```C++
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
#include "qgsrelation.h"
#include "qgsvectorlayer.h"
#include "referencingfeaturelistmodel.h"

#include <QSortFilterProxyModel>

class FeatureExpressionValuesGatherer;

class OrderedRelationModel : public ReferencingFeatureListModelBase
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
    void sortEntries();

    QString mOrderingField;
    QString mImagePath;
    QString mDescription;
};

#endif // ORDEREDRELATIONMODEL_H
```


