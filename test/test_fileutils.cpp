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

#include <QtTest>

#include "qfield_testbase.h"

#include "utils/fileutils.h"


class TestFileUtils: public QObject
{
    Q_OBJECT
  private slots:


    void testMimeTypeName()
    {
      QCOMPARE( FileUtils::mimeTypeName( QStringLiteral( "filename.txt" ) ), QStringLiteral( "text/plain" ) );
    }


    void testFileName()
    {
      QCOMPARE( FileUtils::fileName( QStringLiteral( "filename.suffix" ) ), QStringLiteral( "filename.suffix" ) );
      QCOMPARE( FileUtils::fileName( QStringLiteral( "path/to/filename.suffix" ) ), QStringLiteral( "filename.suffix" ) );
      QCOMPARE( FileUtils::fileName( QStringLiteral( "./path/to/filename.suffix" ) ), QStringLiteral( "filename.suffix" ) );
      QCOMPARE( FileUtils::fileName( QStringLiteral( "../path/to/filename.suffix" ) ), QStringLiteral( "filename.suffix" ) );
      QCOMPARE( FileUtils::fileName( QStringLiteral( "/path/to/filename.suffix" ) ), QStringLiteral( "filename.suffix" ) );
      QCOMPARE( FileUtils::fileName( QStringLiteral( "filename.dbl.suffix" ) ), QStringLiteral( "filename.dbl.suffix" ) );
      QCOMPARE( FileUtils::fileName( QStringLiteral( "nosuffix" ) ), QStringLiteral( "nosuffix" ) );
    }


    void testFileSuffix()
    {
      QCOMPARE( FileUtils::fileSuffix( QStringLiteral( "filename.suffix" ) ), QStringLiteral( "suffix" ) );
      QCOMPARE( FileUtils::fileSuffix( QStringLiteral( "path/to/filename.suffix" ) ), QStringLiteral( "suffix" ) );
      QCOMPARE( FileUtils::fileSuffix( QStringLiteral( "./path/to/filename.suffix" ) ), QStringLiteral( "suffix" ) );
      QCOMPARE( FileUtils::fileSuffix( QStringLiteral( "../path/to/filename.suffix" ) ), QStringLiteral( "suffix" ) );
      QCOMPARE( FileUtils::fileSuffix( QStringLiteral( "/path/to/filename.suffix" ) ), QStringLiteral( "suffix" ) );
      QCOMPARE( FileUtils::fileSuffix( QStringLiteral( "filename.dbl.suffix" ) ), QStringLiteral( "suffix" ) );
      QCOMPARE( FileUtils::fileSuffix( QStringLiteral( "nosuffix" ) ), QString() );
    }


    void testFileExists()
    {
      QTemporaryFile *f = new QTemporaryFile();

      QVERIFY( f->open() );
      QString fileName( f->fileName() );
      QVERIFY( FileUtils::fileExists( fileName ) );
      delete f;
      QVERIFY( ! FileUtils::fileExists( fileName ) );
    }
};

QFIELDTEST_MAIN( TestFileUtils )
#include "test_fileutils.moc"
