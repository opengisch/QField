#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <qgsgeometry.h>

#include <QtPositioning/QGeoCoordinate>

class Geometry : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QPointF currentCoordinate READ currentCoordinate WRITE setCurrentCoordinate NOTIFY currentCoordinateChanged)

  public:
    explicit Geometry( QObject* parent = nullptr );

    QgsGeometry asQgsGeometry() const;

    QPointF currentCoordinate();
    void setCurrentCoordinate( QPointF coord );

  signals:
    void currentCoordinateChanged();

  private:
    QPointF mCurrentCoordinate;
};

#endif // GEOMETRY_H
