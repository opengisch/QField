/***************************************************************************
  qgsgeometrywrapper.h - QgsGeometryWrapper

 ---------------------
 begin                : 18.08.2018
 copyright            : (C) 2018 by Denis Rouzaud
 email                : denis@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSGEOMETRYWRAPPER_H
#define QGSGEOMETRYWRAPPER_H

#include <QObject>
#include <QStandardItemModel>
#include <qgscoordinatereferencesystem.h>
#include <qgsgeometry.h>

/**
 * @brief The QgsGeometryWrapper class wraps QGIS geometry and CRS classes
 * to have their contents available from QML
 */
class QgsGeometryWrapper : public QObject
{
  Q_OBJECT
  Q_PROPERTY( QgsGeometry qgsGeometry READ qgsGeometry WRITE setQgsGeometry NOTIFY qgsGeometryChanged )
  Q_PROPERTY( QgsCoordinateReferenceSystem crs READ crs WRITE setCrs NOTIFY crsChanged )

public:
  explicit QgsGeometryWrapper( QObject *parent = nullptr )
    : QObject( parent )
  {}

  QgsGeometryWrapper( const QgsGeometryWrapper &other )
    : QObject( other.parent() )
    , mQgsGeometry( QgsGeometry( other.mQgsGeometry ) )
    , mCrs( QgsCoordinateReferenceSystem( other.mCrs ) )
  {}

  QgsGeometryWrapper( QgsGeometry geometry, QgsCoordinateReferenceSystem crs, QObject *parent = nullptr )
    : QObject( parent )
    , mQgsGeometry( QgsGeometry( geometry ) )
    , mCrs( crs )
  {}

  //! Returns a list of points if the geometry has point type (point, multipoint), an empty list otherwise
  Q_INVOKABLE QVariantList pointList() const;

  //! Clear the wrapper by setting empty geometry and invalid CRS
  Q_INVOKABLE void clear();

  QgsGeometry qgsGeometry() const;
  void setQgsGeometry( const QgsGeometry &qgsGeometry );

  QgsCoordinateReferenceSystem crs() const;
  void setCrs( const QgsCoordinateReferenceSystem &crs );

signals:
  void qgsGeometryChanged();
  void crsChanged();

private:
  QgsGeometry mQgsGeometry;
  QgsCoordinateReferenceSystem mCrs;
};


Q_DECLARE_METATYPE( QgsGeometryWrapper )

#endif // QGSGEOMETRYWRAPPER_H
