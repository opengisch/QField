

# File multifeaturelistmodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**multifeaturelistmodel.h**](multifeaturelistmodel_8h.md)

[Go to the documentation of this file](multifeaturelistmodel_8h.md)


```C++
/***************************************************************************
                            featurelistmodel.cpp
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

#ifndef MULTIFEATURELISTMODEL_H
#define MULTIFEATURELISTMODEL_H

#include "identifytool.h"
#include "multifeaturelistmodelbase.h"

#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
#include <qgsfeaturerequest.h>

class MultiFeatureListModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY( int count READ count NOTIFY countChanged )
    Q_PROPERTY( QList<QgsFeature> selectedFeatures READ selectedFeatures NOTIFY selectedCountChanged )
    Q_PROPERTY( QgsVectorLayer *selectedLayer READ selectedLayer NOTIFY selectedLayerChanged )
    Q_PROPERTY( int selectedCount READ selectedCount NOTIFY selectedCountChanged )
    Q_PROPERTY( bool canEditAttributesSelection READ canEditAttributesSelection NOTIFY selectedCountChanged )
    Q_PROPERTY( bool canMergeSelection READ canMergeSelection NOTIFY selectedCountChanged )
    Q_PROPERTY( bool canDeleteSelection READ canDeleteSelection NOTIFY selectedCountChanged )
    Q_PROPERTY( bool canDuplicateSelection READ canDuplicateSelection NOTIFY selectedCountChanged )
    Q_PROPERTY( bool canMoveSelection READ canMoveSelection NOTIFY selectedCountChanged )
    Q_PROPERTY( bool canRotateSelection READ canRotateSelection NOTIFY selectedCountChanged )
    Q_PROPERTY( bool canProcessSelection READ canProcessSelection NOTIFY selectedCountChanged )

  public:
    enum FeatureListRoles
    {
      FeatureIdRole = Qt::UserRole + 1,
      FeatureSelectedRole,
      FeatureNameRole,
      FeatureRole,
      LayerNameRole,
      LayerRole,
      GeometryRole,
      CrsRole,
      DeleteFeatureRole,
      EditGeometryRole
    };

    explicit MultiFeatureListModel( QObject *parent = nullptr );

    void setFeatures( const QMap<QgsVectorLayer *, QgsFeatureRequest> &requests );

    Q_INVOKABLE void setFeatures( QgsVectorLayer *vl, const QString &filter, const QgsRectangle &extent = QgsRectangle() );

    void appendFeatures( const QList<IdentifyTool::IdentifyResult> &results );

    Q_INVOKABLE void clear( const bool keepSelected = false );

    Q_INVOKABLE void clearSelection();

    int count() const;

    int selectedCount() const;

    bool canEditAttributesSelection() const;

    bool canMergeSelection() const;

    bool canDeleteSelection() const;

    bool canDuplicateSelection() const;

    bool canMoveSelection() const;

    bool canRotateSelection() const;

    bool canProcessSelection() const;

    Q_INVOKABLE bool mergeSelection();

    Q_INVOKABLE bool deleteFeature( QgsVectorLayer *layer, QgsFeatureId fid );

    Q_INVOKABLE bool deleteSelection();

    Q_INVOKABLE bool duplicateFeature( QgsVectorLayer *layer, const QgsFeature &feature );

    Q_INVOKABLE bool duplicateSelection();

    Q_INVOKABLE bool moveSelection( const double x, const double y );

    Q_INVOKABLE bool rotateSelection( const double angle );

    Q_INVOKABLE void toggleSelectedItem( int item );

    QList<QgsFeature> selectedFeatures();

    QgsVectorLayer *selectedLayer();

  signals:

    void countChanged();

    void selectedCountChanged();

    void selectedLayerChanged();

  protected:
    virtual bool filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const override;

    void adjustFilterToSelectedCount();

  private:
    MultiFeatureListModelBase *mSourceModel = nullptr;

    QPointer<QgsVectorLayer> mFilterLayer;
};

#endif // MULTIFEATURELISTMODEL_H
```


