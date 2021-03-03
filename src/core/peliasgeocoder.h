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

#include "qgis_core.h"
#include "qgsgeocoder.h"

#include <QMutex>

/**
 * \ingroup core
 * \brief A geocoder which uses the Pelias geocoding API to retrieve results.
 *
 * This geocoder utilizes the Pelias geocoding API in order to geocode
 * strings from an endpoint server.
 *
 * \since QField 1.9
*/
class CORE_EXPORT PeliasGeocoder : public QgsGeocoderInterface
{

  public:

    /**
     * Constructor for PeliasGeocoder.
     *
     * The \a endpoint argument is used to specify an endpoint to use for request.
     */
    PeliasGeocoder( const QString &endpoint = QString() );

    Flags flags() const override;
    QgsFields appendedFields() const override;
    QgsWkbTypes::Type wkbType() const override;
    QList< QgsGeocoderResult > geocodeString( const QString &string, const QgsGeocoderContext &context, QgsFeedback *feedback = nullptr ) const override;

    /**
     * Returns the URL generated for geocoding the specified \a address.
     */
    QUrl requestUrl( const QString &address, const QgsRectangle &bounds = QgsRectangle() ) const;

    /**
     * Converts a JSON result returned from the Pelias service to a geocoder result object.
     */
    QgsGeocoderResult jsonToResult( const QVariantMap &json ) const;

    /**
     * Returns the API endpoint used for requests.
     *
     * \see setEndpoint()
     */
    QString endpoint() const;

    /**
     * Sets a specific API \a endpoint to use for requests. This is for internal testing purposes only.
     *
     * \see endpoint()
     */
    void setEndpoint( const QString &endpoint );

    /**
     * Returns the number of requests per seconds to the endpoint.
     *
     * \see setRequestsPerSecond()
     */
    double requestsPerSecond() const { return mRequestsPerSecond; }

    /**
     * Sets the \a number of request per seconds to the endpoint.
     *
     * \see requestsPerSecond()
     */
    void setRequestsPerSecond( double number ) { mRequestsPerSecond = number; }

  private:

    QString mEndpoint;
    double mRequestsPerSecond = 10;

    static QMutex sMutex;
    static QMap< QUrl, QList< QgsGeocoderResult > > sCachedResults;

    static qint64 sLastRequestTimestamp;

};

#endif // PELIASGEOCODER_H
