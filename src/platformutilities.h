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

class PlatformUtilities : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString configDir READ configDir )
    Q_PROPERTY( QString shareDir READ shareDir )

  public:
    virtual ~PlatformUtilities();

    virtual QString configDir() const;
    virtual QString shareDir() const;

    /**
     * Get a picture and copy it to the requested prefix
     * @param prefix The folder where the picture should be put
     * @return The name of the picture or null
     */
    Q_INVOKABLE virtual void getPicture( const QString& prefix );

  signals:
    void pictureReceived( const QString& path );
};
#endif // PLATFORMUTILITIES_H
