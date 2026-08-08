

# File qfield3dqmlregistration.cpp

[**File List**](files.md) **>** [**3d**](dir_4b0af5720da35ca08cf9fbb2fd056889.md) **>** [**qfield3dqmlregistration.cpp**](qfield3dqmlregistration_8cpp.md)

[Go to the documentation of this file](qfield3dqmlregistration_8cpp.md)


```C++
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

#include "maptoview3d.h"
#include "qfield3dqmlregistration.h"
#include "quick3dgeometry.h"
#include "quick3dgeometryconfiguration.h"
#include "quick3dmaptexturedata.h"
#include "quick3drubberbandgeometry.h"
#include "quick3dterraingeometry.h"
#include "quick3dterrainprovider.h"

#include <QtQml>

namespace QField3D
{
  void registerQmlTypes()
  {
    qmlRegisterType<MapToView3D>( "org.qfield._3d", 1, 0, "MapToView3D" );
    qmlRegisterType<Quick3DGeometry>( "org.qfield._3d", 1, 0, "Quick3DGeometry" );
    qmlRegisterType<Quick3DGeometryConfiguration>( "org.qfield._3d", 1, 0, "Quick3DGeometryConfiguration" );
    qmlRegisterType<Quick3DMapTextureData>( "org.qfield._3d", 1, 0, "Quick3DMapTextureData" );
    qmlRegisterType<Quick3DRubberbandGeometry>( "org.qfield._3d", 1, 0, "Quick3DRubberbandGeometry" );
    qmlRegisterType<Quick3DTerrainGeometry>( "org.qfield._3d", 1, 0, "Quick3DTerrainGeometry" );
    qmlRegisterType<Quick3DTerrainProvider>( "org.qfield._3d", 1, 0, "Quick3DTerrainProvider" );
  }
} // namespace QField3D
```


