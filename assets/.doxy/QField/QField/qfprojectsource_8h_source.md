

# File qfprojectsource.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfprojectsource.h**](qfprojectsource_8h.md)

[Go to the documentation of this file](qfprojectsource_8h.md)


```C++
/***************************************************************************
  qfprojectsource.h - QfProjectSource

 ---------------------
 begin                : 19.3.2018
 copyright            : (C) 2018 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFPROJECTSOURCE_H
#define QFPROJECTSOURCE_H

#include <QObject>

class QfProjectSource : public QObject
{
    Q_OBJECT
  public:
    explicit QfProjectSource( QObject *parent = nullptr );

    virtual ~QfProjectSource() = default;

  signals:
    void projectOpened( const QString &path );
};

#endif // QFPROJECTSOURCE_H
```


