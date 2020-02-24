#ifndef QFIELDUTILS_H
#define QFIELDUTILS_H

#include <QObject>

#include <qgsgeometry.h>
#include <qgsfeature.h>

class QgsVectorLayer;
class RubberbandModel;


class QFieldUtils : public QObject
{
  Q_OBJECT
public:
  explicit QFieldUtils(QObject *parent = nullptr);

  static Q_INVOKABLE QgsFeature initFeature(QgsVectorLayer *layer, QgsGeometry geometry = QgsGeometry());

  static Q_INVOKABLE QgsGeometry polygonFromRubberband(RubberbandModel *rubberBandModel , const QgsCoordinateReferenceSystem &crs);

  static Q_INVOKABLE QgsGeometry::OperationResult addRingFromRubberBand(QgsVectorLayer* layer, QgsFeatureId fid, RubberbandModel* rubberBandModel );

signals:

public slots:
};

#endif // QFIELDUTILS_H
