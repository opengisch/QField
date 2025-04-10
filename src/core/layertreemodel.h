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

/**
 * \ingroup core
 */
class FlatLayerTreeModelBase : public QAbstractProxyModel
{
    Q_OBJECT

  public:
    explicit FlatLayerTreeModelBase( QgsLayerTree *layerTree, QgsProject *project, QObject *parent = nullptr );

    void updateMap( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles );
    int buildMap( QgsLayerTreeModel *model, const QModelIndex &parent = QModelIndex(), int row = 0, int treeLevel = 0 );
    void clearMap();

    void removeFromMap( const QModelIndex &parent, int first, int last );
    void insertInMap( const QModelIndex &parent, int first, int last );

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

    //! Update map theme as currently used by the model
    //! This should be triggered after a project has been loaded
    Q_INVOKABLE void updateCurrentMapTheme();

    //! Returns TRUE if the model is frozen
    bool isFrozen() const;
    //! Freezes the model as is, with any source model signals ignored
    Q_INVOKABLE void freeze();
    //! Unfreezes the model and resume listening to source model signals
    Q_INVOKABLE void unfreeze( bool resetModel = false );

    //! Sets the information if the \a nodeLayer is currently in \a tracking state
    void setLayerInTracking( QgsLayerTreeLayer *nodeLayer, bool tracking );

    //! Returns whether the current layer tree has temporal awareness
    bool isTemporal() const { return mIsTemporal; }

    //! Calculate layer tree node extent and add optional buffer
    Q_INVOKABLE QgsRectangle nodeExtent( const QModelIndex &index, QgsQuickMapSettings *mapSettings, const float buffer );

  signals:
    void mapThemeChanged();
    void isTemporalChanged();
    void isFrozenChanged();

  private:
    void featureCountChanged();
    void updateTemporalState();
    void adjustTemporalStateFromAddedLayers( const QList<QgsMapLayer *> &layers );

    QMap<QModelIndex, int> mRowMap;
    QMap<int, QModelIndex> mIndexMap;
    QMap<int, int> mTreeLevelMap;
    QList<QModelIndex> mCollapsedItems;

    QgsLayerTreeModel *mLayerTreeModel = nullptr;
    QString mMapTheme;
    QgsProject *mProject = nullptr;
    QList<QgsLayerTreeLayer *> mLayersInTracking;

    bool mIsTemporal = false;

    int mFrozen = 0;
};

/**
 * \ingroup core
 */
class FlatLayerTreeModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY( QString mapTheme READ mapTheme WRITE setMapTheme NOTIFY mapThemeChanged )
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
      GeometryLocked,
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
    };
    Q_ENUM( Roles )

    explicit FlatLayerTreeModel( QgsLayerTree *layerTree, QgsProject *project, QObject *parent = nullptr );

    Q_INVOKABLE QVariant data( const QModelIndex &index, int role ) const override;
    Q_INVOKABLE bool setData( const QModelIndex &index, const QVariant &value, int role ) override;

    QString mapTheme() const;
    void setMapTheme( const QString &mapTheme );

    bool isTemporal() const;

    //! Update map theme as currently used by the model
    //! This should be triggered after a project has been loaded
    Q_INVOKABLE void updateCurrentMapTheme();

    //! Returns TRUE if the model is frozen
    bool isFrozen() const;
    //! Freezes the model as is, with any source model signals ignored
    Q_INVOKABLE void freeze();
    //! Unfreezes the model and resume listening to source model signals
    Q_INVOKABLE void unfreeze( bool resetModel = false );

    //! Sets the information if the \a nodeLayer is currently in \a tracking state
    void setLayerInTracking( QgsLayerTreeLayer *nodeLayer, bool tracking );

    QgsProject *project() const;

    QgsLayerTreeModel *layerTreeModel() const;

    QgsLayerTree *layerTree() const;

    //! Calculate layer tree node extent
    Q_INVOKABLE QgsRectangle nodeExtent( const QModelIndex &index, QgsQuickMapSettings *mapSettings, const float buffer = 0.02 );

  signals:
    void mapThemeChanged();
    void isTemporalChanged();
    void isFrozenChanged();

  protected:
    virtual bool filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const override;

  private:
    FlatLayerTreeModelBase *mSourceModel = nullptr;
};


#endif // LAYERTREEMODEL_H
