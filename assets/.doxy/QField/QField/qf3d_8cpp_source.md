

# File qf3d.cpp

[**File List**](files.md) **>** [**3d**](dir_4b0af5720da35ca08cf9fbb2fd056889.md) **>** [**qf3d.cpp**](qf3d_8cpp.md)

[Go to the documentation of this file](qf3d_8cpp.md)


```C++
/***************************************************************************
    qf3d.cpp
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

#include "qf3d.h"
#include "qf3dgeometry.h"
#include "qf3dgeometryconfiguration.h"
#include "qf3dmaptexturedata.h"
#include "qf3drubberbandgeometry.h"
#include "qf3dterraingeometry.h"
#include "qf3dterrainprovider.h"
#include "qfmaptoview3d.h"

#include <QtQml>

namespace Qf3D
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

    // Pre-Qf names, reached through org.qfield or the legacy Theme module.
    qmlRegisterType<QfMapToView3D>( "org.qfield._3d", 1, 0, "MapToView3D" );
    qmlRegisterType<Qf3DGeometry>( "org.qfield._3d", 1, 0, "Quick3DGeometry" );
    qmlRegisterType<Qf3DGeometryConfiguration>( "org.qfield._3d", 1, 0, "Quick3DGeometryConfiguration" );
    qmlRegisterType<Qf3DMapTextureData>( "org.qfield._3d", 1, 0, "Quick3DMapTextureData" );
    qmlRegisterType<Qf3DRubberbandGeometry>( "org.qfield._3d", 1, 0, "Quick3DRubberbandGeometry" );
    qmlRegisterType<Qf3DTerrainGeometry>( "org.qfield._3d", 1, 0, "Quick3DTerrainGeometry" );
    qmlRegisterType<Qf3DTerrainProvider>( "org.qfield._3d", 1, 0, "Quick3DTerrainProvider" );
  }
} // namespace Qf3D
```


