/***************************************************************************
    qfield3dqmlregistration.cpp
    ---------------------
    begin                : August 2026
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

#include "qf3dgeometry.h"
#include "qf3dgeometryconfiguration.h"
#include "qf3dmaptexturedata.h"
#include "qf3drubberbandgeometry.h"
#include "qf3dterraingeometry.h"
#include "qf3dterrainprovider.h"
#include "qfield3dqmlregistration.h"
#include "qfmaptoview3d.h"

#include <QtQml>

namespace QField3D
{
  void registerQmlTypes()
  {
    qmlRegisterType<QfMapToView3D>( "org.qfield._3d", 1, 0, "QfMapToView3D" );
    qmlRegisterType<Qf3DGeometry>( "org.qfield._3d", 1, 0, "Qf3DGeometry" );
    qmlRegisterType<Qf3DGeometryConfiguration>( "org.qfield._3d", 1, 0, "Qf3DGeometryConfiguration" );
    qmlRegisterType<Qf3DMapTextureData>( "org.qfield._3d", 1, 0, "Qf3DMapTextureData" );
    qmlRegisterType<Qf3DRubberbandGeometry>( "org.qfield._3d", 1, 0, "Qf3DRubberbandGeometry" );
    qmlRegisterType<Qf3DTerrainGeometry>( "org.qfield._3d", 1, 0, "Qf3DTerrainGeometry" );
    qmlRegisterType<Qf3DTerrainProvider>( "org.qfield._3d", 1, 0, "Qf3DTerrainProvider" );
  }
} // namespace QField3D
