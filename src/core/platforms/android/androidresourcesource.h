/***************************************************************************
  androidresourcesource.h - AndroidResourceSource

 ---------------------
 begin                : 5.7.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef ANDROIDRESOURCESOURCE_H
#define ANDROIDRESOURCESOURCE_H

#include "resourcesource.h"

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
#include <QAndroidActivityResultReceiver>
#else
#include <QtCore/private/qandroidextras_p.h>
#endif

class AndroidResourceSource : public ResourceSource, public QAndroidActivityResultReceiver
{
    Q_OBJECT

  public:
    /**
     * Construct a new AndroidResourceSource object
     *
     * @param prefix The project folder. Base directory path for all relative paths.
     */
    explicit AndroidResourceSource( const QString &prefix );

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
    void handleActivityResult( int receiverRequestCode, int resultCode, const QAndroidJniObject &data ) override;
#else
    void handleActivityResult( int receiverRequestCode, int resultCode, const QJniObject &data ) override;
#endif

  private:
    /**
     * Base directory path for all relative paths.
     */
    QString mPrefix;
};

#endif // ANDROIDRESOURCESOURCE_H
