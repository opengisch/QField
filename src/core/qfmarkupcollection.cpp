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
#include <qgsannotationlineitem.h>
#include <qgsannotationmarkeritem.h>
#include <qgsannotationpolygonitem.h>
#include <qgscolorutils.h>
#include <qgscurve.h>
#include <qgscurvepolygon.h>
#include <qgsfillsymbol.h>
#include <qgsfillsymbollayer.h>
#include <qgsgeometrycollection.h>
#include <qgslinesymbol.h>
#include <qgslinesymbollayer.h>
#include <qgsmarkersymbol.h>
#include <qgsmarkersymbollayer.h>
#include <qgssymbol.h>
#include <qgssymbollayer.h>


QfMarkupCollection::QfMarkupCollection( const QString &name, QObject *parent )
  : QObject( parent )
  , mUuid( QUuid::createUuid().toString( QUuid::WithoutBraces ) )
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

QString QfMarkupCollection::addItem( const QfMarkupItem &item )
{
  QfMarkupItem addedItem( item );
  while ( mItems.contains( addedItem.uuid() ) )
  {
    addedItem.mUuid = QUuid::createUuid().toString( QUuid::WithoutBraces );
  }

  mItems.insert( addedItem.uuid(), addedItem );
  mAnnotationLayer.reset();

  emit countChanged();
  emit itemsChanged();

  return addedItem.uuid();
}

void QfMarkupCollection::replaceItem( const QString &uuid, const QfMarkupItem &item )
{
  if ( mItems.contains( uuid ) )
  {
    mItems[uuid] = item;
    mItems[uuid].mUuid = uuid;

    mAnnotationLayer.reset();

    emit itemsChanged();
  }
}

void QfMarkupCollection::removeItem( const QString &uuid )
{
  if ( mItems.contains( uuid ) )
  {
    mItems.remove( uuid );

    mAnnotationLayer.reset();

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

  const QJsonObject metadataObject = geoJsonObject.value( QStringLiteral( "metadata" ) ).toObject();
  if ( metadataObject.contains( QStringLiteral( "uuid" ) ) )
  {
    mUuid = metadataObject.value( QStringLiteral( "uuid" ) ).toString();
    emit uuidChanged();
  }

  mItems.clear();

  mAnnotationLayer.reset();

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
      geometry = QgsGeometry::fromPolylineXY( polyline );
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
      geometry = QgsGeometry::fromPolygonXY( polygon );
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

    QfMarkupItem item( label, description, geometry, color );
    item.mUuid = uuid;
    mItems.insert( uuid, item );
  }

  emit countChanged();
  emit itemsChanged();

  return true;
}

