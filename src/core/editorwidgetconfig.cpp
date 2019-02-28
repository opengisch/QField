#include "editorwidgetconfig.h"

EditorWidgetConfig::EditorWidgetConfig( const QVariantMap &configuration )
{
  mConfiguration = configuration;
}

QVariantMap EditorWidgetConfig::configuration() const
{
  return mConfiguration;
}

QVariant EditorWidgetConfig::value( const QString &key ) const
{
  return mConfiguration.value( key );
}
