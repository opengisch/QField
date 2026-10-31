

# File qfsettings.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfsettings.cpp**](qfsettings_8cpp.md)

[Go to the documentation of this file](qfsettings_8cpp.md)


```C++
/***************************************************************************
                            qfsettings.cpp
                              -------------------
              begin                : 10.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "qfsettings.h"

#include <QDebug>

QfSettings::QfSettings( QObject *parent )
  : QSettings( parent )
{
}

void QfSettings::setValue( const QString &key, const QVariant &value )
{
  QSettings::setValue( key, value );
  emit settingChanged( key );
}

QVariant QfSettings::value( const QString &key, const QVariant &defaultValue )
{
  return QSettings::value( key, defaultValue );
}

bool QfSettings::valueBool( const QString &key, bool defaultValue )
{
  return QSettings::value( key, defaultValue ).toBool();
}

int QfSettings::valueInt( const QString &key, int defaultValue )
{
  return QSettings::value( key, defaultValue ).toInt();
}

void QfSettings::remove( const QString &key )
{
  QSettings::remove( key );
}

void QfSettings::sync()
{
  QSettings::sync();
}
```


