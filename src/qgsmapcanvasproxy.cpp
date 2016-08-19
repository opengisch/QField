#include "qgsmapcanvasproxy.h"

#include <qgsmapcanvas.h>
#include <QApplication>

QgsMapCanvasProxy::QgsMapCanvasProxy( QGraphicsItem *parent ) :
  QGraphicsProxyWidget( parent )
{
  mMapCanvas = new QgsMapCanvas();
  mMapCanvas->enableAntiAliasing( true );
  mMapCanvas->setCanvasColor( Qt::white );

  setWidget( mMapCanvas );

  grabGesture( Qt::TapAndHoldGesture );

  connect( mMapCanvas, SIGNAL( renderStarting() ), this, SIGNAL( renderStarted() ) );
  connect( mMapCanvas, SIGNAL( renderComplete( QPainter* ) ), this, SIGNAL( renderCompleted() ) );
}

QgsMapCanvasProxy::~QgsMapCanvasProxy()
{
}

QgsMapCanvas *QgsMapCanvasProxy::mapCanvas() const
{
  return mMapCanvas;
}

bool QgsMapCanvasProxy::event( QEvent * event )
{
  mMapCanvas->repaint();

  bool done = false;

  if ( event->type() == QEvent::Gesture )
  {
    done = gestureEvent( static_cast<QGestureEvent*>( event ) );
  }
  else
  {
    // pass other events to base class
    done = QGraphicsProxyWidget::event( event );
  }

  return done;
}

bool QgsMapCanvasProxy::gestureEvent( QGestureEvent *event )
{
  if ( QGesture *tapAndHold = event->gesture( Qt::TapAndHoldGesture ) )
  {
    tapAndHoldTriggered( static_cast<QTapAndHoldGesture *>( tapAndHold ) );
  }
  return true;
}

void QgsMapCanvasProxy::tapAndHoldTriggered( QTapAndHoldGesture *gesture )
{
  if ( gesture->state() == Qt::GestureFinished )
  {
    QPoint pos = gesture->position().toPoint();
    QWidget *receiver = QApplication::widgetAt( pos );
    qDebug() << "tapAndHoldTriggered: LONG CLICK gesture happened at " << pos;
    qDebug() << "widget under point of click: " << receiver;

    QApplication::postEvent( receiver, new QMouseEvent( QEvent::MouseButtonPress, receiver->mapFromGlobal( pos ), Qt::RightButton, Qt::RightButton, Qt::NoModifier ) );
    QApplication::postEvent( receiver, new QMouseEvent( QEvent::MouseButtonRelease, receiver->mapFromGlobal( pos ), Qt::RightButton, Qt::RightButton, Qt::NoModifier ) );
  }
}
