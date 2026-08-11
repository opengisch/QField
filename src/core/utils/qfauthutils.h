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

/**
 * \ingroup core
 */
class QfAuthUtils : public QObject
{
    Q_OBJECT

  public:
    explicit QfAuthUtils( QObject *parent = nullptr );

    /**
     * Returns TRUE if the authentication manager has one or more stored configurations.
     */
    Q_INVOKABLE static bool hasAuthenticationConfigurations();

    /**
     * Returns authentication configuration \a id details. If no id is provided,
     * details for all authentication configuration will be returned.
     */
    Q_INVOKABLE static QVariantList authenticationConfigurationDetails( const QString &id = QString() );

    /**
     * Returns TRUE if the authentication configuration \a id is available.
     */
    Q_INVOKABLE static bool isAuthenticationConfigurationAvailable( const QString &id );

    /**
     * Clears any cache associated to the authentication configuration \a id. If no id is
     * provided, cache for all authentication configurations will be cleared.
     */
    Q_INVOKABLE static void clearAuthenticationConfigurationCache( const QString &id = QString() );
};

#endif // QFAUTHUTILS_H
