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

#include "qfield_core_export.h"

#include <QCryptographicHash>
#include <QObject>
#include <qgsfeedback.h>

class GnssPositionInformation;

/**
 * \ingroup core
 */
class QFIELD_CORE_EXPORT FileUtils : public QObject
{
    Q_OBJECT

  public:
    explicit FileUtils( QObject *parent = nullptr );
    ~FileUtils() = default;

    //! Returns the mimetype of a filepath as string
    Q_INVOKABLE static QString mimeTypeName( const QString &filePath );
    //! Returns TRUE if the provided mimetype is a supported image
    Q_INVOKABLE static bool isImageMimeTypeSupported( const QString &mimeType );
    //! Returns the filename of a \a filePath - if no file name exists it's empty
    Q_INVOKABLE static QString fileName( const QString &filePath );
    //! Returns true if the \a filePath exists (false if it's a directory)
    Q_INVOKABLE static bool fileExists( const QString &filePath );
    //! Returns the suffix (extension)
    Q_INVOKABLE static QString fileSuffix( const QString &filePath );
    //! Returns a human-friendly size from bytes
    Q_INVOKABLE static QString representFileSize( qint64 bytes );
    //! Returns the absolute path of tghe folder containing the \a filePath.
    Q_INVOKABLE static QString absolutePath( const QString &filePath );

    /**
     * Insures that a given image's width and height are restricted to a maximum size.
     * \param imagePath the image file path
     * \param maximumWidthHeight the maximum width and height size
     */
    Q_INVOKABLE void restrictImageSize( const QString &imagePath, int maximumWidthHeight );

    Q_INVOKABLE void addImageMetadata( const QString &imagePath, const GnssPositionInformation &positionInformation );

    Q_INVOKABLE void addImageStamp( const QString &imagePath, const QString &text );

    static bool copyRecursively( const QString &sourceFolder, const QString &destFolder, QgsFeedback *feedback = nullptr, bool wipeDestFolder = true );

    /**
     * Returns the checksum of a file. An empty QByteArray will be returned if it cannot be calculated.
     * \param fileName file name to get checksum of
     * \param hashAlgorithm hash algorithm (md5, sha1, sha256 etc)
     * \return QByteArray checksum value
     */
    Q_INVOKABLE static QByteArray fileChecksum( const QString &fileName, const QCryptographicHash::Algorithm hashAlgorithm );

    /**
     * Returns an Object Storage (S3) ETag of a file. An empty string will be returned if it cannot be calculated.
     * \param fileName file name to get checksum of
     * \param partSize maximum size used to divide the file content into parts
     * \return QString Etag value
     */
    Q_INVOKABLE static QString fileEtag( const QString &fileName, int partSize = 8 * 1024 * 1024 );

  private:
    static int copyRecursivelyPrepare( const QString &sourceFolder, const QString &destFolder, QList<QPair<QString, QString>> &mapping );
};

#endif // FILEUTILS_H
