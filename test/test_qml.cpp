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

#include "platformutilities.h"
#include "qfield.h"
#include "qfield_qml_init.h"
#include "qgismobileapp.h"

#include <qgis.h>
#include <qgsapplication.h>
#include <qgsproject.h>

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

      QCoreApplication::setOrganizationName( "OPENGIS.ch" );
      QCoreApplication::setOrganizationDomain( "opengis.ch" );
      QCoreApplication::setApplicationName( qfield::appName );

      QgsApplication::setPrefixPath( QGIS_PREFIX_PATH, true );

      QgsApplication::initQgis();
#ifdef RELATIVE_PREFIX_PATH
      QgsApplication::setPkgDataPath( PlatformUtilities::instance()->systemSharedDataLocation() + QStringLiteral( "/qgis" ) );
#endif
      QgsApplication::createDatabase();
    }

    void cleanupTestCase()
    {
      mNmeaServerTrimbleR1.kill();
      mNmeaServerHappy.kill();
      mNmeaServerHappyWithIMU.kill();
      mNmeaServerHappyMonch2WithIMU.kill();

      QgsApplication::exitQgis();
    }

    void qmlEngineAvailable( QQmlEngine *engine )
    {
      qmlInit( engine );

      QgisMobileapp::initDeclarative( engine );

      QgsProject::instance()->read( mDataDir + "/test_bees.qgz", Qgis::ProjectReadFlag::DontLoadProjectStyles | Qgis::ProjectReadFlag::DontLoad3DViews | Qgis::ProjectReadFlag::DontLoadLayouts );

      engine->rootContext()->setContextProperty( QStringLiteral( "ppi" ), 96 );
      engine->rootContext()->setContextProperty( QStringLiteral( "qgisProject" ), QgsProject::instance() );
      engine->rootContext()->setContextProperty( QStringLiteral( "dataDir" ), mDataDir );

      // Test credentials from environment variables (for CI/CD)
      // Set these in your environment: QFIELDCLOUD_TEST_USERNAME, QFIELDCLOUD_TEST_PASSWORD, QFIELDCLOUD_TEST_SERVER_URL
      QString testUsername = QString::fromUtf8( qgetenv( "QFIELDCLOUD_TEST_USERNAME" ) );
      QString testPassword = QString::fromUtf8( qgetenv( "QFIELDCLOUD_TEST_PASSWORD" ) );
      QString testServerUrl = QString::fromUtf8( qgetenv( "QFIELDCLOUD_TEST_SERVER_URL" ) );

      qDebug() << "=== QFieldCloud Test Credentials Debug ===";
      qDebug() << "Username length:" << testUsername.length() << "isEmpty:" << testUsername.isEmpty() << "value:" << testUsername;
      qDebug() << "Password length:" << testPassword.length() << "isEmpty:" << testPassword.isEmpty();
      qDebug() << "ServerUrl length:" << testServerUrl.length() << "isEmpty:" << testServerUrl.isEmpty() << "value:" << testServerUrl;
      qDebug() << "==========================================";

      engine->rootContext()->setContextProperty( QStringLiteral( "qfcTestUsername" ), testUsername );
      engine->rootContext()->setContextProperty( QStringLiteral( "qfcTestPassword" ), testPassword );
      engine->rootContext()->setContextProperty( QStringLiteral( "qfcTestServerUrl" ), testServerUrl );

      QgsExifTools mExifTools;
      engine->rootContext()->setContextProperty( "ExifTools", QVariant::fromValue<QgsExifTools>( mExifTools ) );

      Settings mSettings;
      engine->rootContext()->setContextProperty( "settings", &mSettings );
    }
};

QUICK_TEST_MAIN_WITH_SETUP( test_qml, Setup )

#include "test_qml.moc"
