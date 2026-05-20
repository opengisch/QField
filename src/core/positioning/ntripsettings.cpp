/***************************************************************************
  ntripsettings.h - NtripSettings
 ---------------------
 begin                : 2026/05/14
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

#include "ntripsettings.h"

#include <QVariantMap>

NtripSettings::NtripSettings()
{
}

bool NtripSettings::operator==( const NtripSettings &other ) const
{
  // clang-format off
  return mForwardNmeaSentences == other.mForwardNmeaSentences &&
         mProtocol == other.mProtocol &&
         mPort == other.mPort &&
         mHost == other.mHost &&
         mMountPoint == other.mMountPoint &&
         mUsername == other.mUsername &&
         mPassword == other.mPassword;
  // clang-format on
}

bool NtripSettings::isValid() const
{
  return mPort > 0 && !mHost.isEmpty() && !mMountPoint.isEmpty() && !mUsername.isEmpty() && !mPassword.isEmpty();
}

void NtripSettings::setHost( const QString &host )
{
  mHost = host;
}


void NtripSettings::setPort( int port )
{
  mPort = port;
}

void NtripSettings::setProtocol( Protocol protocol )
{
  mProtocol = protocol;
}

void NtripSettings::setMountPoint( const QString &mountPoint )
{
  mMountPoint = mountPoint;
}

void NtripSettings::setUsername( const QString &username )
{
  mUsername = username;
}

void NtripSettings::setPassword( const QString &password )
{
  mPassword = password;
}

void NtripSettings::setForwardNmeaSentences( bool forwardNmeaSentences )
{
  mForwardNmeaSentences = forwardNmeaSentences;
}

QVariantMap NtripSettings::toMap() const
{
  QVariantMap settings;
  settings[QStringLiteral( "host" )] = mHost;
  settings[QStringLiteral( "port" )] = mPort;
  settings[QStringLiteral( "protocol" )] = mProtocol;
  settings[QStringLiteral( "mountPoint" )] = mMountPoint;
  settings[QStringLiteral( "username" )] = mUsername;
  settings[QStringLiteral( "password" )] = mPassword;
  settings[QStringLiteral( "forwardNmeaSentences" )] = mForwardNmeaSentences;
  return settings;
}

NtripSettings NtripSettings::fromMap( const QVariantMap &settings )
{
  NtripSettings ntripSettings;
  ntripSettings.setHost( settings.value( QStringLiteral( "host" ) ).toString() );
  ntripSettings.setPort( settings.value( QStringLiteral( "port" ), 2101 ).toInt() );
  ntripSettings.setProtocol( static_cast<Protocol>( settings.value( QStringLiteral( "protocol" ), static_cast<int>( NtripVersion2 ) ).toInt() ) );
  ntripSettings.setMountPoint( settings.value( QStringLiteral( "mountPoint" ) ).toString() );
  ntripSettings.setUsername( settings.value( QStringLiteral( "username" ) ).toString() );
  ntripSettings.setPassword( settings.value( QStringLiteral( "password" ) ).toString() );
  ntripSettings.setForwardNmeaSentences( settings.value( QStringLiteral( "forwardNmeaSentences" ), true ).toBool() );
  return ntripSettings;
}

QDataStream &operator<<( QDataStream &stream, const NtripSettings &settings )
{
  return stream << settings.mHost << settings.mPort << settings.mProtocol << settings.mMountPoint
                << settings.mUsername << settings.mPassword << settings.mForwardNmeaSentences;
}

//cppcheck-suppress constParameter
QDataStream &operator>>( QDataStream &stream, NtripSettings &settings )
{
  return stream >> settings.mHost >> settings.mPort >> settings.mProtocol >> settings.mMountPoint
         >> settings.mUsername >> settings.mPassword >> settings.mForwardNmeaSentences;
}
