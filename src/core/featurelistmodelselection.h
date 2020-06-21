/***************************************************************************
                            featurelistmodelselection.h
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

#ifndef FEATURELISTMODELSELECTION_H
#define FEATURELISTMODELSELECTION_H

#include <QObject>
#include <QItemSelectionModel>

#include "multifeaturelistmodel.h"

class FeatureListModelSelection : public QObject
{
    Q_OBJECT
    Q_PROPERTY( MultiFeatureListModel *model READ model WRITE setModel NOTIFY modelChanged )
    Q_PROPERTY( int focusedItem READ focusedItem WRITE setFocusedItem NOTIFY focusedItemChanged )
    Q_PROPERTY( QgsVectorLayer *focusedLayer READ focusedLayer NOTIFY focusedItemChanged )
    Q_PROPERTY( QgsFeature focusedFeature READ focusedFeature NOTIFY focusedItemChanged )
    Q_PROPERTY( QgsGeometry focusedGeometry READ focusedGeometry NOTIFY focusedItemChanged )
    Q_PROPERTY( QList<QgsFeature> selectedFeatures READ selectedFeatures NOTIFY selectedFeaturesChanged )

  public:
    explicit FeatureListModelSelection( QObject *parent = nullptr );

    int focusedItem() const;

    void setFocusedItem( int item );

    QList<QgsFeature> selectedFeatures() const;

    Q_INVOKABLE void toggleSelectedItem( int item );

    MultiFeatureListModel *model() const;
    void setModel( MultiFeatureListModel *model );

    QgsVectorLayer *focusedLayer() const;
    QgsFeature focusedFeature() const;
    QgsGeometry focusedGeometry() const;

  signals:
    void modelChanged();
    void focusedItemChanged();
    void selectedFeaturesChanged();

  private:
    MultiFeatureListModel *mModel = nullptr;
    QItemSelectionModel *mSelection = nullptr;
    int mFocusedItem = -1;
};

#endif // FEATURELISTMODELSELECTION_H
