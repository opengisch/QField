/***************************************************************************
 parameterizedimage.h - ParameterizedImage

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

#ifndef PARAMETERIZEDIMAGE_H
#define PARAMETERIZEDIMAGE_H

#include <QObject>
#include <QQuickPaintedItem>

#define DEFAULT_STROKE_WIDTH 5

/**
 * \ingroup core
 */
class ParameterizedImage : public QQuickPaintedItem
{
    Q_OBJECT

    /**
     * The source image.
     */
    Q_PROPERTY( QString source READ source WRITE setSource NOTIFY sourceChanged )

    /**
     * The fill color used to paint an image.
     * \note Compatible with parameterized SVG images only.
     */
    Q_PROPERTY( QColor fillColor READ fillColor WRITE setFillColor NOTIFY fillColorChanged )

    /**
     * The stroke color used to paint an image.
     * \note Compatible with parameterized SVG images only.
     */
    Q_PROPERTY( QColor strokeColor READ strokeColor WRITE setStrokeColor NOTIFY strokeColorChanged )

    /**
     * The stroke width used to paint an image.
     * \note Compatible with parameterized SVG images only.
     */
    Q_PROPERTY( double strokeWidth READ strokeWidth WRITE setStrokeWidth NOTIFY strokeWidthChanged )

    /**
     * The parameters used to paint an image.
     * \note Compatible with parameterized SVG images only.
     */
    Q_PROPERTY( QVariantMap parameters READ parameters WRITE setParameters NOTIFY parametersChanged )

  public:
    ParameterizedImage( QQuickItem *parent = nullptr );
    ~ParameterizedImage() = default;

    void paint( QPainter *painter ) override;

    //! \copydoc ParameterizedImage::source
    QString source() const;
    //! \copydoc ParameterizedImage::source
    void setSource( const QString &source );

    //! \copydoc ParameterizedImage::fillColor
    QColor fillColor() const;
    //! \copydoc ParameterizedImage::fillColor
    void setFillColor( const QColor &color );

    //! \copydoc ParameterizedImage::strokeColor
    QColor strokeColor() const;
    //! \copydoc ParameterizedImage::strokeColor
    void setStrokeColor( const QColor &color );

    //! \copydoc ParameterizedImage::strokeWidth
    double strokeWidth() const;
    //! \copydoc ParameterizedImage::strokeWidth
    void setStrokeWidth( double width );

    //! \copydoc ParameterizedImage::parameters
    QVariantMap parameters() const;

    //! \copydoc ParameterizedImage::parameters
    void setParameters( const QVariantMap &parameters );

  signals:
    void sourceChanged();
    void fillColorChanged();
    void strokeColorChanged();
    void strokeWidthChanged();
    void parametersChanged();

  private:
    bool mIsValid = false;
    bool mIsRaster = true;

    QString mSource;
    QSizeF mSourceSize;

    QColor mFillColor = QColor( 0, 0, 0 );
    QColor mStrokeColor = QColor( 255, 255, 255 );
    double mStrokeWidth = 1.0;

    QVariantMap mParameters;
    QMap<QString, QString> mPreparedParameters;
};

#endif // PARAMETERIZEDIMAGE_H
