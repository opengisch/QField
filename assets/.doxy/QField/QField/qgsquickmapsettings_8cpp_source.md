

# File qgsquickmapsettings.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qgsquick**](dir_19c3d7c8ba6dc95926fecc96295d1560.md) **>** [**qgsquickmapsettings.cpp**](qgsquickmapsettings_8cpp.md)

[Go to the documentation of this file](qgsquickmapsettings_8cpp.md)


```C++
/***************************************************************************
  qgsquickmapsettings.cpp
  --------------------------------------
  Date                 : 27.12.2014
  Copyright            : (C) 2014 by Matthias Kuhn
  Email                : matthias (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "qgsquickmapsettings.h"

#include <qgis.h>
#include <qgsmaplayer.h>
#include <qgsmaplayerstylemanager.h>
#include <qgsmessagelog.h>
#include <qgsproject.h>
#include <qgsprojectelevationproperties.h>
#include <qgsprojectviewsettings.h>

QgsQuickMapSettings::QgsQuickMapSettings( QObject *parent )
  : QObject( parent )
{
  // Connect signals for derived values
  connect( this, &QgsQuickMapSettings::destinationCrsChanged, this, &QgsQuickMapSettings::mapUnitsPerPointChanged );
  connect( this, &QgsQuickMapSettings::extentChanged, this, &QgsQuickMapSettings::mapUnitsPerPointChanged );
  connect( this, &QgsQuickMapSettings::outputSizeChanged, this, &QgsQuickMapSettings::mapUnitsPerPointChanged );
  connect( this, &QgsQuickMapSettings::extentChanged, this, &QgsQuickMapSettings::visibleExtentChanged );
  connect( this, &QgsQuickMapSettings::rotationChanged, this, &QgsQuickMapSettings::visibleExtentChanged );
  connect( this, &QgsQuickMapSettings::outputSizeChanged, this, &QgsQuickMapSettings::visibleExtentChanged );
}

void QgsQuickMapSettings::setProject( QgsProject *project )
{
  if ( project == mProject )
    return;

  // If we have already something connected, disconnect it!
  if ( mProject )
  {
    mProject->disconnect( this );
  }

  mProject = project;

  // Connect all signals
  if ( mProject )
  {
    connect( mProject, &QgsProject::readProject, this, &QgsQuickMapSettings::onReadProject );
    connect( mProject, &QgsProject::crsChanged, this, &QgsQuickMapSettings::onCrsChanged );

    setDestinationCrs( mProject->crs() );
    mMapSettings.setTransformContext( mProject->transformContext() );
    mMapSettings.setPathResolver( mProject->pathResolver() );
  }
  else
  {
    mMapSettings.setTransformContext( QgsCoordinateTransformContext() );
  }

  emit projectChanged();
}

QgsProject *QgsQuickMapSettings::project() const
{
  return mProject;
}

QgsCoordinateTransformContext QgsQuickMapSettings::transformContext() const
{
  return mMapSettings.transformContext();
}

QgsRectangle QgsQuickMapSettings::extent() const
{
  return mMapSettings.extent();
}

void QgsQuickMapSettings::setExtent( const QgsRectangle &extent, bool handleMargins )
{
  if ( applyExtent( mMapSettings, extent, handleMargins ) )
  {
    emit extentChanged();
  }
}

double QgsQuickMapSettings::computeScaleForExtent( const QgsRectangle &extent, bool handleMargins )
{
  QgsMapSettings settings( mMapSettings );
  applyExtent( settings, extent, handleMargins );
  return settings.scale();
}

bool QgsQuickMapSettings::applyExtent( QgsMapSettings &mapSettings, const QgsRectangle &extent, bool handleMargins )
{
  if ( handleMargins && ( !qgsDoubleNear( mRightMargin, 0.0 ) || !qgsDoubleNear( mBottomMargin, 0.0 ) ) )
  {
    const double rightMargin = mRightMargin * devicePixelRatio();
    const double bottomMargin = mBottomMargin * devicePixelRatio();
    const double outputSizeRatio = static_cast<double>( outputSize().width() ) / outputSize().height();
    const double extentRatio = extent.width() / extent.height();

    // Create an extent that matches the output size translated by current margin values
    QgsRectangle outputSizeExtent;
    if ( extentRatio > outputSizeRatio )
    {
      const double widthAdjustement = ( extent.width() / outputSize().width() ) * rightMargin;
      const double height = extent.width() / outputSizeRatio;
      const double heightAdjustment = ( height / outputSize().height() ) * bottomMargin;
      const double heightMiddle = extent.yMinimum() + ( extent.height() / 2 );

      outputSizeExtent.setXMinimum( extent.xMinimum() + widthAdjustement );
      outputSizeExtent.setXMaximum( extent.xMaximum() + widthAdjustement );
      outputSizeExtent.setYMinimum( heightMiddle - ( height / 2 ) - heightAdjustment );
      outputSizeExtent.setYMaximum( heightMiddle + ( height / 2 ) - heightAdjustment );
    }
    else
    {
      const double heightAdjustment = ( extent.height() / outputSize().height() ) * bottomMargin;
      const double width = extent.height() * outputSizeRatio;
      const double widthAdjustement = ( width / outputSize().width() ) * rightMargin;
      const double widthMiddle = extent.xMinimum() + ( extent.width() / 2 );

      outputSizeExtent.setXMinimum( widthMiddle - ( width / 2 ) + widthAdjustement );
      outputSizeExtent.setXMaximum( widthMiddle + ( width / 2 ) + widthAdjustement );
      outputSizeExtent.setYMinimum( extent.yMinimum() - heightAdjustment );
      outputSizeExtent.setYMaximum( extent.yMaximum() - heightAdjustment );
    }

    // Combine the extent with the translated output size extent
    QgsRectangle modifiedExtent = extent;
    modifiedExtent.combineExtentWith( outputSizeExtent );
    if ( modifiedExtent.isNull() )
      return false;

    // Set the extent to get an updated map units per pixel value
    const QgsRectangle originalExtent = mapSettings.extent();
    mapSettings.setExtent( modifiedExtent );
    modifiedExtent = mapSettings.extent();

    const QgsVector delta = QgsPointXY( extent.center() ) - mapSettings.extent().center();

    // Calculate base margin adjustments (split in half for centering)
    const double baseXAdjustment = -rightMargin * mapSettings.mapUnitsPerPixel() / 2;
    const double baseYAdjustment = bottomMargin * mapSettings.mapUnitsPerPixel() / 2;

    // Adjust margins based on rotation
    const double rotationRadians = mapSettings.rotation() * M_PI / 180.0;
    const double xAdjustment = baseXAdjustment * cos( rotationRadians ) - baseYAdjustment * sin( rotationRadians );
    const double yAdjustment = baseXAdjustment * sin( rotationRadians ) + baseYAdjustment * cos( rotationRadians );

    modifiedExtent.setXMinimum( modifiedExtent.xMinimum() + delta.x() - xAdjustment );
    modifiedExtent.setXMaximum( modifiedExtent.xMaximum() + delta.x() - xAdjustment );
    modifiedExtent.setYMinimum( modifiedExtent.yMinimum() + delta.y() - yAdjustment );
    modifiedExtent.setYMaximum( modifiedExtent.yMaximum() + delta.y() - yAdjustment );

    if ( originalExtent == modifiedExtent || modifiedExtent.isNull() )
      return false;

    mapSettings.setExtent( modifiedExtent );
  }
  else
  {
    if ( mapSettings.extent() == extent || extent.isNull() )
      return false;

    mapSettings.setExtent( extent );
  }

  return true;
}

QgsPoint QgsQuickMapSettings::getCenter( bool handleMargins ) const
{
  if ( handleMargins && ( !qgsDoubleNear( mRightMargin, 0.0 ) || !qgsDoubleNear( mBottomMargin, 0.0 ) ) )
  {
    // Get current extent
    QgsRectangle e = mMapSettings.extent();

    // Calculate base margin adjustments (split in half for centering)
    const double baseXAdjustment = -mRightMargin * devicePixelRatio() * mMapSettings.mapUnitsPerPixel() / 2;
    const double baseYAdjustment = mBottomMargin * devicePixelRatio() * mMapSettings.mapUnitsPerPixel() / 2;

    // Adjust margins based on rotation
    const double rotationRadians = mMapSettings.rotation() * M_PI / 180.0;
    const double xAdjustment = baseXAdjustment * cos( rotationRadians ) - baseYAdjustment * sin( rotationRadians );
    const double yAdjustment = baseXAdjustment * sin( rotationRadians ) + baseYAdjustment * cos( rotationRadians );

    // Calculate the adjusted center
    QgsPointXY adjustedCenter = e.center();
    adjustedCenter.setX( adjustedCenter.x() + xAdjustment );
    adjustedCenter.setY( adjustedCenter.y() + yAdjustment );

    return QgsPoint( adjustedCenter );
  }

  return QgsPoint( extent().center() );
}

void QgsQuickMapSettings::setCenter( const QgsPoint &center, bool handleMargins )
{
  if ( center.isEmpty() )
    return;

  const QgsVector delta = QgsPointXY( center ) - mMapSettings.extent().center();
  QgsRectangle e = mMapSettings.extent();
  if ( handleMargins && ( !qgsDoubleNear( mRightMargin, 0.0 ) || !qgsDoubleNear( mBottomMargin, 0.0 ) ) )
  {
    // Calculate base margin adjustments (split in half for centering)
    const double baseXAdjustment = -mRightMargin * devicePixelRatio() * mMapSettings.mapUnitsPerPixel() / 2;
    const double baseYAdjustment = mBottomMargin * devicePixelRatio() * mMapSettings.mapUnitsPerPixel() / 2;

    // Adjust margins based on rotation
    const double rotationRadians = mMapSettings.rotation() * M_PI / 180.0;
    const double xAdjustment = baseXAdjustment * cos( rotationRadians ) - baseYAdjustment * sin( rotationRadians );
    const double yAdjustment = baseXAdjustment * sin( rotationRadians ) + baseYAdjustment * cos( rotationRadians );

    e.setXMinimum( e.xMinimum() + delta.x() - xAdjustment );
    e.setXMaximum( e.xMaximum() + delta.x() - xAdjustment );
    e.setYMinimum( e.yMinimum() + delta.y() - yAdjustment );
    e.setYMaximum( e.yMaximum() + delta.y() - yAdjustment );
  }
  else
  {
    e.setXMinimum( e.xMinimum() + delta.x() );
    e.setXMaximum( e.xMaximum() + delta.x() );
    e.setYMinimum( e.yMinimum() + delta.y() );
    e.setYMaximum( e.yMaximum() + delta.y() );
  }

  setExtent( e );
}

void QgsQuickMapSettings::setCenter( const QPointF &center, bool handleMargins )
{
  return setCenter( QgsPoint( center ), handleMargins );
}

void QgsQuickMapSettings::setCenterToLayer( QgsMapLayer *layer, bool shouldZoom )
{
  Q_ASSERT( layer );

  const QgsRectangle extent = mapSettings().layerToMapCoordinates( layer, layer->extent() );

  if ( !extent.isEmpty() )
  {
    if ( shouldZoom )
      setExtent( extent );
    else
      setCenter( QgsPoint( extent.center() ) );
  }
}

void QgsQuickMapSettings::setExtentFromPoints( const QVariantList &points, const double &minimumScale, bool handleMargins )
{
  if ( points.isEmpty() )
    return;

  if ( points.size() == 1 )
  {
    QgsPoint p = points.at( 0 ).value<QgsPoint>();
    if ( !p.isEmpty() )
      setCenter( p, handleMargins );
    return;
  }

  QgsRectangle extent;
  for ( auto &point : points )
  {
    QgsPoint p = point.value<QgsPoint>();
    if ( p.isEmpty() )
      continue;
    extent.combineExtentWith( p.x(), p.y() );
  }

  QSize size = outputSize();
  size /= devicePixelRatio();
  double unitPerPoint = 0.0;
  double buffer = 0.0;
  if ( extent.width() > extent.height() )
  {
    unitPerPoint = extent.width() / size.width();
  }
  else
  {
    unitPerPoint = extent.height() / size.height();
  }
  buffer = 80 * unitPerPoint;

  if ( minimumScale > 0.0 )
  {
    QgsScaleCalculator calc;
    calc.setMapUnits( mMapSettings.destinationCrs().mapUnits() );
    calc.setDpi( outputDpi() );
    double scale = calc.calculate( extent, outputSize().width() );
    if ( scale < minimumScale )
    {
      extent.scale( minimumScale / scale );
    }
  }

  setExtent( extent.buffered( buffer ), handleMargins );
}

double QgsQuickMapSettings::mapUnitsPerPoint() const
{
  return mMapSettings.mapUnitsPerPixel() * devicePixelRatio();
}

QgsRectangle QgsQuickMapSettings::visibleExtent() const
{
  return mMapSettings.visibleExtent();
}

double QgsQuickMapSettings::scale() const
{
  return mMapSettings.scale();
}

QPointF QgsQuickMapSettings::coordinateToScreen( const QgsPoint &point ) const
{
  QgsPointXY pt( point.x(), point.y() );
  QgsPointXY pp = mMapSettings.mapToPixel().transform( pt );
  pp.setX( pp.x() / devicePixelRatio() );
  pp.setY( pp.y() / devicePixelRatio() );
  return pp.toQPointF();
}

QgsPoint QgsQuickMapSettings::screenToCoordinate( const QPointF &point ) const
{
  const QgsPointXY pp = mMapSettings.mapToPixel().toMapCoordinates( point.x() * devicePixelRatio(), point.y() * devicePixelRatio() );
  return QgsPoint( pp );
}

QgsMapSettings QgsQuickMapSettings::mapSettings() const
{
  return mMapSettings;
}

QSize QgsQuickMapSettings::outputSize() const
{
  return mMapSettings.outputSize();
}

void QgsQuickMapSettings::setOutputSize( QSize outputSize )
{
  outputSize.setWidth( std::ceil( outputSize.width() * devicePixelRatio() ) );
  outputSize.setHeight( std::ceil( outputSize.height() * devicePixelRatio() ) );
  if ( mMapSettings.outputSize() == outputSize )
    return;

  mMapSettings.setOutputSize( outputSize );
  emit outputSizeChanged();
}

double QgsQuickMapSettings::outputDpi() const
{
  return mMapSettings.outputDpi();
}

void QgsQuickMapSettings::setOutputDpi( double outputDpi )
{
  outputDpi *= devicePixelRatio();
  if ( qgsDoubleNear( mMapSettings.outputDpi(), outputDpi ) )
    return;

  mMapSettings.setOutputDpi( outputDpi );
  emit outputDpiChanged();
}

QgsCoordinateReferenceSystem QgsQuickMapSettings::destinationCrs() const
{
  return mMapSettings.destinationCrs();
}

void QgsQuickMapSettings::setDestinationCrs( const QgsCoordinateReferenceSystem &destinationCrs )
{
  if ( mMapSettings.destinationCrs() == destinationCrs )
    return;

  mMapSettings.setDestinationCrs( destinationCrs );
  emit destinationCrsChanged();
}

QList<QgsMapLayer *> QgsQuickMapSettings::layers() const
{
  return mMapSettings.layers();
}

void QgsQuickMapSettings::setLayers( const QList<QgsMapLayer *> &layers )
{
  mMapSettings.setLayers( layers );
  emit layersChanged();
}

#if 0
void MapSettings::setMapTheme( QgsProject *project, const QString &mapThemeName )
{
  QStringList layerIds;

  QgsMapThemeCollection::MapThemeRecord mapTheme = project->mapThemeCollection()->mapThemeState( mapThemeName );

  const QList<QgsMapThemeCollection::MapThemeLayerRecord> layerRecords { mapTheme.layerRecords() };

  for ( const QgsMapThemeCollection::MapThemeLayerRecord &record : layerRecords )
  {
    record.layer()->styleManager()->setCurrentStyle( mapTheme.perLayerCurrentStyle().value( layerId ) );

    layerIds << layerId;
  }

  mMapSettings.setLayers( layerIds );
  emit layersChanged();
}
#endif

void QgsQuickMapSettings::onCrsChanged()
{
  setDestinationCrs( mProject->crs() );
}

void QgsQuickMapSettings::onReadProject( const QDomDocument &doc )
{
  if ( mProject )
  {
    int red = mProject->readNumEntry( QStringLiteral( "Gui" ), QStringLiteral( "/CanvasColorRedPart" ), 255 );
    int green = mProject->readNumEntry( QStringLiteral( "Gui" ), QStringLiteral( "/CanvasColorGreenPart" ), 255 );
    int blue = mProject->readNumEntry( QStringLiteral( "Gui" ), QStringLiteral( "/CanvasColorBluePart" ), 255 );
    mMapSettings.setBackgroundColor( QColor( red, green, blue ) );

    const bool isTemporal = mProject->readNumEntry( QStringLiteral( "TemporalControllerWidget" ), QStringLiteral( "/NavigationMode" ), 0 ) != 0;
    const QString startString = QgsProject::instance()->readEntry( QStringLiteral( "TemporalControllerWidget" ), QStringLiteral( "/StartDateTime" ) );
    const QString endString = QgsProject::instance()->readEntry( QStringLiteral( "TemporalControllerWidget" ), QStringLiteral( "/EndDateTime" ) );
    mMapSettings.setTemporalRange( QgsDateTimeRange( QDateTime::fromString( startString, Qt::ISODateWithMs ),
                                                     QDateTime::fromString( endString, Qt::ISODateWithMs ) ) );
    mMapSettings.setIsTemporal( isTemporal );
  }

  QDomNodeList nodes = doc.elementsByTagName( "mapcanvas" );
  bool foundTheMapCanvas = false;
  for ( int i = 0; i < nodes.size(); i++ )
  {
    QDomNode node = nodes.item( i );
    QDomElement element = node.toElement();

    if ( element.hasAttribute( QStringLiteral( "name" ) ) && element.attribute( QStringLiteral( "name" ) ) == QStringLiteral( "theMapCanvas" ) )
    {
      foundTheMapCanvas = true;
      mMapSettings.readXml( node );
    }
  }
  if ( !foundTheMapCanvas )
  {
    mMapSettings.setDestinationCrs( mProject->crs() );
    mMapSettings.setExtent( mProject->viewSettings()->fullExtent() );
  }

  mMapSettings.setTransformContext( mProject->transformContext() );
  mMapSettings.setPathResolver( mProject->pathResolver() );
  mMapSettings.setElevationShadingRenderer( mProject->elevationShadingRenderer() );

  emit extentChanged();
  emit rotationChanged();
  emit destinationCrsChanged();
  emit outputSizeChanged();
  emit outputDpiChanged();
  emit layersChanged();
  emit backgroundColorChanged();
  emit temporalStateChanged();
  emit zRangeChanged();
}

double QgsQuickMapSettings::rotation() const
{
  return mMapSettings.rotation();
}

void QgsQuickMapSettings::setRotation( double rotation )
{
  if ( mMapSettings.rotation() == rotation )
    return;

  mMapSettings.setRotation( rotation );
  emit rotationChanged();
}

QColor QgsQuickMapSettings::backgroundColor() const
{
  return mMapSettings.backgroundColor();
}

void QgsQuickMapSettings::setBackgroundColor( const QColor &color )
{
  if ( mMapSettings.backgroundColor() == color )
    return;

  mMapSettings.setBackgroundColor( color );
  emit backgroundColorChanged();
}

qreal QgsQuickMapSettings::devicePixelRatio() const
{
  return mDevicePixelRatio;
}

void QgsQuickMapSettings::setDevicePixelRatio( const qreal &devicePixelRatio )
{
  mDevicePixelRatio = devicePixelRatio;
}

bool QgsQuickMapSettings::isTemporal() const
{
  return mMapSettings.isTemporal();
}

void QgsQuickMapSettings::setIsTemporal( bool temporal )
{
  mMapSettings.setIsTemporal( temporal );
  emit temporalStateChanged();
}

QDateTime QgsQuickMapSettings::temporalBegin() const
{
  return mMapSettings.temporalRange().begin();
}

void QgsQuickMapSettings::setTemporalBegin( const QDateTime &begin )
{
  if ( mMapSettings.temporalRange().begin() == begin )
    return;

  const QgsDateTimeRange range = mMapSettings.temporalRange();
  mMapSettings.setTemporalRange( QgsDateTimeRange( begin, range.end() ) );
  emit temporalStateChanged();
}

QDateTime QgsQuickMapSettings::temporalEnd() const
{
  return mMapSettings.temporalRange().end();
}

void QgsQuickMapSettings::setTemporalEnd( const QDateTime &end )
{
  if ( mMapSettings.temporalRange().end() == end )
    return;

  const QgsDateTimeRange range = mMapSettings.temporalRange();
  mMapSettings.setTemporalRange( QgsDateTimeRange( range.begin(), end ) );
  emit temporalStateChanged();
}

double QgsQuickMapSettings::zRangeLower() const
{
  return mMapSettings.zRange().lower();
}

void QgsQuickMapSettings::setZRangeLower( double lower )
{
  if ( mMapSettings.zRange().lower() == lower )
    return;

  const QgsDoubleRange zRange( lower, mMapSettings.zRange().upper() );
  mMapSettings.setZRange( zRange );
  emit zRangeChanged();
}

double QgsQuickMapSettings::zRangeUpper() const
{
  return mMapSettings.zRange().upper();
}

void QgsQuickMapSettings::setZRangeUpper( double upper )
{
  if ( mMapSettings.zRange().upper() == upper )
    return;

  const QgsDoubleRange zRange( mMapSettings.zRange().lower(), upper );
  mMapSettings.setZRange( zRange );
  emit zRangeChanged();
}

double QgsQuickMapSettings::bottomMargin() const
{
  return mBottomMargin;
}

void QgsQuickMapSettings::setBottomMargin( double bottomMargin )
{
  if ( mBottomMargin == bottomMargin )
    return;

  mBottomMargin = bottomMargin;
  emit bottomMarginChanged();
}

double QgsQuickMapSettings::rightMargin() const
{
  return mRightMargin;
}

void QgsQuickMapSettings::setRightMargin( double rightMargin )
{
  if ( mRightMargin == rightMargin )
    return;

  mRightMargin = rightMargin;
  emit rightMarginChanged();
}
```


