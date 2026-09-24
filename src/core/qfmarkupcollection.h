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
#include <qgsannotationlayer.h>
#include <qgsvectorlayer.h>

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
     * Returns the UUID of the collection.
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
    Q_INVOKABLE QString addItem( const QfMarkupItem &item, bool resetVectorLayer = true );

    /**
     * Replaces an item matching the provided \a uuid with a new \a item within the collection.
     */
    Q_INVOKABLE void replaceItem( const QString &uuid, const QfMarkupItem &item, bool resetVectorLayer = true );

    /**
     * Removes an item matching the provided \a uuid from the collection.
     */
    Q_INVOKABLE void removeItem( const QString &uuid, bool resetVectorLayer = true );

    /**
     * Restore a collection from the content of a GeoJSON at the provided \a path.
     */
    bool readGeoJson( const QString &path );

    /**
     * Saves the collection from the content of a GeoJSON at the provided \a path.
     */
    bool writeGeoJson( const QString &path );

    /**
     * Returns a pointer to an annotation layer matching the content of the collection.
     * \note The ownership remains with the collection.
     */
    QgsAnnotationLayer *asAnnotationLayer();

    /**
     * Returns a pointer to a memory vector layer matching the content of the collection.
     *
     * The layer geometry type is a GeometryCollection and it can be used to modify
     * the attributes and geometries of individual markup items within the collection.
     *
     * \note The ownership remains with the collection.
     */
    QgsVectorLayer *asVectorLayer();

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
    void processFeatureDeleted( QgsFeatureId fid );
    void processGeometryChanged( QgsFeatureId fid, const QgsGeometry &geometry );

    QString mUuid;
    QString mName;
    QMap<QString, QfMarkupItem> mItems;
    std::unique_ptr<QgsAnnotationLayer> mAnnotationLayer;
    std::unique_ptr<QgsVectorLayer> mVectorLayer;
    QMap<QgsFeatureId, QString> mFeatureUuids;
};

#endif // QFMARKUPCOLLECTION_H
