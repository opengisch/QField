/***************************************************************************
  authutils.cpp - AuthUtils

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

#include "authutils.h"

#include <qgsapplication.h>
#include <qgsauthmanager.h>

AuthUtils::AuthUtils( QObject *parent )
  : QObject( parent )
{
}

bool AuthUtils::isAuthenticationConfigurationAvailable( const QString &id )
{
  QgsAuthManager *authManager = QgsApplication::authManager();
  QgsAuthMethodConfigsMap configs = authManager->availableAuthMethodConfigs();
  return configs.contains( id );
}
