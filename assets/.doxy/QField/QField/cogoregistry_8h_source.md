

# File cogoregistry.h

[**File List**](files.md) **>** [**cogo**](dir_bee6354f604373d3fe8d8d947aaacc65.md) **>** [**cogoregistry.h**](cogoregistry_8h.md)

[Go to the documentation of this file](cogoregistry_8h.md)


```C++
/***************************************************************************
                              cogoregistry.h
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

#ifndef COGOREGISTRY_H
#define COGOREGISTRY_H

#include "cogooperation.h"

#include <QMap>
#include <QObject>

class CogoRegistry : public QObject
{
    Q_OBJECT

  public:
    explicit CogoRegistry( QObject *parent = nullptr );
    ~CogoRegistry();

    bool registerOperation( CogoOperation *operation );

    CogoOperation *operation( const QString &name );

    QStringList availableOperations() const { return mOperations.keys(); }

    static void setInstance( CogoRegistry *instance ) { sCogoRegistryInterface = instance; }

    static CogoRegistry *instance() { return sCogoRegistryInterface; }

  signals:
    void operationAdded();

  private:
    QMap<QString, CogoOperation *> mOperations;

    static CogoRegistry *sCogoRegistryInterface;
};

#endif // COGOREGISTRY_H
```


