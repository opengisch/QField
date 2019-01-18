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

class QTimer;

class QgsGeometryWrapper;
class QgsGeometry;


/**
 * LocatorHighlight allows highlighting geometries
 * on the canvas for the specific needs of the locator.
 */
class LinePolygonHighlight : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY( QColor color MEMBER mColor NOTIFY colorChanged )
    Q_PROPERTY( unsigned int width MEMBER mWidth NOTIFY widthChanged )
    Q_PROPERTY( QgsQuickMapSettings *mapSettings MEMBER mMapSettings NOTIFY mapSettingsChanged )
    Q_PROPERTY( QgsGeometryWrapper *geometry READ geometry WRITE setGeometry NOTIFY qgsGeometryChanged )

  public:
    explicit LinePolygonHighlight( QQuickItem *parent = nullptr );

    void highlightGeometry( QgsGeometryWrapper *geometry );

    QgsGeometryWrapper *geometry() const;

    //! Sets the geometry, ownership is transfered
    void setGeometry( QgsGeometryWrapper *geometry );

  signals:
    void colorChanged();
    void widthChanged();
    void mapSettingsChanged();
    void qgsGeometryChanged();

  private:
    virtual QSGNode *updatePaintNode( QSGNode *n, UpdatePaintNodeData * ) override;

    QColor mColor;
    bool mDirty;
    unsigned int mWidth;
    QgsQuickMapSettings *mMapSettings;
    QgsGeometryWrapper *mGeometry;
    QgsGeometry mQgsGeometry;
    QTimer *mTimer = nullptr;
};

#endif // LOCATORHIGHLIGHT_H
