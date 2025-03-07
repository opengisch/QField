/***************************************************************************
  externalstorage.cpp - ExternalStorage

 ---------------------
 begin                : 07.04.2025
 copyright            : (C) 2025 by Mathieu Pellerin
 email                : mathieu@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "externalstorage.h"

#include <qgsapplication.h>

ExternalStorage::ExternalStorage( QObject *parent )
  : QObject( parent )
{
}

Qgis::ContentStatus ExternalStorage::status() const
{
  return mFetchedContent ? mFetchedContent->status() : Qgis::ContentStatus::NotStarted;
}

QString ExternalStorage::type() const
{
  return mStorage ? mStorage->type() : QString();
}

void ExternalStorage::setType( const QString &type )
{
  mStorage.reset( QgsApplication::instance()->externalStorageRegistry()->externalStorageFromType( type ) );
  emit typeChanged();
}

void ExternalStorage::fetch( const QString &url, const QString &authenticationId )
{
  if ( mStorage )
  {
    if ( mFetchedContent )
    {
      disconnect( mFetchedContent.get(), &QgsExternalStorageFetchedContent::fetched, this, &ExternalStorage::contentFetched );
      mFetchedContent->cancel();
      mFetchedContent->deleteLater();
    }

    mFetchedContent.reset( mStorage->fetch( url, authenticationId, Qgis::ActionStart::Immediate ) );
    emit statusChanged();

    connect( mFetchedContent.get(), &QgsExternalStorageFetchedContent::fetched, this, &ExternalStorage::contentFetched );
  }
}

QString ExternalStorage::fetchedContent() const
{
  return mFetchedContent && mFetchedContent->status() == Qgis::ContentStatus::Finished ? mFetchedContent->filePath() : QString();
}

void ExternalStorage::contentFetched()
{
  emit statusChanged();
  emit fetchedContentChanged();
}
