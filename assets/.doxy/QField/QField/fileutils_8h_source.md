

# File fileutils.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**fileutils.h**](fileutils_8h.md)

[Go to the documentation of this file](fileutils_8h.md)


```C++
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
#include <QVariantMap>
#include <qgis.h>
#include <qgsfeedback.h>

#define FILENAME_MAX_CHAR_LENGTH 255

class GnssPositionInformation;

class QFIELD_CORE_EXPORT FileUtils : public QObject
{
    Q_OBJECT

  public:
    explicit FileUtils( QObject *parent = nullptr );
    ~FileUtils() = default;

    Q_INVOKABLE static QString mimeTypeName( const QString &filePath );
    Q_INVOKABLE static bool isImageMimeTypeSupported( const QString &mimeType );
    Q_INVOKABLE static QString fileName( const QString &filePath );
    Q_INVOKABLE static bool fileExists( const QString &filePath );
    Q_INVOKABLE static QString fileSuffix( const QString &filePath );
    Q_INVOKABLE static QString representFileSize( qint64 bytes );
    Q_INVOKABLE static QString sanitizeFilePath( const QString &filePath, const QString &replacement = QLatin1String( "_" ) );
    Q_INVOKABLE static QString sanitizeFilePathPart( const QString &filePathPart, const QString &replacement = QLatin1String( "_" ) );
    Q_INVOKABLE static QString absolutePath( const QString &filePath );

    Q_INVOKABLE static bool isWithinProjectDirectory( const QString &filePath );

    Q_INVOKABLE static QByteArray readFileContent( const QString &filePath );

    Q_INVOKABLE static bool writeFileContent( const QString &filePath, const QByteArray &content );

    Q_INVOKABLE static QVariantMap getFileInfo( const QString &filePath, bool fetchContent = false );

    Q_INVOKABLE static void restrictImageSize( const QString &imagePath, int maximumWidthHeight );

    Q_INVOKABLE static void addImageMetadata( const QString &imagePath, const GnssPositionInformation &positionInformation );

    Q_INVOKABLE static void addImageStamp( const QString &imagePath, const QString &text, const QString &textFormat = QString(), Qgis::TextHorizontalAlignment horizontalAlignment = Qgis::TextHorizontalAlignment::Left, const QString &imageDecoration = QString() );

    static bool copyRecursively( const QString &sourceFolder, const QString &destFolder, QgsFeedback *feedback = nullptr, bool wipeDestFolder = true );

    Q_INVOKABLE static QByteArray fileChecksum( const QString &fileName, const QCryptographicHash::Algorithm hashAlgorithm );

    Q_INVOKABLE static QString fileEtag( const QString &fileName, int partSize = 8 * 1024 * 1024 );

    static bool unzip( const QString &zipFilename, const QString &dir, QStringList &files, bool checkConsistency );

    Q_INVOKABLE static bool isDeletable( const QString &filePath );

    Q_INVOKABLE static QVariantMap deleteFiles( const QStringList &filePaths );

  private:
    static int copyRecursivelyPrepare( const QString &sourceFolder, const QString &destFolder, QList<QPair<QString, QString>> &mapping );
};

#endif // FILEUTILS_H
```


