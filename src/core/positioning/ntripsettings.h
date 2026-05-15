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

#ifndef NTRIPSETTINGS_H
#define NTRIPSETTINGS_H

#include <QObject>
#include <QString>


/**
 * \ingroup core
 */
class NtripSettings
{
    Q_GADGET

  public:
    //! NTRIP server version
    enum Protocol
    {
      NtripVersion1,
      NtripVersion2,
    };

    Q_ENUM( Protocol )

    /**
     * NtripSettings constructor
     */
    NtripSettings();

    bool operator==( const NtripSettings &other ) const;
    bool operator!=( const NtripSettings &other ) const { return !operator==( other ); }

    bool isValid() const;

    QString host() const { return mHost; }
    void setHost( const QString &host );

    int port() const { return mPort; }
    void setPort( int port );

    Protocol protocol() const { return mProtocol; }
    void setProtocol( Protocol protocol );

    QString mountPoint() const { return mMountPoint; }
    void setMountPoint( const QString &mountPoint );

    QString username() const { return mUsername; }
    void setUsername( const QString &username );

    QString password() const { return mPassword; }
    void setPassword( const QString &password );

    bool forwardNmeaSentences() const { return mForwardNmeaSentences; }
    void setForwardNmeaSentences( bool forwardNmeaSentences );

    QVariantMap toMap() const;
    static NtripSettings fromMap( const QVariantMap &settings );

  private:
    QString mHost;
    int mPort = 0;
    Protocol mProtocol = NtripVersion1;

    QString mMountPoint;
    QString mUsername;
    QString mPassword;

    bool mForwardNmeaSentences = true;

    friend QDataStream &operator<<( QDataStream &stream, const NtripSettings &settings );
    friend QDataStream &operator>>( QDataStream &stream, NtripSettings &settings );
};

Q_DECLARE_METATYPE( NtripSettings )

QDataStream &operator<<( QDataStream &stream, const NtripSettings &settings );
QDataStream &operator>>( QDataStream &stream, NtripSettings &settings );

#endif // NTRIPSETTINGS_H
