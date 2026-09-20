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


/**
 * \brief This class to manage markup collections.
 * \ingroup core
 */
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

    /**
     * Returns the list of collection names present in the markup manager.
     */
    QStringList collectionNames() const;

    /**
     * Returns the list of collection UUIDs present in the markup manager.
     */
    QStringList collectionUuids() const { return mCollections.keys(); };

    /**
     * Returns the list UUID matching the \a collection.
     */
    QString collectionUuid( QfMarkupCollection *collection ) const;

    /**
     * Returns the collection present in the markup manager matching the \a uuid.
     */
    QfMarkupCollection *collection( const QString &uuid ) const;

    /**
     * Returns the list of collections present in the markup manager.
     */
    QList<QfMarkupCollection *> collections() const { return mCollections.values(); }

    /**
     * Returns the list of collection names that are hidden. Matching collection
     * names will not be part of the list of collections returned by the markup
     * manager visibleCollections() function.
     */
    QStringList hiddenCollectionNames() const { return mHiddenCollectionNames; }

    /**
     * Sets the list of collection names that are hidden. Matching collection
     * names will not be part of the list of collections returned by the markup
     * manager visibleCollections() function.
     */
    void setHiddenCollectionNames( const QStringList &hiddenCollectionNames );

    /**
     * Returns the list of visible collections present in the markup manager.
     */
    QList<QfMarkupCollection *> visibleCollections() const;

    /**
     * Returns TRUE if any of the collections present in the markup manager contains
     * one or more items.
     */
    bool hasItems() const;

    /**
     * Resets the markup manager's list of collections by loading compatible GeoJSON
     * files present in the provided \a path. If a \a prefix is provided, the markup manager
     * will only load GeoJSON files beginning by the prefix.
     */
    Q_INVOKABLE void reset( const QString &path, const QString &prefix = QString() );

  signals:
    //! Emitted when the list of hidden collection names has changed.
    void hiddenCollectionNamesChanged();

    //! Emitted when the list of collections has changed.
    void collectionsChanged();

    //! Emitted when the list of visible collections has changed.
    void visibleCollectionsChanged();

    //! Emitted when the item count of a loaded collection has changed.
    void hasItemsChanged();

    //! Emitted when a collection's list of items has changed.
    void collectionItemsChanged( const QString &uuid );

    //! Emitted when collections are about to be removed.
    void collectionsWillBeRemoved( const QStringList &uuids );

    //! Emitted when collections are about to be removed.
    void collectionsAdded( const QStringList &uuids );

  private slots:
    void processCollectionItemsChanged();

  private:
    void insertCollection( QfMarkupCollection *collection, const QString &collectionPath = QString() );

    QString mPath;
    QString mPrefix;

    QStringList mHiddenCollectionNames;

    QMap<QString, QfMarkupCollection *> mCollections;
    QMap<QString, QString> mCollectionPaths;
};

#endif // QFMARKUPMANAGER_H
