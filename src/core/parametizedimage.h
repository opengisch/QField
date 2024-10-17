/***************************************************************************
 parametizedimage.h - ParametizedImage

 ---------------------
 begin                : 21.05.2024
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

#ifndef PARAMETIZEDIMAGE_H
#define PARAMETIZEDIMAGE_H

#include <QObject>
#include <QQuickPaintedItem>

#define DEFAULT_STROKE_WIDTH 5

/**
 * \ingroup core
 */
class ParametizedImage : public QQuickPaintedItem
{
    Q_OBJECT

    /**
     * Returns the current source image.
     */
    Q_PROPERTY( QString source READ source WRITE setSource NOTIFY sourceChanged )

    /**
     * Returns the current fill color used to paint an image.
     * \note Compatible with parametized SVG images only.
     */
    Q_PROPERTY( QColor fillColor READ fillColor WRITE setFillColor NOTIFY fillColorChanged )

    /**
     * Returns the current stroke color used to paint an image.
     * \note Compatible with parametized SVG images only.
     */
    Q_PROPERTY( QColor strokeColor READ strokeColor WRITE setStrokeColor NOTIFY strokeColorChanged )

    /**
     * Returns the current stroke width used to paint an image.
     * \note Compatible with parametized SVG images only.
     */
    Q_PROPERTY( double strokeWidth READ strokeWidth WRITE setStrokeWidth NOTIFY strokeWidthChanged )

  public:
    ParametizedImage( QQuickItem *parent = nullptr );
    ~ParametizedImage() = default;

    void paint( QPainter *painter ) override;

    //! \copydoc ParametizedImage::source
    QString source() const;
    //! \copydoc ParametizedImage::source
    void setSource( const QString &source );

    //! \copydoc ParametizedImage::fillColor
    QColor fillColor() const;
    //! \copydoc ParametizedImage::fillColor
    void setFillColor( const QColor &color );

    //! \copydoc ParametizedImage::strokeColor
    QColor strokeColor() const;
    //! \copydoc ParametizedImage::strokeColor
    void setStrokeColor( const QColor &color );

    //! \copydoc ParametizedImage::strokeWidth
    double strokeWidth() const;
    //! \copydoc ParametizedImage::strokeWidth
    void setStrokeWidth( double width );

  signals:
    void sourceChanged();
    void fillColorChanged();
    void strokeColorChanged();
    void strokeWidthChanged();

  private:
    bool mIsValid = false;
    bool mIsRaster = true;

    QString mSource;
    QSizeF mSourceSize;

    QColor mFillColor = QColor( 0, 0, 0 );
    QColor mStrokeColor = QColor( 255, 255, 255 );
    double mStrokeWidth = 1.0;
};

#endif // PARAMETIZEDIMAGE_H
