

# File peliasgeocoder.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**peliasgeocoder.h**](peliasgeocoder_8h.md)

[Go to the documentation of this file](peliasgeocoder_8h.md)


```C++
/***************************************************************************
  peliasgeocoder.h
  ---------------
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

#ifndef PELIASGEOCODER_H
#define PELIASGEOCODER_H

#include "qfield_core_export.h"
#include "qgis_core.h"
#include "qgsgeocoder.h"

#include <QMutex>

class QFIELD_CORE_EXPORT PeliasGeocoder : public QgsGeocoderInterface
{
  public:
    explicit PeliasGeocoder( const QString &endpoint = QString() );

    Flags flags() const override;
    QgsFields appendedFields() const override;
    Qgis::WkbType wkbType() const override;
    QList<QgsGeocoderResult> geocodeString( const QString &string, const QgsGeocoderContext &context, QgsFeedback *feedback = nullptr ) const override;

    QUrl requestUrl( const QString &address, const QgsRectangle &bounds = QgsRectangle() ) const;

    QgsGeocoderResult jsonToResult( const QVariantMap &json ) const;

    QString endpoint() const;

    void setEndpoint( const QString &endpoint );

    double requestsPerSecond() const { return mRequestsPerSecond; }

    void setRequestsPerSecond( double number ) { mRequestsPerSecond = number; }

  private:
    QString mEndpoint;
    double mRequestsPerSecond = 10;

    static qint64 sLastRequestTimestamp;
};

#endif // PELIASGEOCODER_H
```


