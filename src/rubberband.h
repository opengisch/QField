/***************************************************************************
  rubberband.h - Rubberband

 ---------------------
 begin                : 11.6.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef RUBBERBAND_H
#define RUBBERBAND_H

#include <QQuickItem>

class RubberbandModel;
class MapSettings;

class Rubberband : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY( RubberbandModel* model READ model WRITE setModel NOTIFY modelChanged )
    Q_PROPERTY( MapSettings* mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )
    Q_PROPERTY( QColor color READ color WRITE setColor NOTIFY colorChanged )
    Q_PROPERTY( qreal width READ width WRITE setWidth NOTIFY widthChanged )

  public:
    Rubberband( QQuickItem* parent = nullptr );

    RubberbandModel* model() const;
    void setModel( RubberbandModel* model );

    MapSettings* mapSettings() const;
    void setMapSettings( MapSettings* mapSettings );

    QColor color() const;
    void setColor( const QColor& color );

    qreal width() const;
    void setWidth( qreal width );

  signals:
    void modelChanged();
    void mapSettingsChanged();
    void colorChanged();
    void widthChanged();

  private slots:
    void markDirty();

  private:
    QSGNode* updatePaintNode( QSGNode* n, QQuickItem::UpdatePaintNodeData* );

    RubberbandModel* mModel = nullptr;
    MapSettings* mMapSettings;
    bool mDirty = false;
    QColor mColor = QColor( 192, 57, 43, 200 );
    qreal mWidth = 1.8;
    QColor mColorCurrentPoint = QColor( 192, 57, 43, 150 );
    qreal mWidthCurrentPoint = 1.2;
};

#endif // RUBBERBAND_H
