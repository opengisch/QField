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


/**
 * \brief This class holds a collection of markup items.
 * \ingroup core
 */
class QfMarkupCollection : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString uuid READ uuid NOTIFY uuidChanged )
    Q_PROPERTY( QString name READ name NOTIFY nameChanged )
    Q_PROPERTY( qsizetype count READ count NOTIFY countChanged )
    Q_PROPERTY( QList<QfMarkupItem> items READ items NOTIFY itemsChanged )

  public:
    explicit QfMarkupCollection( const QString &name = QString(), QObject *parent = nullptr );

    /**
    * Returns the name of the collection.
    */
    QString uuid() const { return mUuid; }

    /**
     * Returns the name of the collection.
     */
    QString name() const { return mName; }

    /**
     * Sets the name of the collection.
     */
    void setName( const QString &name );

    /**
     * Returns the number of items in the collection.
     */
    qsizetype count() const { return mItems.size(); }

    /**
     * Returns the list of items in the collection.
     */
    QList<QfMarkupItem> items() const { return mItems.values(); }

    /**
     * Adds an \a item into the collection and return its UUID.
     */
    Q_INVOKABLE void addItem( const QfMarkupItem &item );

    /**
     * Replaces an item matching the provided \a uuid with a new \a item within the collection.
     */
    Q_INVOKABLE void replaceItem( const QString &uuid, const QfMarkupItem &item );

    /**
     * Removes an item matching the provided \a uuid from the collection.
     */
    Q_INVOKABLE void removeItem( const QString &uuid );

    /**
     * Restore a collection from the content of a GeoJSON at the provided \a path.
     */
    bool readGeoJson( const QString &path );

    /**
     * Saves the collection from the content of a GeoJSON at the provided \a path.
     */
    bool writeGeoJson( const QString &path );

    Q_INVOKABLE static QfMarkupItem createItem( const QString &label, const QString &description, const QgsGeometry &geometry, const QColor &color );

  signals:
    //! Emitted when the UUID changed.
    void uuidChanged();

    //! Emitted when the collection name changed.
    void nameChanged();

    //! Emitted when the number of items in the collection changed.
    void countChanged();

    //! Emitted when the list of items in the collection changed.
    void itemsChanged();

  private:
    QString mUuid;
    QString mName;
    QMap<QString, QfMarkupItem> mItems;
};

#endif // QFMARKUPCOLLECTION_H
