/***************************************************************************
  peliasgeocoder.cpp
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

#include "peliasgeocoder.h"
#include "qgsblockingnetworkrequest.h"
#include "qgsgeocodercontext.h"
#include "qgslogger.h"
#include "qgsnetworkaccessmanager.h"

#include <QDateTime>
#include <QUrl>
#include <QUrlQuery>
#include <QMutex>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonArray>

QMutex PeliasGeocoder::sMutex;
typedef QMap< QUrl, QList< QgsGeocoderResult > > CachedGeocodeResult;
Q_GLOBAL_STATIC( CachedGeocodeResult, sCachedResults )
qint64 PeliasGeocoder::sLastRequestTimestamp = 0;

PeliasGeocoder::PeliasGeocoder( const QString &endpoint )
  : QgsGeocoderInterface()
  , mEndpoint( endpoint )
{
}

QgsGeocoderInterface::Flags PeliasGeocoder::flags() const
{
  return QgsGeocoderInterface::Flag::GeocodesStrings;
}

QgsFields PeliasGeocoder::appendedFields() const
{
  QgsFields fields;
  fields.append( QgsField( QStringLiteral( "name" ), QVariant::String ) );
  fields.append( QgsField( QStringLiteral( "gid" ), QVariant::String ) );
  fields.append( QgsField( QStringLiteral( "layer" ), QVariant::String ) );
  fields.append( QgsField( QStringLiteral( "country" ), QVariant::String ) );
  fields.append( QgsField( QStringLiteral( "region" ), QVariant::String ) );
  fields.append( QgsField( QStringLiteral( "localadmin" ), QVariant::String ) );
  fields.append( QgsField( QStringLiteral( "locality" ), QVariant::String ) );
  fields.append( QgsField( QStringLiteral( "neighbourhood" ), QVariant::String ) );
  fields.append( QgsField( QStringLiteral( "street" ), QVariant::String ) );
  fields.append( QgsField( QStringLiteral( "housenumber" ), QVariant::String ) );
  fields.append( QgsField( QStringLiteral( "postalcode" ), QVariant::String ) );
  return fields;
}

QgsWkbTypes::Type PeliasGeocoder::wkbType() const
{
  return QgsWkbTypes::Point;
}

QList<QgsGeocoderResult> PeliasGeocoder::geocodeString( const QString &string, const QgsGeocoderContext &context, QgsFeedback *feedback ) const
{
  if ( string.trimmed().isEmpty() )
    return QList<QgsGeocoderResult>();

  QgsRectangle bounds;
  if ( !context.areaOfInterest().isEmpty() )
  {
    QgsGeometry g = context.areaOfInterest();
    QgsCoordinateTransform ct( context.areaOfInterestCrs(), QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:4326" ) ), context.transformContext() );
    try
    {
      g.transform( ct );
      bounds = g.boundingBox();
    }
    catch ( QgsCsException & )
    {
      QgsDebugMsg( "Could not transform geocode bounds to WGS84" );
    }
  }

  const QUrl url = requestUrl( string, bounds );

  QMutexLocker locker( &sMutex );
  auto it = sCachedResults()->constFind( url );
  if ( it != sCachedResults()->constEnd() )
  {
    return *it;
  }

  while ( QDateTime::currentMSecsSinceEpoch() - sLastRequestTimestamp < 1000 / mRequestsPerSecond )
  {
    QThread::msleep( 50 );
    if ( feedback && feedback->isCanceled() )
      return QList<QgsGeocoderResult>();
  }

  QNetworkRequest request( url );
  QgsSetRequestInitiatorClass( request, QStringLiteral( "PeliasGeocoder" ) );

  QgsBlockingNetworkRequest newReq;
  const QgsBlockingNetworkRequest::ErrorCode errorCode = newReq.get( request, false, feedback );

  sLastRequestTimestamp = QDateTime::currentMSecsSinceEpoch();

  if ( errorCode != QgsBlockingNetworkRequest::NoError )
  {
    return QList<QgsGeocoderResult>() << QgsGeocoderResult::errorResult( newReq.errorMessage() );
  }

  // Parse data
  QJsonParseError err;
  QJsonDocument doc = QJsonDocument::fromJson( newReq.reply().content(), &err );
  if ( doc.isNull() )
  {
    return QList<QgsGeocoderResult>() << QgsGeocoderResult::errorResult( err.errorString() );
  }

  const QVariantList results = doc.object().toVariantMap().value( QStringLiteral( "features" ) ).toList();
  if ( results.isEmpty() )
  {
    sCachedResults()->insert( url, QList<QgsGeocoderResult>() );
    return QList<QgsGeocoderResult>();
  }

  QList< QgsGeocoderResult > matches;
  matches.reserve( results.size() );
  for ( const QVariant &result : results )
  {
    matches << jsonToResult( result.toMap() );
  }

  sCachedResults()->insert( url, matches );

  return matches;
}

QUrl PeliasGeocoder::requestUrl( const QString &address, const QgsRectangle &bounds ) const
{
  QUrl res( mEndpoint );
  QUrlQuery query;
  query.addQueryItem( QStringLiteral( "sources" ), QStringLiteral( "nlsfi,osm,oa" ) );
  query.addQueryItem( QStringLiteral( "size" ), QStringLiteral( "20" ) );
  if ( !bounds.isNull() )
  {
    query.addQueryItem( QStringLiteral( "focus.point.lat" ), QString::number( bounds.center().y() ) );
    query.addQueryItem( QStringLiteral( "focus.point.lon" ), QString::number( bounds.center().x() ) );
  }
  query.addQueryItem( QStringLiteral( "text" ), address );
  res.setQuery( query );

  return res;
}

QgsGeocoderResult PeliasGeocoder::jsonToResult( const QVariantMap &json ) const
{
  const QVariantMap properties = json.value( QStringLiteral( "properties" ) ).toMap();

  const QVariantList coordinates = json.value( QStringLiteral( "geometry" ) ).toMap().value( "coordinates" ).toList();
  const double latitude = coordinates.at( 1 ).toDouble();
  const double longitude = coordinates.at( 0 ).toDouble();

  const QgsGeometry geom = QgsGeometry::fromPointXY( QgsPointXY( longitude, latitude ) );

  QgsGeocoderResult res( properties.value( QStringLiteral( "label" ) ).toString(),
                         geom,
                         QgsCoordinateReferenceSystem( QStringLiteral( "EPSG:4326" ) ) );

  QVariantMap attributes;
  if ( properties.contains( QStringLiteral( "name" ) ) )
    attributes.insert( QStringLiteral( "name" ), properties.value( QStringLiteral( "name" ) ).toString() );
  if ( properties.contains( QStringLiteral( "gid" ) ) )
    attributes.insert( QStringLiteral( "gid" ), properties.value( QStringLiteral( "gid" ) ).toString() );
  if ( properties.contains( QStringLiteral( "layer" ) ) )
    attributes.insert( QStringLiteral( "layer" ), properties.value( QStringLiteral( "layer" ) ).toString() );

  if ( properties.contains( QStringLiteral( "country" ) ) )
    attributes.insert( QStringLiteral( "country" ), properties.value( QStringLiteral( "country" ) ).toString() );
  if ( properties.contains( QStringLiteral( "region" ) ) )
    attributes.insert( QStringLiteral( "region" ), properties.value( QStringLiteral( "region" ) ).toString() );
  if ( properties.contains( QStringLiteral( "localadmin" ) ) )
    attributes.insert( QStringLiteral( "localadmin" ), properties.value( QStringLiteral( "localadmin" ) ).toString() );
  if ( properties.contains( QStringLiteral( "locality" ) ) )
    attributes.insert( QStringLiteral( "locality" ), properties.value( QStringLiteral( "locality" ) ).toString() );
  if ( properties.contains( QStringLiteral( "neighbourhood" ) ) )
    attributes.insert( QStringLiteral( "neighbourhood" ), properties.value( QStringLiteral( "neighbourhood" ) ).toString() );
  if ( properties.contains( QStringLiteral( "street" ) ) )
    attributes.insert( QStringLiteral( "street" ), properties.value( QStringLiteral( "street" ) ).toString() );
  if ( properties.contains( QStringLiteral( "housenumber" ) ) )
    attributes.insert( QStringLiteral( "housenumber" ), properties.value( QStringLiteral( "housenumber" ) ).toString() );
  if ( properties.contains( QStringLiteral( "postalcode" ) ) )
    attributes.insert( QStringLiteral( "postalcode" ), properties.value( QStringLiteral( "postalcode" ) ).toString() );

  res.setAdditionalAttributes( attributes );
  return res;
}

QString PeliasGeocoder::endpoint() const
{
  return mEndpoint;
}

void PeliasGeocoder::setEndpoint( const QString &endpoint )
{
  mEndpoint = endpoint;
}
