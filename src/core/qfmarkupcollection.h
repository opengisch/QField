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

    Q_PROPERTY( QString name READ name NOTIFY nameChanged )
    Q_PROPERTY( QList<QfMarkupItem> items READ items NOTIFY itemsChanged )

  public:
    explicit QfMarkupCollection( const QString &name = QString(), QObject *parent = nullptr );

    QString name() const { return mName; }
    void setName( const QString &name );

    QList<QfMarkupItem> items() const { return mItems.values(); }

    void addItem( const QfMarkupItem &item );
    void replaceItem( const QString &itemUuid, const QfMarkupItem &item );
    void removeItem( const QString &itemUuid );

    bool readGeoJson( const QString &path );
    bool writeGeoJson( const QString &path );

  signals:
    void nameChanged();
    void itemsChanged();

  private:
    QString mName;
    QMap<QString, QfMarkupItem> mItems;
};

#endif // QFMARKUPCOLLECTION_H
