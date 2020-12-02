/***************************************************************************
                            iosplatformutilities.h  -  utilities for qfield

                              -------------------
              begin                : November 2020
              copyright            : (C) 2020 by Denis Rouzaud
              email                : denis@opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef IOSPLATFORMUTILITIES_H
#define IOSPLATFORMUTILITIES_H

#include "platformutilities.h"

class IosPlatformUtilities : public PlatformUtilities
{
public:
  IosPlatformUtilities();
  QString packagePath() const override;
  bool checkPositioningPermissions() const override;
};

#endif
