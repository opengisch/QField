

# File qfcogoregistry.cpp

[**File List**](files.md) **>** [**cogo**](dir_bee6354f604373d3fe8d8d947aaacc65.md) **>** [**qfcogoregistry.cpp**](qfcogoregistry_8cpp.md)

[Go to the documentation of this file](qfcogoregistry_8cpp.md)


```C++
/***************************************************************************
                              qfcogoregistry.cpp
                              -------------------
              begin                : 2025.12.21
              copyright            : (C) 2025 by Mathieu Pellerin
              email                : mathieu (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfcogoregistry.h"

QfCogoRegistry *QfCogoRegistry::sCogoRegistryInterface = nullptr;

QfCogoRegistry::QfCogoRegistry( QObject *parent )
  : QObject( parent )
{
  registerOperation( new QfCogoOperationPointAtXYZ() );
  registerOperation( new QfCogoOperationPointAtDistanceAngle() );
  registerOperation( new QfCogoOperationPointAtIntersectionCircles() );
}

QfCogoRegistry::~QfCogoRegistry()
{
  qDeleteAll( mOperations );
}

bool QfCogoRegistry::registerOperation( QfCogoOperation *operation )
{
  if ( !mOperations.contains( operation->name() ) )
  {
    mOperations[operation->name()] = operation;
    emit operationAdded();

    return true;
  }
  else
  {
    delete operation;
  }

  return false;
}

QfCogoOperation *QfCogoRegistry::operation( const QString &name )
{
  return mOperations.contains( name ) ? mOperations[name] : nullptr;
}
```


