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

#include "attributeformmodel.h"
#include "expressionevaluator.h"
#include "expressionvariablemodel.h"
#include "featurechecklistmodel.h"
#include "focusstack.h"
#include "geometryeditorsmodel.h"
#include "localfilesmodel.h"
#include "messagelogmodel.h"
#include "orderedrelationmodel.h"
#include "parameterizedimage.h"
#include "permissions.h"
#include "printlayoutlistmodel.h"
#include "qfieldguiqmlregistration.h"
#include "recentprojectlistmodel.h"
#include "referencingfeaturelistmodel.h"
#include "sensorlistmodel.h"
#include "valuemapmodel.h"

#include <QPermissions>
#include <QtQml>

#define REGISTER_SINGLETON( uri, _class, name ) qmlRegisterSingletonType<_class>( uri, 1, 0, name, []( QQmlEngine *engine, QJSEngine *scriptEngine ) -> QObject * { Q_UNUSED( engine ); Q_UNUSED( scriptEngine ); return new _class(); } )

namespace QFieldGui
{
  void registerQmlTypes()
  {
    qmlRegisterType<AttributeFormModel>( "org.qfield", 1, 0, "AttributeFormModel" );
    qmlRegisterType<CameraPermission>( "org.qfield", 1, 0, "QfCameraPermission" );
    qmlRegisterType<ExpressionEvaluator>( "org.qfield", 1, 0, "ExpressionEvaluator" );
    qmlRegisterType<ExpressionVariableModel>( "org.qfield", 1, 0, "ExpressionVariableModel" );
    qmlRegisterType<FeatureCheckListModel>( "org.qfield", 1, 0, "FeatureCheckListModel" );
    qmlRegisterType<FocusStack>( "org.qfield", 1, 0, "FocusStack" );
    qmlRegisterType<GeometryEditorsModel>( "org.qfield", 1, 0, "GeometryEditorsModel" );
    qmlRegisterType<LocalFilesModel>( "org.qfield", 1, 0, "LocalFilesModel" );
    qmlRegisterType<MicrophonePermission>( "org.qfield", 1, 0, "QfMicrophonePermission" );
    qmlRegisterType<OrderedRelationModel>( "org.qfield", 1, 0, "OrderedRelationModel" );
    qmlRegisterType<ParameterizedImage>( "org.qfield", 1, 0, "ParameterizedImage" );
    qmlRegisterType<PrintLayoutListModel>( "org.qfield", 1, 0, "PrintLayoutListModel" );
    qmlRegisterType<RecentProjectListModel>( "org.qfield", 1, 0, "RecentProjectListModel" );
    qmlRegisterType<ReferencingFeatureListModel>( "org.qfield", 1, 0, "ReferencingFeatureListModel" );
    qmlRegisterType<SensorListModel>( "org.qfield", 1, 0, "SensorListModel" );
    qmlRegisterType<ValueMapModel>( "org.qfield", 1, 0, "ValueMapModel" );

    REGISTER_SINGLETON( "org.qfield", GeometryEditorsModel, "GeometryEditorsModelSingleton" );

    qmlRegisterUncreatableType<MessageLogModel>( "org.qfield", 1, 0, "MessageLogModel", "The MessageLogModel is available as context property `messageLogModel`." );
  }
} // namespace QFieldGui

#undef REGISTER_SINGLETON
