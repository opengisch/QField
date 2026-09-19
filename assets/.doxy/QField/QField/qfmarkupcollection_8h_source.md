

# File qfmarkupcollection.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfmarkupcollection.h**](qfmarkupcollection_8h.md)

[Go to the documentation of this file](qfmarkupcollection_8h.md)


```C++
/***************************************************************************
 qfmarkupcollection.h - QfMarkupCollection

 ---------------------
 begin                : 12.09.2026
 copyright            : (C) 2026 by Mathieu Pellerin
 email                : mathieu (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFMARKUPCOLLECTION_H
#define QFMARKUPCOLLECTION_H

#include "qfmarkupitem.h"

#include <QObject>


class QfMarkupCollection : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString name READ name NOTIFY nameChanged )
    Q_PROPERTY( qsizetype count READ count NOTIFY countChanged )
    Q_PROPERTY( QList<QfMarkupItem> items READ items NOTIFY itemsChanged )

  public:
    explicit QfMarkupCollection( const QString &name = QString(), QObject *parent = nullptr );

    QString name() const { return mName; }

    void setName( const QString &name );

    qsizetype count() const { return mItems.size(); }

    QList<QfMarkupItem> items() const { return mItems.values(); }

    void addItem( const QfMarkupItem &item );

    void replaceItem( const QString &uuid, const QfMarkupItem &item );

    void removeItem( const QString &uuid );

    bool readGeoJson( const QString &path );

    bool writeGeoJson( const QString &path );

  signals:
    void nameChanged();

    void countChanged();

    void itemsChanged();

  private:
    QString mName;
    QMap<QString, QfMarkupItem> mItems;
};

#endif // QFMARKUPCOLLECTION_H
```


