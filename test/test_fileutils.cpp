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

#include <QTemporaryFile>

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
}
