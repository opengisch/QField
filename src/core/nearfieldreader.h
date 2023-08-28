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

#include <QNearFieldManager>

#include <memory>

class NearFieldReader : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool active READ active WRITE setActive NOTIFY activeChanged )
    Q_PROPERTY( QString readString READ readString NOTIFY readStringChanged )

  public:
    explicit NearFieldReader( QObject *parent = nullptr );

    bool active() const;

    void setActive( bool active );

    QString readString() const;

  signals:
    void activeChanged();
    void readStringChanged();
    void targetDetected( const QString &targetId );

  private:
    void handleTargetDetected( QNearFieldTarget *target );
    void handleTargetLost( QNearFieldTarget *target );
    void handleNdefMessageRead( const QNdefMessage &message );
    void handleTargetError( QNearFieldTarget::Error error, const QNearFieldTarget::RequestId &id );

    std::unique_ptr<QNearFieldManager> mNearFieldManager;
    bool mActive = false;
    QString mReadString;
};

#endif // NEARFIELDREADER_H
