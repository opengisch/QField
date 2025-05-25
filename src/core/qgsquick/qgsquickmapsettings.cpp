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
  if ( handleMargins )
  {
    const double rightMargin = mRightMargin * devicePixelRatio();
    const double bottomMargin = mBottomMargin * devicePixelRatio();
    const double outputSizeRatio = ( outputSize().width() - rightMargin ) / ( outputSize().height() - bottomMargin );
    const double extentRatio = extent.width() / extent.height();

    QgsRectangle modifiedExtent = extent;
    if ( extentRatio > outputSizeRatio )
    {
      const double heightAdjustment = ( extent.width() / outputSizeRatio - extent.height() ) / 2;
      modifiedExtent.setYMinimum( extent.yMinimum() - heightAdjustment );
      modifiedExtent.setYMaximum( extent.yMaximum() + heightAdjustment );
    }
    else
    {
      const double widthAdjustment = ( extent.height() * outputSizeRatio - extent.width() ) / 2;
      modifiedExtent.setXMinimum( extent.xMinimum() - widthAdjustment );
      modifiedExtent.setXMaximum( extent.xMaximum() + widthAdjustment );
    }

    if ( rightMargin > 0 )
    {
      modifiedExtent.setXMaximum( modifiedExtent.xMaximum() + ( modifiedExtent.width() / ( outputSize().width() - rightMargin ) ) * rightMargin );
    }
    if ( bottomMargin > 0 )
    {
      modifiedExtent.setYMinimum( modifiedExtent.yMinimum() - ( modifiedExtent.height() / ( outputSize().height() - bottomMargin ) ) * bottomMargin );
    }

    if ( mMapSettings.extent() == modifiedExtent || modifiedExtent.isNull() )
      return;

    mMapSettings.setExtent( modifiedExtent );
  }
  else
  {
    if ( mMapSettings.extent() == extent || extent.isNull() )
      return;

    mMapSettings.setExtent( extent );
  }
  emit extentChanged();
}

QgsPoint QgsQuickMapSettings::center() const
{
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
    const double baseXAdjustment = mRightMargin * devicePixelRatio() * mMapSettings.mapUnitsPerPixel() / 2;
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
  emit temporalStateChanged();
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
  const QgsDateTimeRange range = mMapSettings.temporalRange();
  mMapSettings.setTemporalRange( QgsDateTimeRange( range.begin(), end ) );
  emit temporalStateChanged();
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
