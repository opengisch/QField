/***************************************************************************
              locatorhighlight.h
               ----------------------------------------------------
              date                 : 28.11.2018
              copyright            : (C) 2018 by Denis Rouzaud
              email                : denis@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef LOCATORHIGHLIGHT_H
#define LOCATORHIGHLIGHT_H

#include <QtQuick/QQuickItem>

#include "qgsquickmapsettings.h"
#include "qfieldcore_global.h"

class QgsGeometryWrapper;
class QgsGeometry;

/**
 * LocatorHighlight allows highlighting geometries
 * on the canvas for the specific needs of the locator.
 */
class QFIELDCORE_EXPORT LinePolygonHighlight : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY( QColor color READ color WRITE setColor NOTIFY colorChanged )
    Q_PROPERTY( float width READ width WRITE setWidth NOTIFY widthChanged )
    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ mapSettings WRITE setMapSettings NOTIFY mapSettingsChanged )
    Q_PROPERTY( QgsGeometryWrapper *geometry READ geometry WRITE setGeometry NOTIFY qgsGeometryChanged )

  public:
    explicit LinePolygonHighlight( QQuickItem *parent = nullptr );

    QgsGeometryWrapper *geometry() const;
    void setGeometry( QgsGeometryWrapper *geometry );

    QgsQuickMapSettings *mapSettings() const;
    void setMapSettings( QgsQuickMapSettings *mapSettings );

    QColor color() const;
    void setColor( const QColor &color );

    float width() const;
    void setWidth( float width );

  signals:
    void colorChanged();
    void widthChanged();
    void mapSettingsChanged();
    void qgsGeometryChanged();
    void updated();

  private slots:
    void mapCrsChanged();
    void makeDirty();

  private:
    virtual QSGNode *updatePaintNode( QSGNode *n, UpdatePaintNodeData * ) override;

    QColor mColor;
    float mWidth;
    bool mDirty;
    QgsQuickMapSettings *mMapSettings = nullptr;
    QgsGeometryWrapper *mGeometry = nullptr;
};

#endif // LOCATORHIGHLIGHT_H
