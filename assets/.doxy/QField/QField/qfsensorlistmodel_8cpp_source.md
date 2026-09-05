

# File qfsensorlistmodel.cpp

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qfsensorlistmodel.cpp**](qfsensorlistmodel_8cpp.md)

[Go to the documentation of this file](qfsensorlistmodel_8cpp.md)


```C++
/***************************************************************************
 qfsensorlistmodel.cpp
 ---------------------
 begin                : 25.03.2023
 copyright            : (C) 2023 by Mathieu Pellerin
 email                : mathieu at opengisch dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfsensorlistmodel.h"

#include <qgis.h>
#include <qgsabstractsensor.h>
#include <qgsproject.h>
#include <qgssensormanager.h>

QfSensorListModel::QfSensorListModel( QObject *parent )
  : QSortFilterProxyModel( parent )
{
}

QHash<int, QByteArray> QfSensorListModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
  roles[static_cast<int>( QgsSensorModel::CustomRole::SensorId )] = "SensorId";
  roles[static_cast<int>( QgsSensorModel::CustomRole::SensorType )] = "SensorType";
  roles[static_cast<int>( QgsSensorModel::CustomRole::SensorName )] = "SensorName";
  roles[static_cast<int>( QgsSensorModel::CustomRole::SensorStatus )] = "SensorStatus";
  roles[static_cast<int>( QgsSensorModel::CustomRole::SensorLastValue )] = "SensorLastValue";
  roles[static_cast<int>( QgsSensorModel::CustomRole::SensorLastTimestamp )] = "SensorLastTimestamp";
  return roles;
}

QgsProject *QfSensorListModel::project() const
{
  return mProject;
}

void QfSensorListModel::setProject( QgsProject *project )
{
  if ( mProject == project )
    return;

  if ( mProject )
  {
    disconnect( mProject->sensorManager(), &QgsSensorManager::sensorErrorOccurred, this, &QfSensorListModel::handleSensorError );
  }

  mProject = project;
  emit projectChanged();

  if ( mProject )
  {
    connect( mProject->sensorManager(), &QgsSensorManager::sensorErrorOccurred, this, &QfSensorListModel::handleSensorError );
    mSensorModel.reset( new QgsSensorModel( mProject->sensorManager() ) );
  }
  else
  {
    mSensorModel.reset( nullptr );
  }

  setSourceModel( mSensorModel.get() );
}

void QfSensorListModel::setShowConnectedOnly( bool showConnectedOnly )
{
  if ( mShowConnectedOnly == showConnectedOnly )
    return;

  beginFilterChange();
  mShowConnectedOnly = showConnectedOnly;
  endFilterChange( QSortFilterProxyModel::Direction::Rows );

  emit showConnectedOnlyChanged();
}

bool QfSensorListModel::filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const
{
  if ( mShowConnectedOnly )
  {
    return mSensorModel->data( mSensorModel->index( source_row, 0 ), static_cast<int>( QgsSensorModel::CustomRole::SensorStatus ) ).value<Qgis::DeviceConnectionStatus>() == Qgis::DeviceConnectionStatus::Connected;
  }

  return true;
}

void QfSensorListModel::connectSensorId( const QString &id ) const
{
  if ( mProject && mProject->sensorManager()->sensor( id ) )
  {
    mProject->sensorManager()->sensor( id )->connectSensor();
  }
}

void QfSensorListModel::disconnectSensorId( const QString &id ) const
{
  if ( mProject && mProject->sensorManager()->sensor( id ) )
  {
    mProject->sensorManager()->sensor( id )->disconnectSensor();
  }
}

void QfSensorListModel::handleSensorError( const QString &id )
{
  if ( mProject )
  {
    if ( QgsAbstractSensor *sensor = mProject->sensorManager()->sensor( id ) )
    {
      emit sensorErrorOccurred( QStringLiteral( "%1: %2" ).arg( sensor->name(), sensor->errorString() ) );
    }
  }
}
```


