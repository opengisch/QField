

# File androidviewstatus.h

[**File List**](files.md) **>** [**android**](dir_4a4823380b8154b853061c3a6b746fec.md) **>** [**androidviewstatus.h**](androidviewstatus_8h.md)

[Go to the documentation of this file](androidviewstatus_8h.md)


```C++
/***************************************************************************
 androidviewstatus.h

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

#ifndef ANDROIDVIEWSTATUS_H
#define ANDROIDVIEWSTATUS_H

#include "viewstatus.h"

class AndroidViewStatus : public ViewStatus
{
  public:
    AndroidViewStatus( QObject *parent = nullptr );

  private slots:
    void handleResourceOpened( const QString &path );
    void handleResourceCanceled( const QString &message );
};

#endif // ANDROIDVIEWSTATUS_H
```


