

# File qfieldurlhandler.h

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qfieldurlhandler.h**](qfieldurlhandler_8h.md)

[Go to the documentation of this file](qfieldurlhandler_8h.md)


```C++
/***************************************************************************
              qfieldurlhandler.h
              -------------------
              begin                : April 2025
              copyright            : (C) 2025 by Mathieu Pellerin
              email                : mathieu (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFIELDURLHANDLER_H
#define QFIELDURLHANDLER_H

#include <QObject>

class AppInterface;

class QFieldUrlHandler : public QObject
{
    Q_OBJECT

  public:
    QFieldUrlHandler( AppInterface *iface = nullptr, QObject *parent = nullptr );

  public slots:
    void handleUrl( const QUrl &url );

  private:
    AppInterface *mIface = nullptr;
};

#endif // QFIELDURLHANDLER_H
```


