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

class TcpUdpNmeaServer : public QObject
{
    Q_OBJECT

  public:
    void start()
    {
      // start a UDP server streaming NMEA strings (used in tst_positioning.qml)
      const QStringList arguments = QCoreApplication::arguments();
      QString nmeaServer;
      for ( int i = 0; i < arguments.size(); i++ )
      {
        if ( arguments[i] == QStringLiteral( "-input" ) )
        {
          if ( i + 1 < arguments.size() )
          {
            // the nmea server python script location, relative to the absolute input path
            nmeaServer = QString( "%1/../nmea_server" ).arg( arguments[i + 1] );
            mDataDir = QString( "%1/../testdata" ).arg( arguments[i + 1] );
          }
        }
      }
      if ( !nmeaServer.isEmpty() )
      {
        mTcpServerProcess.setProgram( QStringLiteral( "python3" ) );
        mTcpServerProcess.setArguments( QStringList() << QStringLiteral( "%1/nmeaserver.py" ).arg( nmeaServer )
                                                      << QStringLiteral( "--type" )
                                                      << QStringLiteral( "tcp" )
                                                      << QStringLiteral( "--port" )
                                                      << QStringLiteral( "11111" )
                                                      << QStringLiteral( "%1/%2" ).arg( nmeaServer, mTcpTestFile ) );
        mTcpServerProcess.start();

        mUdpServerProcess.setProgram( QStringLiteral( "python3" ) );
        mUdpServerProcess.setArguments( QStringList() << QStringLiteral( "%1/nmeaserver.py" ).arg( nmeaServer )
                                                      << QStringLiteral( "--type" )
                                                      << QStringLiteral( "udp" )
                                                      << QStringLiteral( "%1/%2" ).arg( nmeaServer, mUdpTestFile ) );
        mUdpServerProcess.start();
      }
    }

    void kill()
    {
      // kill the TCP and UDP server
      mTcpServerProcess.kill();
      mUdpServerProcess.kill();
    }

    QString dataDir() const { return mDataDir; }

  public slots:

    void setTcpTestFile( const QString &fileName )
    {
      mTcpTestFile = fileName;
    }

    void setUdpTestFile( const QString &fileName )
    {
      mUdpTestFile = fileName;
    }

  private:
    QString mDataDir;

    QProcess mTcpServerProcess;
    QString mTcpTestFile = "happy.txt";

    QProcess mUdpServerProcess;
    QString mUdpTestFile = "TrimbleR1.txt";
};

class Setup : public QObject
{
    Q_OBJECT

  private:
    TcpUdpNmeaServer mTcpUdpNmeaServer;

  public:
    Setup()
    {
      Q_INIT_RESOURCE( qml );
    }

  public slots:

    void applicationAvailable()
    {
      mTcpUdpNmeaServer.start();
    }

    void cleanupTestCase()
    {
      // kill the TCP and UDP server
      mTcpUdpNmeaServer.kill();
    }

    void qmlEngineAvailable( QQmlEngine *engine )
    {
      QQmlFileSelector *fs = new QQmlFileSelector( engine );
      QStringList selectors;
#if QT_VERSION >= QT_VERSION_CHECK( 6, 0, 0 )
      selectors << QStringLiteral( "Qt6" );
#else
      selectors << QStringLiteral( "Qt5" );
#endif
      fs->setExtraSelectors( selectors );

      qmlInit( engine );
      engine->rootContext()->setContextProperty( QStringLiteral( "tcpUdpNmeaServer" ), &mTcpUdpNmeaServer );
      engine->rootContext()->setContextProperty( QStringLiteral( "dataDir" ), mTcpUdpNmeaServer.dataDir() );

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
