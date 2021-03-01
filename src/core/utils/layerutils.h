/***************************************************************************
  layerutils.h - LayerUtils

 ---------------------
 begin                : 01.03.2021
 copyright            : (C) 2020 by Mathieu Pellerin
 email                : mathieu@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
 
#ifndef LAYERUTILS_H
#define LAYERUTILS_H

#include <QObject>

class QgsVectorLayer;
class QgsSymbol;

class LayerUtils : public QObject
{
  Q_OBJECT

public:

  explicit LayerUtils(QObject *parent = nullptr);

  /**
   * Returns the default symbol for a given layer.
   * \param layer the vector layer used to create the default symbol
   */
  static QgsSymbol *defaultSymbol( QgsVectorLayer *layer );

  /**
   * Returns TRUE if the vector layer is used as an atlas coverage layer in
   * any of the print layouts of the currently opened project.
   * \param layer the vector layer to check against print layouts
   */
  static Q_INVOKABLE bool isAtlasCoverageLayer( QgsVectorLayer *layer );
};

#endif // LAYERUTILS_H
