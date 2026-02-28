

# File positioninginformationmodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**positioninginformationmodel.cpp**](positioninginformationmodel_8cpp.md)

[Go to the documentation of this file](positioninginformationmodel_8cpp.md)


```C++
#include "coordinatereferencesystemutils.h"
#include "geometryutils.h"
#include "positioninginformationmodel.h"

#include <QVariant>
#include <qgsunittypes.h>

PositioningInformationModel::PositioningInformationModel( QObject *parent )
  : QStandardItemModel( parent )
{
  connect( this, &QStandardItemModel::dataChanged, this, &PositioningInformationModel::onDataChanged );
}

void PositioningInformationModel::refreshData()
{
  if ( !mPositioningSource )
  {
    return;
  }

  const double distanceUnitFactor = QgsUnitTypes::fromUnitToUnitFactor( Qgis::DistanceUnit::Meters, distanceUnits() );
  const QString distanceUnitAbbreviation = QgsUnitTypes::toAbbreviatedString( distanceUnits() );
  const GnssPositionDetails deviceDetails = mPositioningSource->deviceDetails();
  const QList<QString> detailNames = deviceDetails.names();
  const QList<QVariant> detailValues = deviceDetails.values();

  updateCoordinates();

  const QString altitude = getAltitude( distanceUnitFactor, distanceUnitAbbreviation );
  const QString speed = getSpeed();
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

void PositioningInformationModel::getCoordinateLabels( QString &coord1Label, QString &coord2Label, bool coordinatesIsXY, bool isGeographic )
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

void PositioningInformationModel::getCoordinateValues( QString &coord1Value, QString &coord2Value, const QgsPoint &coordinates, bool coordinatesIsXY, bool isGeographic )
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

QString PositioningInformationModel::getAltitude( double distanceUnitFactor, const QString &distanceUnitAbbreviation )
{
  if ( positioningSource()->positionInformation().elevationValid() )
  {
    QString altitude = QLocale::system().toString( positioningSource()->projectedPosition().z() * distanceUnitFactor, 'f', 3 ) + ' ' + distanceUnitAbbreviation + ' ';
    QStringList details;

    if ( positioningSource()->elevationCorrectionMode() == PositioningSource::ElevationCorrectionMode::OrthometricFromGeoidFile )
    {
      details.push_back( tr( "grid" ) );
    }
    else if ( positioningSource()->elevationCorrectionMode() == PositioningSource::ElevationCorrectionMode::OrthometricFromDevice )
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

QString PositioningInformationModel::getSpeed()
{
  return positioningSource()->positionInformation().speedValid() ? QLocale::system().toString( positioningSource()->positionInformation().speed(), 'f', 3 ) + " m/s" : tr( "N/A" );
}

QString PositioningInformationModel::getHorizontalAccuracy( double distanceUnitFactor, const QString &distanceUnitAbbreviation )
{
  return positioningSource()->positionInformation().haccValid() ? QLocale::system().toString( positioningSource()->positionInformation().hacc() * distanceUnitFactor, 'f', 3 ) + ' ' + distanceUnitAbbreviation : tr( "N/A" );
}

QString PositioningInformationModel::getVerticalAccuracy( double distanceUnitFactor, const QString &distanceUnitAbbreviation )
{
  return positioningSource()->positionInformation().vaccValid() ? QLocale::system().toString( positioningSource()->positionInformation().vacc() * distanceUnitFactor, 'f', 3 ) + ' ' + distanceUnitAbbreviation : tr( "N/A" );
}

void PositioningInformationModel::updateInfo( const QString &name, const QVariant &value )
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


void PositioningInformationModel::updateCoordinates()
{
  const bool coordinatesIsXY = CoordinateReferenceSystemUtils::defaultCoordinateOrderForCrsIsXY( coordinateDisplayCrs() );
  const bool coordinatesIsGeographic = coordinateDisplayCrs().isGeographic();
  const QgsPoint coordinates = GeometryUtils::reprojectPoint( positioningSource()->sourcePosition(), CoordinateReferenceSystemUtils::wgs84Crs(), coordinateDisplayCrs() );

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

bool PositioningInformationModel::setData( const QModelIndex &index, const QVariant &value, int role )
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

QHash<int, QByteArray> PositioningInformationModel::roleNames() const
{
  QHash<int, QByteArray> names = QStandardItemModel::roleNames();
  names[NameRole] = "Name";
  names[ValueRole] = "Value";
  return names;
}

void PositioningInformationModel::onDataChanged( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles )
{
  Q_UNUSED( bottomRight )
  Q_UNUSED( roles )
}

Positioning *PositioningInformationModel::positioningSource() const
{
  return mPositioningSource;
}

void PositioningInformationModel::setPositioningSource( Positioning *positioningSource )
{
  if ( mPositioningSource == positioningSource )
    return;

  if ( mPositioningSource )
  {
    disconnect( mPositioningSource, &Positioning::positionInformationChanged, this, &PositioningInformationModel::refreshData );
    disconnect( mPositioningSource, &Positioning::deviceIdChanged, this, &PositioningInformationModel::softReset );
  }

  mPositioningSource = positioningSource;
  emit positioningSourceChanged();

  if ( mPositioningSource )
  {
    connect( mPositioningSource, &Positioning::positionInformationChanged, this, &PositioningInformationModel::refreshData );
    connect( mPositioningSource, &Positioning::deviceIdChanged, this, &PositioningInformationModel::softReset );
    refreshData();
  }
}

void PositioningInformationModel::softReset()
{
  if ( mPositioningSource->deviceId() == "" )
    removeRows( 6, rowCount() - 6 );
}

double PositioningInformationModel::antennaHeight() const
{
  return mAntennaHeight;
}

void PositioningInformationModel::setAntennaHeight( double antennaHeight )
{
  if ( qFuzzyCompare( mAntennaHeight, antennaHeight ) )
    return;
  mAntennaHeight = antennaHeight;
  emit antennaHeightChanged();

  const double distanceUnitFactor = QgsUnitTypes::fromUnitToUnitFactor( Qgis::DistanceUnit::Meters, distanceUnits() );
  const QString distanceUnitAbbreviation = QgsUnitTypes::toAbbreviatedString( distanceUnits() );
  const QString altitude = getAltitude( distanceUnitFactor, distanceUnitAbbreviation );
  updateInfo( tr( "Altitude" ), altitude );
}

Qgis::DistanceUnit PositioningInformationModel::distanceUnits() const
{
  return mDistanceUnits;
}

void PositioningInformationModel::setDistanceUnits( Qgis::DistanceUnit distanceUnits )
{
  if ( mDistanceUnits == distanceUnits )
    return;
  mDistanceUnits = distanceUnits;
  emit distanceUnitsChanged();
}

QgsCoordinateReferenceSystem PositioningInformationModel::coordinateDisplayCrs() const
{
  return mCoordinateDisplayCrs;
}

void PositioningInformationModel::setCoordinateDisplayCrs( const QgsCoordinateReferenceSystem &coordinateDisplayCrs )
{
  if ( mCoordinateDisplayCrs == coordinateDisplayCrs )
    return;
  mCoordinateDisplayCrs = coordinateDisplayCrs;
  emit coordinateDisplayCrsChanged();

  updateCoordinates();
}
```


