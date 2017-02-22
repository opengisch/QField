#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <qgsgeometry.h>
#include <rubberbandmodel.h>

#include <QtPositioning/QGeoCoordinate>

class Geometry : public QObject
{
    Q_OBJECT

    Q_PROPERTY( RubberbandModel* rubberbandModel READ rubberbandModel WRITE setRubberbandModel NOTIFY rubberbandModelChanged )
    Q_PROPERTY( QgsVectorLayer* vectorLayer READ vectorLayer WRITE setVectorLayer NOTIFY vectorLayerChanged )
    Q_PROPERTY( qreal length READ length NOTIFY lengthChanged )
    Q_PROPERTY( bool lengthValid READ lengthValid NOTIFY lengthValidChanged )
    Q_PROPERTY( qreal area READ area NOTIFY areaChanged )
    Q_PROPERTY( bool areaValid READ areaValid NOTIFY areaValidChanged )
    /**
     * Contains the length of the last segment
     */
    Q_PROPERTY( qreal segmentLength READ segmentLength NOTIFY segmentLengthChanged )

  public:
    explicit Geometry( QObject* parent = nullptr );

    QgsGeometry asQgsGeometry() const;

    RubberbandModel* rubberbandModel() const;
    void setRubberbandModel( RubberbandModel* rubberbandModel );

    Q_INVOKABLE void applyRubberband();

    QgsVectorLayer* vectorLayer() const;
    void setVectorLayer( QgsVectorLayer* vectorLayer );

    qreal length() const;
    bool lengthValid() const;
    qreal area() const;
    bool areaValid() const;
    qreal segmentLength() const;

  signals:
    void rubberbandModelChanged();
    void vectorLayerChanged();
    void lengthChanged();
    void lengthValidChanged();
    void areaChanged();
    void areaValidChanged();
    void segmentLengthChanged();

  private:
    RubberbandModel* mRubberbandModel;
    QgsVectorLayer* mVectorLayer;
};

#endif // GEOMETRY_H
