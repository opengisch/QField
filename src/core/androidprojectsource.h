/***************************************************************************
  androidprojectsource.h - AndroidProjectSource

 ---------------------
 begin                : 19.3.2018
 copyright            : (C) 2018 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef ANDROIDPROJECTSOURCE_H
#define ANDROIDPROJECTSOURCE_H

#include <QAndroidActivityResultReceiver>
#include "projectsource.h"

#include "qfieldcore_global.h"

class QFIELDCORE_EXPORT AndroidProjectSource : public ProjectSource, public QAndroidActivityResultReceiver
{
  public:
    AndroidProjectSource();

    void handleActivityResult( int receiverRequestCode, int resultCode, const QAndroidJniObject &data ) override;

};

#endif // ANDROIDPROJECTSOURCE_H
