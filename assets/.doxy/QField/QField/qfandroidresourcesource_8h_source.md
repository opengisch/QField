

# File qfandroidresourcesource.h

[**File List**](files.md) **>** [**android**](dir_4a4823380b8154b853061c3a6b746fec.md) **>** [**qfandroidresourcesource.h**](qfandroidresourcesource_8h.md)

[Go to the documentation of this file](qfandroidresourcesource_8h.md)


```C++
/***************************************************************************
  qfandroidresourcesource.h - QfAndroidResourceSource

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
#ifndef QFANDROIDRESOURCESOURCE_H
#define QFANDROIDRESOURCESOURCE_H

#include "qfresourcesource.h"

class QfAndroidResourceSource : public QfResourceSource
{
    Q_OBJECT

  public:
    explicit QfAndroidResourceSource( const QString &prefix, QObject *parent );

  private slots:
    void handleResourceReceived( const QString &path );
    void handleResourceCanceled( const QString &message );

  private:
    QString mPrefix;
};

#endif // QFANDROIDRESOURCESOURCE_H
```


