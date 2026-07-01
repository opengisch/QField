/***************************************************************************
                            bookmarkmodel.cpp
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

#include "bookmarkmodel.h"

#include <qgsapplication.h>
#include <qgscoordinatetransform.h>
#include <qgsgeometry.h>
#include <qgsproject.h>

#include <algorithm>

BookmarkModel::BookmarkModel( QgsBookmarkManager *manager, QgsBookmarkManager *projectManager, QObject *parent )
  : QSortFilterProxyModel( parent )
  , mModel( new QgsBookmarkManagerModel( manager, projectManager, this ) )
  , mManager( manager )
{
  setSourceModel( mModel.get() );

  // Bookmarks are always grouped by color so the list can render color sections.
  setSortRole( BookmarkModel::BookmarkGroup );
  sort( 0 );
}

QVariant BookmarkModel::data( const QModelIndex &index, int role ) const
{
  QModelIndex sourceIndex = mapToSource( index );
  if ( !sourceIndex.isValid() )
    return QVariant();

  switch ( role )
  {
    case BookmarkModel::BookmarkId:
      return mModel->data( sourceIndex, static_cast<int>( QgsBookmarkManagerModel::CustomRole::Id ) );

    case BookmarkModel::BookmarkName:
      return mModel->data( sourceIndex, static_cast<int>( QgsBookmarkManagerModel::CustomRole::Name ) );

    case BookmarkModel::BookmarkGroup:
    {
      const QString group = mModel->data( sourceIndex, static_cast<int>( QgsBookmarkManagerModel::CustomRole::Group ) ).toString();
      return group.isEmpty() ? QStringLiteral( "green" ) : group;
    }

    case BookmarkModel::BookmarkPoint:
    {
      const QgsReferencedRectangle rect = mModel->data( sourceIndex, static_cast<int>( QgsBookmarkManagerModel::CustomRole::Extent ) ).value<QgsReferencedRectangle>();
      const QgsGeometry geom( new QgsPoint( rect.center() ) );
      return geom;
    }

    case BookmarkModel::BookmarkCrs:
    {
      const QgsReferencedRectangle rect = mModel->data( sourceIndex, static_cast<int>( QgsBookmarkManagerModel::CustomRole::Extent ) ).value<QgsReferencedRectangle>();
      return rect.crs();
    }

    case BookmarkModel::BookmarkUser:
    {
      return isUserBookmark( sourceIndex.row() );
    }

    case BookmarkModel::BookmarkSelected:
    {
      const QString id = mModel->data( sourceIndex, static_cast<int>( QgsBookmarkManagerModel::CustomRole::Id ) ).toString();
      return mSelectedIds.contains( id );
    }
  }

  return QVariant();
}

QHash<int, QByteArray> BookmarkModel::roleNames() const
{
  QHash<int, QByteArray> roleNames = QAbstractProxyModel::roleNames();
  roleNames[BookmarkModel::BookmarkId] = "BookmarkId";
  roleNames[BookmarkModel::BookmarkName] = "BookmarkName";
  roleNames[BookmarkModel::BookmarkGroup] = "BookmarkGroup";
  roleNames[BookmarkModel::BookmarkPoint] = "BookmarkPoint";
  roleNames[BookmarkModel::BookmarkCrs] = "BookmarkCrs";
  roleNames[BookmarkModel::BookmarkUser] = "BookmarkUser";
  roleNames[BookmarkModel::BookmarkSelected] = "BookmarkSelected";
  return roleNames;
}

void BookmarkModel::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
    return;

  mMapSettings = mapSettings;

  emit mapSettingsChanged();
}

void BookmarkModel::setExtentFromBookmark( const QModelIndex &index )
{
  QModelIndex sourceIndex = mapToSource( index );
  if ( !sourceIndex.isValid() || !mMapSettings )
    return;

  const QgsReferencedRectangle rect = mModel->data( sourceIndex, static_cast<int>( QgsBookmarkManagerModel::CustomRole::Extent ) ).value<QgsReferencedRectangle>();
  QgsCoordinateTransform transform( rect.crs(), mMapSettings->destinationCrs(), QgsProject::instance()->transformContext() );
  QgsRectangle transformedRect;
  try
  {
    transformedRect = transform.transform( rect );
  }
  catch ( const QgsException &e )
  {
    Q_UNUSED( e )
    return;
  }
  catch ( ... )
  {
    // catch any other errors
    return;
  }

  const double scale = mMapSettings->computeScaleForExtent( transformedRect, true );
  emit requestJumpToPoint( QgsPoint( transformedRect.center() ), scale, true );
}

QString BookmarkModel::addBookmarkAtPoint( QgsPoint point, const QString &name, const QString &group )
{
  if ( !mMapSettings )
    return QString();

  QgsRectangle extent = mMapSettings->extent();
  const QgsPointXY center = extent.center();

  const double xDiff = point.x() - center.x();
  const double yDiff = point.y() - center.y();

  extent.setXMinimum( extent.xMinimum() + xDiff );
  extent.setXMaximum( extent.xMaximum() + xDiff );
  extent.setYMinimum( extent.yMinimum() + yDiff );
  extent.setYMaximum( extent.yMaximum() + yDiff );

  QgsBookmark bookmark;
  bookmark.setExtent( QgsReferencedRectangle( extent, mMapSettings->destinationCrs() ) );
  bookmark.setName( name );
  bookmark.setGroup( group );

  const QString uuid = mManager->addBookmark( bookmark );
  store();

  return uuid;
}

void BookmarkModel::updateBookmarkDetails( const QString &id, const QString &name, const QString &group )
{
  QgsBookmark bookmark = mManager->bookmarkById( id );
  bookmark.setName( name );
  bookmark.setGroup( group );
  mManager->updateBookmark( bookmark );
}

void BookmarkModel::removeBookmark( const QString &id )
{
  mManager->removeBookmark( id );
  store();
}

QgsPoint BookmarkModel::getBookmarkPoint( const QString &id )
{
  const QgsBookmark bookmark = mManager->bookmarkById( id );
  return QgsPoint( bookmark.extent().center() );
}

QgsCoordinateReferenceSystem BookmarkModel::getBookmarkCrs( const QString &id )
{
  const QgsBookmark bookmark = mManager->bookmarkById( id );
  return bookmark.extent().crs();
}

void BookmarkModel::store()
{
  const QString filePath = QStringLiteral( "%1/bookmarks.xml" ).arg( QgsApplication::qgisSettingsDirPath() );
  if ( !filePath.isEmpty() )
  {
    QFile f( filePath );
    if ( !f.open( QFile::WriteOnly | QIODevice::Truncate ) )
    {
      return;
    }

    QDomDocument doc;
    QDomElement elem = mManager->writeXml( doc );
    doc.appendChild( elem );

    QTextStream out( &f );
    doc.save( out, 2 );
    f.close();
  }
}

void BookmarkModel::setHideProjectBookmarks( bool hideProjectBookmarks )
{
  if ( mHideProjectBookmarks == hideProjectBookmarks )
    return;

  beginFilterChange();
  mHideProjectBookmarks = hideProjectBookmarks;
  endFilterChange();

  emit hideProjectBookmarksChanged();
}

bool BookmarkModel::isUserBookmark( int sourceRow ) const
{
  return mModel->data( mModel->index( sourceRow, QgsBookmarkManagerModel::ColumnStore ), Qt::CheckStateRole ).value<Qt::CheckState>() != Qt::Checked;
}

bool BookmarkModel::filterAcceptsRow( int sourceRow, const QModelIndex & ) const
{
  if ( !mHideProjectBookmarks )
    return true;

  // The drawer scopes the list to bookmarks created in QField (user bookmarks).
  return isUserBookmark( sourceRow );
}

void BookmarkModel::toggleSelected( const QString &id )
{
  if ( id.isEmpty() )
    return;

  // Only user bookmarks (those owned by the user manager) can be selected.
  if ( mManager->bookmarkById( id ).id().isEmpty() )
    return;

  if ( mSelectedIds.contains( id ) )
  {
    mSelectedIds.remove( id );
  }
  else
  {
    mSelectedIds.insert( id );
  }

  const int count = rowCount();
  if ( count > 0 )
  {
    emit dataChanged( index( 0, 0 ), index( count - 1, 0 ), { BookmarkModel::BookmarkSelected } );
  }
  emit selectedCountChanged();
}

void BookmarkModel::clearSelection()
{
  if ( mSelectedIds.isEmpty() )
    return;

  mSelectedIds.clear();

  const int count = rowCount();
  if ( count > 0 )
  {
    emit dataChanged( index( 0, 0 ), index( count - 1, 0 ), { BookmarkModel::BookmarkSelected } );
  }
  emit selectedCountChanged();
}

int BookmarkModel::deleteSelected()
{
  if ( mSelectedIds.isEmpty() )
  {
    return 0;
  }

  const int deleted = static_cast<int>( std::count_if( mSelectedIds.constBegin(), mSelectedIds.constEnd(), [this]( const QString &id ) {
    return mManager->removeBookmark( id );
  } ) );

  // Persist once after all removals rather than on every bookmark.
  store();

  mSelectedIds.clear();
  emit selectedCountChanged();

  return deleted;
}

int BookmarkModel::groupRank( const QString &group ) const
{
  if ( group == QLatin1String( "orange" ) )
  {
    return 1;
  }
  else if ( group == QLatin1String( "red" ) )
  {
    return 2;
  }
  else if ( group == QLatin1String( "blue" ) )
  {
    return 3;
  }

  // Default (empty group) comes first.
  return 0;
}

bool BookmarkModel::lessThan( const QModelIndex &sourceLeft, const QModelIndex &sourceRight ) const
{
  const QString leftGroup = mModel->data( sourceLeft, static_cast<int>( QgsBookmarkManagerModel::CustomRole::Group ) ).toString();
  const QString rightGroup = mModel->data( sourceRight, static_cast<int>( QgsBookmarkManagerModel::CustomRole::Group ) ).toString();
  return groupRank( leftGroup ) < groupRank( rightGroup );
}
