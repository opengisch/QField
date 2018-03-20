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

#ifndef MAPSETTINGS_H
#define MAPSETTINGS_H

#include <QObject>

#include <qgsrectangle.h>
#include <qgsmapthemecollection.h>
#include <qgsmapsettings.h>
#include <qgsproject.h>
#include <qgspoint.h>

#include "qgsquickmapsettings.h"


class MapSettings : public QgsQuickMapSettings
{
    Q_OBJECT

  public:
    MapSettings( QObject* parent = 0 );
    ~MapSettings();
};

#endif // MAPSETTINGS_H
