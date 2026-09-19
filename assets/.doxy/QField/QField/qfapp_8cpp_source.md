

# File qfapp.cpp

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qfapp.cpp**](qfapp_8cpp.md)

[Go to the documentation of this file](qfapp_8cpp.md)


```C++
/***************************************************************************
    qfapp.cpp
    ---------------------
    begin                : July 2026
    copyright            : (C) 2026 by Mohsen Dehghanzadeh
    email                : mohsen@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfapp.h"
#include "qfbadlayerhandler.h"
#include "qfchangelogcontents.h"
#include "qfpluginmanager.h"
#include "qfpluginmodel.h"

#include <QtQml>

namespace QfApp
{
  void registerQmlTypes()
  {
    qmlRegisterType<QfBadLayerHandler>( "org.qfield.app", 1, 0, "QfBadLayerHandler" );
    qmlRegisterType<QfChangelogContents>( "org.qfield.app", 1, 0, "QfChangelogContents" );
    qmlRegisterType<QfPluginModel>( "org.qfield.app", 1, 0, "QfPluginModel" );
    qmlRegisterType<QfPluginProxyModel>( "org.qfield.app", 1, 0, "QfPluginProxyModel" );
    qmlRegisterUncreatableType<QfPluginManager>( "org.qfield.app", 1, 0, "QfPluginManager", "" );

    // Pre-Qf names, reached through org.qfield or the legacy Theme module.
    qmlRegisterType<QfBadLayerHandler>( "org.qfield.app", 1, 0, "BadLayerHandler" );
    qmlRegisterType<QfChangelogContents>( "org.qfield.app", 1, 0, "ChangelogContents" );
    qmlRegisterType<QfPluginModel>( "org.qfield.app", 1, 0, "PluginModel" );
    qmlRegisterType<QfPluginProxyModel>( "org.qfield.app", 1, 0, "PluginProxyModel" );
    qmlRegisterUncreatableType<QfPluginManager>( "org.qfield.app", 1, 0, "PluginManager", "" );

    // Existing plugins import the flat org.qfield module. Alias it rather than
    // registering everything twice, which would duplicate the singletons.
    qmlRegisterModule( "org.qfield", 1, 0 );
    qmlRegisterModuleImport( "org.qfield", QQmlModuleImportModuleAny, "org.qfield.core", QQmlModuleImportLatest );
    qmlRegisterModuleImport( "org.qfield", QQmlModuleImportModuleAny, "org.qfield._3d", QQmlModuleImportLatest );
    qmlRegisterModuleImport( "org.qfield", QQmlModuleImportModuleAny, "org.qfield.gui", QQmlModuleImportLatest );
    qmlRegisterModuleImport( "org.qfield", QQmlModuleImportModuleAny, "org.qfield.app", QQmlModuleImportLatest );

    // Same for QfTheme, which used to hold the Qf components now living in gui.
    qmlRegisterModule( "Theme", 1, 0 );
    qmlRegisterModuleImport( "Theme", QQmlModuleImportModuleAny, "org.qfield.gui", QQmlModuleImportLatest );
  }
} // namespace QfApp
```


