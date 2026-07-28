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
#include "permissions.h"
#include "pluginmanager.h"
#include "pluginmodel.h"
#include "qfieldappqmlregistration.h"

#include <QPermissions>
#include <QtQml>

namespace QFieldApp
{
  void registerQmlTypes()
  {
    qmlRegisterType<BadLayerHandler>( "org.qfield", 1, 0, "BadLayerHandler" );
    qmlRegisterType<CameraPermission>( "org.qfield", 1, 0, "QfCameraPermission" );
    qmlRegisterType<ChangelogContents>( "org.qfield", 1, 0, "ChangelogContents" );
    qmlRegisterType<MicrophonePermission>( "org.qfield", 1, 0, "QfMicrophonePermission" );
    qmlRegisterType<PluginModel>( "org.qfield", 1, 0, "PluginModel" );
    qmlRegisterType<PluginProxyModel>( "org.qfield", 1, 0, "PluginProxyModel" );

    qmlRegisterUncreatableType<PluginManager>( "org.qfield", 1, 0, "PluginManager", "" );
  }
} // namespace QFieldApp
