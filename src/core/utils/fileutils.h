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
#include <QCryptographicHash>
#include <qgsfeedback.h>

class FileUtils : public QObject
{
    Q_OBJECT

  public:
    explicit FileUtils( QObject *parent = nullptr );
    //! Destructor
    ~FileUtils() = default;

    //! returns the mimetype of a filepath as string
    Q_INVOKABLE static QString mimeTypeName( const QString &filePath );
    //! returns the filename of a filepath - if no file name exists it's empty
    Q_INVOKABLE static QString fileName( const QString &filePath );
    //! returns true if the file exists (false if it's a directory)
    Q_INVOKABLE static bool fileExists( const QString &filePath );
    //! returns the suffix (extension)
    Q_INVOKABLE static QString fileSuffix( const QString &filePath );
    static bool copyRecursively( const QString &sourceFolder, const QString &destFolder, QgsFeedback *feedback );
    /**
     * Creates checksum of a file. Returns null QByteArray if cannot be calculated.
     *
     * @param fileName file name to get checksum of
     * @param hashAlgorithm hash algorithm (md5, sha1, sha256 etc)
     * @return QByteArray checksum
     */
    Q_INVOKABLE static QByteArray fileChecksum( const QString &fileName, const QCryptographicHash::Algorithm hashAlgorithm = QCryptographicHash::Sha256 );

  private:
    static int copyRecursivelyPrepare(const QString &sourceFolder, const QString &destFolder, QList<QPair<QString, QString> > &mapping );
};

#endif // FILEUTILS_H
