

# File qfauthutils.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**qfauthutils.h**](qfauthutils_8h.md)

[Go to the documentation of this file](qfauthutils_8h.md)


```C++
/***************************************************************************
  qfauthutils.h - QfAuthUtils

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

#ifndef QFAUTHUTILS_H
#define QFAUTHUTILS_H

#include <QObject>

class QfAuthUtils : public QObject
{
    Q_OBJECT

  public:
    explicit QfAuthUtils( QObject *parent = nullptr );

    Q_INVOKABLE static bool hasAuthenticationConfigurations();

    Q_INVOKABLE static QVariantList authenticationConfigurationDetails( const QString &id = QString() );

    Q_INVOKABLE static bool isAuthenticationConfigurationAvailable( const QString &id );

    Q_INVOKABLE static void clearAuthenticationConfigurationCache( const QString &id = QString() );
};

#endif // QFAUTHUTILS_H
```


