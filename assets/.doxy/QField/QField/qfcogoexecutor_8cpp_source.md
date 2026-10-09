

# File qfcogoexecutor.cpp

[**File List**](files.md) **>** [**cogo**](dir_bee6354f604373d3fe8d8d947aaacc65.md) **>** [**qfcogoexecutor.cpp**](qfcogoexecutor_8cpp.md)

[Go to the documentation of this file](qfcogoexecutor_8cpp.md)


```C++
/***************************************************************************
                              qfcogoexecutor.cpp
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

#include "qfcogoexecutor.h"
#include "qfcogoregistry.h"

#include <qgsvectorlayer.h>


QfCogoExecutor::QfCogoExecutor( QObject *parent )
  : QObject( parent )
{
}

void QfCogoExecutor::setName( const QString &name )
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

void QfCogoExecutor::setParameterValues( const QVariantMap &parameterValues )
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

void QfCogoExecutor::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
  {
    return;
  }

  if ( mMapSettings )
  {
    disconnect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, &QfCogoExecutor::generateVisualGuides );
    disconnect( mMapSettings, &QgsQuickMapSettings::rotationChanged, this, &QfCogoExecutor::generateVisualGuides );
  }

  mMapSettings = mapSettings;

  if ( mMapSettings )
  {
    connect( mMapSettings, &QgsQuickMapSettings::visibleExtentChanged, this, &QfCogoExecutor::generateVisualGuides );
    connect( mMapSettings, &QgsQuickMapSettings::rotationChanged, this, &QfCogoExecutor::generateVisualGuides );
  }

  emit mapSettingsChanged();

  generateVisualGuides();
}

void QfCogoExecutor::setRubberbandModel( QfRubberbandModel *rubberbandModel )
{
  if ( mRubberbandModel == rubberbandModel )
  {
    return;
  }

  if ( mRubberbandModel )
  {
    disconnect( mRubberbandModel, &QfRubberbandModel::vectorLayerChanged, this, &QfCogoExecutor::getParameters );
  }

  mRubberbandModel = rubberbandModel;

  if ( mRubberbandModel )
  {
    connect( mRubberbandModel, &QfRubberbandModel::vectorLayerChanged, this, &QfCogoExecutor::getParameters );
  }

  emit rubberbandModelChanged();
}

QList<QfCogoVisualGuide> QfCogoExecutor::visualGuides() const
{
  return mVisualGuides;
}

void QfCogoExecutor::getParameters()
{
  QList<QfCogoParameter> parameters;

  if ( mRubberbandModel )
  {
    if ( const QfCogoOperation *operation = QfCogoRegistry::instance()->operation( mName ) )
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

void QfCogoExecutor::generateVisualGuides()
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

  if ( const QfCogoOperation *operation = QfCogoRegistry::instance()->operation( mName ) )
  {
    mVisualGuides = operation->visualGuides( mParameterValues, mMapSettings );
  }
  else
  {
    mVisualGuides.clear();
  }

  emit visualGuidesChanged();
}

void QfCogoExecutor::checkReadiness()
{
  bool isReady = false;

  if ( mRubberbandModel && !mParameterValues.isEmpty() )
  {
    if ( const QfCogoOperation *operation = QfCogoRegistry::instance()->operation( mName ) )
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

bool QfCogoExecutor::execute()
{
  if ( !mRubberbandModel )
  {
    return false;
  }

  if ( const QfCogoOperation *operation = QfCogoRegistry::instance()->operation( mName ) )
  {
    return operation->execute( mRubberbandModel, mParameterValues, mRubberbandModel->vectorLayer() ? mRubberbandModel->vectorLayer()->wkbType() : Qgis::WkbType::Unknown );
  }

  return false;
}
```


