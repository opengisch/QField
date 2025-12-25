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
  generateVisualGuides();
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
  generateVisualGuides();
}

void CogoExecutor::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
  {
    return;
  }

  if ( mMapSettings )
  {
    disconnect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, &CogoExecutor::generateVisualGuides );
    disconnect( mMapSettings, &QgsQuickMapSettings::rotationChanged, this, &CogoExecutor::generateVisualGuides );
  }

  mMapSettings = mapSettings;

  if ( mMapSettings )
  {
    connect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, &CogoExecutor::generateVisualGuides );
    connect( mMapSettings, &QgsQuickMapSettings::rotationChanged, this, &CogoExecutor::generateVisualGuides );
  }

  emit mapSettingsChanged();

  generateVisualGuides();
}

void CogoExecutor::setRubberbandModel( RubberbandModel *rubberbandModel )
{
  if ( mRubberbandModel == rubberbandModel )
    return;

  mRubberbandModel = rubberbandModel;
  emit rubberbandModelChanged();
}

QList<CogoVisualGuide> CogoExecutor::visualGuides() const
{
  return mVisualGuides;
}

void CogoExecutor::generateVisualGuides()
{
  if ( !mMapSettings || mName.isEmpty() || mParameters.isEmpty() )
  {
    if ( !mVisualGuides.isEmpty() )
    {
      mVisualGuides.clear();
      emit visualGuidesChanged();
    }
    return;
  }

  if ( const CogoOperation *operation = CogoRegistry::instance()->operation( mName ) )
  {
    mVisualGuides = operation->visualGuides( mParameters, mMapSettings );
  }
  else
  {
    mVisualGuides.clear();
  }

  emit visualGuidesChanged();
}

void CogoExecutor::checkReadiness()
{
  bool isReady = false;
  if ( const CogoOperation *operation = CogoRegistry::instance()->operation( mName ) )
  {
    isReady = operation->checkReadiness( mParameters );
  }

  if ( mIsReady != isReady )
  {
    mIsReady = isReady;
    emit isReadyChanged();
  }
}

bool CogoExecutor::execute()
{
  if ( !mRubberbandModel )
  {
    return false;
  }

  if ( const CogoOperation *operation = CogoRegistry::instance()->operation( mName ) )
  {
    return operation->execute( mParameters, mRubberbandModel );
  }

  return false;
}
