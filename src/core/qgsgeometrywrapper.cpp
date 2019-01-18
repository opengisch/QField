
#include "qgsgeometrywrapper.h"

 QVariantList QgsGeometryWrapper::pointList() const
{
  QVariantList pointList;
  if (mQgsGeometry.type() != QgsWkbTypes::PointGeometry)
    return pointList;

  QgsVertexIterator vertexIterator = mQgsGeometry.vertices();
  while ( vertexIterator.hasNext() )
  {
    const QgsPoint &pt = vertexIterator.next();
    pointList.append( QVariant::fromValue<QgsPoint>( pt ) );
    }

  return pointList;
}

QgsGeometry QgsGeometryWrapper::qgsGeometry() const
{
return mQgsGeometry;
}

void QgsGeometryWrapper::setQgsGeometry(const QgsGeometry &qgsGeometry)
{
  if (qgsGeometry.constGet() == mQgsGeometry.constGet())
    return;

mQgsGeometry = qgsGeometry;
emit geometryChanged();
}

QgsCoordinateReferenceSystem QgsGeometryWrapper::crs() const
{
return mCrs;
}

void QgsGeometryWrapper::setCrs(const QgsCoordinateReferenceSystem &crs)
{
  if (mCrs == crs)
    return;

mCrs = crs;
emit crsChanged();
}
