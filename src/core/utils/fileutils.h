/***************************************************************************
 fileutils.h

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

#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QObject>

class FileUtils : public QObject
{
    Q_OBJECT

  public:
    FileUtils( QObject *parent = nullptr );
    //! Destructor
    ~FileUtils() = default;

    //! returns the mimetype of a filepath as string
    Q_INVOKABLE static QString mimeTypeName( const QString &filePath );
    //! returns the filename of a filepath - if no file name exists it's empty
    Q_INVOKABLE static QString fileName( const QString &filePath );
    //! returns the suffix (extension)
    Q_INVOKABLE static QString fileSuffix( const QString &filePath );

    //! returns true if the file exists
    Q_INVOKABLE static bool fileExists( const QString &filePath );

};

#endif // FILEUTILS_H
