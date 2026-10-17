

# File qforderedrelationmodel.h

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qforderedrelationmodel.h**](qforderedrelationmodel_8h.md)

[Go to the documentation of this file](qforderedrelationmodel_8h.md)


```C++
/***************************************************************************
  qforderedrelationmodel.h - QfOrderedRelationModel

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

#ifndef QFORDEREDRELATIONMODEL_H
#define QFORDEREDRELATIONMODEL_H

#include "qfreferencingfeaturelistmodel.h"
#include "qgsfeature.h"
#include "qgsrelation.h"
#include "qgsvectorlayer.h"

#include <QSortFilterProxyModel>

class QfFeatureExpressionValuesGatherer;

class QfOrderedRelationModel : public QfReferencingFeatureListModelBase
{
    Q_OBJECT

    Q_PROPERTY( QString orderingField READ orderingField WRITE setOrderingField NOTIFY orderingFieldChanged )
    Q_PROPERTY( QString imagePath READ imagePath WRITE setImagePath NOTIFY imagePathChanged )
    Q_PROPERTY( QString description READ description WRITE setDescription NOTIFY descriptionChanged )

  public:
    explicit QfOrderedRelationModel( QObject *parent = nullptr );

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

#endif // QFORDEREDRELATIONMODEL_H
```


