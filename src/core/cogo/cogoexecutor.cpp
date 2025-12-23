/***************************************************************************
                              cogoexecutor.cpp
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

#include "cogoexecutor.h"
#include "cogooperation.h"
#include "cogoregistry.h"


CogoExecutor::CogoExecutor( QObject *parent )
  : QObject( parent )
{
}

void CogoExecutor::setName( const QString &name )
{
  if ( mName == name )
  {
    return;
  }

  mName = name;
  emit nameChanged();

  checkReadiness();
}

void CogoExecutor::setParameters( const QVariantMap &parameters )
{
  if ( mParameters == parameters )
  {
    return;
  }

  mParameters = parameters;
  emit parametersChanged();

  checkReadiness();
}

void CogoExecutor::checkReadiness()
{
  bool isReady = false;
  if ( CogoOperation *operation = CogoRegistry::instance()->operation( mName ) )
  {
    isReady = operation->checkReadiness( mParameters );
  }

  if ( mIsReady != isReady )
  {
    mIsReady = isReady;
    emit isReadyChanged();
  }
}
