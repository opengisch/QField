/***************************************************************************
  featureutils.h - FeatureUtils

 ---------------------
 begin                : 05.03.2020
 copyright            : (C) 2020 by Denis Rouzaud
 email                : denis@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef FEATUREUTILS_H
#define FEATUREUTILS_H

#include <QObject>

#include <qgsgeometry.h>
#include <qgsfeature.h>

class QgsVectorLayer;


class FeatureUtils : public QObject
{
  Q_OBJECT
public:
  explicit FeatureUtils(QObject *parent = nullptr);

  static Q_INVOKABLE QgsFeature initFeature(QgsVectorLayer *layer, QgsGeometry geometry = QgsGeometry());

};

#endif // FEATUREUTILS_H
