/***************************************************************************
                        test_qml_test.cpp
                        --------------------
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

#include "layerutils.h"

#include <qgsfield.h>

#include <QtQuickTest>
#include <QQmlEngine>
#include <QQmlContext>

#define REGISTER_SINGLETON( uri, _class, name ) qmlRegisterSingletonType<_class>( uri, 1, 0, name, []( QQmlEngine *engine, QJSEngine *scriptEngine ) -> QObject * { Q_UNUSED(engine); Q_UNUSED(scriptEngine); return new _class(); } )

class Setup : public QObject
{
    Q_OBJECT

  public:
    Setup() {}

  public slots:
    void qmlEngineAvailable( QQmlEngine *engine )
    {
      Q_UNUSED( engine )

      REGISTER_SINGLETON( "org.qfield", LayerUtils, "LayerUtils" );

      qRegisterMetaType<QgsField>( "QgsField" );
      qRegisterMetaType<QVariant::Type>( "QVariant::Type" );
    }
};

QUICK_TEST_MAIN_WITH_SETUP( test_qml, Setup )

#include "test_qml.moc"
