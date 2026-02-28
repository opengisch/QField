

# File cogoexecutor.cpp

[**File List**](files.md) **>** [**cogo**](dir_bee6354f604373d3fe8d8d947aaacc65.md) **>** [**cogoexecutor.cpp**](cogoexecutor_8cpp.md)

[Go to the documentation of this file](cogoexecutor_8cpp.md)


```C++
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

#include <qgsvectorlayer.h>


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

  setParameterValues( QVariantMap() );
  getParameters();
}

void CogoExecutor::setParameterValues( const QVariantMap &parameterValues )
{
  if ( mParameterValues == parameterValues )
  {
    return;
  }

  mParameterValues = parameterValues;
  emit parameterValuesChanged();

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
  {
    return;
  }

  if ( mRubberbandModel )
  {
    disconnect( mRubberbandModel, &RubberbandModel::vectorLayerChanged, this, &CogoExecutor::getParameters );
  }

  mRubberbandModel = rubberbandModel;

  if ( mRubberbandModel )
  {
    connect( mRubberbandModel, &RubberbandModel::vectorLayerChanged, this, &CogoExecutor::getParameters );
  }

  emit rubberbandModelChanged();
}

QList<CogoVisualGuide> CogoExecutor::visualGuides() const
{
  return mVisualGuides;
}

void CogoExecutor::getParameters()
{
  QList<CogoParameter> parameters;

  if ( mRubberbandModel )
  {
    if ( const CogoOperation *operation = CogoRegistry::instance()->operation( mName ) )
    {
      parameters = operation->parameters( mRubberbandModel->vectorLayer() ? mRubberbandModel->vectorLayer()->wkbType() : Qgis::WkbType::Unknown );
    }
  }

  if ( mParameters != parameters )
  {
    mParameters = parameters;
    parametersChanged();
  }
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
    mVisualGuides = operation->visualGuides( mParameterValues, mMapSettings );
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

  if ( mRubberbandModel && !mParameterValues.isEmpty() )
  {
    if ( const CogoOperation *operation = CogoRegistry::instance()->operation( mName ) )
    {
      isReady = operation->checkReadiness( mParameterValues, mRubberbandModel->vectorLayer() ? mRubberbandModel->vectorLayer()->wkbType() : Qgis::WkbType::Unknown );
    }
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
    return operation->execute( mRubberbandModel, mParameterValues, mRubberbandModel->vectorLayer() ? mRubberbandModel->vectorLayer()->wkbType() : Qgis::WkbType::Unknown );
  }

  return false;
}
```


