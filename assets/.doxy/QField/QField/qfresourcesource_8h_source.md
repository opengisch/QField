

# File qfresourcesource.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfresourcesource.h**](qfresourcesource_8h.md)

[Go to the documentation of this file](qfresourcesource_8h.md)


```C++
/***************************************************************************
  qfresourcesource.h - QfResourceSource

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
#ifndef QFRESOURCESOURCE_H
#define QFRESOURCESOURCE_H

#include <QObject>

class QfResourceSource : public QObject
{
    Q_OBJECT
  public:
    explicit QfResourceSource( QObject *parent = nullptr, const QString &prefix = QString(), const QString &resourceFilePath = QString() );

    virtual ~QfResourceSource();

  signals:

    void resourceReceived( const QString &path );

  private:
    QString mPrefix;

    QString mResourceFilePath;
};

#endif // QFRESOURCESOURCE_H
```


