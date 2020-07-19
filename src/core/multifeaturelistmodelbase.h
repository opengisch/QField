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

#include <QAbstractItemModel>

#include <qgsfeaturerequest.h>

#include "identifytool.h"

class MultiFeatureListModelBase : public QAbstractItemModel
{
    Q_OBJECT

  public:

    explicit MultiFeatureListModelBase( QObject *parent = nullptr );

    /**
     * Resets the model to contain features found from a list of \a requests.
     */
    void setFeatures( const QMap<QgsVectorLayer *, QgsFeatureRequest> requests );

    /**
     * Appends features from a list of \a results.
     */
    void appendFeatures( const QList<IdentifyTool::IdentifyResult> &results );

    /**
     * Resets the model to either an empty feature list or one that contains only the selected features.
     * \param keepSelected if set to TRUE, selected features will be kept when resetting the model.
     */
    void clear( const bool keepSelected = false );

    /**
     * Empties the list of selected features.
     */
    void clearSelection();

    QString featureDisplayName( QgsVectorLayer *vlayer, QgsFeature feature );

    QHash<int, QByteArray> roleNames() const override;
    QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const override;
    QModelIndex parent( const QModelIndex &child ) const override;
    int rowCount( const QModelIndex &parent ) const override;
    int columnCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;

    /**
     * Removes a defined number of rows starting from a given position. The parent index is not
     * used as we have a list only.
     *
     * @param row   The first row to remove
     * @param count The numbe rof rows to remove
     * @param parent Can savely be omitted as it is unused and defaults to an invalid index
     */
    virtual bool removeRows( int row, int count, const QModelIndex &parent ) override;

    /**
     * Returns the number of features in the model.
     */
    int count() const;

    /**
     * Returns the number of selected features in the model.
     */
    int selectedCount() const;

    bool canEditAttributesSelection();

    bool canMergeSelection();

    bool canDeleteSelection();

    bool mergeSelection();

    bool deleteFeature( QgsVectorLayer *layer, QgsFeatureId fid, bool selectionAction = false );

    bool deleteSelection();

    void toggleSelectedItem( int item );

    QList<QgsFeature> selectedFeatures();

  signals:

    void countChanged();

    void selectedCountChanged();

  private slots:

    void layerDeleted( QObject *object );

    void featureDeleted( QgsFeatureId fid );

    void attributeValueChanged( QgsFeatureId fid, int idx, const QVariant &value );

  private:

    inline QPair< QgsVectorLayer *, QgsFeature > *toFeature( const QModelIndex &index ) const
    {
      return static_cast<QPair< QgsVectorLayer *, QgsFeature >*>( index.internalPointer() );
    }

    QList< QPair< QgsVectorLayer *, QgsFeature > > mFeatures;
    QList< QPair< QgsVectorLayer *, QgsFeature > > mSelectedFeatures;
};

#endif // MULTIFEATURELISTMODELBASE_H
