

# File qfdeltalistmodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldcloud**](dir_d27ac98b23548f5810403c734ce9817b.md) **>** [**qfdeltalistmodel.cpp**](qfdeltalistmodel_8cpp.md)

[Go to the documentation of this file](qfdeltalistmodel_8cpp.md)


```C++
/***************************************************************************
    qfdeltalistmodel.cpp
    ---------------------
    begin                : December 2020
    copyright            : (C) 2020 by Ivan Ivanov
    email                : ivan at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfdeltalistmodel.h"

#include <QJsonArray>
#include <QJsonObject>


QfDeltaListModel::QfDeltaListModel()
{
}

int QfDeltaListModel::rowCount( const QModelIndex &parent ) const
{
  if ( parent.isValid() )
  {
    return 0;
  }

  return static_cast<int>( mDeltas.size() );
}

QVariant QfDeltaListModel::data( const QModelIndex &index, int role ) const
{
  if ( index.row() >= mDeltas.size() || index.row() < 0 )
    return QVariant();

  switch ( static_cast<ColumnRole>( role ) )
  {
    case IdRole:
      return mDeltas.at( index.row() ).id;
    case DeltafileIdRole:
      return mDeltas.at( index.row() ).deltafileId;
    case CreatedByRole:
      return mDeltas.at( index.row() ).createdBy;
    case CreatedAtRole:
      return mDeltas.at( index.row() ).createdAt;
    case UpdatedAtRole:
      return mDeltas.at( index.row() ).updatedAt;
    case StatusRole:
      return mDeltas.at( index.row() ).status;
    case SummaryRole:
      return mDeltas.at( index.row() ).summary;
    case OutputRole:
      return mDeltas.at( index.row() ).output;
  }

  return QVariant();
}

QHash<int, QByteArray> QfDeltaListModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[IdRole] = "Id";
  roles[DeltafileIdRole] = "DeltafileId";
  roles[CreatedByRole] = "CreatedBy";
  roles[CreatedAtRole] = "CreatedAt";
  roles[UpdatedAtRole] = "UpdatedAt";
  roles[StatusRole] = "Status";
  roles[SummaryRole] = "Summary";
  roles[OutputRole] = "Output";
  return roles;
}

bool QfDeltaListModel::isValid() const
{
  return mIsValid;
}

bool QfDeltaListModel::isRefreshing() const
{
  return mIsRefreshing;
}

bool QfDeltaListModel::hasNextPage() const
{
  return mHasNextPage;
}

QJsonDocument QfDeltaListModel::lastJson() const
{
  return mLastJson;
}

QString QfDeltaListModel::errorString() const
{
  return mErrorString;
}

void QfDeltaListModel::refresh()
{
  if ( !mDeltas.isEmpty() )
  {
    beginResetModel();
    mDeltas.clear();
    mLastJson = QJsonDocument();
    endResetModel();

    mHasNextPage = false;
    emit hasNextPageChanged();
  }

  if ( !mCloudConnection || mCloudProjectId.isEmpty() )
  {
    setIsRefreshing( false );
    return;
  }

  if ( !mIsRefreshing )
  {
    mOffset = 0;
    fetchDeltaList();
  }
}

void QfDeltaListModel::fetchNextPage()
{
  if ( !mHasNextPage || mIsRefreshing )
  {
    return;
  }

  mOffset += mDeltasPerFetch;
  fetchDeltaList();
}

void QfDeltaListModel::fetchDeltaList()
{
  setIsRefreshing( true );

  const QString url = QStringLiteral( "/api/v1/deltas/%1/?ordering=-created_at" ).arg( mCloudProjectId );
  QVariantMap params;
  params["limit"] = QString::number( mDeltasPerFetch );
  params["offset"] = QString::number( mOffset );

  QfNetworkReply *deltaStatusReply = mCloudConnection->get( url, params );
  connect( deltaStatusReply, &QfNetworkReply::finished, this, [this, deltaStatusReply]() {
    QNetworkReply *rawReply = deltaStatusReply->currentRawReply();
    deltaStatusReply->deleteLater();

    Q_ASSERT( deltaStatusReply->isFinished() );
    Q_ASSERT( rawReply );

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      setIsRefreshing( false );
      return;
    }

    mLastJson = QJsonDocument::fromJson( rawReply->readAll() );

    QString errorString;
    bool isValid = false;
    if ( mOffset == 0 )
    {
      beginResetModel();
      mDeltas = QfCloudUtils::parseDeltaJsonDocument( mLastJson, errorString, isValid );
      endResetModel();
    }
    else
    {
      const QList<QfCloudDelta> deltas = QfCloudUtils::parseDeltaJsonDocument( mLastJson, errorString, isValid );
      if ( !deltas.isEmpty() )
      {
        beginInsertRows( QModelIndex(), mDeltas.size(), mDeltas.size() + deltas.size() - 1 );
        mDeltas << deltas;
        endInsertRows();
      }
    }

    if ( !errorString.isEmpty() )
    {
      mErrorString = errorString;
      emit errorStringChanged();
    }

    if ( mIsValid != isValid )
    {
      mIsValid = isValid;
      emit isValidChanged();
    }

    const bool hasNextPage = errorString.isEmpty() && mIsValid && rawReply->hasRawHeader( QStringLiteral( "X-Next-Page" ) );
    if ( mHasNextPage != hasNextPage )
    {
      mHasNextPage = hasNextPage;
      emit hasNextPageChanged();
    }

    setIsRefreshing( false );
  } );
}

void QfDeltaListModel::setCloudConnection( QfCloudConnection *cloudConnection )
{
  if ( mCloudConnection == cloudConnection )
  {
    return;
  }

  mCloudConnection = cloudConnection;
  emit cloudConnectionChanged();
}

void QfDeltaListModel::setCloudProjectId( const QString &cloudProjectId )
{
  if ( mCloudProjectId == cloudProjectId )
  {
    return;
  }

  mCloudProjectId = cloudProjectId;
  emit cloudProjectIdChanged();
}

void QfDeltaListModel::setIsRefreshing( bool isRefreshing )
{
  if ( mIsRefreshing == isRefreshing )
  {
    return;
  }

  mIsRefreshing = isRefreshing;
  emit isRefreshingChanged();
}
```


