/***************************************************************************
  qfgeofencer.h - QfGeofencer

 ---------------------
 begin                : 27.06.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfgeofencer.h"
#include "qfplatformutilities.h"

#include <qgsgeometryengine.h>
#include <qgsproject.h>

QfGeofencer::QfGeofencer( QObject *parent )
  : QObject( parent )
{
}

QfGeofencer::~QfGeofencer()
{
}

void QfGeofencer::cleanupGatherer()
{
  if ( mGatherer )
  {
    disconnect( mGatherer, &QThread::finished, this, &QfGeofencer::processAreas );
    connect( mGatherer, &QThread::finished, mGatherer, &QObject::deleteLater );
    mGatherer->stop();
    mGatherer = nullptr;
  }
}

void QfGeofencer::gatherAreas()
{
  if ( !mAreasLayer || !mAreasLayer->isValid() || !mPositionCrs.isValid() )
  {
    return;
  }

  QgsFeatureRequest request;
  request.setDestinationCrs( mPositionCrs, QgsProject::instance()->transformContext() );

  QgsExpressionContext context = mAreasLayer->createExpressionContext();
  request.setExpressionContext( context );

  QgsExpression expression( mAreasLayer->displayExpression() );
  expression.prepare( &context );
  QSet<QString> referencedColumns = expression.referencedColumns();
  if ( !mAreasFilterExpression.isEmpty() )
  {
    request.setFilterExpression( mAreasFilterExpression );
    QgsExpression filterExpression( mAreasFilterExpression );
    filterExpression.prepare( &context );
    referencedColumns.unite( filterExpression.referencedColumns() );
  }

  QgsFields fields = mAreasLayer->fields();
  request.setSubsetOfAttributes( referencedColumns, fields );

  cleanupGatherer();

  mGatherer = new QfFeatureExpressionValuesGatherer( mAreasLayer, mAreasLayer->displayExpression(), request );
  connect( mGatherer, &QThread::finished, this, &QfGeofencer::processAreas );
  mGatherer->start();
}

void QfGeofencer::processAreas()
{
  if ( !mGatherer )
    return;

  mAreas.clear();

  mAreas = mGatherer->entries();
  mGatherer->deleteLater();
  mGatherer = nullptr;

  if ( mActive )
  {
    checkWithin( true );
  }
}

void QfGeofencer::checkWithin( bool forceProcess )
{
  int isWithinIndex = -1;
  if ( mActive && !mAreas.isEmpty() && !mPosition.isEmpty() )
  {
    std::unique_ptr<QgsGeometryEngine> geometryEngine;
#if _QGIS_VERSION_INT >= 40300
    geometryEngine = QgsGeometry::createGeometryEngine( &mPosition );
#else
    geometryEngine.reset( QgsGeometry::createGeometryEngine( &mPosition ) );
#endif
    for ( int i = 0; i < mAreas.size(); i++ )
    {
      if ( geometryEngine->within( mAreas.at( i ).feature.geometry().get() ) )
      {
        isWithinIndex = i;
        break;
      }
    }
  }

  if ( mIsWithinIndex != isWithinIndex )
  {
    if ( mIsWithinIndex != -1 )
    {
      mLastWithinIndex = mIsWithinIndex;
    }

    mIsWithinIndex = isWithinIndex;

    emit isWithinChanged();

    processWithinChanged();
  }
  else if ( forceProcess )
  {
    processWithinChanged();
  }
}

void QfGeofencer::processWithinChanged()
{
  QString feedback;
  bool isAlerting = false;
  int vibrationDuration = 0;
  if ( mActive && isWithin() )
  {
    switch ( mBehavior )
    {
      case AlertWhenInsideGeofencedArea:
        feedback = tr( "Position has trespassed into \"%1\"" ).arg( isWithinAreaName() );
        vibrationDuration = 1000;
        isAlerting = true;
        break;

      case InformWhenEnteringLeavingGeofencedArea:
        feedback = tr( "Position entered into \"%1\"" ).arg( isWithinAreaName() );
        vibrationDuration = 500;
        isAlerting = false;
        break;

      case AlertWhenOutsideGeofencedArea:
        feedback = QString();
        vibrationDuration = 0;
        isAlerting = false;
        break;
    }
  }
  else if ( mActive && !isWithin() )
  {
    switch ( mBehavior )
    {
      case AlertWhenInsideGeofencedArea:
        feedback = QString();
        vibrationDuration = 0;
        isAlerting = false;
        break;

      case InformWhenEnteringLeavingGeofencedArea:
        if ( mLastWithinIndex != -1 )
        {
          feedback = tr( "Position left from \"%1\"" ).arg( lastWithinAreaName() );
          vibrationDuration = 500;
        }
        isAlerting = false;
        break;

      case AlertWhenOutsideGeofencedArea:
        if ( mLastWithinIndex != -1 )
        {
          feedback = tr( "Position outside areas after leaving \"%1\"" ).arg( lastWithinAreaName() );
        }
        else
        {
          feedback = tr( "Position outside areas" );
        }
        vibrationDuration = 1000;
        isAlerting = true;
        break;
    }
  }

  if ( mIsAlerting != isAlerting )
  {
    mIsAlerting = isAlerting;
    emit isAlertingChanged();
  }

  if ( mLastFeedback != feedback )
  {
    mLastFeedback = feedback;
    emit lastFeedbackChanged();
  }

  if ( mVibration && vibrationDuration > 0 )
  {
    QfPlatformUtilities::instance()->vibrate( vibrationDuration );
  }

  switch ( mAudio )
  {
    case AudioSpeech:
#ifdef WITH_TEXTTOSPEECH
      if ( !mTextToSpeech )
      {
        mTextToSpeech = std::make_unique<QTextToSpeech>();
        mTextToSpeech->setLocale( QLocale() );
        const QList<QVoice> voices = mTextToSpeech->findVoices( QVoice::Male );
        if ( !voices.isEmpty() )
        {
          mTextToSpeech->setVoice( voices.first() );
        }
        mTextToSpeech->setRate( 0.1 );
      }

      if ( !mLastFeedback.isEmpty() )
      {
        mTextToSpeech->say( mLastFeedback );
      }
      else
      {
        mTextToSpeech->stop();
      }
#endif
      break;

    case AudioAlarm:
      if ( mIsAlerting )
      {
        if ( !mAudioAlarmSound )
        {
          mAudioAlarmSound = std::make_unique<QSoundEffect>();
          mAudioAlarmSound->setSource( QUrl( QStringLiteral( "qrc:/sounds/geofencing_alarm.wav" ) ) );
        }
        mAudioAlarmSound->play();
      }
      break;

    case AudioMuted:
      break;
  }
}

bool QfGeofencer::isWithin() const
{
  return mIsWithinIndex > -1;
}

QString QfGeofencer::isWithinAreaName() const
{
  if ( mIsWithinIndex < 0 || mIsWithinIndex >= mAreas.size() )
  {
    return QString();
  }

  return mAreas.at( mIsWithinIndex ).value;
}

QString QfGeofencer::lastWithinAreaName() const
{
  if ( mLastWithinIndex < 0 || mLastWithinIndex >= mAreas.size() )
  {
    return QString();
  }

  return mAreas.at( mLastWithinIndex ).value;
}

void QfGeofencer::setActive( bool active )
{
  if ( mActive == active )
  {
    return;
  }

  mActive = active;
  emit activeChanged();

  checkWithin( true );
}

void QfGeofencer::setBehavior( Behaviors behavior )
{
  if ( mBehavior == behavior )
  {
    return;
  }

  mBehavior = behavior;
  emit behaviorChanged();

  checkWithin( true );
}

void QfGeofencer::setVibration( bool vibration )
{
  if ( mVibration == vibration )
  {
    return;
  }

  mVibration = vibration;
  emit vibrationChanged();
}

void QfGeofencer::setAudio( AudioTypes audio )
{
  if ( mAudio == audio )
  {
    return;
  }

  mAudio = audio;
  emit audioChanged();
}

void QfGeofencer::setPosition( const QgsPoint &position )
{
  if ( mPosition == position )
  {
    return;
  }

  mPosition = position;
  emit positionChanged();

  checkWithin();
}

void QfGeofencer::setPositionCrs( const QgsCoordinateReferenceSystem &crs )
{
  if ( mPositionCrs == crs )
  {
    return;
  }

  mPositionCrs = crs;
  emit positionCrsChanged();

  gatherAreas();
}

void QfGeofencer::setAreasLayer( QgsVectorLayer *layer )
{
  if ( mAreasLayer == layer )
  {
    return;
  }

  mAreasLayer = layer;
  emit areasLayerChanged();

  gatherAreas();
}

void QfGeofencer::setAreasFilterExpression( const QString &areasFilterExpression )
{
  if ( mAreasFilterExpression == areasFilterExpression )
  {
    return;
  }

  mAreasFilterExpression = areasFilterExpression;
  emit areasFilterExpressionChanged();

  gatherAreas();
}

void QfGeofencer::applyProjectSettings( QgsProject *project )
{
  bool active = false;
  Behaviors behavior = AlertWhenInsideGeofencedArea;
  bool vibration = true;
  AudioTypes audio = AudioSpeech;

  QgsVectorLayer *layer = nullptr;
  QString filterExpression;

  if ( project )
  {
    active = project->readBoolEntry( QStringLiteral( "qfieldsync" ), QStringLiteral( "geofencingIsActive" ), false );
    behavior = static_cast<Behaviors>( project->readNumEntry( QStringLiteral( "qfieldsync" ), QStringLiteral( "geofencingBehavior" ), static_cast<int>( AlertWhenOutsideGeofencedArea ) ) );
    vibration = project->readBoolEntry( QStringLiteral( "qfieldsync" ), QStringLiteral( "geofencingVibration" ), true );
    audio = static_cast<AudioTypes>( project->readNumEntry( QStringLiteral( "qfieldsync" ), QStringLiteral( "geofencingAudio" ), static_cast<int>( AudioSpeech ) ) );
    const QString layerId = project->readEntry( QStringLiteral( "qfieldsync" ), QStringLiteral( "geofencingLayer" ) );
    if ( !layerId.isEmpty() )
    {
      layer = qobject_cast<QgsVectorLayer *>( project->mapLayer( layerId ) );
    }
    filterExpression = project->readEntry( QStringLiteral( "qfieldsync" ), QStringLiteral( "geofencingFilterExpression" ) );
  }

  // Temporarily deactivate to avoid needless feature gathering
  setActive( false );

  setBehavior( behavior );
  setVibration( vibration );
  setAudio( audio );
  setAreasFilterExpression( filterExpression );
  setAreasLayer( layer );

  setActive( active );
}
