#ifndef QGSQUICKMAPCANVASMAP_H
#define QGSQUICKMAPCANVASMAP_H

#include <QtQuick/QQuickPaintedItem>
#include <QTimer>

#include "qgsmapcanvas.h"

class QgsQuickMapCanvasMap : public QQuickPaintedItem
{
    Q_OBJECT

  public:
    QgsQuickMapCanvasMap( QQuickItem* parent = 0 );

    QgsMapCanvas* mapCanvas();

    // QQuickPaintedItem interface
    void paint( QPainter* painter );

    QgsPoint toMapCoordinates( QPoint canvasCoordinates );

  public slots:
    void zoom( QPointF center, qreal scale );
    void pan( QPointF oldPos, QPointF newPos );
    void refresh();

  private:
    QScopedPointer<QgsMapCanvas> mMapCanvas;
    bool mPinching;
    QPoint mPinchStartPoint;
    QTimer mDelayedMapRefresh;

    // QQuickItem interface
  protected:
    void geometryChanged( const QRectF& newGeometry, const QRectF& oldGeometry );
};

#endif // QGSQUICKMAPCANVASMAP_H
