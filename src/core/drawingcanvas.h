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

    Q_PROPERTY( bool isDirty READ isDirty NOTIFY isDirtyChanged )
    Q_PROPERTY( QColor frameColor READ frameColor WRITE setFrameColor NOTIFY frameColorChanged )
    Q_PROPERTY( double zoomFactor READ zoomFactor WRITE setZoomFactor NOTIFY zoomFactorChanged )
    Q_PROPERTY( QPointF offset READ offset WRITE setOffset NOTIFY offsetChanged )

  public:
    DrawingCanvas();
    ~DrawingCanvas() = default;

    void paint( QPainter *painter ) override;

    bool isDirty() const;
    void setIsDirty( bool dirty );

    QColor frameColor() const;
    void setFrameColor( const QColor &color );

    double zoomFactor() const;
    void setZoomFactor( double factor );

    QPointF offset() const;
    void setOffset( const QPointF &offset );

    Q_INVOKABLE void createBlankCanvas( int width, int height, QColor backgroundColor = QColor( 255, 255, 255 ) );
    Q_INVOKABLE void createCanvasFromImage( const QString &path );

    Q_INVOKABLE QString save() const;

    Q_INVOKABLE void fitCanvas();

    Q_INVOKABLE void pan( const QPointF &oldPosition, const QPointF &newPosition );
    Q_INVOKABLE void zoom( double scale );

    Q_INVOKABLE void strokeBegin( const QPointF &point, const QColor color = QColor( 0, 0, 0 ) );
    Q_INVOKABLE void strokeMove( const QPointF &point );
    Q_INVOKABLE void strokeEnd( const QPointF &point );

  signals:
    void isDirtyChanged();
    void frameColorChanged();
    void zoomFactorChanged();
    void offsetChanged();

  private:
    struct Stroke
    {
        double width = 5.0;
        QColor color = QColor( 0, 0, 0 );
        QColor fillColor = QColor( Qt::transparent );
        QList<QPointF> points;
    };

    void drawStroke( QPainter *painter, Stroke &stroke, bool onCanvas = true );

    QPointF itemToCanvas( const QPointF &point );
    QPointF canvasToItem( const QPointF &point );

    bool mIsDirty = false;

    QColor mFrameColor;
    double mZoomFactor = 1.0;
    QPointF mOffset = QPointF( 0, 0 );

    QImage mBackgroundImage;
    QImage mDrawingImage;

    Stroke mCurrentStroke;
};

#endif // DRAWINGCANVAS_H
