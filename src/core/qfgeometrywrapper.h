/***************************************************************************
  qfgeometrywrapper.h - QfGeometryWrapper

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

#ifndef QFGEOMETRYWRAPPER_H
#define QFGEOMETRYWRAPPER_H

#include <QObject>
#include <QStandardItemModel>
#include <qgscoordinatereferencesystem.h>
#include <qgsgeometry.h>

/**
 * @brief The QfGeometryWrapper class wraps QGIS geometry and CRS classes
 * to have their contents available from QML.
 * \ingroup core
 */
class QfGeometryWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QgsGeometry qgsGeometry READ qgsGeometry WRITE setQgsGeometry NOTIFY qgsGeometryChanged )
    Q_PROPERTY( QgsCoordinateReferenceSystem crs READ crs WRITE setCrs NOTIFY crsChanged )

  public:
    explicit QfGeometryWrapper( QObject *parent = nullptr )
      : QObject( parent )
    {}

    QfGeometryWrapper( const QgsGeometry &geometry, QgsCoordinateReferenceSystem crs, QObject *parent = nullptr )
      : QObject( parent )
      , mQgsGeometry( geometry )
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


Q_DECLARE_METATYPE( QfGeometryWrapper * )

#endif // QFGEOMETRYWRAPPER_H
