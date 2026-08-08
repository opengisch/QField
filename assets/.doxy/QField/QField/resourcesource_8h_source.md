

# File resourcesource.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**resourcesource.h**](resourcesource_8h.md)

[Go to the documentation of this file](resourcesource_8h.md)


```C++
/***************************************************************************
  resourcesource.h - ResourceSource

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
#ifndef RESOURCESOURCE_H
#define RESOURCESOURCE_H

#include <QObject>

class ResourceSource : public QObject
{
    Q_OBJECT
  public:
    explicit ResourceSource( QObject *parent = nullptr, const QString &prefix = QString(), const QString &resourceFilePath = QString() );

    virtual ~ResourceSource();

  signals:

    void resourceReceived( const QString &path );

  private:
    QString mPrefix;

    QString mResourceFilePath;
};

#endif // RESOURCESOURCE_H
```


