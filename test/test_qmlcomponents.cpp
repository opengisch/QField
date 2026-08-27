/***************************************************************************
                        test_qmlcomponents.cpp
                        --------------------
  begin                : August 2026
  copyright            : (C) 2026 by Mohsen Dehghanzadeh
  email                : mohsen@opengis.ch
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
#include "qgismobileapp.h"

#include <QDirIterator>
#include <QQmlComponent>
#include <QQmlEngine>

TEST_CASE( "QmlComponents" )
{
  qputenv( "QT_QUICK_CONTROLS_STYLE", QByteArray( "Material" ) );

  QQmlEngine engine;
  QgisMobileapp::initDeclarative( &engine );

  SECTION( "Compile" )
  {
    const QStringList modulePaths = {
      QStringLiteral( ":/qt/qml/org/qfield/core" ),
      QStringLiteral( ":/qt/qml/org/qfield/gui" ),
      QStringLiteral( ":/qt/qml/org/qfield/app" ),
      QStringLiteral( ":/qt/qml/org/qfield/_3d" ),
      QStringLiteral( ":/qml" ),
    };

    QStringList failures;

    for ( const QString &modulePath : modulePaths )
    {
      int modulePathCount = 0;

      QDirIterator iterator( modulePath, { QStringLiteral( "*.qml" ) }, QDir::Files, QDirIterator::Subdirectories );
      while ( iterator.hasNext() )
      {
        const QString filePath = iterator.next();
        modulePathCount++;

        QQmlComponent component( &engine, QUrl( QStringLiteral( "qrc%1" ).arg( filePath ) ) );
        if ( component.status() != QQmlComponent::Ready )
        {
          failures << QStringLiteral( "%1\n    %2" ).arg( filePath, component.errorString().trimmed() );
        }
      }

      INFO( QStringLiteral( "no QML component found in %1" ).arg( modulePath ) );
      REQUIRE( modulePathCount > 0 );
    }

    INFO( QStringLiteral( "%1 components failed to compile:\n%2" ).arg( failures.size() ).arg( failures.join( QStringLiteral( "\n" ) ) ) );
    REQUIRE( failures.isEmpty() );
  }
}
