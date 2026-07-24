

# File projectsource.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**projectsource.h**](projectsource_8h.md)

[Go to the documentation of this file](projectsource_8h.md)


```C++
/***************************************************************************
  projectsource.h - ProjectSource

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

#ifndef PROJECTSOURCE_H
#define PROJECTSOURCE_H

#include <QObject>

class ProjectSource : public QObject
{
    Q_OBJECT
  public:
    explicit ProjectSource( QObject *parent = nullptr );

    virtual ~ProjectSource() = default;

  signals:
    void projectOpened( const QString &path );
};

#endif // PROJECTSOURCE_H
```


