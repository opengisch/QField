

# File externalstorage.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**externalstorage.cpp**](externalstorage_8cpp.md)

[Go to the documentation of this file](externalstorage_8cpp.md)


```C++
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
#include <qgsauthmanager.h>


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
  if ( mStorage && mStorage->type() == type )
    return;

  mStorage = QgsApplication::instance()->externalStorageRegistry()->externalStorageFromType( type );
  emit typeChanged();
}

QString ExternalStorage::lastError() const
{
  return mLastError;
}

void ExternalStorage::fetch( const QString &url, const QString &authenticationConfigurationId )
{
  if ( mStorage )
  {
    if ( mFetchedContent )
    {
      disconnect( mFetchedContent.get(), &QgsExternalStorageFetchedContent::fetched, this, &ExternalStorage::contentFetched );
      mFetchedContent->cancel();
      mFetchedContent->deleteLater();
    }

    mFetchedContent.reset( mStorage->fetch( url, authenticationConfigurationId, Qgis::ActionStart::Immediate ) );
    emit statusChanged();
    emit fetchedContentChanged();

    connect( mFetchedContent.get(), &QgsExternalStorageContent::errorOccurred, this, &ExternalStorage::contentErrorOccurred );
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

void ExternalStorage::contentErrorOccurred( const QString &errorString )
{
  mLastError = errorString;
  emit statusChanged();
  emit lastErrorChanged();
}
```


