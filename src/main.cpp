/***************************************************************************
                            main.cpp  -  main for qgis mobileapp
                            based on src/browser/main.cpp
                              -------------------
              begin                : Wed Apr 04 10:48:28 CET 2012
              copyright            : (C) 2012 by Marco Bernasocchi
              email                : marco@bernawebdesign.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgismobileapp.h"

#include <QLocale>
#include <QSettings>
#include <QTranslator>
#include <QMainWindow>
#include <QLabel>
#include <QDialog>
#include <QApplication>
#include "qgsapplication.h"
#include "qgslogger.h"
#include <qmainwindow.h>
#include <QApplication>

int main( int argc, char ** argv )
{
  QSettings settings;

  QgsApplication app( argc, argv, true );
  app.setThemeName( settings.value( "/Themes", "default" ).toString() );

  // load providers
#if defined(Q_WS_WIN)
  QString prefixPath = QApplication::applicationDirPath();
#else
  QString prefixPath = QApplication::applicationDirPath() + "/..";
#endif
  app.setPrefixPath( prefixPath, true );
#ifdef ANDROID
  app.setPluginPath( QApplication::applicationDirPath() );
#else
  app.setPluginPath( "" QGIS_PLUGIN_DIR );
#endif
  app.initQgis();

  // Set up the QSettings environment must be done after qapp is created
  QCoreApplication::setOrganizationName( "QGIS" );
  QCoreApplication::setOrganizationDomain( "qgis.org" );
  QCoreApplication::setApplicationName( "QGIS" );

  QgisMobileapp mApp(&app);
  return app.exec();
}
