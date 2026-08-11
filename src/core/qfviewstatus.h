/***************************************************************************
 qfviewstatus.h

 ---------------------
 begin                : 29.02.2020
 copyright            : (C) 2020 by david
 email                : david at opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFVIEWSTATUS_H
#define QFVIEWSTATUS_H

#include <QObject>

/**
 * \ingroup core
 */
class QfViewStatus : public QObject
{
    Q_OBJECT

  public:
    explicit QfViewStatus( QObject *parent = nullptr );

    virtual ~QfViewStatus() = default;

  signals:
    //! This signal is emitted when a status about the view action has been received
    void statusReceived( const QString &statusText );

    //! This signal is emitted when a view action is finished
    void finished();
};

#endif // QFVIEWSTATUS_H
