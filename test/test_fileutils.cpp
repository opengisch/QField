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