

# File qfandroidresourcesource.cpp

[**File List**](files.md) **>** [**android**](dir_4a4823380b8154b853061c3a6b746fec.md) **>** [**qfandroidresourcesource.cpp**](qfandroidresourcesource_8cpp.md)

[Go to the documentation of this file](qfandroidresourcesource_8cpp.md)


```C++
/***************************************************************************
  qfandroidresourcesource.cpp - QfAndroidResourceSource

 ---------------------
 begin                : 5.7.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfandroidresourcesource.h"
#include "qfplatformutilities.h"

#include <QDir>
#include <QFile>
#include <QSettings>

QfAndroidResourceSource::QfAndroidResourceSource( const QString &prefix, QObject *parent )
  : QfResourceSource( parent, prefix, QString() )
  , mPrefix( prefix )
{
  connect( QfPlatformUtilities::instance(), &QfPlatformUtilities::resourceReceived, this, &QfAndroidResourceSource::handleResourceReceived );
  connect( QfPlatformUtilities::instance(), &QfPlatformUtilities::resourceCanceled, this, &QfAndroidResourceSource::handleResourceCanceled );
}

void QfAndroidResourceSource::handleResourceReceived( const QString &path )
{
  disconnect( QfPlatformUtilities::instance(), &QfPlatformUtilities::resourceReceived, this, &QfAndroidResourceSource::handleResourceReceived );
  disconnect( QfPlatformUtilities::instance(), &QfPlatformUtilities::resourceCanceled, this, &QfAndroidResourceSource::handleResourceCanceled );

  if ( QSettings().value( QStringLiteral( "QField/nativeCameraLaunched" ), false ).toBool() )
  {
    QSettings().setValue( QStringLiteral( "QField/nativeCameraLaunched" ), false );
  }

  if ( !path.isEmpty() )
  {
    QString relativePath = path;
    relativePath.remove( mPrefix );
    emit resourceReceived( relativePath );
  }
  else
  {
    emit resourceReceived( QString() );
  }
}

void QfAndroidResourceSource::handleResourceCanceled( const QString &message )
{
  disconnect( QfPlatformUtilities::instance(), &QfPlatformUtilities::resourceReceived, this, &QfAndroidResourceSource::handleResourceReceived );
  disconnect( QfPlatformUtilities::instance(), &QfPlatformUtilities::resourceCanceled, this, &QfAndroidResourceSource::handleResourceCanceled );

  if ( QSettings().value( QStringLiteral( "QField/nativeCameraLaunched" ), false ).toBool() )
  {
    QSettings().setValue( QStringLiteral( "QField/nativeCameraLaunched" ), false );
  }
}
```


