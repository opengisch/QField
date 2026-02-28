

# File drawingcanvas.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**drawingcanvas.h**](drawingcanvas_8h.md)

[Go to the documentation of this file](drawingcanvas_8h.md)


```C++
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

#define DEFAULT_STROKE_WIDTH 5

struct DrawingStroke
{
    Q_GADGET

    Q_PROPERTY( double width MEMBER width );
    Q_PROPERTY( QColor color MEMBER color );
    Q_PROPERTY( QColor fillColor MEMBER fillColor );
    Q_PROPERTY( QList<QPointF> scenePoints MEMBER scenePoints );
    Q_PROPERTY( QList<QPointF> points MEMBER points );

  public:
    double width = 5.0;
    QColor color = QColor( 0, 0, 0 );
    QColor fillColor = QColor( Qt::transparent );
    QList<QPointF> scenePoints;
    QList<QPointF> points;
};


class DrawingCanvas : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY( bool isEmpty READ isEmpty NOTIFY isEmptyChanged )

    
    Q_PROPERTY( bool isDirty READ isDirty NOTIFY isDirtyChanged )

    Q_PROPERTY( QColor frameColor READ frameColor WRITE setFrameColor NOTIFY frameColorChanged )

    Q_PROPERTY( double zoomFactor READ zoomFactor WRITE setZoomFactor NOTIFY zoomFactorChanged )

    Q_PROPERTY( QPointF offset READ offset WRITE setOffset NOTIFY offsetChanged )

    Q_PROPERTY( DrawingStroke currentStroke READ currentStroke NOTIFY currentStrokeChanged )

  public:
    DrawingCanvas( QQuickItem *parent = nullptr );
    ~DrawingCanvas() = default;

    void paint( QPainter *painter ) override;

    bool isEmpty() const;

    void setIsEmpty( bool empty );

    bool isDirty() const;

    void setIsDirty( bool dirty );

    QColor frameColor() const;

    void setFrameColor( const QColor &color );

    double zoomFactor() const;

    void setZoomFactor( double factor );

    QPointF offset() const;

    void setOffset( const QPointF &offset );

    DrawingStroke currentStroke() const;

    Q_INVOKABLE void createBlankCanvas( int width, int height, QColor backgroundColor = QColor( 255, 255, 255 ) );

    Q_INVOKABLE void createCanvasFromImage( const QString &path );

    Q_INVOKABLE void clear();

    Q_INVOKABLE void undo();

    Q_INVOKABLE QString save() const;

    Q_INVOKABLE void fitCanvas();

    Q_INVOKABLE void pan( const QPointF &oldPosition, const QPointF &newPosition );

    Q_INVOKABLE void zoom( double scale );

    Q_INVOKABLE void strokeBegin( const QPointF &point, const QColor color = QColor( 0, 0, 0 ) );

    Q_INVOKABLE void strokeMove( const QPointF &point );

    Q_INVOKABLE void strokeEnd( const QPointF &point );

  signals:
    void isEmptyChanged();
    void isDirtyChanged();
    void frameColorChanged();
    void zoomFactorChanged();
    void offsetChanged();
    void currentStrokeChanged();

  private:
    void drawStroke( QPainter *painter, const DrawingStroke &stroke, bool onCanvas = true ) const;

    QPointF itemToCanvas( const QPointF &point ) const;
    QPointF canvasToItem( const QPointF &point ) const;

    bool mIsEmpty = true;
    bool mIsDirty = false;

    QString mLoadedImagePath;

    QColor mFrameColor;
    double mZoomFactor = 1.0;
    QPointF mOffset = QPointF( 0, 0 );

    QImage mBackgroundImage;
    QImage mDrawingImage;

    QList<DrawingStroke> mStrokes;
    DrawingStroke mCurrentStroke;
};

#endif // DRAWINGCANVAS_H
```


