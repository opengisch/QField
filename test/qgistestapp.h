/***************************************************************************

               ----------------------------------------------------
              date                 : 4.1.2015
              copyright            : (C) 2015 by Matthias Kuhn
              email                : matthias.kuhn (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGISTESTAPP
#define QGISTESTAPP

#include <qgsapplication.h>

class QgisTestApp
{
  public:
    QgisTestApp()
    {
      int argc = 0;
      char **argv = 0;
      mApp = new QgsApplication( argc, argv, false );

      // load providers
#if defined(Q_WS_WIN)
      QString prefixPath = QApplication::applicationDirPath();
#else
      QString prefixPath = QApplication::applicationDirPath() + "/..";
#endif

      mApp->setPrefixPath( prefixPath, true );
#ifdef ANDROID
      mApp->setPluginPath( QApplication::applicationDirPath() );
#else
      mApp->setPluginPath( "" QGIS_PLUGIN_DIR );
#endif
      mApp->initQgis();
    }

    ~QgisTestApp()
    {
#if 0
      // Crashes...
      delete mApp;
#endif
    }

  private:
    QgsApplication *mApp;
};

#endif // QGISTESTAPP

