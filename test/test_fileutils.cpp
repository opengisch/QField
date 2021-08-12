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

#include "qfield_testbase.h"
#include "utils/fileutils.h"

#include <QTemporaryFile>
#include <gtest/gtest.h>

TEST( TestFileUtils, MimeTypeName )
{
  EXPECT_EQ( FileUtils::mimeTypeName( QStringLiteral( "filename.txt" ) ), QStringLiteral( "text/plain" ) );
}


TEST( TestFileUtils, FileName )
{
  EXPECT_EQ( FileUtils::fileName( QStringLiteral( "filename.suffix" ) ), QStringLiteral( "filename.suffix" ) );
  EXPECT_EQ( FileUtils::fileName( QStringLiteral( "path/to/filename.suffix" ) ), QStringLiteral( "filename.suffix" ) );
  EXPECT_EQ( FileUtils::fileName( QStringLiteral( "./path/to/filename.suffix" ) ), QStringLiteral( "filename.suffix" ) );
  EXPECT_EQ( FileUtils::fileName( QStringLiteral( "../path/to/filename.suffix" ) ), QStringLiteral( "filename.suffix" ) );
  EXPECT_EQ( FileUtils::fileName( QStringLiteral( "/path/to/filename.suffix" ) ), QStringLiteral( "filename.suffix" ) );
  EXPECT_EQ( FileUtils::fileName( QStringLiteral( "filename.dbl.suffix" ) ), QStringLiteral( "filename.dbl.suffix" ) );
  EXPECT_EQ( FileUtils::fileName( QStringLiteral( "nosuffix" ) ), QStringLiteral( "nosuffix" ) );
}


TEST( TestFileUtils, FileSuffix )
{
  EXPECT_EQ( FileUtils::fileSuffix( QStringLiteral( "filename.suffix" ) ), QStringLiteral( "suffix" ) );
  EXPECT_EQ( FileUtils::fileSuffix( QStringLiteral( "path/to/filename.suffix" ) ), QStringLiteral( "suffix" ) );
  EXPECT_EQ( FileUtils::fileSuffix( QStringLiteral( "./path/to/filename.suffix" ) ), QStringLiteral( "suffix" ) );
  EXPECT_EQ( FileUtils::fileSuffix( QStringLiteral( "../path/to/filename.suffix" ) ), QStringLiteral( "suffix" ) );
  EXPECT_EQ( FileUtils::fileSuffix( QStringLiteral( "/path/to/filename.suffix" ) ), QStringLiteral( "suffix" ) );
  EXPECT_EQ( FileUtils::fileSuffix( QStringLiteral( "filename.dbl.suffix" ) ), QStringLiteral( "suffix" ) );
  EXPECT_EQ( FileUtils::fileSuffix( QStringLiteral( "nosuffix" ) ), QString() );
}


TEST( TestFileUtils, FileExists )
{
  QTemporaryFile *f = new QTemporaryFile();

  EXPECT_TRUE( f->open() );
  QString fileName( f->fileName() );
  EXPECT_TRUE( FileUtils::fileExists( fileName ) );
  delete f;
  EXPECT_TRUE( !FileUtils::fileExists( fileName ) );
}
