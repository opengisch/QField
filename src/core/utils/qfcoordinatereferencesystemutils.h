/***************************************************************************
  qfcoordinatereferencesystemutils.h - QfCoordinateReferenceSystemUtils

 ---------------------
 begin                : 28.05.2022
 copyright            : (C) 2022 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QFCOORDINATEREFERENCESYSTEMUTILS_H
#define QFCOORDINATEREFERENCESYSTEMUTILS_H

#include "qfield_core_export.h"

#include <QObject>
#include <qgscoordinatereferencesystem.h>
#include <qgscoordinatetransformcontext.h>

/**
 * \ingroup core
 */
class QFIELD_CORE_EXPORT QfCoordinateReferenceSystemUtils : public QObject
{
    Q_OBJECT

  public:
    explicit QfCoordinateReferenceSystemUtils( QObject *parent = nullptr );

    //! Returns an CRS matching the provided \a definition.
    static Q_INVOKABLE QgsCoordinateReferenceSystem fromDescription( const QString &definition ) { return QgsCoordinateReferenceSystem( definition ); }

    //! Returns an EPGS:4326 WGS84 CRS
    static Q_INVOKABLE QgsCoordinateReferenceSystem wgs84Crs() { return QgsCoordinateReferenceSystem::fromEpsgId( 4326 ); }

    /**
     * Returns an invalid CRS
     * \note This can be used in QML to avoid errors when a parent object pointer goes undefined
     */
    static Q_INVOKABLE QgsCoordinateReferenceSystem invalidCrs() { return QgsCoordinateReferenceSystem(); }

    /**
     * Returns an empty transform context
     * \note This can be used in QML to avoid errors when a parent object pointer goes undefined
     */
    static Q_INVOKABLE QgsCoordinateTransformContext emptyTransformContext() { return QgsCoordinateTransformContext(); }

    //! Returns whether the default coordinate order of a given \a crs is XY
    static Q_INVOKABLE bool defaultCoordinateOrderForCrsIsXY( const QgsCoordinateReferenceSystem &crs );

    //! Returns a user friendly identifier for a given \a crs, such as "EPSG:4326 - WGS 84"
    static Q_INVOKABLE QString userFriendlyIdentifier( const QgsCoordinateReferenceSystem &crs ) { return crs.userFriendlyIdentifier(); }
};

#endif // QFCOORDINATEREFERENCESYSTEMUTILS_H
