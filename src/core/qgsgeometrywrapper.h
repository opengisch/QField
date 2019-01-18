#ifndef QGSGEOMETRYWRAPPER_H
#define QGSGEOMETRYWRAPPER_H

#include <QObject>
#include <QStandardItemModel>

#include <qgsgeometry.h>
#include <qgscoordinatereferencesystem.h>

/**
 * @brief The QgsGeometryWrapper class wraps QGIS geometry and CRS classes
 * to have their contents available from QML
 */
class QgsGeometryWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QgsGeometry qgsGeometry READ qgsGeometry WRITE setQgsGeometry NOTIFY geometryChanged )
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
      , mQgsGeometry( geometry )
      , mCrs( crs )
    {}

    //! Returns a list of points if the geometry has point type (point, multipoint), an empty list otherwise
    Q_INVOKABLE QVariantList pointList() const;

    QgsGeometry qgsGeometry() const;
    void setQgsGeometry(const QgsGeometry &qgsGeometry);

    QgsCoordinateReferenceSystem crs() const;
    void setCrs(const QgsCoordinateReferenceSystem &crs);

signals:
    void geometryChanged();
    void crsChanged();

private:
    QgsGeometry mQgsGeometry;
    QgsCoordinateReferenceSystem mCrs;
};





Q_DECLARE_METATYPE( QgsGeometryWrapper )

#endif // QGSGEOMETRYWRAPPER_H
