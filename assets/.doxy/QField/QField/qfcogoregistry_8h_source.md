

# File qfcogoregistry.h

[**File List**](files.md) **>** [**cogo**](dir_bee6354f604373d3fe8d8d947aaacc65.md) **>** [**qfcogoregistry.h**](qfcogoregistry_8h.md)

[Go to the documentation of this file](qfcogoregistry_8h.md)


```C++
/***************************************************************************
                              qfcogoregistry.h
                              -------------------
              begin                : 2025.12.21
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

#ifndef QFCOGOREGISTRY_H
#define QFCOGOREGISTRY_H

#include "qfcogooperation.h"

#include <QMap>
#include <QObject>

class QfCogoRegistry : public QObject
{
    Q_OBJECT

  public:
    explicit QfCogoRegistry( QObject *parent = nullptr );
    ~QfCogoRegistry();

    bool registerOperation( QfCogoOperation *operation );

    QfCogoOperation *operation( const QString &name );

    QStringList availableOperations() const { return mOperations.keys(); }

    static void setInstance( QfCogoRegistry *instance ) { sCogoRegistryInterface = instance; }

    static QfCogoRegistry *instance() { return sCogoRegistryInterface; }

  signals:
    void operationAdded();

  private:
    QMap<QString, QfCogoOperation *> mOperations;

    static QfCogoRegistry *sCogoRegistryInterface;
};

#endif // QFCOGOREGISTRY_H
```


