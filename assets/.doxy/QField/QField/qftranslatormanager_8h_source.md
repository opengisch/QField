

# File qftranslatormanager.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qftranslatormanager.h**](qftranslatormanager_8h.md)

[Go to the documentation of this file](qftranslatormanager_8h.md)


```C++
/***************************************************************************
 qftranslatormanager.h - QfTranslatorManager

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

#ifndef QFTRANSLATORMANAGER_H
#define QFTRANSLATORMANAGER_H

#include <memory>

class QTranslator;

class QfTranslatorManager
{
  public:
    QfTranslatorManager();

    QfTranslatorManager( const QfTranslatorManager & ) = delete;

    QfTranslatorManager &operator=( const QfTranslatorManager & ) = delete;

    QTranslator *qfieldTranslator();

    QTranslator *qtTranslator();

    static QfTranslatorManager *instance();

  private:
    std::unique_ptr<QTranslator> mQFieldTranslator;
    std::unique_ptr<QTranslator> mQtTranslator;
};

#endif // QFTRANSLATORMANAGER_H
```


