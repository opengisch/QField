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
 * \brief A class containing NTRIP server settings.
 * \ingroup core
 */
class NtripSettings
{
    Q_GADGET

    Q_PROPERTY( bool isValid READ isValid )

    Q_PROPERTY( QString host READ host )
    Q_PROPERTY( int port READ port )
    Q_PROPERTY( Protocol protocol READ protocol )
    Q_PROPERTY( QString mountPoint READ mountPoint )
    Q_PROPERTY( QString username READ username )
    Q_PROPERTY( QString password READ password )
    Q_PROPERTY( bool forwardNmeaSentences READ forwardNmeaSentences )

  public:
    //! NTRIP server protocol
    enum Protocol
    {
      NtripVersion1,
      NtripVersion2,
      NtripSsl,
    };

    Q_ENUM( Protocol )

    //! The NTRIP settings constructor
    NtripSettings();

    bool operator==( const NtripSettings &other ) const;
    bool operator!=( const NtripSettings &other ) const { return !operator==( other ); }

    //! Returns TRUE if the settings are complete enough to connect to an NTRIP server
    bool isValid() const;

    //! Returns the NTRIP server host
    QString host() const { return mHost; }

    //! Sets the NTRIP server host
    void setHost( const QString &host );

    //! Returns the NTRIP server port
    int port() const { return mPort; }

    //! Sets the NTRIP server port
    void setPort( int port );

    //! Returns the NTRIP server protocol
    Protocol protocol() const { return mProtocol; }

    //! Sets the NTRIP server protocol
    void setProtocol( Protocol protocol );

    //! Returns the NTRIP server mount point
    QString mountPoint() const { return mMountPoint; }

    //! Sets the NTRIP server mount point
    void setMountPoint( const QString &mountPoint );

    //! Returns the username to connect ot he NTRIP server
    QString username() const { return mUsername; }

    //! Sets the username to connect ot he NTRIP server
    void setUsername( const QString &username );

    //! Returns the password to connect ot he NTRIP server
    QString password() const { return mPassword; }

    //! Sets the password to connect ot he NTRIP server
    void setPassword( const QString &password );

    //! Returns TRUE if NMEA sentences will be forwarded to the NTRIP server
    bool forwardNmeaSentences() const { return mForwardNmeaSentences; }

    //! Sets whether NMEA sentences will be forwarded to the NTRIP server
    void setForwardNmeaSentences( bool forwardNmeaSentences );

    //! Return a map of settings
    Q_INVOKABLE QVariantMap toMap() const;

    //! Creates an NtripSettings object from a map of settings
    static NtripSettings fromMap( const QVariantMap &settings );

  private:
    QString mHost;
    int mPort = 2101;
    Protocol mProtocol = NtripVersion2;

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
