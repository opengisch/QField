

# File qfauthutils.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**qfauthutils.cpp**](qfauthutils_8cpp.md)

[Go to the documentation of this file](qfauthutils_8cpp.md)


```C++
/***************************************************************************
  qfauthutils.cpp - QfAuthUtils

 ---------------------
 begin                : 19.06.2026
 copyright            : (C) 2026 by Mathieu Pellerin
 email                : mathieu@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfauthutils.h"

#include <qgsapplication.h>
#include <qgsauthmanager.h>

QfAuthUtils::QfAuthUtils( QObject *parent )
  : QObject( parent )
{
}

bool QfAuthUtils::hasAuthenticationConfigurations()
{
  QgsAuthManager *authManager = QgsApplication::authManager();
  const QgsAuthMethodConfigsMap configs = authManager->availableAuthMethodConfigs();
  return !configs.isEmpty();
}

QVariantList QfAuthUtils::authenticationConfigurationDetails( const QString &id )
{
  QVariantList details;
  QgsAuthManager *authManager = QgsApplication::authManager();
  const QgsAuthMethodConfigsMap configs = authManager->availableAuthMethodConfigs();
  for ( const QgsAuthMethodConfig &config : configs )
  {
    if ( config.name() == QStringLiteral( "qfieldcloud-credentials" ) )
    {
      continue;
    }

    if ( id.isEmpty() || config.id() == id )
    {
      QVariantMap configDetails;
      configDetails.insert( QStringLiteral( "id" ), config.id() );
      configDetails.insert( QStringLiteral( "name" ), config.name() );
      configDetails.insert( QStringLiteral( "uri" ), config.uri() );
      details << configDetails;
    }
  }
  return details;
}

bool QfAuthUtils::isAuthenticationConfigurationAvailable( const QString &id )
{
  QgsAuthManager *authManager = QgsApplication::authManager();
  const QgsAuthMethodConfigsMap configs = authManager->availableAuthMethodConfigs();
  return configs.contains( id );
}

void QfAuthUtils::clearAuthenticationConfigurationCache( const QString &id )
{
  QgsAuthManager *authManager = QgsApplication::authManager();
  if ( !id.isEmpty() )
  {
    authManager->clearCachedConfig( id );
  }
  else
  {
    authManager->clearAllCachedConfigs();
  }
}
```


