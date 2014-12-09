/***************************************************************************
              featurelistmodelhighlight.h
               ----------------------------------------------------
              date                 : 9.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias.kuhn (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef FEATURELISTMODELHIGHLIGHT_H
#define FEATURELISTMODELHIGHLIGHT_H

#include <QObject>

#include <qgshighlight.h>

#include "featurelistmodel.h"
#include "featurelistmodelselection.h"
#include "qgsquickmapcanvasmap.h"

class FeatureListModelHighlight : public QObject
{
    Q_OBJECT

    Q_PROPERTY( FeatureListModel* model MEMBER mModel NOTIFY modelChanged )
    Q_PROPERTY( FeatureListModelSelection* selection READ selection WRITE setSelection NOTIFY selectionChanged )
    Q_PROPERTY( QColor color MEMBER mColor NOTIFY colorChanged )
    Q_PROPERTY( QColor selectionColor MEMBER mSelectionColor NOTIFY selectionColorChanged )
    Q_PROPERTY( QgsQuickMapCanvasMap* mapCanvas MEMBER mMapCanvas NOTIFY mapCanvasChanged )

  public:
    explicit FeatureListModelHighlight( QObject *parent = 0 );

    void setSelection( FeatureListModelSelection* selection );
    FeatureListModelSelection* selection() const;

  signals:
    void modelChanged();
    void selectionChanged();
    void colorChanged();
    void mapCanvasChanged();
    void selectionColorChanged();

  private slots:
    void onDataChanged();
    void onModelDataChanged();
    void onSelectionChanged();

  private:
    QgsQuickMapCanvasMap* mMapCanvas;
    QColor mColor;
    QColor mSelectionColor;
    FeatureListModel* mModel;
    FeatureListModelSelection* mSelection;
    QList<QgsHighlight*> mHighlights;
};

#endif // FEATURELISTMODELHIGHLIGHT_H
