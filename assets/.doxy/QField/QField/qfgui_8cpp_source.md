

# File qfgui.cpp

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qfgui.cpp**](qfgui_8cpp.md)

[Go to the documentation of this file](qfgui_8cpp.md)


```C++
/***************************************************************************
    qfgui.cpp
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
#include "qfgui.h"
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

namespace QfGui
{
  void registerQmlTypes()
  {
    qmlRegisterType<QfAttributeFormModel>( "org.qfield.gui", 1, 0, "QfAttributeFormModel" );
    qmlRegisterType<QfCameraPermission>( "org.qfield.gui", 1, 0, "QfCameraPermission" );
    qmlRegisterType<QfExpressionEvaluator>( "org.qfield.gui", 1, 0, "QfExpressionEvaluator" );
    qmlRegisterType<QfExpressionVariableModel>( "org.qfield.gui", 1, 0, "QfExpressionVariableModel" );
    qmlRegisterType<QfFeatureCheckListModel>( "org.qfield.gui", 1, 0, "QfFeatureCheckListModel" );
    qmlRegisterType<QfFocusStack>( "org.qfield.gui", 1, 0, "QfFocusStack" );
    qmlRegisterType<QfGeometryEditorsModel>( "org.qfield.gui", 1, 0, "QfGeometryEditorsModel" );
    qmlRegisterType<QfLocalFilesModel>( "org.qfield.gui", 1, 0, "QfLocalFilesModel" );
    qmlRegisterType<QfMicrophonePermission>( "org.qfield.gui", 1, 0, "QfMicrophonePermission" );
    qmlRegisterType<QfOrderedRelationModel>( "org.qfield.gui", 1, 0, "QfOrderedRelationModel" );
    qmlRegisterType<QfParameterizedImage>( "org.qfield.gui", 1, 0, "QfParameterizedImage" );
    qmlRegisterType<QfPrintLayoutListModel>( "org.qfield.gui", 1, 0, "QfPrintLayoutListModel" );
    qmlRegisterType<QfRecentProjectListModel>( "org.qfield.gui", 1, 0, "QfRecentProjectListModel" );
    qmlRegisterType<QfReferencingFeatureListModel>( "org.qfield.gui", 1, 0, "QfReferencingFeatureListModel" );
    qmlRegisterType<QfSensorListModel>( "org.qfield.gui", 1, 0, "QfSensorListModel" );
    qmlRegisterType<QfValueMapModel>( "org.qfield.gui", 1, 0, "QfValueMapModel" );

    REGISTER_SINGLETON( "org.qfield.gui", QfGeometryEditorsModel, "QfGeometryEditorsModelSingleton" );

    // One instance behind every name it is registered under, so nobody is left on a stale palette.
    const auto themeInstance = []( QQmlEngine *, QJSEngine * ) -> QObject * {
      static QfTheme *sTheme = []() {
        QScreen *screen = QGuiApplication::primaryScreen();
        const qreal dpi = screen ? screen->logicalDotsPerInch() * screen->devicePixelRatio() : 96.0;
        QfTheme *theme = new QfTheme();
        theme->setScreenPpi( dpi );
        theme->setSystemFontPointSize( QfPlatformUtilities::instance()->systemFontPointSize() );
        return theme;
      }();
      QQmlEngine::setObjectOwnership( sTheme, QQmlEngine::CppOwnership );
      return sTheme;
    };

    qmlRegisterSingletonType<QfTheme>( "org.qfield.gui", 1, 0, "QfTheme", themeInstance );

    qmlRegisterUncreatableType<QfMessageLogModel>( "org.qfield.gui", 1, 0, "QfMessageLogModel", "The MessageLogModel is available as context property `messageLogModel`." );

    // Pre-Qf names, reached through org.qfield or the legacy Theme module.
    qmlRegisterType<QfAttributeFormModel>( "org.qfield.gui", 1, 0, "AttributeFormModel" );
    qmlRegisterType<QfExpressionEvaluator>( "org.qfield.gui", 1, 0, "ExpressionEvaluator" );
    qmlRegisterType<QfExpressionVariableModel>( "org.qfield.gui", 1, 0, "ExpressionVariableModel" );
    qmlRegisterType<QfFeatureCheckListModel>( "org.qfield.gui", 1, 0, "FeatureCheckListModel" );
    qmlRegisterType<QfFocusStack>( "org.qfield.gui", 1, 0, "FocusStack" );
    qmlRegisterType<QfGeometryEditorsModel>( "org.qfield.gui", 1, 0, "GeometryEditorsModel" );
    qmlRegisterType<QfLocalFilesModel>( "org.qfield.gui", 1, 0, "LocalFilesModel" );
    qmlRegisterType<QfOrderedRelationModel>( "org.qfield.gui", 1, 0, "OrderedRelationModel" );
    qmlRegisterType<QfParameterizedImage>( "org.qfield.gui", 1, 0, "ParameterizedImage" );
    qmlRegisterType<QfPrintLayoutListModel>( "org.qfield.gui", 1, 0, "PrintLayoutListModel" );
    qmlRegisterType<QfRecentProjectListModel>( "org.qfield.gui", 1, 0, "RecentProjectListModel" );
    qmlRegisterType<QfReferencingFeatureListModel>( "org.qfield.gui", 1, 0, "ReferencingFeatureListModel" );
    qmlRegisterType<QfSensorListModel>( "org.qfield.gui", 1, 0, "SensorListModel" );
    qmlRegisterType<QfValueMapModel>( "org.qfield.gui", 1, 0, "ValueMapModel" );
    REGISTER_SINGLETON( "org.qfield.gui", QfGeometryEditorsModel, "GeometryEditorsModelSingleton" );
    qmlRegisterSingletonType<QfTheme>( "org.qfield.gui", 1, 0, "Theme", themeInstance );
    qmlRegisterUncreatableType<QfMessageLogModel>( "org.qfield.gui", 1, 0, "MessageLogModel", "The MessageLogModel is available as context property `messageLogModel`." );
  }
} // namespace QfGui

#undef REGISTER_SINGLETON
```


