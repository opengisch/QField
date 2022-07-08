/***************************************************************************
                        catch2.h
                        -----------------------
  begin                : Aug 2021
  copyright            : (C) 2021 by Matthias Kuhn
  email                : matthias@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifdef QFIELDTEST_MAIN
#define CATCH_CONFIG_RUNNER
#endif

#include <QString>
#include <QtGlobal>

#include <catch2/catch_all.hpp>

#ifdef QFIELDTEST_MAIN
#include <qgsapplication.h>
int main( int argc, char *argv[] )
{
  int ac = 0;
  char **av;
  QgsApplication app( ac, av, false );
  app.init();
  app.setPrefixPath( CMAKE_INSTALL_PREFIX, true );
  app.initQgis();
  app.setAttribute( Qt::AA_Use96Dpi, true );
  int result = Catch::Session().run( argc, argv );
  app.exitQgis();
  return result;
}
#endif

QT_BEGIN_NAMESPACE
std::ostream &operator<<( std::ostream &os, const QString &value )
{
  os << qUtf8Printable( value );
  return os;
}
QT_END_NAMESPACE
