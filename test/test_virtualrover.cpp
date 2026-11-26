/***************************************************************************
                        test_virtualrover.cpp
                        ---------------------
  begin                : Nov 2026
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#define QFIELDTEST_MAIN
#include "catch2.h"
#include "positioning/qfpositioningsource.h"

#include <QElapsedTimer>
#include <QProcess>
#include <QTest>

static void testVirtualRover( bool forwardPosition, int expectedQuality )
{
  QProcess rover;
  rover.start( QStringLiteral( "python3" ),
               { QStringLiteral( TEST_DATA_DIR "/../virtual_rover/virtual_rover.py" ),
                 QStringLiteral( TEST_DATA_DIR "/../virtual_rover/rtklib-rover-base.tar.gz" ),
                 forwardPosition ? QStringLiteral( "on" ) : QStringLiteral( "off" ),
                 expectedQuality == 5 ? QStringLiteral( "float" ) : QStringLiteral( "fixed" ) } );
  REQUIRE( rover.waitForStarted() );
  REQUIRE( rover.waitForReadyRead( 5000 ) );
  const QStringList ports = QString::fromUtf8( rover.readLine() ).trimmed().split( ' ' );
  REQUIRE( ports.size() == 2 );

  QfPositioningSource source;
  source.setDeviceId( QStringLiteral( "tcp:127.0.0.1:%1" ).arg( ports.at( 0 ) ) );
  source.setActive( true );

  QElapsedTimer timer;
  timer.start();
  while ( source.deviceSocketState() != QAbstractSocket::ConnectedState && timer.elapsed() < 5000 )
    QTest::qWait( 20 );
  REQUIRE( source.deviceSocketState() == QAbstractSocket::ConnectedState );
  QTest::qWait( 250 );
  REQUIRE( source.ntripBytesReceived() == 0 );
  REQUIRE( source.positionInformation().quality() != expectedQuality );

  QfNtripSettings settings;
  settings.setHost( QStringLiteral( "127.0.0.1" ) );
  settings.setPort( ports.at( 1 ).toInt() );
  settings.setMountPoint( QStringLiteral( "RTK" ) );
  settings.setUsername( QStringLiteral( "rover" ) );
  settings.setPassword( QStringLiteral( "test" ) );
  settings.setForwardNmeaSentences( forwardPosition );
  source.setNtripSettings( settings );
  source.setEnableNtrip( true );

  timer.restart();
  while ( source.positionInformation().quality() != expectedQuality && timer.elapsed() < 30000 )
    QTest::qWait( 20 );

  INFO( "NTRIP error: " << source.ntripLastError() );
  INFO( "Virtual rover: " << QString::fromUtf8( rover.readAllStandardError() ) );
  REQUIRE( source.ntripBytesReceived() > 0 );
  REQUIRE( source.positionInformation().quality() == expectedQuality );
  if ( rover.state() != QProcess::NotRunning )
    REQUIRE( rover.waitForFinished( 5000 ) );
  INFO( "Virtual rover: " << QString::fromUtf8( rover.readAllStandardError() ) );
  REQUIRE( rover.exitStatus() == QProcess::NormalExit );
  REQUIRE( rover.exitCode() == 0 );
  source.setActive( false );
}

TEST_CASE( "NTRIP corrections produce a fixed RTK position" )
{
  SECTION( "Forward position to caster" )
  {
    testVirtualRover( true, 4 );
  }

  SECTION( "Do not forward position to caster" )
  {
    testVirtualRover( false, 4 );
  }
}

TEST_CASE( "NTRIP corrections produce a float RTK position" )
{
  SECTION( "Forward position to caster" )
  {
    testVirtualRover( true, 5 );
  }

  SECTION( "Do not forward position to caster" )
  {
    testVirtualRover( false, 5 );
  }
}
