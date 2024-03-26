/***************************************************************************
 imagecanvas.h - ImageCanvas

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

#ifndef IMAGECANVAS_H
#define IMAGECANVAS_H

#include <QImage>
#include <QObject>
#include <QQuickPaintedItem>

class ImageCanvas : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY( double canvasScale READ canvasScale WRITE setCanvasScale NOTIFY canvasScaleChanged )

  public:
    ImageCanvas();
    ~ImageCanvas() = default;

    void paint( QPainter *painter ) override;

    double canvasScale() const;
    void setCanvasScale( double scale );

    Q_INVOKABLE void createBlankCanvas( int width, int height, QColor backgroundColor = QColor( 255, 255, 255 ) );
    Q_INVOKABLE void createCanvasFromImage( const QString &path );

    Q_INVOKABLE void fitCanvas();

  signals:

    void canvasScaleChanged();

  private:
    double mCanvasScale = 1.0;

    QImage mBackgroundImage;
};

#endif // IMAGECANVAS_H
