

# File multifeaturelistmodelbase.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**multifeaturelistmodelbase.h**](multifeaturelistmodelbase_8h.md)

[Go to the documentation of this file](multifeaturelistmodelbase_8h.md)


```C++
/***************************************************************************
                            featurelistmodelbase.cpp
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

#ifndef MULTIFEATURELISTMODELBASE_H
#define MULTIFEATURELISTMODELBASE_H

#include "identifytool.h"

#include <QAbstractItemModel>
#include <qgis.h>
#include <qgsfeaturerequest.h>

class MultiFeatureListModelBase : public QAbstractItemModel
{
    Q_OBJECT

  public:
    explicit MultiFeatureListModelBase( QObject *parent = nullptr );

    void setFeatures( const QMap<QgsVectorLayer *, QgsFeatureRequest> &requests );

    void appendFeatures( const QList<IdentifyTool::IdentifyResult> &results );

    void clear( const bool keepSelected = false );

    void clearSelection();

    QHash<int, QByteArray> roleNames() const override;
    QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const override;
    QModelIndex parent( const QModelIndex &child ) const override;
    int rowCount( const QModelIndex &parent ) const override;
    int columnCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;

    virtual bool removeRows( int row, int count, const QModelIndex &parent ) override;

    int count() const;

    int selectedCount() const;

    bool canEditAttributesSelection() const;

    bool canMergeSelection() const;

    bool canDeleteSelection() const;

    bool canDuplicateSelection() const;

    bool canMoveSelection() const;

    bool canRotateSelection() const;

    bool canProcessSelection() const;

    bool mergeSelection();

    bool deleteFeature( QgsVectorLayer *layer, QgsFeatureId fid, bool selectionAction = false );

    bool deleteSelection();

    bool duplicateFeature( QgsVectorLayer *layer, const QgsFeature &feature );

    bool duplicateSelection();

    bool moveSelection( const double x, const double y );

    bool rotateSelection( const double angle );

    void toggleSelectedItem( int item );

    QList<QgsFeature> selectedFeatures() const;

    QgsVectorLayer *selectedLayer() const;

  signals:

    void countChanged();

    void selectedCountChanged();

  private slots:

    void layerDeleted( QObject *object );

    void featureDeleted( QgsFeatureId fid );

    void attributeValueChanged( QgsFeatureId fid, int idx, const QVariant &value );

    void geometryChanged( QgsFeatureId fid, const QgsGeometry &geometry );

  private:
    inline QPair<QgsMapLayer *, QgsFeature> *toFeature( const QModelIndex &index ) const
    {
      return static_cast<QPair<QgsMapLayer *, QgsFeature> *>( index.internalPointer() );
    }

    QList<QPair<QgsMapLayer *, QgsFeature>> mFeatures;
    QList<QPair<QgsMapLayer *, QgsFeature>> mSelectedFeatures;

    QMap<QString, QgsVectorLayer *> mRepresentationalLayers;
};

#endif // MULTIFEATURELISTMODELBASE_H
```


