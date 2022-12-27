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

#include "positioning.h"
#include "qfield_qml_init.h"

#include <QProcess>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlFileSelector>
#include <QtQuickTest>

#define REGISTER_SINGLETON( uri, _class, name ) qmlRegisterSingletonType<_class>( uri, 1, 0, name, []( QQmlEngine *engine, QJSEngine *scriptEngine ) -> QObject * { Q_UNUSED(engine); Q_UNUSED(scriptEngine); return new _class(); } )

class Setup : public QObject
{
    Q_OBJECT

  private:
    QProcess mProcess;

  public:
    Setup()
    {
      Q_INIT_RESOURCE( qml );
    }

  public slots:
    void applicationAvailable()
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
            // the nmea server python script, relative to the absolute input path
            nmeaServer = QString( "%1/../nmea_server" ).arg( arguments[i + 1] );
          }
        }
      }
      if ( !nmeaServer.isEmpty() )
      {
        mProcess.setProgram( QStringLiteral( "python3" ) );
        mProcess.setArguments( QStringList() << QStringLiteral( "%1/nmeaserver.py" ).arg( nmeaServer )
                                             << QStringLiteral( "--type" )
                                             << QStringLiteral( "udp" )
                                             << QStringLiteral( "%1/TrimbleR1.txt" ).arg( nmeaServer ) );
        mProcess.start();
      }
    }

    void cleanupTestCase()
    {
      // kill the UDP server
      mProcess.kill();
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
      qmlRegisterUncreatableType<QAbstractSocket>( "org.qfield", 1, 0, "QAbstractSocket", "" );
      qmlRegisterUncreatableType<AbstractGnssReceiver>( "org.qfield", 1, 0, "AbstractGnssReceiver", "" );
      qRegisterMetaType<GnssPositionInformation>( "GnssPositionInformation" );
      qmlRegisterType<Positioning>( "org.qfield", 1, 0, "Positioning" );
    }
};

QUICK_TEST_MAIN_WITH_SETUP( test_qml, Setup )

#include "test_qml.moc"
