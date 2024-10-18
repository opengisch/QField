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

/**
 * The NearFieldReader class allows for the scanning of near-field targets.
 * When text strings are present on targets, the strings will be automatically
 * read and a signal emitted with the string values.
 * \ingroup core
 */
class NearFieldReader : public QObject
{
    Q_OBJECT

    Q_PROPERTY( bool active READ active WRITE setActive NOTIFY activeChanged )
    Q_PROPERTY( bool targetInRange READ targetInRange NOTIFY targetInRangeChanged )
    Q_PROPERTY( QString readString READ readString NOTIFY readStringChanged )

  public:
    explicit NearFieldReader( QObject *parent = nullptr );
    ~NearFieldReader();

    /**
     * Returns whether the device is actively scanning and reading text strings from near-field targets.
     */
    bool active() const;

    /**
     * Sets whether the device is actively scanning and reading text strings from near-field targets.
     */
    void setActive( bool active );

    /**
     * Returns the last read near-field string.
     */
    QString readString() const;

    /**
     * Returns whether a near-field target is currently within range.
     */
    bool targetInRange() const;

    /**
     * Returns whether the current device can read near-field targets.
     */
    static bool isSupported();

  signals:
    //! Emitted when the active state has changed.
    void activeChanged();
    //! Emitted when the last text string read from a near-field target has changed.
    void readStringChanged();
    //! Emitted when a near-field target is within range of the device
    void targetInRangeChanged();
    //! Emitted when a near-field target has been detected.
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
