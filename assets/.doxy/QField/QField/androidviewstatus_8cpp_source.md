

# File androidviewstatus.cpp

[**File List**](files.md) **>** [**android**](dir_4a4823380b8154b853061c3a6b746fec.md) **>** [**androidviewstatus.cpp**](androidviewstatus_8cpp.md)

[Go to the documentation of this file](androidviewstatus_8cpp.md)


```C++
/***************************************************************************
 androidviewstatus.cpp

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

#include "androidviewstatus.h"
#include "platformutilities.h"

AndroidViewStatus::AndroidViewStatus( QObject *parent )
  : ViewStatus( parent )
{
  connect( PlatformUtilities::instance(), &PlatformUtilities::resourceReceived, this, &AndroidViewStatus::handleResourceOpened );
  connect( PlatformUtilities::instance(), &PlatformUtilities::resourceCanceled, this, &AndroidViewStatus::handleResourceCanceled );
}

void AndroidViewStatus::handleResourceOpened( const QString &path )
{
  disconnect( PlatformUtilities::instance(), &PlatformUtilities::resourceReceived, this, &AndroidViewStatus::handleResourceOpened );
  disconnect( PlatformUtilities::instance(), &PlatformUtilities::resourceCanceled, this, &AndroidViewStatus::handleResourceCanceled );

  emit finished();
}

void AndroidViewStatus::handleResourceCanceled( const QString &message )
{
  disconnect( PlatformUtilities::instance(), &PlatformUtilities::resourceReceived, this, &AndroidViewStatus::handleResourceOpened );
  disconnect( PlatformUtilities::instance(), &PlatformUtilities::resourceCanceled, this, &AndroidViewStatus::handleResourceCanceled );

  emit statusReceived( message );
  emit finished();
}
```


