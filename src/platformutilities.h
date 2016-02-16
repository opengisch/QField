/***************************************************************************
                            platformutilities.h  -  utilities for qfield

                              -------------------
              begin                : Wed Dec 04 10:48:28 CET 2015
              copyright            : (C) 2015 by Marco Bernasocchi
              email                : marco@opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PLATFORMUTILITIES_H
#define PLATFORMUTILITIES_H

#include <QObject>
#include <QAndroidJniObject>


class PlatformUtilities: public QObject
{
    Q_OBJECT

  public:
    explicit PlatformUtilities( QObject *parent = 0 );
    QMap<QString, QString> getIntentExtras( QStringList );
    QString getIntentExtra( QString, QAndroidJniObject=0 );
    QAndroidJniObject getNativeIntent();
    QAndroidJniObject getNativeExtras();
};
#endif // PLATFORMUTILITIES_H

