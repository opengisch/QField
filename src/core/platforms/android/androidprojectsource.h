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

#include "projectsource.h"

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
#include <QAndroidActivityResultReceiver>
#else
#include <QtCore/private/qandroidextras_p.h>
#endif

class AndroidProjectSource : public ProjectSource, public QAndroidActivityResultReceiver
{
    Q_OBJECT

  public:
    explicit AndroidProjectSource( QObject *parent = nullptr );

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
    void handleActivityResult( int receiverRequestCode, int resultCode, const QAndroidJniObject &data ) override;
#else
    void handleActivityResult( int receiverRequestCode, int resultCode, const QJniObject &data ) override;
#endif
};

#endif // ANDROIDPROJECTSOURCE_H
