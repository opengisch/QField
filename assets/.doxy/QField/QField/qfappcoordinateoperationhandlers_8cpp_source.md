

# File qfappcoordinateoperationhandlers.cpp

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qfappcoordinateoperationhandlers.cpp**](qfappcoordinateoperationhandlers_8cpp.md)

[Go to the documentation of this file](qfappcoordinateoperationhandlers_8cpp.md)


```C++
/***************************************************************************
    qgsappcoordinateoperationhandlers.cpp
    -------------------------
    begin                : November 2020
    copyright            : (C) 2020 by Mathieu Pellerin
    email                : nirvn dot asia at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfappcoordinateoperationhandlers.h"

#include <qgscoordinatetransform.h>
#include <qgsmessagelog.h>
#include <qgsproject.h>

//
// AppMissingRequiredGridHandler
//
QfAppMissingGridHandler::QfAppMissingGridHandler( QObject *parent )
  : QObject( parent )
{
  QgsCoordinateTransform::setCustomMissingRequiredGridHandler( [this]( const QgsCoordinateReferenceSystem &sourceCrs,
                                                                       const QgsCoordinateReferenceSystem &destinationCrs,
                                                                       const QgsDatumTransform::GridDetails &grid ) {
    emit missingRequiredGrid( sourceCrs, destinationCrs, grid );
  } );

  QgsCoordinateTransform::setCustomMissingPreferredGridHandler( [this]( const QgsCoordinateReferenceSystem &sourceCrs,
                                                                        const QgsCoordinateReferenceSystem &destinationCrs,
                                                                        const QgsDatumTransform::TransformDetails &preferredOperation,
                                                                        const QgsDatumTransform::TransformDetails &availableOperation ) {
    emit missingPreferredGrid( sourceCrs, destinationCrs, preferredOperation, availableOperation );
  } );

  QgsCoordinateTransform::setCustomCoordinateOperationCreationErrorHandler( [this]( const QgsCoordinateReferenceSystem &sourceCrs,
                                                                                    const QgsCoordinateReferenceSystem &destinationCrs,
                                                                                    const QString &error ) {
    emit coordinateOperationCreationError( sourceCrs, destinationCrs, error );
  } );

  QgsCoordinateTransform::setCustomMissingGridUsedByContextHandler( [this]( const QgsCoordinateReferenceSystem &sourceCrs,
                                                                            const QgsCoordinateReferenceSystem &destinationCrs,
                                                                            const QgsDatumTransform::TransformDetails &desired ) {
    emit missingGridUsedByContextHandler( sourceCrs, destinationCrs, desired );
  } );

  QgsCoordinateTransform::setFallbackOperationOccurredHandler( [this]( const QgsCoordinateReferenceSystem &sourceCrs,
                                                                       const QgsCoordinateReferenceSystem &destinationCrs,
                                                                       const QString &desired ) {
    emit fallbackOperationOccurred( sourceCrs, destinationCrs, desired );
  } );

  connect( this, &QfAppMissingGridHandler::missingRequiredGrid, this, &QfAppMissingGridHandler::onMissingRequiredGrid, Qt::QueuedConnection );
  connect( this, &QfAppMissingGridHandler::missingPreferredGrid, this, &QfAppMissingGridHandler::onMissingPreferredGrid, Qt::QueuedConnection );
  connect( this, &QfAppMissingGridHandler::coordinateOperationCreationError, this, &QfAppMissingGridHandler::onCoordinateOperationCreationError, Qt::QueuedConnection );
  connect( this, &QfAppMissingGridHandler::missingGridUsedByContextHandler, this, &QfAppMissingGridHandler::onMissingGridUsedByContextHandler, Qt::QueuedConnection );
  connect( this, &QfAppMissingGridHandler::fallbackOperationOccurred, this, &QfAppMissingGridHandler::onFallbackOperationOccurred, Qt::QueuedConnection );

  connect( QgsProject::instance(), &QgsProject::cleared, this, [this] {
    mAlreadyWarnedPairsForProject.clear();
    mAlreadyWarnedBallparkPairsForProject.clear();
  } );
}

void QfAppMissingGridHandler::onMissingRequiredGrid( const QgsCoordinateReferenceSystem &sourceCrs, const QgsCoordinateReferenceSystem &destinationCrs, const QgsDatumTransform::GridDetails &grid )
{
  if ( !shouldWarnAboutPair( sourceCrs, destinationCrs ) )
    return;

  const QString shortMessage = tr( "No transform available between %1 and %2" ).arg( sourceCrs.userFriendlyIdentifier( Qgis::CrsIdentifierType::ShortString ), destinationCrs.userFriendlyIdentifier( Qgis::CrsIdentifierType::ShortString ) );
  QString downloadMessage;
  if ( !grid.url.isEmpty() )
  {
    if ( !grid.packageName.isEmpty() )
    {
      downloadMessage = tr( "This grid is part of the “<i>%1</i>” package, available for download from <a href=\"%2\">%2</a>." ).arg( grid.packageName, grid.url );
    }
    else
    {
      downloadMessage = tr( "This grid is available for download from <a href=\"%1\">%1</a>." ).arg( grid.url );
    }
  }
  QgsMessageLog::logMessage( QStringLiteral( "%1\n%2" ).arg( shortMessage, downloadMessage ), tr( "projection" ) );
}

void QfAppMissingGridHandler::onMissingPreferredGrid( const QgsCoordinateReferenceSystem &sourceCrs, const QgsCoordinateReferenceSystem &destinationCrs, const QgsDatumTransform::TransformDetails &preferredOperation, const QgsDatumTransform::TransformDetails &availableOperation )
{
  if ( !shouldWarnAboutPair( sourceCrs, destinationCrs ) )
    return;

  QString gridMessage;
  QString downloadMessage;
  for ( const QgsDatumTransform::GridDetails &grid : preferredOperation.grids )
  {
    if ( !grid.isAvailable )
    {
      QString m = tr( "This transformation requires the grid file “%1”, which is not available for use on the system." ).arg( grid.shortName );
      if ( !grid.url.isEmpty() )
      {
        if ( !grid.packageName.isEmpty() )
        {
          downloadMessage = tr( "This grid is part of the <i>%1</i> package, available for download from <a href=\"%2\">%2</a>." ).arg( grid.packageName, grid.url );
        }
        else
        {
          downloadMessage = tr( "This grid is available for download from <a href=\"%1\">%1</a>." ).arg( grid.url );
        }
      }
      gridMessage += QStringLiteral( "<li>%1</li>" ).arg( m );
    }
  }
  if ( !gridMessage.isEmpty() )
  {
    gridMessage = "<ul>" + gridMessage + "</ul>";
  }

  QString accuracyMessage;
  if ( availableOperation.accuracy >= 0 && preferredOperation.accuracy >= 0 )
    accuracyMessage = tr( "<p>Current transform “<i>%1</i>” has an accuracy of %2 meters, while the preferred transformation “<i>%3</i>” has accuracy %4 meters.</p>" ).arg( availableOperation.name ).arg( availableOperation.accuracy ).arg( preferredOperation.name ).arg( preferredOperation.accuracy );
  else if ( preferredOperation.accuracy >= 0 )
    accuracyMessage = tr( "<p>Current transform “<i>%1</i>” has an unknown accuracy, while the preferred transformation “<i>%2</i>” has accuracy %3 meters.</p>" ).arg( availableOperation.name ).arg( preferredOperation.name ).arg( preferredOperation.accuracy );

  const QString longMessage = tr( "<p>The preferred transform between <i>%1</i> and <i>%2</i> is not available for use on the system.</p>" ).arg( sourceCrs.userFriendlyIdentifier(), destinationCrs.userFriendlyIdentifier() )
                              + gridMessage + accuracyMessage;

  QgsMessageLog::logMessage( QStringLiteral( "%1\n%2" ).arg( longMessage, downloadMessage ), tr( "projection" ) );
}

void QfAppMissingGridHandler::onCoordinateOperationCreationError( const QgsCoordinateReferenceSystem &sourceCrs, const QgsCoordinateReferenceSystem &destinationCrs, const QString &error )
{
  if ( !shouldWarnAboutPairForCurrentProject( sourceCrs, destinationCrs ) )
    return;

  const QString longMessage = tr( "No transform is available between <i>%1</i> and <i>%2</i>.<p style=\"color: red\">%3</p>" ).arg( sourceCrs.userFriendlyIdentifier(), destinationCrs.userFriendlyIdentifier(), error );

  QgsMessageLog::logMessage( longMessage, tr( "projection" ) );
}

void QfAppMissingGridHandler::onMissingGridUsedByContextHandler( const QgsCoordinateReferenceSystem &sourceCrs, const QgsCoordinateReferenceSystem &destinationCrs, const QgsDatumTransform::TransformDetails &desired )
{
  if ( !shouldWarnAboutPairForCurrentProject( sourceCrs, destinationCrs ) )
    return;

  const QString shortMessage = tr( "Cannot use project transform between %1 and %2" ).arg( sourceCrs.userFriendlyIdentifier( Qgis::CrsIdentifierType::ShortString ), destinationCrs.userFriendlyIdentifier( Qgis::CrsIdentifierType::ShortString ) );
  QString gridMessage;
  QString downloadMessage;
  for ( const QgsDatumTransform::GridDetails &grid : desired.grids )
  {
    if ( !grid.isAvailable )
    {
      QString m = tr( "This transformation requires the grid file “%1”, which is not available for use on the system." ).arg( grid.shortName );
      if ( !grid.url.isEmpty() )
      {
        if ( !grid.packageName.isEmpty() )
        {
          downloadMessage = tr( "This grid is part of the <i>%1</i> package, available for download from <a href=\"%2\">%2</a>." ).arg( grid.packageName, grid.url );
        }
        else
        {
          downloadMessage = tr( "This grid is available for download from <a href=\"%1\">%1</a>." ).arg( grid.url );
        }
      }
      gridMessage += QStringLiteral( "<li>%1</li>" ).arg( m );
    }
  }

  QgsMessageLog::logMessage( QStringLiteral( "%1\n%2\n%3" ).arg( shortMessage, gridMessage, downloadMessage ), tr( "projection" ) );
}

void QfAppMissingGridHandler::onFallbackOperationOccurred( const QgsCoordinateReferenceSystem &sourceCrs, const QgsCoordinateReferenceSystem &destinationCrs, const QString &desired )
{
  Q_UNUSED( desired )
  if ( !shouldWarnAboutBallparkPairForCurrentProject( sourceCrs, destinationCrs ) )
    return;

  const QString shortMessage = tr( "Used a ballpark transform from %1 to %2" ).arg( sourceCrs.userFriendlyIdentifier( Qgis::CrsIdentifierType::ShortString ), destinationCrs.userFriendlyIdentifier( Qgis::CrsIdentifierType::ShortString ) );
  QgsMessageLog::logMessage( shortMessage, tr( "projection" ) );
}

bool QfAppMissingGridHandler::shouldWarnAboutPair( const QgsCoordinateReferenceSystem &source, const QgsCoordinateReferenceSystem &dest )
{
  if ( mAlreadyWarnedPairs.contains( qMakePair( source, dest ) ) || mAlreadyWarnedPairs.contains( qMakePair( dest, source ) ) )
  {
    return false;
  }

  mAlreadyWarnedPairs.append( qMakePair( source, dest ) );
  return true;
}

bool QfAppMissingGridHandler::shouldWarnAboutPairForCurrentProject( const QgsCoordinateReferenceSystem &source, const QgsCoordinateReferenceSystem &dest )
{
  if ( mAlreadyWarnedPairsForProject.contains( qMakePair( source, dest ) ) || mAlreadyWarnedPairsForProject.contains( qMakePair( dest, source ) ) )
  {
    return false;
  }

  mAlreadyWarnedPairsForProject.append( qMakePair( source, dest ) );
  return true;
}

bool QfAppMissingGridHandler::shouldWarnAboutBallparkPairForCurrentProject( const QgsCoordinateReferenceSystem &source, const QgsCoordinateReferenceSystem &dest )
{
  if ( mAlreadyWarnedBallparkPairsForProject.contains( qMakePair( source, dest ) ) || mAlreadyWarnedBallparkPairsForProject.contains( qMakePair( dest, source ) ) )
  {
    return false;
  }

  mAlreadyWarnedBallparkPairsForProject.append( qMakePair( source, dest ) );
  return true;
}
```


