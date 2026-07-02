

# File viewstatus.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**viewstatus.h**](viewstatus_8h.md)

[Go to the documentation of this file](viewstatus_8h.md)


```C++
/***************************************************************************
 viewstatus.h

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

#ifndef VIEWSTATUS_H
#define VIEWSTATUS_H

#include <QObject>

class ViewStatus : public QObject
{
    Q_OBJECT

  public:
    explicit ViewStatus( QObject *parent = nullptr );

    virtual ~ViewStatus() = default;

  signals:
    void statusReceived( const QString &statusText );

    void finished();
};

#endif // VIEWSTATUS_H
```


