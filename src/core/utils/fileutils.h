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
    //! Tries to convert the given \a filePath with path to a cross OS valid file name with path
    Q_INVOKABLE static QString sanitizeFilePath( const QString &filePath, const QString &replacement = QLatin1String( "_" ) );
    //! Tries to convert the given \a filePathPart to a cross OS valid file name part
    Q_INVOKABLE static QString sanitizeFilePathPart( const QString &filePathPart, const QString &replacement = QLatin1String( "_" ) );
    //! Returns the absolute path of the folder containing the \a filePath.
    Q_INVOKABLE static QString absolutePath( const QString &filePath );

    /**
    * Checks if a file path is securely within the current project directory.
    * Security measures:
    * - Prevents directory traversal attacks using path normalization
    * - Handles symbolic links that could escape project boundaries
    * - Supports cross-platform path comparisons (Windows, macOS, Linux, Android, iOS)
    * - Validates non-existent files safely for write operations
    * - Prevents partial directory matching by enforcing complete path containment
    *
    * \param filePath The path to check
    * \return True if the path is safely within the current project directory
    */
    Q_INVOKABLE static bool isWithinProjectDirectory( const QString &filePath );

    /**
    * Reads the entire content of a file and returns it as a byte array.
    * \param filePath The path to the file to be read
    * \return The file content as a QByteArray
    */
    Q_INVOKABLE static QByteArray readFileContent( const QString &filePath );

    /**
    * Writes content to a file.
    * \param filePath The path to the file to be written
    * \param content The content to write to the file
    * \return True if the write operation was successful, false otherwise
    *
    * \note Platform restrictions apply:
    * - On Android: Writing is only permitted within the app's internal storage or
    *   properly requested scoped storage locations.
    * - On iOS: Writing is restricted to the app's sandbox.
    * - Use PlatformUtilities.applicationDirectory() to get a safe write location.
    */
    Q_INVOKABLE static bool writeFileContent( const QString &filePath, const QByteArray &content );

    /**
    * Gets detailed information about a file including MD5 hash and metadata.
    * Optionally includes the file content when fetchContent is true.
    * This is useful for file validation, caching, and efficient file handling in QML.
    * \param filePath The path to the file
    * \param fetchContent Whether to include the file content in the returned information (default: false)
    * \return A map containing file metadata and optionally its content
    */
    Q_INVOKABLE static QVariantMap getFileInfo( const QString &filePath, bool fetchContent = false );

    /**
     * Insures that a given image's width and height are restricted to a maximum size.
     * \param imagePath the image file path
     * \param maximumWidthHeight the maximum width and height size
     */
    Q_INVOKABLE static void restrictImageSize( const QString &imagePath, int maximumWidthHeight );

    /**
     * Adds positioning metadata to a given image.
     * \param imagePath the image path
     * \param positionInformation the GNSS position information used to add metadata details
     */
    Q_INVOKABLE static void addImageMetadata( const QString &imagePath, const GnssPositionInformation &positionInformation );

    /**
     * Prints details to a given image.
     * \param imagePath the image path
     * \param text the details text
     * \param project an optional project from which custom stamping settings will be retrieved
     */
    Q_INVOKABLE static void addImageStamp( const QString &imagePath, const QString &text, const QString &textFormat = QString(), Qgis::TextHorizontalAlignment horizontalAlignment = Qgis::TextHorizontalAlignment::Left, const QString &imageDecoration = QString() );

    /**
     * Rotates an image file in-place by a clockwise angle.
     * Used to ensure saved photos match the orientation shown in the camera preview on mobile devices.
     * \param imagePath the image file path
     * \param clockwiseDegrees clockwise rotation in degrees (0/90/180/270; other values are normalized)
     * \returns TRUE on success, FALSE otherwise.
     */
    Q_INVOKABLE static bool rotateImageInPlace( const QString &imagePath, int clockwiseDegrees );

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

    /**
     * Unzip a zip file in an output directory.
     * \param zip The zip filename
     * \param dir The output directory
     * \param files The absolute path of unzipped files
     * \param checkConsistency Perform additional stricter consistency checks on the archive, and error if they fail (since QGIS 3.30)
     * \returns FALSE if the zip filename does not exist, the output directory does not exist or is not writable.
     */
    static bool unzip( const QString &zipFilename, const QString &dir, QStringList &files, bool checkConsistency );

    /**
     * Checks if a file can be safely deleted.
     * Security checks:
     * - File must be within application or cloud directories
     * - Only allows deletion of specific file types
     * - Prevents deletion of folders, project files
     * \param filePath The path to check for deletion eligibility
     * \return True if the file can be safely deleted, false otherwise
     */
    Q_INVOKABLE static bool isDeletable( const QString &filePath );

    /**
     * Deletes a list of files with security validation.
     * Only deletes files that pass isDeletable() checks and are within the project or cloud directories.
     * \param filePaths List of file paths to delete
     * \return A map with file paths as keys and boolean success status as values
     */
    Q_INVOKABLE static QVariantMap deleteFiles( const QStringList &filePaths );

  private:
    static int copyRecursivelyPrepare( const QString &sourceFolder, const QString &destFolder, QList<QPair<QString, QString>> &mapping );
};

#endif // FILEUTILS_H
