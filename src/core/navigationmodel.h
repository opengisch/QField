/***************************************************************************
 navigationmodel.h - NavigationModel

 ---------------------
 begin                : 22.02.2022
 copyright            : (C) 2022 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef NAVIGATIONMODEL_H
#define NAVIGATIONMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <qgsabstractgeometry.h>
#include <qgscoordinatereferencesystem.h>

class Navigation;

class NavigationModel : public QAbstractListModel
{
    Q_OBJECT

  public:
    enum Types
    {
      Destination,
      Intermediate,
    };
    Q_ENUM( Types )

    enum Roles
    {
      Point = Qt::UserRole + 1,
      PointType,
    };
    Q_ENUM( Roles )

    NavigationModel();

    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;
    QHash<int, QByteArray> roleNames() const override;

    QgsPointSequence points() const { return mPoints; }

    QgsPoint destination() const;
    void setDestination( const QgsPoint &point );

    void setCrs( QgsCoordinateReferenceSystem crs );

    void save();
    void restore();
    void clear();

  signals:
    void destinationChanged();

  private:
    QgsPointSequence mPoints;
    QgsCoordinateReferenceSystem mCrs;
};

#endif // NAVIGATIONMODEL_H
