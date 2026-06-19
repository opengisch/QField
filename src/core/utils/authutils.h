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

/**
 * \ingroup core
 */
class AuthUtils : public QObject
{
    Q_OBJECT

  public:
    explicit AuthUtils( QObject *parent = nullptr );

    /**
     * Returns TRUE if the authentication configuration \a id is available.
     */
    Q_INVOKABLE static bool isAuthenticationConfigurationAvailable( const QString &id );
};

#endif // AUTHUTILS_H
