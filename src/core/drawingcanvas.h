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

/**
 * \ingroup core
 */
class DrawingCanvas : public QQuickPaintedItem
{
    Q_OBJECT

    /**
     * This property is set to TRUE when no canvas has been created or a previously
     * created canvas has been cleared.
     */
    Q_PROPERTY( bool isEmpty READ isEmpty NOTIFY isEmptyChanged )

    /**
     * This property is set to TRUE when a created canvas has been modified.
     */
    Q_PROPERTY( bool isDirty READ isDirty NOTIFY isDirtyChanged )

    /**
     * This property holds the color of the canvas frame.
     */
    Q_PROPERTY( QColor frameColor READ frameColor WRITE setFrameColor NOTIFY frameColorChanged )

    /**
     * This property holds the current zoom factor of the canvas. A value of 1.0 means the canvas
     * is at native resolutio (i.e. one screen pixel represents one canvas pixel).
     */
    Q_PROPERTY( double zoomFactor READ zoomFactor WRITE setZoomFactor NOTIFY zoomFactorChanged )

    /**
     * This property holds the offset from the center of the canvas as a result of
     * panning operations.
     */
    Q_PROPERTY( QPointF offset READ offset WRITE setOffset NOTIFY offsetChanged )

  public:
    DrawingCanvas( QQuickItem *parent = nullptr );
    ~DrawingCanvas() = default;

    void paint( QPainter *painter ) override;

    //! \copydoc DrawingCanvas::isEmpty
    bool isEmpty() const;

    //! \copydoc DrawingCanvas::isEmpty
    void setIsEmpty( bool empty );

    //! \copydoc DrawingCanvas::isDirty
    bool isDirty() const;

    //! \copydoc DrawingCanvas::isDirty
    void setIsDirty( bool dirty );

    //! \copydoc DrawingCanvas::frameColor
    QColor frameColor() const;

    //! \copydoc DrawingCanvas::frameColor
    void setFrameColor( const QColor &color );

    //! \copydoc DrawingCanvas::zoomFactor
    double zoomFactor() const;

    //! \copydoc DrawingCanvas::zoomFactor
    void setZoomFactor( double factor );

    //! \copydoc DrawingCanvas::offset
    QPointF offset() const;

    //! \copydoc DrawingCanvas::offset
    void setOffset( const QPointF &offset );

    /**
     * Creates a blank drawing canvas.
     * \param width the width of the canvas.
     * \param height the height of the canvas.
     * \param backgroundColor the background color of the canvas.
     */
    Q_INVOKABLE void createBlankCanvas( int width, int height, QColor backgroundColor = QColor( 255, 255, 255 ) );

    /**
     * Creates a drawing canvas from a given image which will be the background on which
     * the drawing will be overlayed.
     * \param path the image path.
     */
    Q_INVOKABLE void createCanvasFromImage( const QString &path );

    /**
     * Clears the drawing canvas.
     * \see isEmpty()
     */
    Q_INVOKABLE void clear();

    /**
     * Undos the last drawing operation.
     * \see isDirty()
     */
    Q_INVOKABLE void undo();

    /**
     * Saves the drawing canvas to a temporary location.
     * \returns the temporary file path of the saved image.
     */
    Q_INVOKABLE QString save() const;

    /**
     * Fits the drawing canvas to match available width and height.
     */
    Q_INVOKABLE void fitCanvas();

    /**
     * Pans the drawing canvas by the distance between two points.
     */
    Q_INVOKABLE void pan( const QPointF &oldPosition, const QPointF &newPosition );

    /**
     * Zooms the drawing canvas by the provided \a scale value.
     */
    Q_INVOKABLE void zoom( double scale );

    /**
     * Begins a stroke operation.
     * \param point the first point of the stroke
     * \param color the color of the stroke
     */
    Q_INVOKABLE void strokeBegin( const QPointF &point, const QColor color = QColor( 0, 0, 0 ) );

    /**
     * Adds a new \a point to the current stroke path.
     * \note The function strokeBegin must have been called prior to this function.
     */
    Q_INVOKABLE void strokeMove( const QPointF &point );

    /**
     * Ends the current stroke with a final \a point added to the stroke path.
     * \note The function strokeBegin must have been called prior to this function.
     */
    Q_INVOKABLE void strokeEnd( const QPointF &point );

  signals:
    void isEmptyChanged();
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

    void drawStroke( QPainter *painter, const Stroke &stroke, bool onCanvas = true ) const;

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

    QList<Stroke> mStrokes;
    Stroke mCurrentStroke;
};

#endif // DRAWINGCANVAS_H
