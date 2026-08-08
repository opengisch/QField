

# File appcontroller.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**appcontroller.h**](appcontroller_8h.md)

[Go to the documentation of this file](appcontroller_8h.md)


```C++
/***************************************************************************
  appcontroller.h - AppController

---------------------
begin                : 19.7.2026
copyright            : (C) 2026 by Mohsen Dehghanzadeh
email                : info@opengis.ch
***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QList>
#include <QString>

class AppController
{
  public:
    virtual ~AppController() = default;

    virtual bool loadProjectFile( const QString &path, const QString &name = QString() ) = 0;

    virtual void reloadProjectFile() = 0;

    virtual void readProjectFile() = 0;

    virtual QString readProjectEntry( const QString &scope, const QString &key, const QString &def = QString() ) const = 0;

    virtual int readProjectNumEntry( const QString &scope, const QString &key, int def = 0 ) const = 0;

    virtual double readProjectDoubleEntry( const QString &scope, const QString &key, double def = 0.0 ) const = 0;

    virtual bool readProjectBoolEntry( const QString &scope, const QString &key, bool def = false ) const = 0;

    virtual bool print( const QString &layoutName ) = 0;

    virtual bool printAtlasFeatures( const QString &layoutName, const QList<long long> &featureIds ) = 0;

    virtual void setScreenDimmerTimeout( int timeoutSeconds ) = 0;

    virtual void clearProject() = 0;
};

#endif // APPCONTROLLER_H
```


