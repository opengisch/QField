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

#include <qgsbookmarkmanager.h>
#include <qgsbookmarkmodel.h>

class BookmarkModel : public QSortFilterProxyModel
{

  public:

    enum Roles
    {
      BookmarkId = Qt::UserRole + 1,
      BookmarkName,
      BookmarkPoint,
      BookmarkCrs,
    };
    Q_ENUM( Roles )

    explicit BookmarkModel( QgsBookmarkManager *manager, QgsBookmarkManager *projectManager = nullptr, QObject *parent = nullptr );

    QVariant data( const QModelIndex &index, int role ) const override;

    QHash<int, QByteArray> roleNames() const override;

  private:

    std::unique_ptr<QgsBookmarkManagerModel> mModel = nullptr;

};

#endif // BOOKMARKMODEL_H
