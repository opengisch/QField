/***************************************************************************
  finlandlocatorfilter.h

 ---------------------
  Date                 : March 2021
  Copyright            : (C) 2021 by Mathieu Pellerin
  Email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef FINLANDLOCATORFILTER_H
#define FINLANDLOCATORFILTER_H

#include <QObject>

#include <qgsabstractgeocoderlocatorfilter.h>


class LocatorModelSuperBridge;

/**
 * FinlandLocatorFilter is a locator filter to search for addresses
 * in Finland through Digitrans's API.
 */
class FinlandLocatorFilter : public QgsAbstractGeocoderLocatorFilter
{
    Q_OBJECT

  public:

    explicit FinlandLocatorFilter( QgsGeocoderInterface *geocoder, LocatorModelSuperBridge *locatorBridge );
    FinlandLocatorFilter *clone() const override;

    const QgsRectangle boundingBox() const { return mBoundingBox; }
    void setBoundingBox( const QgsRectangle boundingBox ) { mBoundingBox = boundingBox; }

  private:
    void handleGeocodeResult( const QgsGeocoderResult &result ) override;

    LocatorModelSuperBridge *mLocatorBridge = nullptr;
    QgsRectangle mBoundingBox;
};

#endif // FINLANDLOCATORFILTER_H
