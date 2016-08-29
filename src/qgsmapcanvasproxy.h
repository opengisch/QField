#ifndef QGSMAPCANVASPROXY_H
#define QGSMAPCANVASPROXY_H

#include <QGraphicsProxyWidget>
#include <qgsmapcanvas.h>

#include <QGestureEvent>
#include <QTapAndHoldGesture>

class QgsMapCanvasProxy : public QGraphicsProxyWidget
{
    Q_OBJECT
  public:
    explicit QgsMapCanvasProxy( QGraphicsItem *parent = 0 );
    ~QgsMapCanvasProxy();

    QgsMapCanvas* mapCanvas() const;

    virtual bool event( QEvent * event );

  signals:
    void renderStarted();
    void renderCompleted();

  public slots:

  private:
    bool gestureEvent( QGestureEvent *event );
    void tapAndHoldTriggered( QTapAndHoldGesture *gesture );

    QgsMapCanvas *mMapCanvas;
};

#endif // QGSMAPCANVASPROXY_H
