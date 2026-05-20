

# File resourcesource.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**resourcesource.cpp**](resourcesource_8cpp.md)

[Go to the documentation of this file](resourcesource_8cpp.md)


```C++
/***************************************************************************
  resourcesource.cpp - ResourceSource

 ---------------------
 begin                : 5.7.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "resourcesource.h"

#include <QTimer>

ResourceSource::ResourceSource( QObject *parent, const QString &prefix, const QString &resourceFilePath )
  : QObject( parent )
  , mPrefix( prefix )
  , mResourceFilePath( resourceFilePath )
{
  // prevent emit signal if the resourceFilePath is empty ( e.g. when AndroidResourceSource )
  if ( resourceFilePath.isEmpty() )
    return;

  if ( mResourceFilePath.startsWith( mPrefix ) )
    mResourceFilePath = mResourceFilePath.remove( mPrefix );

  QTimer::singleShot( 0, this, [this]() {
    emit resourceReceived( mResourceFilePath );
  } );
}

ResourceSource::~ResourceSource()
{
}
```


