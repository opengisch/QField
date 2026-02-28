

# File layertreemodel.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**layertreemodel.h**](layertreemodel_8h.md)

[Go to the documentation of this file](layertreemodel_8h.md)


```C++
/***************************************************************************
  layertreemodel.h - LayerTree

 ---------------------
 begin                : 6.12.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef LAYERTREEMODEL_H
#define LAYERTREEMODEL_H

#include <QSortFilterProxyModel>
#include <qgslayertreelayer.h>

class QgsLayerTree;
class QgsLayerTreeModel;
class QgsProject;
class QgsQuickMapSettings;

class FlatLayerTreeModelBase : public QAbstractProxyModel
{
    Q_OBJECT

  public:
    explicit FlatLayerTreeModelBase( QgsLayerTree *layerTree, QgsProject *project, QObject *parent = nullptr );

    void setSourceModel( QAbstractItemModel *sourceModel ) override;
    QModelIndex mapToSource( const QModelIndex &proxyIndex ) const override;
    QModelIndex mapFromSource( const QModelIndex &sourceIndex ) const override;

    QModelIndex parent( const QModelIndex &child ) const override;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const override;
    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;

    QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const override;

    QVariant data( const QModelIndex &index, int role ) const override;
    bool setData( const QModelIndex &index, const QVariant &value, int role ) override;

    QHash<int, QByteArray> roleNames() const override;

    QgsProject *project() const;

    QgsLayerTreeModel *layerTreeModel() const;

    QgsLayerTree *layerTree() const;

    QString mapTheme() const;
    void setMapTheme( const QString &mapTheme );

    Q_INVOKABLE void updateCurrentMapTheme();

    bool isFrozen() const;
    Q_INVOKABLE void freeze();
    Q_INVOKABLE void unfreeze( bool resetModel = false );

    void setLayerInTracking( QgsLayerTreeLayer *nodeLayer, bool tracking );

    bool isTemporal() const { return mIsTemporal; }

    QgsRectangle nodeExtent( const QModelIndex &index, QgsQuickMapSettings *mapSettings, const float buffer );

    void setAllCollapsed( bool collapsed );

    bool hasCollapsibleItems() const;

    bool isCollapsed() const;

  signals:
    void layersAdded();
    void layersRemoved();
    void mapThemeChanged();
    void hasCollapsibleItemsChanged();
    void isCollapsedChanged();
    void isTemporalChanged();
    void isFrozenChanged();

  private:
    void updateMap( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles );
    int buildMap( QgsLayerTreeModel *model, const QModelIndex &parent = QModelIndex(), int row = 0, int treeLevel = 0 );
    void clearMap();

    void removeFromMap( const QModelIndex &parent, int first, int last );
    void insertInMap( const QModelIndex &parent, int first, int last );

    void featureCountChanged();
    void updateTemporalState();
    void adjustTemporalStateFromAddedLayers( const QList<QgsMapLayer *> &layers );

    void checkHasCollapsibleItems();
    void checkIsCollapsed();

    QMap<QModelIndex, int> mRowMap;
    QMap<int, QModelIndex> mIndexMap;
    QMap<int, int> mTreeLevelMap;
    QList<QModelIndex> mCollapsedItems;

    QgsLayerTreeModel *mLayerTreeModel = nullptr;
    QString mMapTheme;
    QgsProject *mProject = nullptr;
    QList<QgsLayerTreeLayer *> mLayersInTracking;

    bool mHasCollapsibleItems = false;
    bool mIsCollapsed = false;

    bool mIsTemporal = false;
    int mFrozen = 0;

    bool mProjectLayersChanged = false;
};

class FlatLayerTreeModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY( QString mapTheme READ mapTheme WRITE setMapTheme NOTIFY mapThemeChanged )

    Q_PROPERTY( bool hasCollapsibleItems READ hasCollapsibleItems NOTIFY hasCollapsibleItemsChanged )
    Q_PROPERTY( bool isCollapsed READ isCollapsed NOTIFY isCollapsedChanged )

    Q_PROPERTY( bool isTemporal READ isTemporal NOTIFY isTemporalChanged )
    Q_PROPERTY( bool isFrozen READ isFrozen NOTIFY isFrozenChanged )

  public:
    enum Roles
    {
      VectorLayerPointer = Qt::UserRole + 1,
      MapLayerPointer,
      HasSpatialExtent,
      LegendImage,
      Type,
      Name,
      Visible,
      Trackable,
      InTracking,
      ReadOnly,
      FeatureAdditionLocked,
      AttributeEditingLocked,
      GeometryEditingLocked,
      FeatureDeletionLocked,
      TreeLevel,
      LayerType,
      IsValid,
      FeatureCount,
      IsCollapsed,
      IsParentCollapsed,
      HasChildren,
      CanReloadData,
      HasLabels,
      LabelsVisible,
      Opacity,
      FilterExpression,
      Credits,
      SnappingEnabled,
      HasNotes,
      Notes,
      Checkable,
    };
    Q_ENUM( Roles )

    enum Types
    {
      Layer,
      Group,
      Image,
      Legend,
    };
    Q_ENUM( Types )

    explicit FlatLayerTreeModel( QgsLayerTree *layerTree, QgsProject *project, QObject *parent = nullptr );

    Q_INVOKABLE QVariant data( const QModelIndex &index, int role ) const override;
    Q_INVOKABLE bool setData( const QModelIndex &index, const QVariant &value, int role ) override;

    QString mapTheme() const;
    void setMapTheme( const QString &mapTheme );

    bool isTemporal() const;

    Q_INVOKABLE void updateCurrentMapTheme();

    bool isFrozen() const;
    Q_INVOKABLE void freeze();
    Q_INVOKABLE void unfreeze( bool resetModel = false );

    void setLayerInTracking( QgsLayerTreeLayer *nodeLayer, bool tracking );

    QgsProject *project() const;

    QgsLayerTreeModel *layerTreeModel() const;

    QgsLayerTree *layerTree() const;

    Q_INVOKABLE QgsRectangle nodeExtent( const QModelIndex &index, QgsQuickMapSettings *mapSettings, const float buffer = 0.02 );

    Q_INVOKABLE void setAllCollapsed( bool collapsed );

    bool hasCollapsibleItems() const;

    bool isCollapsed() const;

  signals:
    void layersAdded();
    void layersRemoved();
    void mapThemeChanged();
    void hasCollapsibleItemsChanged();
    void isCollapsedChanged();
    void isTemporalChanged();
    void isFrozenChanged();

  protected:
    virtual bool filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const override;

  private:
    FlatLayerTreeModelBase *mSourceModel = nullptr;
};


#endif // LAYERTREEMODEL_H
```


