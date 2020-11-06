/***************************************************************************
    appcoordinateoperationhandlers.h
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

#ifndef APPCOORDINATEOPERATIONHANDLERS_H
#define APPCOORDINATEOPERATIONHANDLERS_H

#include <QObject>

#include <qgscoordinatereferencesystem.h>
#include <qgsdatumtransform.h>

/**
 * Alerts users when a transform grids are required (or desired) for an operation between two
 * CRSes, yet they are not available on the current system. Derived from the
 * QgsAppCoordinateOperationHandlers class created by QGIS developer Nyall Dawson.
 */
class AppMissingGridHandler : public QObject
{
    Q_OBJECT
  public:

    explicit AppMissingGridHandler( QObject *parent );

  signals:

    void missingRequiredGrid( const QgsCoordinateReferenceSystem &sourceCrs,
                              const QgsCoordinateReferenceSystem &destinationCrs,
                              const QgsDatumTransform::GridDetails &grid );

    void missingPreferredGrid( const QgsCoordinateReferenceSystem &sourceCrs,
                               const QgsCoordinateReferenceSystem &destinationCrs,
                               const QgsDatumTransform::TransformDetails &preferredOperation,
                               const QgsDatumTransform::TransformDetails &availableOperation );

    void coordinateOperationCreationError( const QgsCoordinateReferenceSystem &sourceCrs,
                                           const QgsCoordinateReferenceSystem &destinationCrs,
                                           const QString &error );

    void missingGridUsedByContextHandler( const QgsCoordinateReferenceSystem &sourceCrs,
                                          const QgsCoordinateReferenceSystem &destinationCrs,
                                          const QgsDatumTransform::TransformDetails &desired );

    void fallbackOperationOccurred( const QgsCoordinateReferenceSystem &sourceCrs,
                                    const QgsCoordinateReferenceSystem &destinationCrs,
                                    const QString &desired );

  private slots:

    void onMissingRequiredGrid( const QgsCoordinateReferenceSystem &sourceCrs,
                                const QgsCoordinateReferenceSystem &destinationCrs,
                                const QgsDatumTransform::GridDetails &grid );

    void onMissingPreferredGrid( const QgsCoordinateReferenceSystem &sourceCrs,
                                 const QgsCoordinateReferenceSystem &destinationCrs,
                                 const QgsDatumTransform::TransformDetails &preferredOperation,
                                 const QgsDatumTransform::TransformDetails &availableOperation );

    void onCoordinateOperationCreationError( const QgsCoordinateReferenceSystem &sourceCrs,
        const QgsCoordinateReferenceSystem &destinationCrs,
        const QString &error );

    void onMissingGridUsedByContextHandler( const QgsCoordinateReferenceSystem &sourceCrs,
                                            const QgsCoordinateReferenceSystem &destinationCrs,
                                            const QgsDatumTransform::TransformDetails &desired );

    void onFallbackOperationOccurred( const QgsCoordinateReferenceSystem &sourceCrs,
                                      const QgsCoordinateReferenceSystem &destinationCrs,
                                      const QString &desired );

  private:

    bool shouldWarnAboutPair( const QgsCoordinateReferenceSystem &source, const QgsCoordinateReferenceSystem &dest );
    bool shouldWarnAboutPairForCurrentProject( const QgsCoordinateReferenceSystem &source, const QgsCoordinateReferenceSystem &dest );
    bool shouldWarnAboutBallparkPairForCurrentProject( const QgsCoordinateReferenceSystem &source, const QgsCoordinateReferenceSystem &dest );

    QList< QPair< QgsCoordinateReferenceSystem, QgsCoordinateReferenceSystem > > mAlreadyWarnedPairs;
    QList< QPair< QgsCoordinateReferenceSystem, QgsCoordinateReferenceSystem > > mAlreadyWarnedPairsForProject;
    QList< QPair< QgsCoordinateReferenceSystem, QgsCoordinateReferenceSystem > > mAlreadyWarnedBallparkPairsForProject;
};

#endif // QGSAPPCOORDINATEOPERATIONHANDLERS_H
