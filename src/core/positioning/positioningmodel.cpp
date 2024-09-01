#include "coordinatereferencesystemutils.h"
#include "geometryutils.h"
#include "positioningmodel.h"

#include <QVariant>

PositioningModel::PositioningModel( QObject *parent )
  : QStandardItemModel( parent )
{
  connect( this, &QStandardItemModel::dataChanged, this, &PositioningModel::onDataChanged );
}

void PositioningModel::setupConnections()
{
  connect( mPositioningSource, &Positioning::positionInformationChanged, this, &PositioningModel::refreshData );
}

void PositioningModel::refreshData()
{
  if ( !mPositioningSource )
  {
    return;
  }

  //! Update all info dynamically :)
  //QList<QPair<QString, QVariant>> details = mPositioningSource->device()->details();
  // for ( int i = 0; i < details.size(); ++i )
  // {
  //   const QString key = details[i].first;
  //   const QVariant value = details[i].second;

  //   updateInfo( key, value );
  // }

  bool coordinatesIsXY = CoordinateReferenceSystemUtils::defaultCoordinateOrderForCrsIsXY( coordinateDisplayCrs() );
  bool coordinatesIsGeographic = coordinateDisplayCrs().isGeographic();
  QgsPoint coordinates = GeometryUtils::reprojectPoint( positioningSource()->sourcePosition(), CoordinateReferenceSystemUtils::wgs84Crs(), coordinateDisplayCrs() );


  QString labelLongitudeOrX = coordinatesIsXY ? coordinatesIsGeographic ? tr( "Lon" ) : tr( "X" ) : coordinatesIsGeographic ? tr( "Lat" )
                                                                                                                            : tr( "Y" );
  QString valueLongitudeOrLatitude = "";
  if ( coordinatesIsXY )
  {
    if ( positioningSource()->positionInformation().longitudeValid() )
    {
      valueLongitudeOrLatitude = QLocale::system().toString( coordinates.x(), 'f', coordinatesIsGeographic ? 7 : 3 );
    }
    else
    {
      valueLongitudeOrLatitude = tr( "N/A" );
    }
  }
  else
  {
    if ( positioningSource()->positionInformation().latitudeValid() )
    {
      valueLongitudeOrLatitude = QLocale::system().toString( coordinates.y(), 'f', coordinatesIsGeographic ? 7 : 3 );
    }
    else
    {
      valueLongitudeOrLatitude = tr( "N/A" );
    }
  }
  updateInfo( labelLongitudeOrX, valueLongitudeOrLatitude );

  QString labelLatitudeOrY = coordinatesIsXY ? coordinatesIsGeographic ? tr( "Lat" ) : tr( "Y" ) : coordinatesIsGeographic ? tr( "Lon" )
                                                                                                                           : tr( "X" );
  QString valueLatitudeOrLongitude = "";
  if ( coordinatesIsXY )
  {
    if ( positioningSource()->positionInformation().longitudeValid() )
    {
      valueLatitudeOrLongitude = QLocale::system().toString( coordinates.y(), 'f', coordinatesIsGeographic ? 7 : 3 );
    }
    else
    {
      valueLatitudeOrLongitude = tr( "N/A" );
    }
  }
  else
  {
    if ( positioningSource()->positionInformation().latitudeValid() )
    {
      valueLatitudeOrLongitude = QLocale::system().toString( coordinates.x(), 'f', coordinatesIsGeographic ? 7 : 3 );
    }
    else
    {
      valueLatitudeOrLongitude = tr( "N/A" );
    }
  }
  updateInfo( labelLatitudeOrY, valueLatitudeOrLongitude );
}

void PositioningModel::updateInfo( const QString &name, const QVariant &value )
{
  // Check if the item already exists
  for ( int row = 0; row < rowCount(); ++row )
  {
    QStandardItem *rowItem = item( row );
    if ( rowItem->data( VariableNameRole ).toString() == name )
    {
      // Item exists, update its value
      rowItem->setData( value.toString(), VariableValueRole );
      return;
    }
  }

  QStandardItem *nameItem = new QStandardItem( name );
  nameItem->setData( name, VariableNameRole );
  nameItem->setData( value.toString(), VariableValueRole );
  insertRow( rowCount(), QList<QStandardItem *>() << nameItem );
}


bool PositioningModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
  QStandardItem *rowItem = item( index.row() );
  if ( !rowItem )
  {
    return false;
  }

  switch ( role )
  {
    case VariableNameRole:
      if ( rowItem->data( VariableNameRole ) == value )
      {
        return false;
      }

      rowItem->setData( value, VariableNameRole );
      return true;

    case VariableValueRole:

      if ( rowItem->data( VariableValueRole ) == value )
      {
        return false;
      }

      rowItem->setData( value, VariableValueRole );
      return true;

    default:
      break;
  }

  return false;
}

QHash<int, QByteArray> PositioningModel::roleNames() const
{
  QHash<int, QByteArray> names = QStandardItemModel::roleNames();
  names[VariableNameRole] = "VariableName";
  names[VariableValueRole] = "VariableValue";
  return names;
}

void PositioningModel::onDataChanged( const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles )
{
  Q_UNUSED( bottomRight )
  Q_UNUSED( roles )
}

Positioning *PositioningModel::positioningSource() const
{
  return mPositioningSource;
}

void PositioningModel::setPositioningSource( Positioning *newPositioningSource )
{
  if ( mPositioningSource == newPositioningSource )
    return;
  mPositioningSource = newPositioningSource;
  emit positioningSourceChanged();
}

double PositioningModel::antennaHeight() const
{
  return mAntennaHeight;
}

void PositioningModel::setAntennaHeight( double newAntennaHeight )
{
  if ( qFuzzyCompare( mAntennaHeight, newAntennaHeight ) )
    return;
  mAntennaHeight = newAntennaHeight;
  emit antennaHeightChanged();
}

Qgis::DistanceUnit PositioningModel::distanceUnits() const
{
  return mDistanceUnits;
}

void PositioningModel::setDistanceUnits( Qgis::DistanceUnit newDistanceUnits )
{
  if ( mDistanceUnits == newDistanceUnits )
    return;
  mDistanceUnits = newDistanceUnits;
  emit distanceUnitsChanged();
}

QgsCoordinateReferenceSystem PositioningModel::coordinateDisplayCrs() const
{
  return mCoordinateDisplayCrs;
}

void PositioningModel::setCoordinateDisplayCrs( const QgsCoordinateReferenceSystem &newCoordinateDisplayCrs )
{
  if ( mCoordinateDisplayCrs == newCoordinateDisplayCrs )
    return;
  mCoordinateDisplayCrs = newCoordinateDisplayCrs;
  emit coordinateDisplayCrsChanged();
}
