

# File translatormanager.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**translatormanager.h**](translatormanager_8h.md)

[Go to the documentation of this file](translatormanager_8h.md)


```C++
/***************************************************************************
 translatormanager.h - TranslatorManager

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

#ifndef TRANSLATORMANAGER_H
#define TRANSLATORMANAGER_H

#include <memory>

class QTranslator;

class TranslatorManager
{
  public:
    TranslatorManager();

    TranslatorManager( const TranslatorManager & ) = delete;

    TranslatorManager &operator=( const TranslatorManager & ) = delete;

    QTranslator *qfieldTranslator();

    QTranslator *qtTranslator();

    static TranslatorManager *instance();

  private:
    std::unique_ptr<QTranslator> mQFieldTranslator;
    std::unique_ptr<QTranslator> mQtTranslator;
};

#endif // TRANSLATORMANAGER_H
```


