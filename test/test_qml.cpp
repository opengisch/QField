/***************************************************************************
                        test_qml_editorwidgets.cpp
                        --------------------------
  begin                : Jul 2021
  copyright            : (C) 2021 by Mathieu Pellerin
  email                : mathieu@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "coordinatereferencesystemutils.h"
#include "positioning.h"
#include "qfield_qml_init.h"
#include "qgsquickcoordinatetransformer.h"

#include <QProcess>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlFileSelector>
#include <QtQuickTest>
#include <qgsfeature.h>
#include <qgsgeometry.h>
#include <qgspoint.h>

#define REGISTER_SINGLETON( uri, _class, name ) qmlRegisterSingletonType<_class>( uri, 1, 0, name, []( QQmlEngine *engine, QJSEngine *scriptEngine ) -> QObject * { Q_UNUSED(engine); Q_UNUSED(scriptEngine); return new _class(); } )

class NmeaServer : public QObject
{
    Q_OBJECT

  public:
    enum Protocol
    {
      Udp,
      Tcp,
    };

    NmeaServer( const QString &filename,
                Protocol protocol,
                int port )
      : QObject()
      , mFilename( filename )
      , mProtocol( protocol )
      , mPort( port )
    {
    }

    void start( const QString &nmeaServerLocation )
    {
      // start a UDP server streaming NMEA strings (used in tst_positioning.qml)
      if ( nmeaServerLocation.isEmpty() )
        return;

      QString type;
      switch ( mProtocol )
      {
        case Protocol::Tcp:
          type = "tcp";
          break;
        case Protocol::Udp:
          type = "udp";
          break;
      }

      mServerProcess.setProgram( QStringLiteral( "python3" ) );
      mServerProcess.setArguments( QStringList() << QStringLiteral( "%1/nmeaserver.py" ).arg( nmeaServerLocation )
                                                 << QStringLiteral( "--type" )
                                                 << type
                                                 << QStringLiteral( "--port" )
                                                 << QString::number( mPort )
                                                 << QStringLiteral( "%1/%2" ).arg( nmeaServerLocation, mFilename ) );
      mServerProcess.start();
    }

    void kill()
    {
      // kill the server process
      mServerProcess.kill();

      if ( !mServerProcess.waitForFinished() )
        qDebug() << "Waiting for processes to terminate timed out";
    }

    QString dataDir() const { return mDataDir; }

  private:
    QString mDataDir;

    QProcess mServerProcess;

    QString mFilename;
    Protocol mProtocol;
    int mPort;
};


class Setup : public QObject
{
    Q_OBJECT

  private:
    NmeaServer mNmeaServerTrimbleR1 = NmeaServer( "TrimbleR1.txt", NmeaServer::Udp, 1958 );
    NmeaServer mNmeaServerHappy = NmeaServer( "happy.txt", NmeaServer::Tcp, 11111 );
    NmeaServer mNmeaServerHappyWithIMU = NmeaServer( "happyWithIMU.txt", NmeaServer::Udp, 1959 );
    NmeaServer mNmeaServerHappyMonch2WithIMU = NmeaServer( "happyMonch2WithIMU.txt", NmeaServer::Udp, 1960 );

    QString mDataDir;

  public:
    Setup()
    {
      Q_INIT_RESOURCE( qml );
    }

  public slots:
    void applicationAvailable()
    {
      const QStringList arguments = QCoreApplication::arguments();
      QString nmeaServerLocation;
      for ( int i = 0; i < arguments.size(); i++ )
      {
        if ( arguments[i] == QStringLiteral( "-input" ) )
        {
          if ( i + 1 < arguments.size() )
          {
            // the nmea server python script, relative to the absolute input path
            nmeaServerLocation = QString( "%1/../nmea_server" ).arg( arguments[i + 1] );
            mDataDir = QString( "%1/../testdata" ).arg( arguments[i + 1] );
          }
        }
      }

      // Start tcp/udp test servers
      mNmeaServerTrimbleR1.start( nmeaServerLocation );
      mNmeaServerHappy.start( nmeaServerLocation );
      mNmeaServerHappyWithIMU.start( nmeaServerLocation );
      mNmeaServerHappyMonch2WithIMU.start( nmeaServerLocation );
    }

    void cleanupTestCase()
    {
      mNmeaServerTrimbleR1.kill();
      mNmeaServerHappy.kill();
      mNmeaServerHappyWithIMU.kill();
      mNmeaServerHappyMonch2WithIMU.kill();
    }

    void qmlEngineAvailable( QQmlEngine *engine )
    {
      qmlInit( engine );
      engine->rootContext()->setContextProperty( QStringLiteral( "dataDir" ), mDataDir );

      qRegisterMetaType<QgsGeometry>( "QgsGeometry" );
      qRegisterMetaType<QgsFeature>( "QgsFeature" );
      qRegisterMetaType<QgsPoint>( "QgsPoint" );
      qRegisterMetaType<QgsPointXY>( "QgsPointXY" );

      qmlRegisterType<QgsQuickCoordinateTransformer>( "org.qfield", 1, 0, "CoordinateTransformer" );
      qmlRegisterUncreatableType<QAbstractSocket>( "org.qfield", 1, 0, "QAbstractSocket", "" );
      qmlRegisterUncreatableType<AbstractGnssReceiver>( "org.qfield", 1, 0, "AbstractGnssReceiver", "" );
      qRegisterMetaType<GnssPositionInformation>( "GnssPositionInformation" );
      qmlRegisterType<Positioning>( "org.qfield", 1, 0, "Positioning" );

      REGISTER_SINGLETON( "org.qfield", CoordinateReferenceSystemUtils, "CoordinateReferenceSystemUtils" );
    }
};

QUICK_TEST_MAIN_WITH_SETUP( test_qml, Setup )

#include "test_qml.moc"
