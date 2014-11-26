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

#include <QtQuick/QQuickWindow>
#include <qgsapplication.h>
#include <QtQuick/QQuickView>
#include <QStandardItemModel>

#include "qgslayertreemapcanvasbridge.h"

class QgisMobileapp : public QQuickView
{
  Q_OBJECT
public:
  QgisMobileapp(QgsApplication *app, QWindow* parent = 0 );
  ~QgisMobileapp();

  void initDeclarative();

  private slots:
    void readProject();
    void openProjectDialog();

private:
  QgsMapCanvas* mMapCanvas;
  QgsLayerTreeMapCanvasBridge* mLayerTreeCanvasBridge;
  QStandardItemModel mFeatureModel;
};

#endif // QGISMOBILEAPP_H
