#include "vendorcommandhelper.h"

#include <QTextStream>

VendorCommandHelper::VendorCommandHelper( QIODevice *ioDevice, QObject *parent )
  : QObject( parent )
  , mIoDevice( ioDevice )
{
}

void VendorCommandHelper::setDevice( QIODevice *ioDevice )
{
  mIoDevice = ioDevice;
}

bool VendorCommandHelper::sendCommand( const QString &command )
{
  if ( !mIoDevice )
    return false;

  QByteArray data = command.toUtf8();
  if ( !data.endsWith( "\r\n" ) )
    data.append( "\r\n" );
  return mIoDevice->write( data ) == data.size();
}

bool VendorCommandHelper::configImuToAntOffset( double x, double y, double z,
                                                double stdx, double stdy, double stdz )
{
  QString cmd = QStringLiteral( "CONFIG IMUTOANT OFFSET %1 %2 %3 %4 %5 %6" )
                   .arg( x )
                   .arg( y )
                   .arg( z )
                   .arg( stdx )
                   .arg( stdy )
                   .arg( stdz );
  return sendCommand( cmd );
}

bool VendorCommandHelper::configInsSlantMeas()
{
  return sendCommand( QStringLiteral( "CONFIG INS SLANTMEAS" ) );
}

bool VendorCommandHelper::configInsDisable()
{
  return sendCommand( QStringLiteral( "CONFIG INS DISABLE" ) );
}

bool VendorCommandHelper::configInsReset()
{
  return sendCommand( QStringLiteral( "CONFIG INS RESET" ) );
}

bool VendorCommandHelper::configAntennaDeltaHen( double len )
{
  QString cmd = QStringLiteral( "CONFIG ANTENNADELTAHEN %1" ).arg( len );
  return sendCommand( cmd );
}

bool VendorCommandHelper::eraseImuParam()
{
  return sendCommand( QStringLiteral( "ERASE IMUPARAM" ) );
}

bool VendorCommandHelper::configInsReliability( int level )
{
  QString cmd = QStringLiteral( "CONFIG INSRELIABILITY %1" ).arg( level );
  return sendCommand( cmd );
}

bool VendorCommandHelper::saveConfig()
{
  return sendCommand( QStringLiteral( "SAVECONFIG" ) );
}

