

# File cogooperation.h

[**File List**](files.md) **>** [**cogo**](dir_bee6354f604373d3fe8d8d947aaacc65.md) **>** [**cogooperation.h**](cogooperation_8h.md)

[Go to the documentation of this file](cogooperation_8h.md)


```C++
/***************************************************************************
                              cogooperation.h
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

#include "qgsquickmapsettings.h"
#include "rubberbandmodel.h"

#include <QColor>
#include <QObject>
#include <QString>

#define SIZE_LABEL 14
#define SIZE_LINE 2
#define SIZE_POINT 10
#define SIZE_RESULT_FACTOR 1.8

#define COLOR_NEUTRAL QColor( 127, 127, 127 )
#define COLOR_GROUP_1 QColor( 228, 26, 28 )
#define COLOR_GROUP_2 QColor( 55, 126, 184 )
#define COLOR_GROUP_3 QColor( 152, 78, 163 )
#define COLOR_GROUP_4 QColor( 255, 127, 0 )
#define COLOR_RESULT QColor( 77, 175, 74 )

class CogoParameter
{
    Q_GADGET

    Q_PROPERTY( QString type MEMBER type )

    
    Q_PROPERTY( QString name MEMBER name )

    Q_PROPERTY( QString label MEMBER label )

    Q_PROPERTY( QColor color MEMBER color )

    Q_PROPERTY( QVariantMap configuration MEMBER configuration )

  public:
    explicit CogoParameter( const QString &type = QString(), const QString &name = QString(), const QString &label = QString(), const QColor &color = Qt::transparent, const QVariantMap &configuration = QVariantMap() )
      : type( type )
      , name( name )
      , label( label )
      , color( color )
      , configuration( configuration )
    {}

    QString type;
    QString name;
    QString label;
    QColor color;
    QVariantMap configuration;

    bool operator==( const CogoParameter &other ) const
    {
      return type == other.type && name == other.name && label == other.label && color == other.color && configuration == other.configuration;
    }

    bool operator!=( const CogoParameter &other ) const { return !operator==( other ); }
};


class CogoVisualGuide
{
    Q_GADGET

    Q_PROPERTY( Type type MEMBER type )

    
    Q_PROPERTY( QVariantMap details MEMBER details )

    Q_PROPERTY( QColor color MEMBER color )

    Q_PROPERTY( QColor outlineColor MEMBER outlineColor )

  public:
    enum Type
    {
      Point,
      Line,
      Circle,
      Label,
    };
    Q_ENUM( Type )

    explicit CogoVisualGuide( Type type = Point, const QVariantMap &details = QVariantMap(), const QColor &color = COLOR_NEUTRAL, const QColor &outlineColor = QColor( 255, 255, 255, 127 ) )
      : type( type )
      , details( details )
      , color( color )
      , outlineColor( outlineColor )
    {}

    Type type = Point;
    QVariantMap details;
    QColor color = QColor( 255, 0, 0 );
    QColor outlineColor = QColor( 255, 255, 255, 100 );
};


class CogoOperation
{
  public:
    CogoOperation() {}
    virtual ~CogoOperation() = default;

    virtual QString name() const { return QString(); }

    virtual QString displayName() const { return QString(); }

    virtual QString icon() const { return QString(); }

    virtual QList<CogoParameter> parameters( Qgis::WkbType wkbType = Qgis::WkbType::Unknown ) const { return QList<CogoParameter>(); }

    virtual QList<CogoVisualGuide> visualGuides( const QVariantMap &parameters, QgsQuickMapSettings *mapSettings ) const { return QList<CogoVisualGuide>(); }

    virtual bool checkReadiness( const QVariantMap &parameters, Qgis::WkbType wkbType = Qgis::WkbType::Unknown ) const { return false; }

    virtual bool execute( RubberbandModel *rubberbandModel, const QVariantMap &parameters, Qgis::WkbType wkbType = Qgis::WkbType::Unknown ) const { return false; }
};


class CogoOperationPointAtXYZ : public CogoOperation
{
  public:
    CogoOperationPointAtXYZ() {}
    ~CogoOperationPointAtXYZ() = default;

    QString name() const override { return QStringLiteral( "point_at_xyz" ); }
    QString displayName() const override { return QObject::tr( "XYZ Parameters" ); }
    QString icon() const override { return QStringLiteral( "ic_cogo_xy_white_24dp" ); }
    QList<CogoParameter> parameters( Qgis::WkbType wkbType ) const override;
    QList<CogoVisualGuide> visualGuides( const QVariantMap &parameters, QgsQuickMapSettings *mapSettings ) const override;
    bool checkReadiness( const QVariantMap &parameters, Qgis::WkbType wkbType = Qgis::WkbType::Unknown ) const override;
    bool execute( RubberbandModel *rubberbandModel, const QVariantMap &parameters, Qgis::WkbType wkbType ) const override;
};


class CogoOperationPointAtDistanceAngle : public CogoOperation
{
  public:
    CogoOperationPointAtDistanceAngle() {}
    ~CogoOperationPointAtDistanceAngle() = default;

    QString name() const override { return QStringLiteral( "point_at_distance_angle" ); }
    QString displayName() const override { return QObject::tr( "Distance/Angle from Point" ); }
    QString icon() const override { return QStringLiteral( "ic_cogo_angle_distance_white_24dp" ); }
    QList<CogoParameter> parameters( Qgis::WkbType wkbType ) const override;
    QList<CogoVisualGuide> visualGuides( const QVariantMap &parameters, QgsQuickMapSettings *mapSettings ) const override;
    bool checkReadiness( const QVariantMap &parameters, Qgis::WkbType wkbType = Qgis::WkbType::Unknown ) const override;
    bool execute( RubberbandModel *rubberbandModel, const QVariantMap &parameters, Qgis::WkbType wkbType ) const override;
};


class CogoOperationPointAtIntersectionCircles : public CogoOperation
{
  public:
    CogoOperationPointAtIntersectionCircles() {}
    ~CogoOperationPointAtIntersectionCircles() = default;

    QString name() const override { return QStringLiteral( "point_at_intersection_circles" ); }
    QString displayName() const override { return QObject::tr( "Circles Intersection" ); }
    QString icon() const override { return QStringLiteral( "ic_cogo_intersection_circles_white_24dp" ); }
    QList<CogoParameter> parameters( Qgis::WkbType wkbType ) const override;
    QList<CogoVisualGuide> visualGuides( const QVariantMap &parameters, QgsQuickMapSettings *mapSettings ) const override;
    bool checkReadiness( const QVariantMap &parameters, Qgis::WkbType wkbType = Qgis::WkbType::Unknown ) const override;
    bool execute( RubberbandModel *rubberbandModel, const QVariantMap &parameters, Qgis::WkbType wkbType ) const override;
};
Q_DECLARE_METATYPE( CogoOperation )
Q_DECLARE_METATYPE( CogoParameter )
Q_DECLARE_METATYPE( CogoVisualGuide )

#endif // COGOOPERATION_H
```


