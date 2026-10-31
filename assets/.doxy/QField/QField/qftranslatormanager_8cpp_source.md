

# File qftranslatormanager.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qftranslatormanager.cpp**](qftranslatormanager_8cpp.md)

[Go to the documentation of this file](qftranslatormanager_8cpp.md)


```C++
/***************************************************************************
 qftranslatormanager.cpp - QfTranslatorManager

 ---------------------
 begin                : June 2025
 copyright            : (C) 2025 by Mohsen Dehghanzadeh
 email                : mohsen@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "qftranslatormanager.h"

#include <QTranslator>

QfTranslatorManager::QfTranslatorManager()
  : mQFieldTranslator( std::make_unique<QTranslator>() )
  , mQtTranslator( std::make_unique<QTranslator>() )
{
}

QTranslator *QfTranslatorManager::qfieldTranslator()
{
  return mQFieldTranslator.get();
}

QTranslator *QfTranslatorManager::qtTranslator()
{
  return mQtTranslator.get();
}

QfTranslatorManager *QfTranslatorManager::instance()
{
  static QfTranslatorManager *sInstance = new QfTranslatorManager();
  return sInstance;
}
```


