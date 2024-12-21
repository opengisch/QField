/***************************************************************************
  qFieldpositioningservice.cpp - QFieldPositioningService

 ---------------------
 begin                : 21.12.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfield_android.h"
#include "qfieldpositioningservice.h"


QFieldPositioningService::QFieldPositioningService( int &argc, char **argv )
  : QAndroidService( argc, argv )
{
  return exec();
}

QFieldPositioningService::~QFieldPositioningService()
{
}
