/***************************************************************************
  featureutils.cpp - FeatureUtils

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

#include "featureutils.h"

#include <qgsvectorlayer.h>


FeatureUtils::FeatureUtils(QObject *parent) : QObject(parent)
{

}

QgsFeature FeatureUtils::initFeature(QgsVectorLayer *layer, QgsGeometry geometry)
{
  QgsFeature f(layer->fields());
  f.setGeometry(geometry);
  return f;
}
