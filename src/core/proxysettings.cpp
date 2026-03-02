/***************************************************************************
                            proxysettings.cpp
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

#include "proxysettings.h"

#include <QSettings>

ProxySettings::ProxySettings( QObject *parent )
  : QObject( parent )
{
  QSettings settings;
  mEnabled = settings.value( QStringLiteral( "/qgis/networkAndProxy/proxyEnabled" ), false ).toBool();
  mProxyType = settings.value( QStringLiteral( "/qgis/networkAndProxy/proxyType" ), QStringLiteral( "HttpProxy" ) ).toString();
  mHost = settings.value( QStringLiteral( "/qgis/networkAndProxy/proxyHost" ) ).toString();
  mPort = settings.value( QStringLiteral( "/qgis/networkAndProxy/proxyPort" ), 0 ).toInt();
  mUser = settings.value( QStringLiteral( "/qgis/networkAndProxy/proxyUser" ) ).toString();
  mPassword = settings.value( QStringLiteral( "/qgis/networkAndProxy/proxyPassword" ) ).toString();
  mExcludedUrls = settings.value( QStringLiteral( "/qgis/networkAndProxy/proxyExcludedUrls" ) ).toStringList().join( QStringLiteral( ", " ) );
}

bool ProxySettings::enabled() const
{
  return mEnabled;
}

void ProxySettings::setEnabled( bool enabled )
{
  if ( mEnabled == enabled )
    return;

  mEnabled = enabled;
  QSettings().setValue( QStringLiteral( "/qgis/networkAndProxy/proxyEnabled" ), mEnabled );
  emit enabledChanged();
  applyProxy();
}

QString ProxySettings::proxyType() const
{
  return mProxyType;
}

void ProxySettings::setProxyType( const QString &proxyType )
{
  if ( mProxyType == proxyType )
    return;

  mProxyType = proxyType;
  QSettings().setValue( QStringLiteral( "/qgis/networkAndProxy/proxyType" ), mProxyType );
  emit proxyTypeChanged();
  applyProxy();
}

QString ProxySettings::host() const
{
  return mHost;
}

void ProxySettings::setHost( const QString &host )
{
  if ( mHost == host )
    return;

  mHost = host;
  QSettings().setValue( QStringLiteral( "/qgis/networkAndProxy/proxyHost" ), mHost );
  emit hostChanged();
  applyProxy();
}

int ProxySettings::port() const
{
  return mPort;
}

void ProxySettings::setPort( int port )
{
  if ( mPort == port )
    return;

  mPort = port;
  QSettings().setValue( QStringLiteral( "/qgis/networkAndProxy/proxyPort" ), mPort );
  emit portChanged();
  applyProxy();
}

QString ProxySettings::user() const
{
  return mUser;
}

void ProxySettings::setUser( const QString &user )
{
  if ( mUser == user )
    return;

  mUser = user;
  QSettings().setValue( QStringLiteral( "/qgis/networkAndProxy/proxyUser" ), mUser );
  emit userChanged();
  applyProxy();
}

QString ProxySettings::password() const
{
  return mPassword;
}

void ProxySettings::setPassword( const QString &password )
{
  if ( mPassword == password )
    return;

  mPassword = password;
  QSettings().setValue( QStringLiteral( "/qgis/networkAndProxy/proxyPassword" ), mPassword );
  emit passwordChanged();
  applyProxy();
}

QString ProxySettings::excludedUrls() const
{
  return mExcludedUrls;
}

void ProxySettings::setExcludedUrls( const QString &excludedUrls )
{
  if ( mExcludedUrls == excludedUrls )
    return;

  mExcludedUrls = excludedUrls;
  const QStringList urlList = excludedUrls.split( QStringLiteral( "," ), Qt::SkipEmptyParts );
  QStringList trimmed;
  for ( const QString &url : urlList )
  {
    trimmed.append( url.trimmed() );
  }
  QSettings().setValue( QStringLiteral( "/qgis/networkAndProxy/proxyExcludedUrls" ), trimmed );
  emit excludedUrlsChanged();
  applyProxy();
}

void ProxySettings::applyProxy()
{
  if ( !mEnabled )
  {
    QNetworkProxy::setApplicationProxy( QNetworkProxy::NoProxy );
    return;
  }

  QNetworkProxy proxy;

  if ( mProxyType == QLatin1String( "Socks5Proxy" ) )
  {
    proxy.setType( QNetworkProxy::Socks5Proxy );
  }
  else if ( mProxyType == QLatin1String( "DefaultProxy" ) )
  {
    proxy.setType( QNetworkProxy::DefaultProxy );
  }
  else if ( mProxyType == QLatin1String( "HttpCachingProxy" ) )
  {
    proxy.setType( QNetworkProxy::HttpCachingProxy );
  }
  else if ( mProxyType == QLatin1String( "FtpCachingProxy" ) )
  {
    proxy.setType( QNetworkProxy::FtpCachingProxy );
  }
  else
  {
    proxy.setType( QNetworkProxy::HttpProxy );
  }

  proxy.setHostName( mHost );
  proxy.setPort( static_cast<quint16>( mPort ) );

  if ( !mUser.isEmpty() )
  {
    proxy.setUser( mUser );
  }
  if ( !mPassword.isEmpty() )
  {
    proxy.setPassword( mPassword );
  }

  QNetworkProxy::setApplicationProxy( proxy );
}
