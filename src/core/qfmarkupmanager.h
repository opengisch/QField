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

  public:
    explicit QfMarkupManager( QObject *parent = nullptr );

    QStringList collectionNames() const { return mCollections.keys(); };
    QList<QfMarkupCollection *> collections() const { return mCollections.values(); }

    Q_INVOKABLE void reset( const QString &path, const QString &prefix = QString() );

  signals:
    void collectionsChanged();

  private slots:
    void collectionItemsChanged();

  private:
    void insertCollection( QfMarkupCollection *collection );

    QString mPath;
    QString mPrefix;

    QMap<QString, QfMarkupCollection *> mCollections;
};

#endif // QFMARKUPMANAGER_H
