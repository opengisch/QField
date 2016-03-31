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

#ifndef FEATURELISTMODEL_H
#define FEATURELISTMODEL_H

#include <QAbstractItemModel>

#include <qgsmaptoolidentify.h>

#include "feature.h"

class FeatureListModel : public QAbstractItemModel
{
    Q_OBJECT

    Q_PROPERTY( int count READ count NOTIFY countChanged )

  public:
    enum FeatureListRoles
    {
      FeatureIdRole = Qt::UserRole + 1,
      FeatureRole,
      LayerNameRole,
      DeleteFeatureRole
    };

    explicit FeatureListModel( QObject *parent = 0 );
    FeatureListModel( QList<QgsMapToolIdentify::IdentifyResult> features, QObject* parent = 0 );

    /**
     * @brief setFeatures
     * @param results
     */
    void setFeatures( const QList<QgsMapToolIdentify::IdentifyResult>& results );

    /**
     * @brief setFeatures
     * @param requests
     */
    void setFeatures( const QMap<QgsVectorLayer*, QgsFeatureRequest> requests );

    QHash<int, QByteArray> roleNames() const override;
    QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const override;
    QModelIndex parent( const QModelIndex& child ) const override;
    int rowCount( const QModelIndex& parent ) const override;
    int columnCount( const QModelIndex& parent ) const override;
    QVariant data( const QModelIndex& index, int role ) const override;

    /**
     * Removes a defined number of rows starting from a given position. The parent index is not
     * used as we have a list only.
     *
     * @param row   The first row to remove
     * @param count The numbe rof rows to remove
     * @param parent Can savely be omitted as it is unused and defaults to an invalid index
     */
    virtual bool removeRows( int row, int count, const QModelIndex &parent ) override;

    int count() const;

  signals:
    void countChanged();

  private slots:
    void layerDeleted();

    void featureDeleted( QgsFeatureId fid );

  private:
    inline Feature* toFeature( const QModelIndex& index ) const
    {
      return static_cast<Feature*>( index.internalPointer() );
    }

  private:
    QList<Feature*> mFeatures;
};

#endif // FEATURELISTMODEL_H
