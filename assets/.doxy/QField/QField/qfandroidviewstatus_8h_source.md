

# File qfandroidviewstatus.h

[**File List**](files.md) **>** [**android**](dir_4a4823380b8154b853061c3a6b746fec.md) **>** [**qfandroidviewstatus.h**](qfandroidviewstatus_8h.md)

[Go to the documentation of this file](qfandroidviewstatus_8h.md)


```C++
/***************************************************************************
 qfandroidviewstatus.h

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

#ifndef QFANDROIDVIEWSTATUS_H
#define QFANDROIDVIEWSTATUS_H

#include "qfviewstatus.h"

class QfAndroidViewStatus : public QfViewStatus
{
  public:
    QfAndroidViewStatus( QObject *parent = nullptr );

  private slots:
    void handleResourceOpened( const QString &path );
    void handleResourceCanceled( const QString &message );
};

#endif // QFANDROIDVIEWSTATUS_H
```


