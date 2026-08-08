

# File authutils.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**authutils.h**](authutils_8h.md)

[Go to the documentation of this file](authutils_8h.md)


```C++
/***************************************************************************
  authutils.h - AuthUtils

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

#ifndef AUTHUTILS_H
#define AUTHUTILS_H

#include <QObject>

class AuthUtils : public QObject
{
    Q_OBJECT

  public:
    explicit AuthUtils( QObject *parent = nullptr );

    Q_INVOKABLE static bool hasAuthenticationConfigurations();

    Q_INVOKABLE static QVariantList authenticationConfigurationDetails( const QString &id = QString() );

    Q_INVOKABLE static bool isAuthenticationConfigurationAvailable( const QString &id );

    Q_INVOKABLE static void clearAuthenticationConfigurationCache( const QString &id = QString() );
};

#endif // AUTHUTILS_H
```


