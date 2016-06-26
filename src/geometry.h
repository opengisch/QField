#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <qgsgeometry.h>
#include <rubberbandmodel.h>

#include <QtPositioning/QGeoCoordinate>

class Geometry : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QPointF currentCoordinate READ currentCoordinate WRITE setCurrentCoordinate NOTIFY currentCoordinateChanged )
    Q_PROPERTY( RubberbandModel* rubberbandModel READ rubberbandModel WRITE setRubberbandModel NOTIFY rubberbandModelChanged )

  public:
    explicit Geometry( QObject* parent = nullptr );

    QgsGeometry asQgsGeometry() const;

    QPointF currentCoordinate();
    void setCurrentCoordinate( QPointF coord );

    RubberbandModel* rubberbandModel() const;
    void setRubberbandModel( RubberbandModel* rubberbandModel );

  signals:
    void currentCoordinateChanged();
    void rubberbandModelChanged();

  private:
    QPointF mCurrentCoordinate;
    RubberbandModel* mRubberbandModel;
};

#endif // GEOMETRY_H
