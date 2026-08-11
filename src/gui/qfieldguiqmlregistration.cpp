/***************************************************************************
    qfieldguiqmlregistration.cpp
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

#include "qfattributeformmodel.h"
#include "qfexpressionevaluator.h"
#include "qfexpressionvariablemodel.h"
#include "qffeaturechecklistmodel.h"
#include "qffocusstack.h"
#include "qfgeometryeditorsmodel.h"
#include "qfieldguiqmlregistration.h"
#include "qflocalfilesmodel.h"
#include "qfmessagelogmodel.h"
#include "qforderedrelationmodel.h"
#include "qfparameterizedimage.h"
#include "qfpermissions.h"
#include "qfplatformutilities.h"
#include "qfprintlayoutlistmodel.h"
#include "qfrecentprojectlistmodel.h"
#include "qfreferencingfeaturelistmodel.h"
#include "qfsensorlistmodel.h"
#include "qftheme.h"
#include "qfvaluemapmodel.h"

#include <QGuiApplication>
#include <QPermissions>
#include <QScreen>
#include <QtQml>

#define REGISTER_SINGLETON( uri, _class, name ) qmlRegisterSingletonType<_class>( uri, 1, 0, name, []( QQmlEngine *engine, QJSEngine *scriptEngine ) -> QObject * { Q_UNUSED( engine ); Q_UNUSED( scriptEngine ); return new _class(); } )

namespace QFieldGui
{
  void registerQmlTypes()
  {
    qmlRegisterType<AttributeFormModel>( "org.qfield.gui", 1, 0, "AttributeFormModel" );
    qmlRegisterType<CameraPermission>( "org.qfield.gui", 1, 0, "QfCameraPermission" );
    qmlRegisterType<ExpressionEvaluator>( "org.qfield.gui", 1, 0, "ExpressionEvaluator" );
    qmlRegisterType<ExpressionVariableModel>( "org.qfield.gui", 1, 0, "ExpressionVariableModel" );
    qmlRegisterType<FeatureCheckListModel>( "org.qfield.gui", 1, 0, "FeatureCheckListModel" );
    qmlRegisterType<FocusStack>( "org.qfield.gui", 1, 0, "FocusStack" );
    qmlRegisterType<GeometryEditorsModel>( "org.qfield.gui", 1, 0, "GeometryEditorsModel" );
    qmlRegisterType<LocalFilesModel>( "org.qfield.gui", 1, 0, "LocalFilesModel" );
    qmlRegisterType<MicrophonePermission>( "org.qfield.gui", 1, 0, "QfMicrophonePermission" );
    qmlRegisterType<OrderedRelationModel>( "org.qfield.gui", 1, 0, "OrderedRelationModel" );
    qmlRegisterType<ParameterizedImage>( "org.qfield.gui", 1, 0, "ParameterizedImage" );
    qmlRegisterType<PrintLayoutListModel>( "org.qfield.gui", 1, 0, "PrintLayoutListModel" );
    qmlRegisterType<RecentProjectListModel>( "org.qfield.gui", 1, 0, "RecentProjectListModel" );
    qmlRegisterType<ReferencingFeatureListModel>( "org.qfield.gui", 1, 0, "ReferencingFeatureListModel" );
    qmlRegisterType<SensorListModel>( "org.qfield.gui", 1, 0, "SensorListModel" );
    qmlRegisterType<ValueMapModel>( "org.qfield.gui", 1, 0, "ValueMapModel" );

    REGISTER_SINGLETON( "org.qfield.gui", GeometryEditorsModel, "GeometryEditorsModelSingleton" );

    qmlRegisterSingletonType<Theme>( "org.qfield.gui", 1, 0, "Theme", []( QQmlEngine *, QJSEngine * ) -> QObject * {
      QScreen *screen = QGuiApplication::primaryScreen();
      const qreal dpi = screen ? screen->logicalDotsPerInch() * screen->devicePixelRatio() : 96.0;
      Theme *theme = new Theme();
      theme->setScreenPpi( dpi );
      theme->setSystemFontPointSize( PlatformUtilities::instance()->systemFontPointSize() );
      return theme;
    } );

    qmlRegisterUncreatableType<MessageLogModel>( "org.qfield.gui", 1, 0, "MessageLogModel", "The MessageLogModel is available as context property `messageLogModel`." );
  }
} // namespace QFieldGui

#undef REGISTER_SINGLETON
