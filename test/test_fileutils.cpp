/***************************************************************************
                        test_fileutils.h
                        --------------------
  begin                : Jun 2020
  copyright            : (C) 2020 by Ivan Ivanov
  email                : ivan@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "catch2.h"
#include "utils/qffileutils.h"

#include <QDir>
#include <QFileInfo>
#include <QTemporaryFile>
#include <qgsproject.h>

TEST_CASE( "FileUtils" )
{
  SECTION( "MimeTypeName" )
  {
    REQUIRE( QfFileUtils::mimeTypeName( QStringLiteral( "filename.txt" ) ) == QStringLiteral( "text/plain" ) );
  }


  SECTION( "FileName" )
  {
    REQUIRE( QfFileUtils::fileName( QStringLiteral( "filename.suffix" ) ) == QStringLiteral( "filename.suffix" ) );
    REQUIRE( QfFileUtils::fileName( QStringLiteral( "path/to/filename.suffix" ) ) == QStringLiteral( "filename.suffix" ) );
    REQUIRE( QfFileUtils::fileName( QStringLiteral( "./path/to/filename.suffix" ) ) == QStringLiteral( "filename.suffix" ) );
    REQUIRE( QfFileUtils::fileName( QStringLiteral( "../path/to/filename.suffix" ) ) == QStringLiteral( "filename.suffix" ) );
    REQUIRE( QfFileUtils::fileName( QStringLiteral( "/path/to/filename.suffix" ) ) == QStringLiteral( "filename.suffix" ) );
    REQUIRE( QfFileUtils::fileName( QStringLiteral( "filename.dbl.suffix" ) ) == QStringLiteral( "filename.dbl.suffix" ) );
    REQUIRE( QfFileUtils::fileName( QStringLiteral( "nosuffix" ) ) == QStringLiteral( "nosuffix" ) );
  }


  SECTION( "FileSuffix" )
  {
    REQUIRE( QfFileUtils::fileSuffix( QStringLiteral( "filename.suffix" ) ) == QStringLiteral( "suffix" ) );
    REQUIRE( QfFileUtils::fileSuffix( QStringLiteral( "path/to/filename.suffix" ) ) == QStringLiteral( "suffix" ) );
    REQUIRE( QfFileUtils::fileSuffix( QStringLiteral( "./path/to/filename.suffix" ) ) == QStringLiteral( "suffix" ) );
    REQUIRE( QfFileUtils::fileSuffix( QStringLiteral( "../path/to/filename.suffix" ) ) == QStringLiteral( "suffix" ) );
    REQUIRE( QfFileUtils::fileSuffix( QStringLiteral( "/path/to/filename.suffix" ) ) == QStringLiteral( "suffix" ) );
    REQUIRE( QfFileUtils::fileSuffix( QStringLiteral( "filename.dbl.suffix" ) ) == QStringLiteral( "suffix" ) );
    REQUIRE( QfFileUtils::fileSuffix( QStringLiteral( "nosuffix" ) ) == QString() );
  }


  SECTION( "FileExists" )
  {
    QTemporaryFile *f = new QTemporaryFile();

    REQUIRE( f->open() );
    QString fileName( f->fileName() );
    REQUIRE( QfFileUtils::fileExists( fileName ) );
    delete f;
    REQUIRE( !QfFileUtils::fileExists( fileName ) );
  }


  SECTION( "IsWithinProjectDirectory" )
  {
    // Create a temporary directory for the project
    QTemporaryDir tempProjectDir;
    REQUIRE( tempProjectDir.isValid() );
    QString projectPath = tempProjectDir.path() + QDir::separator() + "test_project.qgs";
    QgsProject::instance()->setFileName( projectPath );
    REQUIRE( QgsProject::instance()->write() );

    // Test with a file inside the project directory
    QString validPath = tempProjectDir.path() + QDir::separator() + "test_file.txt";
    REQUIRE( QfFileUtils::isWithinProjectDirectory( validPath ) == true );

    // Test with a file outside the project directory
    QString invalidPath = QDir::tempPath() + QDir::separator() + "outside_dir" + QDir::separator() + "test_file.txt";
    REQUIRE( QfFileUtils::isWithinProjectDirectory( invalidPath ) == false );

    QgsProject::instance()->clear();
  }


  SECTION( "ReadWriteFileContent" )
  {
    QTemporaryDir tempProjectDir;
    REQUIRE( tempProjectDir.isValid() );
    QString projectPath = tempProjectDir.path() + QDir::separator() + "test_project.qgs";
    QgsProject::instance()->setFileName( projectPath );
    REQUIRE( QgsProject::instance()->write() );

    // Create a file inside the project directory
    QString filePath = tempProjectDir.path() + QDir::separator() + "test_file.txt";
    QByteArray testData = "Test content for file operations";

    REQUIRE( QfFileUtils::writeFileContent( filePath, testData ) == true );
    REQUIRE( QfFileUtils::readFileContent( filePath ) == testData );

    // Test with a file outside the project directory
    QString outsidePath = QDir::tempPath() + QDir::separator() + "outside_dir" + QDir::separator() + "test.txt";
    REQUIRE( QfFileUtils::writeFileContent( outsidePath, testData ) == false );
    REQUIRE( QfFileUtils::readFileContent( outsidePath ).isEmpty() );

    QgsProject::instance()->clear();
  }


  SECTION( "GetFileInfo" )
  {
    QTemporaryDir tempProjectDir;
    REQUIRE( tempProjectDir.isValid() );
    QString projectPath = tempProjectDir.path() + QDir::separator() + "test_project.qgs";
    QgsProject::instance()->setFileName( projectPath );
    REQUIRE( QgsProject::instance()->write() );

    // Create a file inside the project directory
    QString filePath = tempProjectDir.path() + QDir::separator() + "test_file.txt";
    QByteArray testData = "Test content for file info";

    // Write the file directly to ensure it exists
    QFile file( filePath );
    REQUIRE( file.open( QIODevice::WriteOnly ) );
    file.write( testData );
    file.close();

    // Test with fetchContent = false ( default )
    QVariantMap info = QfFileUtils::getFileInfo( filePath );
    REQUIRE( info["exists"].toBool() == true );
    REQUIRE( info["fileName"].toString() == QFileInfo( filePath ).fileName() );
    REQUIRE( info["fileSize"].toLongLong() == testData.size() );
    REQUIRE( info.contains( "content" ) ); // Key exists but should be empty
    REQUIRE( info["content"].toByteArray().isEmpty() );

    // Test with fetchContent = true
    QVariantMap infoWithContent = QfFileUtils::getFileInfo( filePath, true );
    REQUIRE( infoWithContent["exists"].toBool() == true );
    REQUIRE( infoWithContent["fileName"].toString() == QFileInfo( filePath ).fileName() );
    REQUIRE( infoWithContent["fileSize"].toLongLong() == testData.size() );
    REQUIRE( infoWithContent["content"].toByteArray() == testData );

    // Test with a file outside the project directory
    QString outsidePath = QDir::tempPath() + QDir::separator() + "outside_dir" + QDir::separator() + "test.txt";
    QVariantMap restrictedInfo = QfFileUtils::getFileInfo( outsidePath );
    REQUIRE( restrictedInfo["exists"].toBool() == false );
    REQUIRE( restrictedInfo.contains( "error" ) );
    REQUIRE( !restrictedInfo["error"].toString().isEmpty() );

    QgsProject::instance()->clear();
  }


  SECTION( "FileUtils_SanitizeFilePath" )
  {
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "/path/to/file.txt" ) ) == QStringLiteral( "/path/to/file.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path/to/file.txt" ) ) == QStringLiteral( "path/to/file.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path/to/.bashrc" ) ) == QStringLiteral( "path/to/.bashrc" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "./path/to/file.txt" ) ) == QStringLiteral( "./path/to/file.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "../path/to/file.txt" ) ) == QStringLiteral( "../path/to/file.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "file.txt" ) ) == QStringLiteral( "file.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( ".bashrc" ) ) == QStringLiteral( ".bashrc" ) );
    // Remove leading or trailing whitespace
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( " path/to/file.txt " ) ) == QStringLiteral( "path/to/file.txt" ) );
    // no dot suffix
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "file." ) ) == QStringLiteral( "file_" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path./to./file." ) ) == QStringLiteral( "path_/to_/file_" ) );
    // Windows paths
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "C:\\path\\to\\filename.txt" ) ) == QStringLiteral( "/C/path/to/filename.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path\\to\\filename.txt" ) ) == QStringLiteral( "path/to/filename.txt" ) );
    // no wrapping whitespace
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "file " ) ) == QStringLiteral( "file" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( " file" ) ) == QStringLiteral( "file" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( " file " ) ) == QStringLiteral( "file" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path / to / file " ) ) == QStringLiteral( "path/to/file" ) );
    // single slash is enough
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "//path////to///filename.txt" ) ) == QStringLiteral( "/path/to/filename.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "\\\\\\path\\\\to\\filename.txt" ) ) == QStringLiteral( "/path/to/filename.txt" ) );
    // allow whitespace whitin the name
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path  to / file " ) ) == QStringLiteral( "path  to/file" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path  to file " ) ) == QStringLiteral( "path  to file" ) );
    // no forbidden characters
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "<path>/<to>/<filename>.txt" ) ) == QStringLiteral( "_path_/_to_/_filename_.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "\"path\"/\"to\"/\"filename\".txt" ) ) == QStringLiteral( "_path_/_to_/_filename_.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "\\path\\/\\to\\/\\filename.txt" ) ) == QStringLiteral( "/path/to/filename.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "|path|/|to|/|filename|.txt" ) ) == QStringLiteral( "_path_/_to_/_filename_.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "?path?/?to?/?filename?.txt" ) ) == QStringLiteral( "_path_/_to_/_filename_.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "*path*/*to*/*filename*.txt" ) ) == QStringLiteral( "_path_/_to_/_filename_.txt" ) );
    // no null char
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "%1path%1/%1to%1/%1filename%1.txt" ).arg( QString( QChar( '\0' ) ) ) ) == QStringLiteral( "_path_/_to_/_filename_.txt" ) );
    // no reserved words
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path/NUL/NUL" ) ) == QStringLiteral( "path/_NUL/_NUL" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path/CON/CON" ) ) == QStringLiteral( "path/_CON/_CON" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path/COM0/COM0" ) ) == QStringLiteral( "path/_COM0/_COM0" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path/LPT0/LPT0" ) ) == QStringLiteral( "path/_LPT0/_LPT0" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path/PRN/PRN" ) ) == QStringLiteral( "path/_PRN/_PRN" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path/AUX/AUX" ) ) == QStringLiteral( "path/_AUX/_AUX" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path/nul/nul" ) ) == QStringLiteral( "path/_nul/_nul" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path/con/con" ) ) == QStringLiteral( "path/_con/_con" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path/com0/com0" ) ) == QStringLiteral( "path/_com0/_com0" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path/lpt0/lpt0" ) ) == QStringLiteral( "path/_lpt0/_lpt0" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path/prn/prn" ) ) == QStringLiteral( "path/_prn/_prn" ) );
    // no reserved words with file extension either
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path/NUL.dir/NUL.txt" ) ) == QStringLiteral( "path/_NUL.dir/_NUL.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path/CON.dir/CON.txt" ) ) == QStringLiteral( "path/_CON.dir/_CON.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path/COM0.dir/COM0.txt" ) ) == QStringLiteral( "path/_COM0.dir/_COM0.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path/LPT0.dir/LPT0.txt" ) ) == QStringLiteral( "path/_LPT0.dir/_LPT0.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path/PRN.dir/PRN.txt" ) ) == QStringLiteral( "path/_PRN.dir/_PRN.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path/AUX.dir/AUX.txt" ) ) == QStringLiteral( "path/_AUX.dir/_AUX.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path/nul.dir/nul.txt" ) ) == QStringLiteral( "path/_nul.dir/_nul.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path/con.dir/con.txt" ) ) == QStringLiteral( "path/_con.dir/_con.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path/com0.dir/com0.txt" ) ) == QStringLiteral( "path/_com0.dir/_com0.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path/lpt0.dir/lpt0.txt" ) ) == QStringLiteral( "path/_lpt0.dir/_lpt0.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path/prn.dir/prn.txt" ) ) == QStringLiteral( "path/_prn.dir/_prn.txt" ) );
    // too long strings should result in empty sanitized string
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "x" ).repeated( FILENAME_MAX_CHAR_LENGTH ) ) == QStringLiteral( "x" ).repeated( FILENAME_MAX_CHAR_LENGTH ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "x" ).repeated( FILENAME_MAX_CHAR_LENGTH + 1 ) ).isEmpty() );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "x" ).repeated( FILENAME_MAX_CHAR_LENGTH - 3 ).append( QStringLiteral( "/NUL" ) ) ).isEmpty() );
    // preserve the plus sign (#6334)
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "path+to/file+name.txt" ) ) == QStringLiteral( "path+to/file+name.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePath( QStringLiteral( "file/+name.txt" ) ) == QStringLiteral( "file/+name.txt" ) );
  }


  SECTION( "FileUtils_SanitizeFilePathPart" )
  {
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "filename.txt" ) ) == QStringLiteral( "filename.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( ".bashrc" ) ) == QStringLiteral( ".bashrc" ) );
    // no forbidden characters
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "<filename>.txt" ) ) == QStringLiteral( "_filename_.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "\"filename\".txt" ) ) == QStringLiteral( "_filename_.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "\\filename/.txt" ) ) == QStringLiteral( "_filename_.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "|filename|.txt" ) ) == QStringLiteral( "_filename_.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "?filename?.txt" ) ) == QStringLiteral( "_filename_.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "*filename*.txt" ) ) == QStringLiteral( "_filename_.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "path/to/filename*.txt" ) ) == QStringLiteral( "path_to_filename_.txt" ) );
    // no dot suffix
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "file." ) ) == QStringLiteral( "file_" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "file.txt." ) ) == QStringLiteral( "file.txt_" ) );
    // no wrapping whitespace
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "file " ) ) == QStringLiteral( "file" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( " file" ) ) == QStringLiteral( "file" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( " file " ) ) == QStringLiteral( "file" ) );
    // allow whitespace whitin the name
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "path  to file " ) ) == QStringLiteral( "path  to file" ) );
    // no null char
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "%1filename%1.txt" ).arg( QString( QChar( '\0' ) ) ) ) == QStringLiteral( "_filename_.txt" ) );
    // no reserved words
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "NUL" ) ) == QStringLiteral( "_NUL" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "CON" ) ) == QStringLiteral( "_CON" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "COM0" ) ) == QStringLiteral( "_COM0" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "LPT0" ) ) == QStringLiteral( "_LPT0" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "PRN" ) ) == QStringLiteral( "_PRN" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "AUX" ) ) == QStringLiteral( "_AUX" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "nul" ) ) == QStringLiteral( "_nul" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "con" ) ) == QStringLiteral( "_con" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "com0" ) ) == QStringLiteral( "_com0" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "lpt0" ) ) == QStringLiteral( "_lpt0" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "prn" ) ) == QStringLiteral( "_prn" ) );
    // no reserved words with file extension either
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "NUL.txt" ) ) == QStringLiteral( "_NUL.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "CON.txt" ) ) == QStringLiteral( "_CON.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "COM0.txt" ) ) == QStringLiteral( "_COM0.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "LPT0.txt" ) ) == QStringLiteral( "_LPT0.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "PRN.txt" ) ) == QStringLiteral( "_PRN.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "AUX.txt" ) ) == QStringLiteral( "_AUX.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "nul.txt" ) ) == QStringLiteral( "_nul.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "con.txt" ) ) == QStringLiteral( "_con.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "com0.txt" ) ) == QStringLiteral( "_com0.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "lpt0.txt" ) ) == QStringLiteral( "_lpt0.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "prn.txt" ) ) == QStringLiteral( "_prn.txt" ) );
    // preserve the plus sign (#6334)
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "file+name.txt" ) ) == QStringLiteral( "file+name.txt" ) );
    REQUIRE( QfFileUtils::sanitizeFilePathPart( QStringLiteral( "file/+name.txt" ) ) == QStringLiteral( "file_+name.txt" ) );
  }


  SECTION( "SecurityChecks_DirectoryTraversal" )
  {
    QTemporaryDir tempProjectDir;
    REQUIRE( tempProjectDir.isValid() );
    QString projectPath = tempProjectDir.path() + QDir::separator() + "test_project.qgs";
    QgsProject::instance()->setFileName( projectPath );
    REQUIRE( QgsProject::instance()->write() );

    QDir subDir( tempProjectDir.path() + QDir::separator() + "subdir" );
    REQUIRE( subDir.mkpath( "." ) );

    REQUIRE( QfFileUtils::isWithinProjectDirectory( tempProjectDir.path() + QDir::separator() + "test_file.txt" ) == true );
    REQUIRE( QfFileUtils::isWithinProjectDirectory( subDir.path() + QDir::separator() + "test_file.txt" ) == true );

    // Directory traversal attempts
    REQUIRE( QfFileUtils::isWithinProjectDirectory( tempProjectDir.path() + QDir::separator() + ".." + QDir::separator() + "outside.txt" ) == false );
    REQUIRE( QfFileUtils::isWithinProjectDirectory( tempProjectDir.path() + QDir::separator() + "subdir" + QDir::separator() + ".." + QDir::separator() + ".." + QDir::separator() + "outside.txt" ) == false );

    QString escapePath = QDir::cleanPath( tempProjectDir.path() + QDir::separator() + ".." + QDir::separator() + "outside.txt" );
    REQUIRE( QfFileUtils::isWithinProjectDirectory( escapePath ) == false );

    QgsProject::instance()->clear();
  }


  SECTION( "SecurityChecks_SymbolicLinks" )
  {
#ifndef Q_OS_WIN
    QTemporaryDir tempProjectDir;
    REQUIRE( tempProjectDir.isValid() );
    QString projectPath = tempProjectDir.path() + QDir::separator() + "test_project.qgs";
    QgsProject::instance()->setFileName( projectPath );
    REQUIRE( QgsProject::instance()->write() );

    QTemporaryDir outsideDir;
    REQUIRE( outsideDir.isValid() );

    QString outsideFilePath = outsideDir.path() + QDir::separator() + "outside.txt";
    QFile outsideFile( outsideFilePath );
    REQUIRE( outsideFile.open( QIODevice::WriteOnly ) );
    outsideFile.write( "Outside content" );
    outsideFile.close();

    QString symlinkPath = tempProjectDir.path() + QDir::separator() + "symlink.txt";
    QFile::link( outsideFilePath, symlinkPath );

    REQUIRE( QfFileUtils::isWithinProjectDirectory( symlinkPath ) == false );

    QgsProject::instance()->clear();
#endif
  }


  SECTION( "SecurityChecks_PartialPathMatching" )
  {
    QTemporaryDir tempProjectDir;
    REQUIRE( tempProjectDir.isValid() );

    QString projectDirName = "project_abc";
    QString projectPath = tempProjectDir.path() + QDir::separator() + projectDirName;
    QDir().mkdir( projectPath );

    QString projectFilePath = projectPath + QDir::separator() + "test_project.qgs";
    QgsProject::instance()->setFileName( projectFilePath );
    REQUIRE( QgsProject::instance()->write() );

    QString similarDirName = "project_abc_similar";
    QString similarPath = tempProjectDir.path() + QDir::separator() + similarDirName;
    QDir().mkdir( similarPath );

    REQUIRE( QfFileUtils::isWithinProjectDirectory( projectPath + QDir::separator() + "valid.txt" ) == true );
    REQUIRE( QfFileUtils::isWithinProjectDirectory( similarPath + QDir::separator() + "invalid.txt" ) == false );

    QgsProject::instance()->clear();
  }


  SECTION( "SecurityChecks_CaseSensitivity" )
  {
    QTemporaryDir tempProjectDir;
    REQUIRE( tempProjectDir.isValid() );
    QString projectPath = tempProjectDir.path() + QDir::separator() + "test_project.qgs";
    QgsProject::instance()->setFileName( projectPath );
    REQUIRE( QgsProject::instance()->write() );

    QString mixedCasePath = tempProjectDir.path();
    if ( !mixedCasePath.isEmpty() )
    {
      for ( int i = 0; i < mixedCasePath.length(); i += 2 )
      {
        if ( mixedCasePath[i].isLetter() )
        {
          mixedCasePath[i] = mixedCasePath[i].toUpper();
        }
      }
    }
    mixedCasePath += QDir::separator();
    mixedCasePath += "Test_FILE.txt";

#ifdef Q_OS_WIN
    REQUIRE( QfFileUtils::isWithinProjectDirectory( mixedCasePath ) == true );
#endif

    QgsProject::instance()->clear();
  }


  SECTION( "SecurityChecks_NonExistentFiles" )
  {
    QTemporaryDir tempProjectDir;
    REQUIRE( tempProjectDir.isValid() );
    QString projectPath = tempProjectDir.path() + QDir::separator() + "test_project.qgs";
    QgsProject::instance()->setFileName( projectPath );
    REQUIRE( QgsProject::instance()->write() );

    QDir subDir( tempProjectDir.path() + QDir::separator() + "subdir" );
    REQUIRE( subDir.mkpath( "." ) );

    REQUIRE( QfFileUtils::isWithinProjectDirectory( subDir.path() + QDir::separator() + "nonexistent.txt" ) == true );
    REQUIRE( QfFileUtils::isWithinProjectDirectory( tempProjectDir.path() + QDir::separator() + "nonexistent_dir" + QDir::separator() + "file.txt" ) == true );
    REQUIRE( QfFileUtils::isWithinProjectDirectory( QDir::tempPath() + QDir::separator() + "nonexistent_outside_dir" + QDir::separator() + "file.txt" ) == false );

    QgsProject::instance()->clear();
  }


  SECTION( "SecurityChecks_EmptyProject" )
  {
    QgsProject::instance()->clear();

    REQUIRE( QfFileUtils::isWithinProjectDirectory( QDir::tempPath() + QDir::separator() + "any_file.txt" ) == false );
    REQUIRE( QfFileUtils::isWithinProjectDirectory( "/absolute/path/file.txt" ) == false );
    REQUIRE( QfFileUtils::isWithinProjectDirectory( "relative/path/file.txt" ) == false );
  }
}
