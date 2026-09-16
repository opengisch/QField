

# File qfurlhandler.cpp

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qfurlhandler.cpp**](qfurlhandler_8cpp.md)

[Go to the documentation of this file](qfurlhandler_8cpp.md)


```C++
/***************************************************************************
              qfurlhandler.cpp
              -------------------
              begin                : April 2025
              copyright            : (C) 2025 by Mathieu Pellerin
              email                : mathieu (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfappinterface.h"
#include "qfurlhandler.h"

QfUrlHandler::QfUrlHandler( QfAppInterface *iface, QObject *parent )
  : QObject( parent ), mIface( iface )
{
}

void QfUrlHandler::handleUrl( const QUrl &url )
{
  if ( mIface )
  {
    mIface->executeAction( url.toString() );
  }
}
```


