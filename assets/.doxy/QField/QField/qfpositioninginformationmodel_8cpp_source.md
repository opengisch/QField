

# File qfpositioninginformationmodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**qfpositioninginformationmodel.cpp**](qfpositioninginformationmodel_8cpp.md)

[Go to the documentation of this file](qfpositioninginformationmodel_8cpp.md)


```C++
#include "qfcoordinatereferencesystemutils.h"
#include "qfgeometryutils.h"
#include "qfpositioninginformationmodel.h"

#include <QVariant>
#include <qgsunittypes.h>

QfPositioningInformationModel::QfPositioningInformationModel( QObject *parent )
  : QStandardItemModel( parent )
{
  connect( this, &QStandardItemModel::dataChanged, this, &QfPositioningInformationModel::onDataChanged );
}

void QfPositioningInformationModel::refreshData()
{
  if ( !mPositioningSource )
  {
    return;
  }

  const double distanceUnitFactor = QgsUnitTypes::fromUnitToUnitFactor( Qgis::DistanceUnit::Meters, mDistanceUnits );
  const QString distanceUnitAbbreviation = QgsUnitTypes::toAbbreviatedString( mDistanceUnits );
  const QfGnssPositionDetails deviceDetails = mPositioningSource->deviceDetails();
  const QList<QString> detailNames = deviceDetails.names();
  const QList<QVariant> detailValues = deviceDetails.values();

  updateCoordinates();

  const QString altitude = getAltitude( distanceUnitFactor, distanceUnitAbbreviation );
  const QString speed = getSpeed( distanceUnitFactor, distanceUnitAbbreviation );
  const QString hAccuracy = getHorizontalAccuracy( distanceUnitFactor, distanceUnitAbbreviation );
  const QString vAccuracy = getVerticalAccuracy( distanceUnitFactor, distanceUnitAbbreviation );

  updateInfo( tr( "Altitude" ), altitude );
  updateInfo( tr( "Speed" ), speed );
  updateInfo( tr( "H. Accuracy" ), hAccuracy );
  updateInfo( tr( "V. Accuracy" ), vAccuracy );

  for ( int i = 0; i < detailNames.size(); ++i )
  {
    updateInfo( detailNames[i], detailValues[i] );
  }
}

void QfPositioningInformationModel::getCoordinateLabels( QString &coord1Label, QString &coord2Label, bool coordinatesIsXY, bool isGeographic )
{
  if ( coordinatesIsXY )
  {
    coord1Label = isGeographic ? tr( "Lon" ) : tr( "X" );
    coord2Label = isGeographic ? tr( "Lat" ) : tr( "Y" );
  }
  else
  {
    coord1Label = isGeographic ? tr( "Lat" ) : tr( "Y" );
    coord2Label = isGeographic ? tr( "Lon" ) : tr( "X" );
  }
}

void QfPositioningInformationModel::getCoordinateValues( QString &coord1Value, QString &coord2Value, const QgsPoint &coordinates, bool coordinatesIsXY, bool isGeographic )
{
  if ( coordinatesIsXY )
  {
    if ( positioningSource()->positionInformation().longitudeValid() )
    {
      coord1Value = QLocale::system().toString( coordinates.x(), 'f', isGeographic ? 7 : 3 );
      coord2Value = QLocale::system().toString( coordinates.y(), 'f', isGeographic ? 7 : 3 );
    }
    else
    {
      coord1Value = coord2Value = tr( "N/A" );
    }
  }
  else
  {
    if ( positioningSource()->positionInformation().latitudeValid() )
    {
      coord1Value = QLocale::system().toString( coordinates.y(), 'f', isGeographic ? 7 : 3 );
      coord2Value = QLocale::system().toString( coordinates.x(), 'f', isGeographic ? 7 : 3 );
    }
    else
    {
      coord1Value = coord2Value = tr( "N/A" );
    }
  }
}

QString QfPositioningInformationModel::getAltitude( double distanceUnitFactor, const QString &distanceUnitAbbreviation )
{
  if ( positioningSource()->positionInformation().elevationValid() )
  {
    QString altitude = QLocale::system().toString( positioningSource()->projectedPosition().z() * distanceUnitFactor, 'f', 3 ) + ' ' + distanceUnitAbbreviation + ' ';
    QStringList details;

    if ( positioningSource()->elevationCorrectionMode() == QfPositioningSource::ElevationCorrectionMode::OrthometricFromGeoidFile )
    {
      details.push_back( tr( "grid" ) );
    }
    else if ( positioningSource()->elevationCorrectionMode() == QfPositioningSource::ElevationCorrectionMode::OrthometricFromDevice )
    {
      details.push_back( tr( "ortho." ) );
    }
    if ( antennaHeight() != 0 )
    {
      details.push_back( tr( "ant." ) );
    }
    if ( details.length() > 0 )
    {
      altitude += QString( " (%1)" ).arg( details.join( ", " ) );
    }
    return altitude;
  }
  return tr( "N/A" );
}

QString QfPositioningInformationModel::getSpeed( double distanceUnitFactor, const QString &distanceUnitAbbreviation )
{
  return positioningSource()->positionInformation().speedValid() ? QLocale::system().toString( positioningSource()->positionInformation().speed() * distanceUnitFactor, 'f', 3 ) + ' ' + tr( "%1/s" ).arg( distanceUnitAbbreviation ) : tr( "N/A" );
}

QString QfPositioningInformationModel::getHorizontalAccuracy( double distanceUnitFactor, const QString &distanceUnitAbbreviation )
{
  return positioningSource()->positionInformation().haccValid() ? QLocale::system().toString( positioningSource()->positionInformation().hacc() * distanceUnitFactor, 'f', 3 ) + ' ' + distanceUnitAbbreviation : tr( "N/A" );
}

QString QfPositioningInformationModel::getVerticalAccuracy( double distanceUnitFactor, const QString &distanceUnitAbbreviation )
{
  return positioningSource()->positionInformation().vaccValid() ? QLocale::system().toString( positioningSource()->positionInformation().vacc() * distanceUnitFactor, 'f', 3 ) + ' ' + distanceUnitAbbreviation : tr( "N/A" );
}

void QfPositioningInformationModel::updateInfo( const QString &name, const QVariant &value )
{
  for ( int row = 0; row < rowCount(); ++row )
  {
    QStandardItem *rowItem = item( row );

    if ( rowItem->data( NameRole ).toString() == name )
    {
      rowItem->setData( value.toString(), ValueRole );
      return;
    }
  }

  QStandardItem *nameItem = new QStandardItem( name );
  nameItem->setData( name, NameRole );
  nameItem->setData( value.toString(), ValueRole );
  insertRow( rowCount(), QList<QStandardItem *>() << nameItem );
}


void QfPositioningInformationModel::updateCoordinates()
{
  const bool coordinatesIsXY = QfCoordinateReferenceSystemUtils::defaultCoordinateOrderForCrsIsXY( coordinateDisplayCrs() );
  const bool coordinatesIsGeographic = coordinateDisplayCrs().isGeographic();
  const QgsPoint coordinates = QfGeometryUtils::reprojectPoint( positioningSource()->sourcePosition(), QfCoordinateReferenceSystemUtils::wgs84Crs(), coordinateDisplayCrs() );

  QString coord1Label, coord2Label;
  QString coord1Value, coord2Value;

  getCoordinateLabels( coord1Label, coord2Label, coordinatesIsXY, coordinatesIsGeographic );
  getCoordinateValues( coord1Value, coord2Value, coordinates, coordinatesIsXY, coordinatesIsGeographic );

  if ( rowCount() == 0 )
  {
    updateInfo( coord1Label, coord1Value );
    updateInfo( coord2Label, coord2Value );
  }
  else
  {
    QStandardItem *coordinates1 = item( 0 );
    QStandardItem *coordinates2 = item( 1 );

    coordinates1->setData( coord1Label, NameRole );
    coordinates1->setData( coord1Value, ValueRole );
    coordinates2->setData( coord2Label, NameRole );
    coordinates2->setData( coord2Value, ValueRole );
  }
}

bool QfPositioningInformationModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
  QStandardItem *rowItem = item( index.row() );
  if ( !rowItem )
  {
    return false;
  }

  switch ( role )
  {
    case NameRole:
      if ( rowItem->data( NameRole ) == value )
      {
        return false;
      }

      rowItem->setData( value, NameRole );
      return true;

    case ValueRole:

      if ( rowItem->data( ValueRole ) == value )
      {
        return false;
      }

      rowItem->setData( value, ValueRole );
      return true;

    default:
      break;
  }

  return false;
}

QHash<int, QByteArray> QfPositioningInformationModel::roleNames() const
{
  QHash<int, QByteArray> names = QStandardItemModel::roleNames();
  names[NameRole] = "Name";
  names[ValueRole] = "Value";
  return names;
}

void QfPositioningInformationModel::onDataChanged( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles )
{
  Q_UNUSED( bottomRight )
  Q_UNUSED( roles )
}

QfPositioning *QfPositioningInformationModel::positioningSource() const
{
  return mPositioningSource;
}

void QfPositioningInformationModel::setPositioningSource( QfPositioning *positioningSource )
{
  if ( mPositioningSource == positioningSource )
    return;

  if ( mPositioningSource )
  {
    disconnect( mPositioningSource, &QfPositioning::positionInformationChanged, this, &QfPositioningInformationModel::refreshData );
    disconnect( mPositioningSource, &QfPositioning::deviceIdChanged, this, &QfPositioningInformationModel::softReset );
  }

  mPositioningSource = positioningSource;
  emit positioningSourceChanged();

  if ( mPositioningSource )
  {
    connect( mPositioningSource, &QfPositioning::positionInformationChanged, this, &QfPositioningInformationModel::refreshData );
    connect( mPositioningSource, &QfPositioning::deviceIdChanged, this, &QfPositioningInformationModel::softReset );
    refreshData();
  }
}

void QfPositioningInformationModel::softReset()
{
  if ( mPositioningSource->deviceId() == "" )
    removeRows( 6, rowCount() - 6 );
}

double QfPositioningInformationModel::antennaHeight() const
{
  return mAntennaHeight;
}

void QfPositioningInformationModel::setAntennaHeight( double antennaHeight )
{
  if ( qFuzzyCompare( mAntennaHeight, antennaHeight ) )
    return;

  mAntennaHeight = antennaHeight;
  emit antennaHeightChanged();

  const double distanceUnitFactor = QgsUnitTypes::fromUnitToUnitFactor( Qgis::DistanceUnit::Meters, mDistanceUnits );
  const QString distanceUnitAbbreviation = QgsUnitTypes::toAbbreviatedString( mDistanceUnits );
  const QString altitude = getAltitude( distanceUnitFactor, distanceUnitAbbreviation );
  updateInfo( tr( "Altitude" ), altitude );
}

Qgis::DistanceUnit QfPositioningInformationModel::distanceUnits() const
{
  return mDistanceUnits;
}

void QfPositioningInformationModel::setDistanceUnits( Qgis::DistanceUnit distanceUnits )
{
  if ( mDistanceUnits == distanceUnits )
    return;

  mDistanceUnits = distanceUnits;
  emit distanceUnitsChanged();
}

QgsCoordinateReferenceSystem QfPositioningInformationModel::coordinateDisplayCrs() const
{
  return mCoordinateDisplayCrs;
}

void QfPositioningInformationModel::setCoordinateDisplayCrs( const QgsCoordinateReferenceSystem &coordinateDisplayCrs )
{
  if ( mCoordinateDisplayCrs == coordinateDisplayCrs )
    return;

  mCoordinateDisplayCrs = coordinateDisplayCrs;
  emit coordinateDisplayCrsChanged();

  updateCoordinates();
}
```


