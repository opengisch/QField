/***************************************************************************
 qfmarkupcollection.cpp - QfMarkupCollection

 ---------------------
 begin                : 12.09.2026
 copyright            : (C) 2026 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfmarkupcollection.h"

#include <QDir>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <qgscolorutils.h>


QfMarkupCollection::QfMarkupCollection( const QString &name, QObject *parent )
  : QObject( parent )
  , mName( name )
{
}

void QfMarkupCollection::setName( const QString &name )
{
  if ( mName == name )
  {
    return;
  }

  mName = name;
  emit nameChanged();
}

void QfMarkupCollection::addItem( const QfMarkupItem &item )
{
  const QString uuid = QUuid::createUuid().toString( QUuid::WithoutBraces );
  mItems.insert( uuid, item );

  emit countChanged();
  emit itemsChanged();
}

void QfMarkupCollection::replaceItem( const QString &uuid, const QfMarkupItem &item )
{
  if ( mItems.contains( uuid ) )
  {
    mItems[uuid] = item;

    emit itemsChanged();
  }
}

void QfMarkupCollection::removeItem( const QString &uuid )
{
  if ( mItems.contains( uuid ) )
  {
    mItems.remove( uuid );

    emit countChanged();
    emit itemsChanged();
  }
}

bool QfMarkupCollection::readGeoJson( const QString &path )
{
  QFileInfo fi( path );
  if ( !fi.isFile() )
  {
    return false;
  }

  QFile geoJsonFile( path );
  if ( !geoJsonFile.open( QIODevice::ReadOnly ) )
  {
    return false;
  }

  QJsonParseError parseError;
  QJsonObject geoJsonObject = QJsonDocument::fromJson( geoJsonFile.readAll(), &parseError ).object();
  if ( parseError.error != QJsonParseError::NoError )
  {
    return false;
  }

  if ( geoJsonObject.value( QStringLiteral( "type" ) ).toString() != QStringLiteral( "FeatureCollection" ) )
  {
    return false;
  }

  mName = geoJsonObject.value( QStringLiteral( "name" ) ).toString();
  emit nameChanged();

  mItems.clear();
  const QJsonArray features = geoJsonObject.value( QStringLiteral( "features" ) ).toArray();
  for ( const QJsonValueConstRef &feature : features )
  {
    if ( !feature.isObject() )
    {
      continue;
    }

    const QJsonObject featureObject = feature.toObject();
    if ( featureObject.value( QStringLiteral( "type" ) ).toString() != QStringLiteral( "Feature" ) )
    {
      continue;
    }

    if ( !featureObject.contains( "geometry" ) || !featureObject.contains( "properties" ) )
    {
      continue;
    }

    QgsGeometry geometry;
    const QJsonObject geometryObject = featureObject.value( "geometry" ).toObject();
    const QString geometryType = geometryObject.value( QStringLiteral( "type" ) ).toString();
    const QJsonArray geometryCoordinates = geometryObject.value( QStringLiteral( "coordinates" ) ).toArray();
    if ( geometryType == QStringLiteral( "Point" ) && geometryCoordinates.size() >= 2 )
    {
      geometry = QgsGeometry::fromPointXY( QgsPointXY( geometryCoordinates.at( 0 ).toDouble(), geometryCoordinates.at( 1 ).toDouble() ) );
    }
    else if ( geometryType == QStringLiteral( "LineString" ) && geometryCoordinates.size() >= 2 )
    {
      QgsPolylineXY polyline;
      for ( const QJsonValueConstRef &point : geometryCoordinates )
      {
        if ( !point.isArray() )
        {
          continue;
        }

        const QJsonArray pointArray = point.toArray();
        if ( pointArray.size() < 2 )
        {
          continue;
        }

        polyline << QgsPointXY( pointArray.at( 0 ).toDouble(), pointArray.at( 1 ).toDouble() );
      }
      if ( polyline.size() >= 2 )
      {
        geometry = QgsGeometry::fromPolylineXY( polyline );
      }
    }
    else if ( geometryType == QStringLiteral( "Polygon" ) && geometryCoordinates.size() >= 1 )
    {
      QgsPolygonXY polygon;
      for ( const QJsonValueConstRef &part : geometryCoordinates )
      {
        QgsPolylineXY polyline;

        if ( !part.isArray() )
        {
          continue;
        }

        const QJsonArray partArray = part.toArray();
        if ( partArray.size() < 4 )
        {
          continue;
        }

        for ( const QJsonValueConstRef &point : partArray )
        {
          if ( !point.isArray() )
          {
            continue;
          }

          const QJsonArray pointArray = point.toArray();
          if ( pointArray.size() < 2 )
          {
            continue;
          }

          polyline << QgsPointXY( pointArray.at( 0 ).toDouble(), pointArray.at( 1 ).toDouble() );
        }

        polygon << polyline;
      }
    }

    if ( geometry.isEmpty() )
    {
      continue;
    }

    const QJsonObject propertiesObject = featureObject.value( "properties" ).toObject();
    const QString uuid = propertiesObject.value( QStringLiteral( "uuid" ) ).toString();
    if ( uuid.isEmpty() )
    {
      continue;
    }

    const QString label = propertiesObject.value( QStringLiteral( "label" ) ).toString();
    const QString description = propertiesObject.value( QStringLiteral( "description" ) ).toString();
    const QColor color = QgsColorUtils::colorFromString( propertiesObject.value( QStringLiteral( "color" ) ).toString() );

    mItems.insert( uuid, QfMarkupItem( label, description, geometry, color ) );
  }

  emit countChanged();
  emit itemsChanged();

  return true;
}

bool QfMarkupCollection::writeGeoJson( const QString &path )
{
  const QFileInfo fi( path );
  const QDir dir( fi.absolutePath() );
  if ( !dir.exists() && dir.mkpath( fi.absolutePath() ) )
  {
    return false;
  }

  QFile geoJsonFile( path );
  if ( !geoJsonFile.open( QIODevice::WriteOnly ) )
  {
    return false;
  }

  QJsonObject geoJsonObject;
  geoJsonObject.insert( QStringLiteral( "type" ), QJsonValue( QStringLiteral( "FeatureCollection" ) ) );
  geoJsonObject.insert( QStringLiteral( "name" ), QJsonValue( mName ) );

  QJsonArray featuresArray;
  for ( auto [uuid, item] : mItems.asKeyValueRange() )
  {
    const QgsGeometry geometry = item.geometry();
    if ( !QgsWkbTypes::isSingleType( geometry.wkbType() ) )
    {
      continue;
    }

    QString geometryType;
    QJsonArray geometryCoordinates;
    switch ( geometry.type() )
    {
      case Qgis::GeometryType::Point:
      {
        const QgsPointXY point = geometry.asPoint();
        QJsonArray pointArray;
        pointArray.append( QJsonValue( point.x() ) );
        pointArray.append( QJsonValue( point.y() ) );
        geometryCoordinates.append( QJsonValue( pointArray ) );
        geometryType = QStringLiteral( "Point" );
        break;
      }

      case Qgis::GeometryType::Line:
      {
        const QgsPolylineXY polyline = geometry.asPolyline();
        QJsonArray pointsArray;
        for ( const QgsPointXY &point : polyline )
        {
          QJsonArray pointArray;
          pointArray.append( QJsonValue( point.x() ) );
          pointArray.append( QJsonValue( point.y() ) );
          pointsArray.append( QJsonValue( pointArray ) );
        }
        geometryCoordinates.append( QJsonValue( pointsArray ) );
        geometryType = QStringLiteral( "LineString" );
        break;
      }

      case Qgis::GeometryType::Polygon:
      {
        const QgsPolygonXY polygon = geometry.asPolygon();
        QJsonArray partsArray;
        for ( const QgsPolylineXY &part : polygon )
        {
          QJsonArray partArray;
          for ( const QgsPointXY &point : part )
          {
            QJsonArray pointArray;
            pointArray.append( QJsonValue( point.x() ) );
            pointArray.append( QJsonValue( point.y() ) );
            partArray.append( QJsonValue( pointArray ) );
          }
          partsArray.append( QJsonValue( partArray ) );
        }
        geometryCoordinates.append( QJsonValue( partsArray ) );
        geometryType = QStringLiteral( "Polygon" );
        break;
      }

      case Qgis::GeometryType::Null:
      case Qgis::GeometryType::Unknown:
        break;
    }

    if ( geometryType.isEmpty() )
    {
      continue;
    }

    QJsonObject geometryObject;
    geometryObject.insert( QStringLiteral( "type" ), QJsonValue( geometryType ) );
    geometryObject.insert( QStringLiteral( "coordinates" ), QJsonValue( geometryCoordinates ) );

    QJsonObject propertiesObject;
    geometryObject.insert( QStringLiteral( "uuid" ), QJsonValue( uuid ) );
    geometryObject.insert( QStringLiteral( "label" ), QJsonValue( item.label() ) );
    geometryObject.insert( QStringLiteral( "description" ), QJsonValue( item.description() ) );
    geometryObject.insert( QStringLiteral( "color" ), QJsonValue( QgsColorUtils::colorToString( item.color() ) ) );

    QJsonObject featureObject;
    featureObject.insert( QStringLiteral( "type" ), QJsonValue( QStringLiteral( "Feature" ) ) );
    featureObject.insert( QStringLiteral( "geometry" ), QJsonValue( geometryObject ) );
    featureObject.insert( QStringLiteral( "properties" ), QJsonValue( propertiesObject ) );

    featuresArray.append( QJsonValue( featureObject ) );
  }

  geoJsonObject.insert( QStringLiteral( "features" ), QJsonValue( featuresArray ) );

  QJsonDocument geoJsonDocument( geoJsonObject );
  const QString geoJsonString = geoJsonDocument.toJson();
  geoJsonFile.write( geoJsonString.toUtf8() );

  return true;
}
