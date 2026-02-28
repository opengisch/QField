

# File finlandlocatorfilter.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**locator**](dir_952e15d753357bc11da55d627a96f3ed.md) **>** [**finlandlocatorfilter.h**](finlandlocatorfilter_8h.md)

[Go to the documentation of this file](finlandlocatorfilter_8h.md)


```C++
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
```


