/***************************************************************************
  iospicturesource.h - IosPictureSource

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
#ifndef IOSPICTURESOURCE_H
#define IOSPICTURESOURCE_H

#include "picturesource.h"


class IosPictureSource : public PictureSource
{
    Q_OBJECT

  public:
    /**
     * Construct a new Ios Picture Source object
     *
     * @param prefix The project folder. Base directory path for all relative paths.
     */
    explicit IosPictureSource( const QString &prefix );

    void handleActivityResult( int receiverRequestCode, int resultCode );

  private:
    /**
     * Base directory path for all relative paths.
     */
    QString mPrefix;
};

#endif // IOSPICTURESOURCE_H
