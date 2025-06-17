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
#include "utils/fileutils.h"

#include <QDir>
#include <QFileInfo>
#include <QTemporaryFile>
#include <qgsproject.h>

TEST_CASE( "FileUtils" )
{
  SECTION( "MimeTypeName" )
  {
    REQUIRE( FileUtils::mimeTypeName( QStringLiteral( "filename.txt" ) ) == QStringLiteral( "text/plain" ) );
  }


  SECTION( "FileName" )
  {
    REQUIRE( FileUtils::fileName( QStringLiteral( "filename.suffix" ) ) == QStringLiteral( "filename.suffix" ) );
    REQUIRE( FileUtils::fileName( QStringLiteral( "path/to/filename.suffix" ) ) == QStringLiteral( "filename.suffix" ) );
    REQUIRE( FileUtils::fileName( QStringLiteral( "./path/to/filename.suffix" ) ) == QStringLiteral( "filename.suffix" ) );
    REQUIRE( FileUtils::fileName( QStringLiteral( "../path/to/filename.suffix" ) ) == QStringLiteral( "filename.suffix" ) );
    REQUIRE( FileUtils::fileName( QStringLiteral( "/path/to/filename.suffix" ) ) == QStringLiteral( "filename.suffix" ) );
    REQUIRE( FileUtils::fileName( QStringLiteral( "filename.dbl.suffix" ) ) == QStringLiteral( "filename.dbl.suffix" ) );
    REQUIRE( FileUtils::fileName( QStringLiteral( "nosuffix" ) ) == QStringLiteral( "nosuffix" ) );
  }


  SECTION( "FileSuffix" )
  {
    REQUIRE( FileUtils::fileSuffix( QStringLiteral( "filename.suffix" ) ) == QStringLiteral( "suffix" ) );
    REQUIRE( FileUtils::fileSuffix( QStringLiteral( "path/to/filename.suffix" ) ) == QStringLiteral( "suffix" ) );
    REQUIRE( FileUtils::fileSuffix( QStringLiteral( "./path/to/filename.suffix" ) ) == QStringLiteral( "suffix" ) );
    REQUIRE( FileUtils::fileSuffix( QStringLiteral( "../path/to/filename.suffix" ) ) == QStringLiteral( "suffix" ) );
    REQUIRE( FileUtils::fileSuffix( QStringLiteral( "/path/to/filename.suffix" ) ) == QStringLiteral( "suffix" ) );
    REQUIRE( FileUtils::fileSuffix( QStringLiteral( "filename.dbl.suffix" ) ) == QStringLiteral( "suffix" ) );
    REQUIRE( FileUtils::fileSuffix( QStringLiteral( "nosuffix" ) ) == QString() );
  }


  SECTION( "FileExists" )
  {
    QTemporaryFile *f = new QTemporaryFile();

    REQUIRE( f->open() );
    QString fileName( f->fileName() );
    REQUIRE( FileUtils::fileExists( fileName ) );
    delete f;
    REQUIRE( !FileUtils::fileExists( fileName ) );
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
    REQUIRE( FileUtils::isWithinProjectDirectory( validPath ) == true );

    // Test with a file outside the project directory
    QString invalidPath = QDir::tempPath() + QDir::separator() + "outside_dir" + QDir::separator() + "test_file.txt";
    REQUIRE( FileUtils::isWithinProjectDirectory( invalidPath ) == false );

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

    REQUIRE( FileUtils::writeFileContent( filePath, testData ) == true );
    REQUIRE( FileUtils::readFileContent( filePath ) == testData );

    // Test with a file outside the project directory
    QString outsidePath = QDir::tempPath() + QDir::separator() + "outside_dir" + QDir::separator() + "test.txt";
    REQUIRE( FileUtils::writeFileContent( outsidePath, testData ) == false );
    REQUIRE( FileUtils::readFileContent( outsidePath ).isEmpty() );

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
    QVariantMap info = FileUtils::getFileInfo( filePath );
    REQUIRE( info["exists"].toBool() == true );
    REQUIRE( info["fileName"].toString() == QFileInfo( filePath ).fileName() );
    REQUIRE( info["fileSize"].toLongLong() == testData.size() );
    REQUIRE( info.contains( "content" ) ); // Key exists but should be empty
    REQUIRE( info["content"].toByteArray().isEmpty() );

    // Test with fetchContent = true
    QVariantMap infoWithContent = FileUtils::getFileInfo( filePath, true );
    REQUIRE( infoWithContent["exists"].toBool() == true );
    REQUIRE( infoWithContent["fileName"].toString() == QFileInfo( filePath ).fileName() );
    REQUIRE( infoWithContent["fileSize"].toLongLong() == testData.size() );
    REQUIRE( infoWithContent["content"].toByteArray() == testData );

    // Test with a file outside the project directory
    QString outsidePath = QDir::tempPath() + QDir::separator() + "outside_dir" + QDir::separator() + "test.txt";
    QVariantMap restrictedInfo = FileUtils::getFileInfo( outsidePath );
    REQUIRE( restrictedInfo["exists"].toBool() == false );
    REQUIRE( restrictedInfo.contains( "error" ) );
    REQUIRE( !restrictedInfo["error"].toString().isEmpty() );

    QgsProject::instance()->clear();
  }


  SECTION( "FileUtils_SanitizeFilePath" )
  {
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "/path/to/file.txt" ) ) == QStringLiteral( "/path/to/file.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path/to/file.txt" ) ) == QStringLiteral( "path/to/file.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path/to/.bashrc" ) ) == QStringLiteral( "path/to/.bashrc" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "./path/to/file.txt" ) ) == QStringLiteral( "./path/to/file.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "../path/to/file.txt" ) ) == QStringLiteral( "../path/to/file.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "file.txt" ) ) == QStringLiteral( "file.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( ".bashrc" ) ) == QStringLiteral( ".bashrc" ) );
    // Remove leading or trailing whitespace
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( " path/to/file.txt " ) ) == QStringLiteral( "path/to/file.txt" ) );
    // no dot suffix
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "file." ) ) == QStringLiteral( "file_" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path./to./file." ) ) == QStringLiteral( "path_/to_/file_" ) );
    // Windows paths
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "C:\\path\\to\\filename.txt" ) ) == QStringLiteral( "/C/path/to/filename.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path\\to\\filename.txt" ) ) == QStringLiteral( "path/to/filename.txt" ) );
    // no wrapping whitespace
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "file " ) ) == QStringLiteral( "file" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( " file" ) ) == QStringLiteral( "file" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( " file " ) ) == QStringLiteral( "file" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path / to / file " ) ) == QStringLiteral( "path/to/file" ) );
    // single slash is enough
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "//path////to///filename.txt" ) ) == QStringLiteral( "/path/to/filename.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "\\\\\\path\\\\to\\filename.txt" ) ) == QStringLiteral( "/path/to/filename.txt" ) );
    // allow whitespace whitin the name
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path  to / file " ) ) == QStringLiteral( "path  to/file" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path  to file " ) ) == QStringLiteral( "path  to file" ) );
    // no forbidden characters
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "<path>/<to>/<filename>.txt" ) ) == QStringLiteral( "_path_/_to_/_filename_.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "\"path\"/\"to\"/\"filename\".txt" ) ) == QStringLiteral( "_path_/_to_/_filename_.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "\\path\\/\\to\\/\\filename.txt" ) ) == QStringLiteral( "/path/to/filename.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "|path|/|to|/|filename|.txt" ) ) == QStringLiteral( "_path_/_to_/_filename_.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "?path?/?to?/?filename?.txt" ) ) == QStringLiteral( "_path_/_to_/_filename_.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "*path*/*to*/*filename*.txt" ) ) == QStringLiteral( "_path_/_to_/_filename_.txt" ) );
    // no null char
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "%1path%1/%1to%1/%1filename%1.txt" ).arg( QString( QChar( '\0' ) ) ) ) == QStringLiteral( "_path_/_to_/_filename_.txt" ) );
    // no reserved words
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path/NUL/NUL" ) ) == QStringLiteral( "path/_NUL/_NUL" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path/CON/CON" ) ) == QStringLiteral( "path/_CON/_CON" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path/COM0/COM0" ) ) == QStringLiteral( "path/_COM0/_COM0" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path/LPT0/LPT0" ) ) == QStringLiteral( "path/_LPT0/_LPT0" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path/PRN/PRN" ) ) == QStringLiteral( "path/_PRN/_PRN" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path/AUX/AUX" ) ) == QStringLiteral( "path/_AUX/_AUX" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path/nul/nul" ) ) == QStringLiteral( "path/_nul/_nul" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path/con/con" ) ) == QStringLiteral( "path/_con/_con" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path/com0/com0" ) ) == QStringLiteral( "path/_com0/_com0" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path/lpt0/lpt0" ) ) == QStringLiteral( "path/_lpt0/_lpt0" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path/prn/prn" ) ) == QStringLiteral( "path/_prn/_prn" ) );
    // no reserved words with file extension either
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path/NUL.dir/NUL.txt" ) ) == QStringLiteral( "path/_NUL.dir/_NUL.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path/CON.dir/CON.txt" ) ) == QStringLiteral( "path/_CON.dir/_CON.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path/COM0.dir/COM0.txt" ) ) == QStringLiteral( "path/_COM0.dir/_COM0.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path/LPT0.dir/LPT0.txt" ) ) == QStringLiteral( "path/_LPT0.dir/_LPT0.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path/PRN.dir/PRN.txt" ) ) == QStringLiteral( "path/_PRN.dir/_PRN.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path/AUX.dir/AUX.txt" ) ) == QStringLiteral( "path/_AUX.dir/_AUX.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path/nul.dir/nul.txt" ) ) == QStringLiteral( "path/_nul.dir/_nul.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path/con.dir/con.txt" ) ) == QStringLiteral( "path/_con.dir/_con.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path/com0.dir/com0.txt" ) ) == QStringLiteral( "path/_com0.dir/_com0.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path/lpt0.dir/lpt0.txt" ) ) == QStringLiteral( "path/_lpt0.dir/_lpt0.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path/prn.dir/prn.txt" ) ) == QStringLiteral( "path/_prn.dir/_prn.txt" ) );
    // too long strings should result in empty sanitized string
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "x" ).repeated( FILENAME_MAX_CHAR_LENGTH ) ) == QStringLiteral( "x" ).repeated( FILENAME_MAX_CHAR_LENGTH ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "x" ).repeated( FILENAME_MAX_CHAR_LENGTH + 1 ) ).isEmpty() );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "x" ).repeated( FILENAME_MAX_CHAR_LENGTH - 3 ).append( QStringLiteral( "/NUL" ) ) ).isEmpty() );
    // preserve the plus sign (#6334)
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "path+to/file+name.txt" ) ) == QStringLiteral( "path+to/file+name.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePath( QStringLiteral( "file/+name.txt" ) ) == QStringLiteral( "file/+name.txt" ) );
  }


  SECTION( "FileUtils_SanitizeFilePathPart" )
  {
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "filename.txt" ) ) == QStringLiteral( "filename.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( ".bashrc" ) ) == QStringLiteral( ".bashrc" ) );
    // no forbidden characters
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "<filename>.txt" ) ) == QStringLiteral( "_filename_.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "\"filename\".txt" ) ) == QStringLiteral( "_filename_.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "\\filename/.txt" ) ) == QStringLiteral( "_filename_.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "|filename|.txt" ) ) == QStringLiteral( "_filename_.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "?filename?.txt" ) ) == QStringLiteral( "_filename_.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "*filename*.txt" ) ) == QStringLiteral( "_filename_.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "path/to/filename*.txt" ) ) == QStringLiteral( "path_to_filename_.txt" ) );
    // no dot suffix
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "file." ) ) == QStringLiteral( "file_" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "file.txt." ) ) == QStringLiteral( "file.txt_" ) );
    // no wrapping whitespace
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "file " ) ) == QStringLiteral( "file" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( " file" ) ) == QStringLiteral( "file" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( " file " ) ) == QStringLiteral( "file" ) );
    // allow whitespace whitin the name
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "path  to file " ) ) == QStringLiteral( "path  to file" ) );
    // no null char
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "%1filename%1.txt" ).arg( QString( QChar( '\0' ) ) ) ) == QStringLiteral( "_filename_.txt" ) );
    // no reserved words
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "NUL" ) ) == QStringLiteral( "_NUL" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "CON" ) ) == QStringLiteral( "_CON" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "COM0" ) ) == QStringLiteral( "_COM0" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "LPT0" ) ) == QStringLiteral( "_LPT0" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "PRN" ) ) == QStringLiteral( "_PRN" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "AUX" ) ) == QStringLiteral( "_AUX" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "nul" ) ) == QStringLiteral( "_nul" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "con" ) ) == QStringLiteral( "_con" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "com0" ) ) == QStringLiteral( "_com0" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "lpt0" ) ) == QStringLiteral( "_lpt0" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "prn" ) ) == QStringLiteral( "_prn" ) );
    // no reserved words with file extension either
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "NUL.txt" ) ) == QStringLiteral( "_NUL.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "CON.txt" ) ) == QStringLiteral( "_CON.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "COM0.txt" ) ) == QStringLiteral( "_COM0.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "LPT0.txt" ) ) == QStringLiteral( "_LPT0.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "PRN.txt" ) ) == QStringLiteral( "_PRN.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "AUX.txt" ) ) == QStringLiteral( "_AUX.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "nul.txt" ) ) == QStringLiteral( "_nul.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "con.txt" ) ) == QStringLiteral( "_con.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "com0.txt" ) ) == QStringLiteral( "_com0.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "lpt0.txt" ) ) == QStringLiteral( "_lpt0.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "prn.txt" ) ) == QStringLiteral( "_prn.txt" ) );
    // preserve the plus sign (#6334)
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "file+name.txt" ) ) == QStringLiteral( "file+name.txt" ) );
    REQUIRE( FileUtils::sanitizeFilePathPart( QStringLiteral( "file/+name.txt" ) ) == QStringLiteral( "file_+name.txt" ) );
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

    REQUIRE( FileUtils::isWithinProjectDirectory( tempProjectDir.path() + QDir::separator() + "test_file.txt" ) == true );
    REQUIRE( FileUtils::isWithinProjectDirectory( subDir.path() + QDir::separator() + "test_file.txt" ) == true );

    // Directory traversal attempts
    REQUIRE( FileUtils::isWithinProjectDirectory( tempProjectDir.path() + QDir::separator() + ".." + QDir::separator() + "outside.txt" ) == false );
    REQUIRE( FileUtils::isWithinProjectDirectory( tempProjectDir.path() + QDir::separator() + "subdir" + QDir::separator() + ".." + QDir::separator() + ".." + QDir::separator() + "outside.txt" ) == false );

    QString escapePath = QDir::cleanPath( tempProjectDir.path() + QDir::separator() + ".." + QDir::separator() + "outside.txt" );
    REQUIRE( FileUtils::isWithinProjectDirectory( escapePath ) == false );

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

    REQUIRE( FileUtils::isWithinProjectDirectory( symlinkPath ) == false );

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

    REQUIRE( FileUtils::isWithinProjectDirectory( projectPath + QDir::separator() + "valid.txt" ) == true );
    REQUIRE( FileUtils::isWithinProjectDirectory( similarPath + QDir::separator() + "invalid.txt" ) == false );

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
    REQUIRE( FileUtils::isWithinProjectDirectory( mixedCasePath ) == true );
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

    REQUIRE( FileUtils::isWithinProjectDirectory( subDir.path() + QDir::separator() + "nonexistent.txt" ) == true );
    REQUIRE( FileUtils::isWithinProjectDirectory( tempProjectDir.path() + QDir::separator() + "nonexistent_dir" + QDir::separator() + "file.txt" ) == true );
    REQUIRE( FileUtils::isWithinProjectDirectory( QDir::tempPath() + QDir::separator() + "nonexistent_outside_dir" + QDir::separator() + "file.txt" ) == false );

    QgsProject::instance()->clear();
  }


  SECTION( "SecurityChecks_EmptyProject" )
  {
    QgsProject::instance()->clear();

    REQUIRE( FileUtils::isWithinProjectDirectory( QDir::tempPath() + QDir::separator() + "any_file.txt" ) == false );
    REQUIRE( FileUtils::isWithinProjectDirectory( "/absolute/path/file.txt" ) == false );
    REQUIRE( FileUtils::isWithinProjectDirectory( "relative/path/file.txt" ) == false );
  }
}
