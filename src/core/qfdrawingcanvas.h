/***************************************************************************
 qfdrawingcanvas.h - QfDrawingCanvas

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

#ifndef QFDRAWINGCANVAS_H
#define QFDRAWINGCANVAS_H

#include <QImage>
#include <QObject>
#include <QQuickPaintedItem>

#define DEFAULT_STROKE_WIDTH 5

/**
 * \ingroup core
 */
struct QfDrawingStroke
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


/**
 * \ingroup core
 */
class QfDrawingCanvas : public QQuickPaintedItem
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

    Q_PROPERTY( QfDrawingStroke currentStroke READ currentStroke NOTIFY currentStrokeChanged )

  public:
    QfDrawingCanvas( QQuickItem *parent = nullptr );
    ~QfDrawingCanvas() = default;

    void paint( QPainter *painter ) override;

    //! \copydoc QfDrawingCanvas::isEmpty
    bool isEmpty() const;

    //! \copydoc QfDrawingCanvas::isEmpty
    void setIsEmpty( bool empty );

    //! \copydoc QfDrawingCanvas::isDirty
    bool isDirty() const;

    //! \copydoc QfDrawingCanvas::isDirty
    void setIsDirty( bool dirty );

    //! \copydoc QfDrawingCanvas::frameColor
    QColor frameColor() const;

    //! \copydoc QfDrawingCanvas::frameColor
    void setFrameColor( const QColor &color );

    //! \copydoc QfDrawingCanvas::zoomFactor
    double zoomFactor() const;

    //! \copydoc QfDrawingCanvas::zoomFactor
    void setZoomFactor( double factor );

    //! \copydoc QfDrawingCanvas::offset
    QPointF offset() const;

    //! \copydoc QfDrawingCanvas::offset
    void setOffset( const QPointF &offset );

    QfDrawingStroke currentStroke() const;

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
    void currentStrokeChanged();

  private:
    void drawStroke( QPainter *painter, const QfDrawingStroke &stroke, bool onCanvas = true ) const;

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

    QList<QfDrawingStroke> mStrokes;
    QfDrawingStroke mCurrentStroke;
};

#endif // QFDRAWINGCANVAS_H
