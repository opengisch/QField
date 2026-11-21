

# File qfpeliasgeocoder.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfpeliasgeocoder.h**](qfpeliasgeocoder_8h.md)

[Go to the documentation of this file](qfpeliasgeocoder_8h.md)


```C++
/***************************************************************************
  qfpeliasgeocoder.h
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

#ifndef QFPELIASGEOCODER_H
#define QFPELIASGEOCODER_H

#include "qfield_core_export.h"

#include <QMutex>
#include <qgsgeocoder.h>

class QFIELD_CORE_EXPORT QfPeliasGeocoder : public QgsGeocoderInterface
{
  public:
    explicit QfPeliasGeocoder( const QString &endpoint = QString() );

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

#endif // QFPELIASGEOCODER_H
```


