#include "settings.h"

Settings::Settings( QObject *parent ) :
  QSettings( parent )
{
}

void Settings::setValue( const QString& key, const QVariant& value )
{
  QSettings::setValue( key, value );
  emit settingChanged( key );
}

QVariant Settings::value( const QString& key, const QVariant& defaultValue )
{
  return QSettings::value( key, defaultValue );
}
