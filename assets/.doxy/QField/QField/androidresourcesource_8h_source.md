

# File androidresourcesource.h

[**File List**](files.md) **>** [**android**](dir_4a4823380b8154b853061c3a6b746fec.md) **>** [**androidresourcesource.h**](androidresourcesource_8h.md)

[Go to the documentation of this file](androidresourcesource_8h.md)


```C++
/***************************************************************************
  androidresourcesource.h - AndroidResourceSource

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
#ifndef ANDROIDRESOURCESOURCE_H
#define ANDROIDRESOURCESOURCE_H

#include "resourcesource.h"

class AndroidResourceSource : public ResourceSource
{
    Q_OBJECT

  public:
    explicit AndroidResourceSource( const QString &prefix, QObject *parent );

  private slots:
    void handleResourceReceived( const QString &path );
    void handleResourceCanceled( const QString &message );

  private:
    QString mPrefix;
};

#endif // ANDROIDRESOURCESOURCE_H
```


