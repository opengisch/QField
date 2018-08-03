/***************************************************************************

               ----------------------------------------------------
              date                 : 27.12.2014
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

#ifndef FEATURELISTEXTENTCONTROLLER_H
#define FEATURELISTEXTENTCONTROLLER_H

#include <QObject>

#include "multifeaturelistmodel.h"
#include "featurelistmodelselection.h"
#include "mapsettings.h"

class FeatureListExtentController : public QObject
{
    Q_OBJECT

    Q_PROPERTY( MultiFeatureListModel* model MEMBER mModel NOTIFY modelChanged )
    Q_PROPERTY( FeatureListModelSelection* selection MEMBER mSelection NOTIFY selectionChanged )
    Q_PROPERTY( bool autoZoom MEMBER mAutoZoom NOTIFY autoZoomChanged )
    Q_PROPERTY( MapSettings* mapSettings MEMBER mMapSettings NOTIFY mapSettingsChanged )

  public:
    FeatureListExtentController( QObject* parent = nullptr );
    ~FeatureListExtentController();

  public slots:
    void zoomToSelected() const;

  signals:
    void autoZoomChanged();
    void selectionChanged();
    void modelChanged();
    void mapSettingsChanged();

  private slots:
    void onModelChanged();
    void onCurrentSelectionChanged();

  private:
    MultiFeatureListModel* mModel;
    FeatureListModelSelection* mSelection;
    MapSettings* mMapSettings;
    bool mAutoZoom;
};

#endif // FEATURELISTEXTENTCONTROLLER_H
