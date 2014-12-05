/***************************************************************************
                            qgismobileapp.h  -  description
                              -------------------
              begin                : Wed Apr 04 10:48:28 CET 2012
              copyright            : (C) 2012 by Marco Bernasocchi
              email                : marco@bernawebdesign.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGISMOBILEAPP_H
#define QGISMOBILEAPP_H

// Qt includes
#include <QtQuick/QQuickWindow>
#include <QtQuick/QQuickView>

// QGIS includes
#include <qgsapplication.h>
#include <qgslayertreemapcanvasbridge.h>

// QGIS mobile includes
#include "featuremodel.h"
#include "featurelistmodel.h"

class QgsQmlInterface;

class QgisMobileapp : public QQuickView
{
    Q_OBJECT
  public:
    QgisMobileapp( QgsApplication *app, QWindow* parent = 0 );
    ~QgisMobileapp();

    // interface methods
    void identifyFeatures( const QPointF& point );
    void openProjectDialog();
    void showFeatureForm( const Feature& feature );

  private slots:
    void readProject();

  private:
    void initDeclarative();

    QgsMapCanvas* mMapCanvas;
    QgsLayerTreeMapCanvasBridge* mLayerTreeCanvasBridge;
    QgsQmlInterface* mIface;
    FeatureModel mFeatureModel;
    FeatureListModel mFeatureListModel;
};

#endif // QGISMOBILEAPP_H
