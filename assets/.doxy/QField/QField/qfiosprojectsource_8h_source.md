

# File qfiosprojectsource.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**platforms**](dir_4ed093858358730befc336a35ce794a4.md) **>** [**ios**](dir_493ad750a5d458d3fc370128c0d017ff.md) **>** [**qfiosprojectsource.h**](qfiosprojectsource_8h.md)

[Go to the documentation of this file](qfiosprojectsource_8h.md)


```C++
/***************************************************************************
  qfiosprojectsource.h - QfIosProjectSource

  begin                : September 2021
  copyright            : (C) 2020 by Denis Rouzaud
  email                : denis@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QFIOSPROJECTSOURCE_H
#define QFIOSPROJECTSOURCE_H

#include "qfprojectsource.h"


class QfIosProjectSource : public QfProjectSource
{
    Q_OBJECT

  public:
    explicit QfIosProjectSource( QObject *parent = nullptr );

    QString projectFromFolder( const QString &folder ) const;

  public slots:
    void pickProject();

  private:
    QQuickItem *mParent = nullptr;
    class ProjectDelegateContainer;
    ProjectDelegateContainer *mDelegate = nullptr;
};


#endif // QFIOSPROJECTSOURCE_H
```


