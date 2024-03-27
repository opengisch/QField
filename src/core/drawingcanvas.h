/***************************************************************************
 drawingcanvas.h - DrawingCanvas

 ---------------------
 begin                : 24.03.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef DRAWINGCANVAS_H
#define DRAWINGCANVAS_H

#include <QImage>
#include <QObject>
#include <QQuickPaintedItem>

class DrawingCanvas : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY( double zoomFactor READ zoomFactor WRITE setZoomFactor NOTIFY zoomFactorChanged )
    Q_PROPERTY( QPointF offset READ offset WRITE setOffset NOTIFY offsetChanged )

  public:
    DrawingCanvas();
    ~DrawingCanvas() = default;

    void paint( QPainter *painter ) override;

    double zoomFactor() const;
    void setZoomFactor( double factor );

    QPointF offset() const;
    void setOffset( const QPointF &offset );

    Q_INVOKABLE void createBlankCanvas( int width, int height, QColor backgroundColor = QColor( 255, 255, 255 ) );
    Q_INVOKABLE void createCanvasFromImage( const QString &path );

    Q_INVOKABLE void fitCanvas();

    Q_INVOKABLE void pan( const QPointF &oldPosition, const QPointF &newPosition );
    Q_INVOKABLE void zoom( double scale );

  signals:

    void zoomFactorChanged();
    void offsetChanged();

  private:
    double mZoomFactor = 1.0;
    QPointF mOffset = QPointF( 0, 0 );

    QImage mBackgroundImage;
};

#endif // DRAWINGCANVAS_H
