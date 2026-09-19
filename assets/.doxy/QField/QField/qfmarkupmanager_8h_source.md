

# File qfmarkupmanager.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfmarkupmanager.h**](qfmarkupmanager_8h.md)

[Go to the documentation of this file](qfmarkupmanager_8h.md)


```C++
/***************************************************************************
 qfmarkupmanager.h - QfMarkupManager

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

#ifndef QFMARKUPMANAGER_H
#define QFMARKUPMANAGER_H

#include "qfmarkupcollection.h"

#include <QObject>


class QfMarkupManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QStringList collectionNames READ collectionNames NOTIFY collectionsChanged )
    Q_PROPERTY( QList<QfMarkupCollection *> collections READ collections NOTIFY collectionsChanged )

    Q_PROPERTY( QStringList hiddenCollectionNames READ hiddenCollectionNames WRITE setHiddenCollectionNames NOTIFY hiddenCollectionNamesChanged )
    Q_PROPERTY( QList<QfMarkupCollection *> visibleCollections READ visibleCollections NOTIFY visibleCollectionsChanged )

    Q_PROPERTY( bool hasItems READ hasItems NOTIFY hasItemsChanged )

  public:
    explicit QfMarkupManager( QObject *parent = nullptr );

    QStringList collectionNames() const;

    QStringList collectionUuids() const { return mCollections.keys(); };

    QString collectionUuid( QfMarkupCollection *collection ) const;

    QfMarkupCollection *collection( const QString &uuid ) const;

    QList<QfMarkupCollection *> collections() const { return mCollections.values(); }

    QStringList hiddenCollectionNames() const { return mHiddenCollectionNames; }

    void setHiddenCollectionNames( const QStringList &hiddenCollectionNames );

    QList<QfMarkupCollection *> visibleCollections() const;

    bool hasItems() const;

    Q_INVOKABLE void reset( const QString &path, const QString &prefix = QString() );

  signals:
    void hiddenCollectionNamesChanged();

    void collectionsChanged();

    void visibleCollectionsChanged();

    void hasItemsChanged();

    void collectionItemsChanged( const QString &uuid );

    void collectionsWillBeRemoved( const QStringList &uuids );

    void collectionsAdded( const QStringList &uuids );

  private slots:
    void processCollectionItemsChanged();

  private:
    void insertCollection( const QString &collectionUuid, QfMarkupCollection *collection );

    QString mPath;
    QString mPrefix;

    QStringList mHiddenCollectionNames;

    QMap<QString, QfMarkupCollection *> mCollections;
};

#endif // QFMARKUPMANAGER_H
```


