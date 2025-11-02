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

BookmarkModel::BookmarkModel( QgsBookmarkManager *manager, QgsBookmarkManager *projectManager, QObject *parent )
  : QSortFilterProxyModel( parent )
  , mModel( new QgsBookmarkManagerModel( manager, projectManager, this ) )
  , mManager( manager )
{
  setSourceModel( mModel.get() );
}

QVariant BookmarkModel::data( const QModelIndex &index, int role ) const
{
  QModelIndex sourceIndex = mapToSource( index );
  if ( !sourceIndex.isValid() )
    return QVariant();

  switch ( role )
  {
    case BookmarkModel::BookmarkId:
#if _QGIS_VERSION_INT >= 33500
      return mModel->data( sourceIndex, static_cast<int>( QgsBookmarkManagerModel::CustomRole::Id ) );
#else
      return mModel->data( sourceIndex, QgsBookmarkManagerModel::RoleId );
#endif

    case BookmarkModel::BookmarkName:
#if _QGIS_VERSION_INT >= 33500
      return mModel->data( sourceIndex, static_cast<int>( QgsBookmarkManagerModel::CustomRole::Name ) );
#else
      return mModel->data( sourceIndex, QgsBookmarkManagerModel::RoleName );
#endif

    case BookmarkModel::BookmarkGroup:
#if _QGIS_VERSION_INT >= 33500
      return mModel->data( sourceIndex, static_cast<int>( QgsBookmarkManagerModel::CustomRole::Group ) );
#else
      return mModel->data( sourceIndex, QgsBookmarkManagerModel::RoleGroup );
#endif

    case BookmarkModel::BookmarkPoint:
    {
#if _QGIS_VERSION_INT >= 33500
      QgsReferencedRectangle rect = mModel->data( sourceIndex, static_cast<int>( QgsBookmarkManagerModel::CustomRole::Extent ) ).value<QgsReferencedRectangle>();
#else
      QgsReferencedRectangle rect = mModel->data( sourceIndex, QgsBookmarkManagerModel::RoleExtent ).value<QgsReferencedRectangle>();
#endif
      QgsGeometry geom( new QgsPoint( rect.center() ) );
      return geom;
    }

    case BookmarkModel::BookmarkCrs:
    {
#if _QGIS_VERSION_INT >= 33500
      QgsReferencedRectangle rect = mModel->data( sourceIndex, static_cast<int>( QgsBookmarkManagerModel::CustomRole::Extent ) ).value<QgsReferencedRectangle>();
#else
      QgsReferencedRectangle rect = mModel->data( sourceIndex, QgsBookmarkManagerModel::RoleExtent ).value<QgsReferencedRectangle>();
#endif
      return rect.crs();
    }

    case BookmarkModel::BookmarkUser:
    {
      return mModel->data( mModel->index( sourceIndex.row(), QgsBookmarkManagerModel::ColumnStore ), Qt::CheckStateRole ).value<Qt::CheckState>() != Qt::Checked;
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

#if _QGIS_VERSION_INT >= 33500
  QgsReferencedRectangle rect = mModel->data( sourceIndex, static_cast<int>( QgsBookmarkManagerModel::CustomRole::Extent ) ).value<QgsReferencedRectangle>();
#else
  QgsReferencedRectangle rect = mModel->data( sourceIndex, QgsBookmarkManagerModel::RoleExtent ).value<QgsReferencedRectangle>();
#endif

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

  mMapSettings->setExtent( transformedRect, true );
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
