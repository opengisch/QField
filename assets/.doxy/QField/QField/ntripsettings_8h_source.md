

# File ntripsettings.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**positioning**](dir_a0fecca670957e90f44ebd037fd955ed.md) **>** [**ntripsettings.h**](ntripsettings_8h.md)

[Go to the documentation of this file](ntripsettings_8h.md)


```C++
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
    enum Protocol
    {
      NtripVersion1,
      NtripVersion2,
    };

    Q_ENUM( Protocol )

    
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

    Q_INVOKABLE QVariantMap toMap() const;

    static NtripSettings fromMap( const QVariantMap &settings );

  private:
    QString mHost;
    int mPort = 2101;
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
```


