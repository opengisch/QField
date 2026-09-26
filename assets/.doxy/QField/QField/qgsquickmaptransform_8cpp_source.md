

# File qgsquickmaptransform.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qgsquick**](dir_19c3d7c8ba6dc95926fecc96295d1560.md) **>** [**qgsquickmaptransform.cpp**](qgsquickmaptransform_8cpp.md)

[Go to the documentation of this file](qgsquickmaptransform_8cpp.md)


```C++
/***************************************************************************
  qgsquickmaptransform.cpp
  --------------------------------------
  Date                 : 27.12.2014
  Copyright            : (C) 2014 by Matthias Kuhn
  Email                : matthias (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qgsquickmapsettings.h"
#include "qgsquickmaptransform.h"

void QgsQuickMapTransform::applyTo( QMatrix4x4 *matrix ) const
{
  *matrix *= mMatrix;
  matrix->optimize();
}

QgsQuickMapSettings *QgsQuickMapTransform::mapSettings() const
{
  return mMapSettings;
}

void QgsQuickMapTransform::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mapSettings == mMapSettings )
    return;

  if ( mMapSettings )
    disconnect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, &QgsQuickMapTransform::updateMatrix );

  mMapSettings = mapSettings;

  if ( mMapSettings )
    connect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, &QgsQuickMapTransform::updateMatrix );

  updateMatrix();

  emit mapSettingsChanged();
}

void QgsQuickMapTransform::updateMatrix()
{
  QMatrix4x4 matrix;
  float scaleFactor = static_cast<float>( 1.0 / mMapSettings->mapUnitsPerPoint() );

  matrix.scale( scaleFactor, -scaleFactor );
  matrix.translate( static_cast<float>( -mMapSettings->visibleExtent().xMinimum() ),
                    static_cast<float>( -mMapSettings->visibleExtent().yMaximum() ) );

  mMatrix = matrix;
  update();
}
```


