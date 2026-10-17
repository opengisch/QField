

# File qfviewstatus.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfviewstatus.h**](qfviewstatus_8h.md)

[Go to the documentation of this file](qfviewstatus_8h.md)


```C++
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

class QfViewStatus : public QObject
{
    Q_OBJECT

  public:
    explicit QfViewStatus( QObject *parent = nullptr );

    virtual ~QfViewStatus() = default;

  signals:
    void statusReceived( const QString &statusText );

    void finished();
};

#endif // QFVIEWSTATUS_H
```


