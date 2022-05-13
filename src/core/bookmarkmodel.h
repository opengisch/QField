/***************************************************************************
                            bookmarkmodel.h
                              -------------------
              begin                : 12.12.2021
              copyright            : (C) 2021 by Mathieu Pellerin
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

#ifndef BOOKMARKMODEL_H
#define BOOKMARKMODEL_H

#include "qgsquickmapsettings.h"

#include <qgsbookmarkmanager.h>
#include <qgsbookmarkmodel.h>
#include <qobjectuniqueptr.h>

class BookmarkModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ setMapSettings READ mapSettings NOTIFY mapSettingsChanged )

  public:
    enum Roles
    {
      BookmarkId = Qt::UserRole + 1,
      BookmarkName,
      BookmarkGroup,
      BookmarkPoint,
      BookmarkCrs,
      BookmarkUser,
    };
    Q_ENUM( Roles )

    explicit BookmarkModel( QgsBookmarkManager *manager, QgsBookmarkManager *projectManager = nullptr, QObject *parent = nullptr );

    QVariant data( const QModelIndex &index, int role ) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void setExtentFromBookmark( const QModelIndex &index );

    Q_INVOKABLE QString addBookmarkAtPoint( QgsPoint point, const QString &name = QString(), const QString &group = QString() );

    Q_INVOKABLE void updateBookmarkDetails( const QString &id, const QString &name, const QString &group );

    Q_INVOKABLE void removeBookmark( const QString &id );

    Q_INVOKABLE QgsPointXY getBookmarkPoint( const QString &id );

    Q_INVOKABLE QgsCoordinateReferenceSystem getBookmarkCrs( const QString &id );

    void setMapSettings( QgsQuickMapSettings *mapSettings );

    QgsQuickMapSettings *mapSettings() const { return mMapSettings; }

  signals:
    void mapSettingsChanged();

  private:
    QObjectUniquePtr<QgsBookmarkManagerModel> mModel;
    QgsBookmarkManager *mManager = nullptr;
    QgsQuickMapSettings *mMapSettings = nullptr;
};

#endif // BOOKMARKMODEL_H
