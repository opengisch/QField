/***************************************************************************
  peliaslocatorfilter.h

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


#ifndef PELIASLOCATORFILTER_H
#define PELIASLOCATORFILTER_H

#include <QObject>

#include <qgsabstractgeocoderlocatorfilter.h>


class LocatorModelSuperBridge;

/**
 * PeliasLocatorFilter is a locator filter to search
 * for and display coordinates.
 */
class PeliasLocatorFilter : public QgsAbstractGeocoderLocatorFilter
{
    Q_OBJECT

  public:

    explicit PeliasLocatorFilter( QgsGeocoderInterface *geocoder, LocatorModelSuperBridge *locatorBridge );
    PeliasLocatorFilter *clone() const override;

    //void triggerResultFromAction( const QgsLocatorResult &result, const int actionId ) override;

  private:
    void handleGeocodeResult( const QgsGeocoderResult &result ) override;

    LocatorModelSuperBridge *mLocatorBridge = nullptr;
};

#endif // PELIASLOCATORFILTER_H
