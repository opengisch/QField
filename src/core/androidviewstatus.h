/***************************************************************************
 androidviewstatus.h

 ---------------------
 begin                : 29.02.2020
 copyright            : (C) 2020 by david
 email                : david at opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef ANDROIDVIEWSTATUS_H
#define ANDROIDVIEWSTATUS_H

#include "viewstatus.h"

#include <QAndroidActivityResultReceiver>

class AndroidViewStatus : public ViewStatus, public QAndroidActivityResultReceiver
{
public:
  AndroidViewStatus();

  void handleActivityResult( int receiverRequestCode, int resultCode, const QAndroidJniObject &data ) override;
};

#endif // ANDROIDVIEWSTATUS_H
