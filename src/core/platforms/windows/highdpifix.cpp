/***************************************************************************
              highdpifix.cpp
               ----------------------------------------------------
              date                 : 18.10.2021
              copyright            : (C) 2021 by Olivier Le Doeuff
              email                : olivier.ldff@gmail.com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QtGui/QGuiApplication>
#ifdef Q_OS_WIN
#    include <Windows.h>
#endif

namespace
{

// Fix dpi at application startup
  class HighDpiFix
  {
      HighDpiFix()
      {
#ifdef Q_OS_WIN
#    if _WIN32_WINNT >= 0x0600
        ::SetProcessDPIAware();
#    endif
#endif // Q_OS_WIN

#if QT_VERSION_MAJOR < 6 // AA_EnableHighDpiScaling is enabled by default on qt6
        QGuiApplication::setAttribute( Qt::AA_EnableHighDpiScaling );
#endif
      }
      static HighDpiFix singleton;
  };

  HighDpiFix HighDpiFix::singleton;

}
