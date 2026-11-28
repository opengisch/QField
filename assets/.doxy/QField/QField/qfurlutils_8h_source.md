

# File qfurlutils.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**utils**](dir_a8053b3d69cfefb2b2e6ec7bf9dd5c1a.md) **>** [**qfurlutils.h**](qfurlutils_8h.md)

[Go to the documentation of this file](qfurlutils_8h.md)


```C++
/***************************************************************************
                        qfurlutils.h
                        ---------------
  begin                : Jun 2020
  copyright            : (C) 2020 by Ivan Ivanov
  email                : ivan@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFURLUTILS_H
#define QFURLUTILS_H

#include "qfield_core_export.h"

#include <QObject>

class QFIELD_CORE_EXPORT QfUrlUtils : public QObject
{
    Q_OBJECT

  public:
    explicit QfUrlUtils( QObject *parent = nullptr );

    static Q_INVOKABLE bool isRelativeOrFileUrl( const QString &url );

    static Q_INVOKABLE QUrl fromString( const QString &url );

    static Q_INVOKABLE QString toLocalFile( const QString &url );

    static Q_INVOKABLE QString urlDetail( const QString &url, const QString &detail );

    static Q_INVOKABLE QVariantMap getActionDetails( const QString &url );

    static Q_INVOKABLE QString createActionUrl( const QString &scheme, const QString &type, const QVariantMap &details );

    static Q_INVOKABLE QString createEncodedUrl( const QVariantMap &parameters );
};

#endif // QFURLUTILS_H
```


