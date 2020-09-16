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

class FlatLayerTreeModel : public QAbstractProxyModel
{
    Q_OBJECT

    Q_PROPERTY( QString mapTheme READ mapTheme WRITE setMapTheme NOTIFY mapThemeChanged )

  public:
    enum Roles
    {
      VectorLayerPointer = Qt::UserRole + 1,
      MapLayerPointer,
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
      IsCollapsed,
      IsParentCollapsed
    };
    Q_ENUM( Roles )

    explicit FlatLayerTreeModel( QgsLayerTree *layerTree, QgsProject *project, QObject *parent = nullptr );

    void updateMap( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles );
    int buildMap( QgsLayerTreeModel *model, const QModelIndex &parent = QModelIndex(), int row = 0, int treeLevel = 0 );

    void setSourceModel( QgsLayerTreeModel *sourceModel );
    QModelIndex mapToSource( const QModelIndex &proxyIndex ) const override;
    QModelIndex mapFromSource( const QModelIndex &sourceIndex ) const override;

    QModelIndex parent( const QModelIndex &child ) const override;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const override;
    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;

    QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const override;

    Q_INVOKABLE QVariant data( const QModelIndex &index, int role ) const override;
    Q_INVOKABLE bool setData( const QModelIndex &index, const QVariant &value, int role ) override;

    QHash<int, QByteArray> roleNames() const override;

    QgsProject *project() const;

    QgsLayerTreeModel *layerTreeModel() const;

    QgsLayerTree *layerTree() const;

    QString mapTheme() const;
    void setMapTheme( const QString &mapTheme );

    //! Update map theme as currently used by the model
    //! This should be triggered after a project has been loaded
    Q_INVOKABLE void updateCurrentMapTheme();

    //! Sets the information if the \a nodeLayer is currently in \a tracking state
    void setLayerInTracking( QgsLayerTreeLayer *nodeLayer, bool tracking );

  signals:
    void mapThemeChanged();

  private:
    QMap<QModelIndex, int> mRowMap;
    QMap<int, QModelIndex> mIndexMap;
    QMap<int, int> mTreeLevelMap;
    QList<QModelIndex> mCollapsedItems;

    QgsLayerTreeModel *mLayerTreeModel = nullptr;
    QString mMapTheme;
    QgsProject *mProject = nullptr;
    QList<QgsLayerTreeLayer *> mLayersInTracking;
};

#endif // LAYERTREEMODEL_H
