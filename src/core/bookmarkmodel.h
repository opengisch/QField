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

#include <QSet>
#include <qgsbookmarkmanager.h>
#include <qgsbookmarkmodel.h>
#include <qobjectuniqueptr.h>

/**
 * \ingroup core
 */
class BookmarkModel : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY( QgsQuickMapSettings *mapSettings READ setMapSettings READ mapSettings NOTIFY mapSettingsChanged )

    //! When TRUE, project bookmarks are hidden so only user bookmarks (those created in QField) are listed.
    Q_PROPERTY( bool hideProjectBookmarks READ hideProjectBookmarks WRITE setHideProjectBookmarks NOTIFY hideProjectBookmarksChanged )

    //! Number of currently selected bookmarks.
    Q_PROPERTY( qsizetype selectedCount READ selectedCount NOTIFY selectedCountChanged )

  public:
    enum Roles
    {
      BookmarkId = Qt::UserRole + 1,
      BookmarkName,
      BookmarkGroup,
      BookmarkPoint,
      BookmarkCrs,
      BookmarkUser,
      BookmarkSelected,
    };
    Q_ENUM( Roles )

    explicit BookmarkModel( QgsBookmarkManager *manager, QgsBookmarkManager *projectManager = nullptr, QObject *parent = nullptr );

    QVariant data( const QModelIndex &index, int role ) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void setExtentFromBookmark( const QModelIndex &index );

    Q_INVOKABLE QString addBookmarkAtPoint( QgsPoint point, const QString &name = QString(), const QString &group = QString() );

    Q_INVOKABLE void updateBookmarkDetails( const QString &id, const QString &name, const QString &group );

    Q_INVOKABLE void removeBookmark( const QString &id );

    Q_INVOKABLE QgsPoint getBookmarkPoint( const QString &id );

    Q_INVOKABLE QgsCoordinateReferenceSystem getBookmarkCrs( const QString &id );

    Q_INVOKABLE void store();

    void setMapSettings( QgsQuickMapSettings *mapSettings );

    QgsQuickMapSettings *mapSettings() const { return mMapSettings; }

    bool hideProjectBookmarks() const { return mHideProjectBookmarks; }
    void setHideProjectBookmarks( bool hideProjectBookmarks );

    qsizetype selectedCount() const { return mSelectedIds.size(); }

    //! Toggles the selection state of the bookmark identified by \a id.
    Q_INVOKABLE void toggleSelected( const QString &id );

    //! Clears the current selection.
    Q_INVOKABLE void clearSelection();

    //! Deletes all currently selected bookmarks, persisting once. Returns the number deleted.
    Q_INVOKABLE int deleteSelected();

  signals:
    void mapSettingsChanged();
    void hideProjectBookmarksChanged();
    void selectedCountChanged();
    void requestJumpToPoint( const QgsPoint &center, const double &scale = -1.0, bool handleMargins = false ) const;

  protected:
    bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const override;
    bool lessThan( const QModelIndex &sourceLeft, const QModelIndex &sourceRight ) const override;

  private:
    //! Returns TRUE if the source row is a user/global bookmark, FALSE if it is a project bookmark.
    bool isUserBookmark( int sourceRow ) const;

    //! Returns the sort rank for a bookmark group/color ("" first, then orange, red, blue).
    int groupRank( const QString &group ) const;

    QObjectUniquePtr<QgsBookmarkManagerModel> mModel;
    QgsBookmarkManager *mManager = nullptr;
    QgsQuickMapSettings *mMapSettings = nullptr;
    bool mHideProjectBookmarks = false;
    QSet<QString> mSelectedIds;
};

#endif // BOOKMARKMODEL_H
