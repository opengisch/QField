

# File parametizedimage.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**parametizedimage.h**](parametizedimage_8h.md)

[Go to the documentation of this file](parametizedimage_8h.md)


```C++
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

class ParametizedImage : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY( QString source READ source WRITE setSource NOTIFY sourceChanged )

    
    Q_PROPERTY( QColor fillColor READ fillColor WRITE setFillColor NOTIFY fillColorChanged )

    Q_PROPERTY( QColor strokeColor READ strokeColor WRITE setStrokeColor NOTIFY strokeColorChanged )

    Q_PROPERTY( double strokeWidth READ strokeWidth WRITE setStrokeWidth NOTIFY strokeWidthChanged )

  public:
    ParametizedImage( QQuickItem *parent = nullptr );
    ~ParametizedImage() = default;

    void paint( QPainter *painter ) override;

    QString source() const;
    void setSource( const QString &source );

    QColor fillColor() const;
    void setFillColor( const QColor &color );

    QColor strokeColor() const;
    void setStrokeColor( const QColor &color );

    double strokeWidth() const;
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
```


