/***************************************************************************
                              cogoregistry.cpp
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

#include "cogoregistry.h"

CogoRegistry *CogoRegistry::sCogoRegistryInterface = nullptr;

CogoRegistry::CogoRegistry( QObject *parent )
  : QObject( parent )
{
  registerOperation( new CogoOperationPointAtXYZ() );
  registerOperation( new CogoOperationPointAtDistanceAngle() );
  registerOperation( new CogoOperationPointAtIntersectionCircles() );
}

CogoRegistry::~CogoRegistry()
{
  qDeleteAll( mOperations );
}

bool CogoRegistry::registerOperation( CogoOperation *operation )
{
  if ( !mOperations.contains( operation->name() ) )
  {
    mOperations[operation->name()] = operation;
    emit operationAdded();

    return true;
  }

  return false;
}

CogoOperation *CogoRegistry::operation( const QString &name )
{
  return mOperations.contains( name ) ? mOperations[name] : nullptr;
}
