/***************************************************************************
                              cogoregistry.h
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

#ifndef COGOOPERATION_H
#define COGOOPERATION_H

#include <QObject>
#include <QString>

/**
 * \ingroup core
 */
class CogoOperation
{
  public:
    CogoOperation() {}
    ~CogoOperation() = default;

    virtual QString name() const { return QStringLiteral(); }

    virtual QString displayName() const { return QStringLiteral(); }

    virtual bool checkReadiness( const QVariantMap &parameters ) const { return false; }

    virtual bool execute( const QVariantMap &parameters ) const { return false; }
};


/**
 * \ingroup core
 */
class CogoOperationPointAtXYZ : public CogoOperation
{
  public:
    CogoOperationPointAtXYZ() {}
    ~CogoOperationPointAtXYZ() = default;

    QString name() const override { return QStringLiteral( "point_at_xyz" ); }
    QString displayName() const override { return QObject::tr( "Point at XYZ" ); }
};


/**
 * \ingroup core
 */
class CogoOperationPointAtDistanceAngle : public CogoOperation
{
  public:
    CogoOperationPointAtDistanceAngle() {}
    ~CogoOperationPointAtDistanceAngle() = default;

    QString name() const override { return QStringLiteral( "point_at_distance_angle" ); }
    QString displayName() const override { return QObject::tr( "Point at Distance/Angle" ); }
};


/**
 * \ingroup core
 */
class CogoOperationPointAtIntersectionCircles : public CogoOperation
{
  public:
    CogoOperationPointAtIntersectionCircles() {}
    ~CogoOperationPointAtIntersectionCircles() = default;

    QString name() const override { return QStringLiteral( "point_at_intersection_circles" ); }
    QString displayName() const override { return QObject::tr( "Point at Circles' Intersection" ); }
};

#endif // COGOOPERATION_H