bool QfMarkupCollection::writeGeoJson( const QString &path )
{
  const QFileInfo fi( path );
  const QDir dir( fi.absolutePath() );
  if ( !dir.exists() && !dir.mkpath( fi.absolutePath() ) )
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

  QJsonObject metadataObject;
  metadataObject.insert( QStringLiteral( "uuid" ), mUuid );
  geoJsonObject.insert( QStringLiteral( "metadata" ), metadataObject );

  QJsonArray featuresArray;
  for ( auto [uuid, item] : mItems.asKeyValueRange() )
  {
    const QgsGeometry &geometry = item.geometry();
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
        geometryCoordinates.append( QJsonValue( point.x() ) );
        geometryCoordinates.append( QJsonValue( point.y() ) );
        geometryType = QStringLiteral( "Point" );
        break;
      }

      case Qgis::GeometryType::Line:
      {
        const QgsPolylineXY polyline = geometry.asPolyline();
        for ( const QgsPointXY &point : polyline )
        {
          QJsonArray pointArray;
          pointArray.append( QJsonValue( point.x() ) );
          pointArray.append( QJsonValue( point.y() ) );
          geometryCoordinates.append( QJsonValue( pointArray ) );
        }
        geometryType = QStringLiteral( "LineString" );
        break;
      }

      case Qgis::GeometryType::Polygon:
      {
        const QgsPolygonXY polygon = geometry.asPolygon();
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
          geometryCoordinates.append( QJsonValue( partArray ) );
        }
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
    propertiesObject.insert( QStringLiteral( "uuid" ), QJsonValue( uuid ) );
    propertiesObject.insert( QStringLiteral( "label" ), QJsonValue( item.label() ) );
    propertiesObject.insert( QStringLiteral( "description" ), QJsonValue( item.description() ) );
    propertiesObject.insert( QStringLiteral( "color" ), QJsonValue( QgsColorUtils::colorToString( item.color() ) ) );

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

QgsAnnotationLayer *QfMarkupCollection::asAnnotationLayer()
{
  if ( mAnnotationLayer )
  {
    return mAnnotationLayer.get();
  }

  QgsAnnotationLayer::LayerOptions options( QgsProject::instance()->transformContext() );
  mAnnotationLayer.reset( new QgsAnnotationLayer( mName, options ) );
  mAnnotationLayer->setCrs( QgsCoordinateReferenceSystem( "EPSG:4326" ) );

  for ( const QfMarkupItem &item : mItems )
  {
    QgsSymbolLayerList symbolLayers;
    const QColor semiOpaqueColor = QColor( item.color().red(), item.color().green(), item.color().blue(), 100 );
    switch ( item.geometry().type() )
    {
      case Qgis::GeometryType::Polygon:
      {
        if ( const QgsCurvePolygon *polygon = qgsgeometry_cast<const QgsCurvePolygon *>( item.geometry().constGet() ) )
        {
          QgsSimpleFillSymbolLayer *symbolLayer = new QgsSimpleFillSymbolLayer( semiOpaqueColor, DEFAULT_SIMPLEFILL_STYLE, item.color(), DEFAULT_SIMPLEFILL_BORDERSTYLE, 0.6 ); // cppcheck-suppress constVariablePointer
          symbolLayers << symbolLayer;

          QgsAnnotationPolygonItem *polygonItem = new QgsAnnotationPolygonItem( polygon->clone() );
          polygonItem->setSymbol( new QgsFillSymbol( symbolLayers ) );
          mAnnotationLayer->addItem( polygonItem );
          break;
        }
      }

      case Qgis::GeometryType::Line:
      {
        if ( const QgsCurve *line = qgsgeometry_cast<const QgsCurve *>( item.geometry().constGet() ) )
        {
          QgsSimpleLineSymbolLayer *symbolLayer = new QgsSimpleLineSymbolLayer( item.color(), 0.6 ); // cppcheck-suppress constVariablePointer
          symbolLayers << symbolLayer;

          QgsAnnotationLineItem *lineItem = new QgsAnnotationLineItem( line->clone() );
          lineItem->setSymbol( new QgsLineSymbol( symbolLayers ) );
          mAnnotationLayer->addItem( lineItem );
          break;
        }
      }

      case Qgis::GeometryType::Point:
      {
        QgsSimpleMarkerSymbolLayer *symbolLayer = new QgsSimpleMarkerSymbolLayer( Qgis::MarkerShape::Circle, 2.6, 0.0, DEFAULT_SCALE_METHOD, semiOpaqueColor, item.color() ); // cppcheck-suppress constVariablePointer
        symbolLayer->setStrokeWidth( 0.6 );
        symbolLayers << symbolLayer;

        QgsAnnotationMarkerItem *markerItem = new QgsAnnotationMarkerItem( QgsPoint( item.geometry().asPoint() ) );
        markerItem->setSymbol( new QgsMarkerSymbol( symbolLayers ) );
        mAnnotationLayer->addItem( markerItem );
        break;
      }

      case Qgis::GeometryType::Unknown:
      case Qgis::GeometryType::Null:
        break;
    }
  }

  return mAnnotationLayer.get();
}

QgsVectorLayer *QfMarkupCollection::asVectorLayer()
{
  if ( mVectorLayer )
  {
    return mVectorLayer.get();
  }

  mVectorLayer.reset( new QgsVectorLayer( QStringLiteral( "GeometryCollection?crs=EPSG:4326&field=uuid:string&field=label:string&field=description:string&field=color:string" ), mName, QStringLiteral( "memory" ) ) );
  for ( const QfMarkupItem &item : mItems )
  {
    QgsFeature feature( mVectorLayer->fields() );
    feature.setAttribute( QStringLiteral( "uuid" ), item.uuid() );
    feature.setAttribute( QStringLiteral( "label" ), item.label() );
    feature.setAttribute( QStringLiteral( "description" ), item.description() );
    feature.setAttribute( QStringLiteral( "color" ), item.color().name( QColor::HexArgb ) );

    QgsGeometryCollection geometryCollection;
    geometryCollection.addGeometry( item.geometry().get()->clone() );
    feature.setGeometry( QgsGeometry( geometryCollection.clone() ) );

    mVectorLayer->dataProvider()->addFeature( feature, QgsFeatureSink::FastInsert );
  }

  connect( mVectorLayer.get(), &QgsVectorLayer::geometryChanged, this, &QfMarkupCollection::processGeometryChanged );
  return mVectorLayer.get();
}

void QfMarkupCollection::processGeometryChanged( QgsFeatureId fid, const QgsGeometry &geometry )
{
  QgsFeature feature = mVectorLayer->getFeature( fid );
  const QString uuid = feature.attribute( QStringLiteral( "uuid" ) ).toString();
  if ( mItems.contains( uuid ) )
  {
    if ( QgsWkbTypes::flatType( geometry.wkbType() ) == Qgis::WkbType::GeometryCollection )
    {
      const QgsGeometryCollection *geometryCollection = qgsgeometry_cast<const QgsGeometryCollection *>( geometry.constGet() );
      if ( !geometryCollection->isEmpty() )
      {
        QfMarkupItem item = mItems.value( uuid );
        item.mGeometry = QgsGeometry( geometryCollection->geometryN( 0 )->clone() );
        replaceItem( uuid, item );
      }
    }
  }
}

QfMarkupItem QfMarkupCollection::createItem( const QString &label, const QString &description, const QgsGeometry &geometry, const QColor &color )
{
  return QfMarkupItem( label, description, geometry, color );
}
