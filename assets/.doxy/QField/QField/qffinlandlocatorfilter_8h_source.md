

# File qffinlandlocatorfilter.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**locator**](dir_952e15d753357bc11da55d627a96f3ed.md) **>** [**qffinlandlocatorfilter.h**](qffinlandlocatorfilter_8h.md)

[Go to the documentation of this file](qffinlandlocatorfilter_8h.md)


```C++
/***************************************************************************
  qffinlandlocatorfilter.h

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


#ifndef QFFINLANDLOCATORFILTER_H
#define QFFINLANDLOCATORFILTER_H

#include <QObject>
#include <qgsabstractgeocoderlocatorfilter.h>


class QfLocatorModelSuperBridge;

class QfFinlandLocatorFilter : public QgsAbstractGeocoderLocatorFilter
{
    Q_OBJECT

  public:
    explicit QfFinlandLocatorFilter( QgsGeocoderInterface *geocoder, QfLocatorModelSuperBridge *locatorBridge );
    QfFinlandLocatorFilter *clone() const override;

    const QgsRectangle boundingBox() const { return mBoundingBox; }
    void setBoundingBox( const QgsRectangle boundingBox ) { mBoundingBox = boundingBox; }

  private:
    void handleGeocodeResult( const QgsGeocoderResult &result ) override;

    QfLocatorModelSuperBridge *mLocatorBridge = nullptr;
    QgsRectangle mBoundingBox;
};

#endif // QFFINLANDLOCATORFILTER_H
```


