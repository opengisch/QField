

# File nearfieldreader.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**nearfieldreader.h**](nearfieldreader_8h.md)

[Go to the documentation of this file](nearfieldreader_8h.md)


```C++
/***************************************************************************
 nearfieldreader.h - NearFieldReader

 ---------------------
 begin                : 27.08.2023
 copyright            : (C) 2023 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef NEARFIELDREADER_H
#define NEARFIELDREADER_H

#ifdef WITH_NFC
#include <QNearFieldManager>
#else
#include <QObject>
#endif

class NearFieldReader : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool active READ active WRITE setActive NOTIFY activeChanged )
    Q_PROPERTY( bool targetInRange READ targetInRange NOTIFY targetInRangeChanged )
    Q_PROPERTY( QString readString READ readString NOTIFY readStringChanged )

  public:
    explicit NearFieldReader( QObject *parent = nullptr );
    ~NearFieldReader();

    bool active() const;

    void setActive( bool active );

    QString readString() const;

    bool targetInRange() const;

    static bool isSupported();

  signals:
    void activeChanged();
    void readStringChanged();
    void targetInRangeChanged();
    void targetDetected( const QString &targetId );

  private:
#ifdef WITH_NFC
    void handleTargetDetected( QNearFieldTarget *target );
    void handleTargetLost( QNearFieldTarget *target );
    void handleNdefMessageRead( const QNdefMessage &message );
    void handleTargetError( QNearFieldTarget::Error error, const QNearFieldTarget::RequestId &id );

    QNearFieldManager *mNearFieldManager = nullptr;
#endif
    bool mActive = false;
    bool mTargetInRange = false;
    QString mReadString;
};

#endif // NEARFIELDREADER_H
```


