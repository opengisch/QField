/***************************************************************************
                            proxysettings.h
                              -------------------
              begin                : March 2026
              copyright            : (C) 2026
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PROXYSETTINGS_H
#define PROXYSETTINGS_H

#include "qfield_core_export.h"

#include <QNetworkProxy>
#include <QObject>

/**
 * \ingroup core
 * \brief Manages proxy configuration for all Qt/QGIS network traffic.
 *
 * Reads and writes proxy settings to QSettings under the
 * /qgis/networkAndProxy/ path (compatible with QGIS Desktop) and applies
 * them as the application-wide proxy via QNetworkProxy::setApplicationProxy().
 */
class QFIELD_CORE_EXPORT ProxySettings : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged )
    Q_PROPERTY( QString proxyType READ proxyType WRITE setProxyType NOTIFY proxyTypeChanged )
    Q_PROPERTY( QString host READ host WRITE setHost NOTIFY hostChanged )
    Q_PROPERTY( int port READ port WRITE setPort NOTIFY portChanged )
    Q_PROPERTY( QString user READ user WRITE setUser NOTIFY userChanged )
    Q_PROPERTY( QString password READ password WRITE setPassword NOTIFY passwordChanged )
    Q_PROPERTY( QString excludedUrls READ excludedUrls WRITE setExcludedUrls NOTIFY excludedUrlsChanged )

  public:
    explicit ProxySettings( QObject *parent = nullptr );

    bool enabled() const;
    void setEnabled( bool enabled );

    QString proxyType() const;
    void setProxyType( const QString &proxyType );

    QString host() const;
    void setHost( const QString &host );

    int port() const;
    void setPort( int port );

    QString user() const;
    void setUser( const QString &user );

    QString password() const;
    void setPassword( const QString &password );

    QString excludedUrls() const;
    void setExcludedUrls( const QString &excludedUrls );

    /**
     * Applies the current proxy settings as the application-wide proxy.
     */
    Q_INVOKABLE void applyProxy();

  signals:
    void enabledChanged();
    void proxyTypeChanged();
    void hostChanged();
    void portChanged();
    void userChanged();
    void passwordChanged();
    void excludedUrlsChanged();

  private:
    bool mEnabled = false;
    QString mProxyType;
    QString mHost;
    int mPort = 0;
    QString mUser;
    QString mPassword;
    QString mExcludedUrls;
};

#endif // PROXYSETTINGS_H
