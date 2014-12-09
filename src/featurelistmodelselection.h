/***************************************************************************
                            featurelistmodelselection.h  -  description
                              -------------------
              begin                : 12/9/2014
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

#include "featurelistmodel.h"

class FeatureListModelSelection : public QObject
{
    Q_OBJECT
    Q_PROPERTY( FeatureListModel* model READ model WRITE setModel NOTIFY modelChanged )
    Q_PROPERTY( int selection READ selection WRITE setSelection NOTIFY selectionChanged )
    Q_PROPERTY( QVariant selectedFeature READ selectedFeature NOTIFY selectionChanged )

  public:
    explicit FeatureListModelSelection( QObject *parent = 0 );

    int selection();
    void setSelection( int selection );

    FeatureListModel* model() const;
    void setModel( FeatureListModel* model );

    const QVariant selectedFeature() const;

  signals:
    void modelChanged();
    void selectionChanged();

  private:
    FeatureListModel* mModel;
    QItemSelectionModel* mSelection;
};

#endif // FEATURELISTMODELSELECTION_H
