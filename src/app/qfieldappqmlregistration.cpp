/***************************************************************************
    qfieldappqmlregistration.cpp
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

#include "badlayerhandler.h"
#include "changelogcontents.h"
#include "pluginmanager.h"
#include "pluginmodel.h"
#include "qfieldappqmlregistration.h"

#include <QtQml>

namespace QFieldApp
{
  void registerQmlTypes()
  {
    qmlRegisterType<BadLayerHandler>( "org.qfield.app", 1, 0, "BadLayerHandler" );
    qmlRegisterType<ChangelogContents>( "org.qfield.app", 1, 0, "ChangelogContents" );
    qmlRegisterType<PluginModel>( "org.qfield.app", 1, 0, "PluginModel" );
    qmlRegisterType<PluginProxyModel>( "org.qfield.app", 1, 0, "PluginProxyModel" );

    qmlRegisterUncreatableType<PluginManager>( "org.qfield.app", 1, 0, "PluginManager", "" );

    // Existing plugins import the flat org.qfield module. Alias it rather than
    // registering everything twice, which would duplicate the singletons.
    qmlRegisterModule( "org.qfield", 1, 0 );
    qmlRegisterModuleImport( "org.qfield", QQmlModuleImportModuleAny, "org.qfield.core", QQmlModuleImportLatest );
    qmlRegisterModuleImport( "org.qfield", QQmlModuleImportModuleAny, "org.qfield.gui", QQmlModuleImportLatest );
    qmlRegisterModuleImport( "org.qfield", QQmlModuleImportModuleAny, "org.qfield.app", QQmlModuleImportLatest );

    // Same for Theme, which used to hold the Qf components now living in gui.
    qmlRegisterModule( "Theme", 1, 0 );
    qmlRegisterModuleImport( "Theme", QQmlModuleImportModuleAny, "org.qfield.gui", QQmlModuleImportLatest );
  }
} // namespace QFieldApp
