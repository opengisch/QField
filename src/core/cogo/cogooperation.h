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
class CogoParameter
{
    Q_GADGET

    Q_PROPERTY( QString type MEMBER type )
    Q_PROPERTY( QString name MEMBER name )
    Q_PROPERTY( QString label MEMBER label )

  public:
    explicit CogoParameter( const QString &type = QString(), const QString &name = QString(), const QString &label = QString() )
      : type( type )
      , name( name )
      , label( label )
    {}

    QString type;
    QString name;
    QString label;
};


/**
 * \ingroup core
 */
class CogoOperation
{
  public:
    CogoOperation() {}
    ~CogoOperation() = default;

    virtual QString name() const { return QString(); }

    virtual QString displayName() const { return QString(); }

    virtual QString icon() const { return QString(); }

    virtual QList<CogoParameter> parameters() const { return QList<CogoParameter>(); }

    virtual bool checkReadiness( const QVariantMap &parameters ) const { return false; }

    virtual bool execute( const QVariantMap &parameters ) const { return false; }
};
Q_DECLARE_METATYPE( CogoOperation )
Q_DECLARE_METATYPE( CogoParameter )


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
    QString icon() const override { return QStringLiteral( "ic_cogo_xy_white_24dp" ); }
    QList<CogoParameter> parameters() const override;
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
    QString icon() const override { return QStringLiteral( "ic_cogo_angle_distance_white_24dp" ); }
    QList<CogoParameter> parameters() const override;
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
    QString icon() const override { return QStringLiteral( "ic_cogo_intersection_circles_white_24dp" ); }
    QList<CogoParameter> parameters() const override;
};

#endif // COGOOPERATION_H
