

# File deltalistmodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldcloud**](dir_d27ac98b23548f5810403c734ce9817b.md) **>** [**deltalistmodel.cpp**](deltalistmodel_8cpp.md)

[Go to the documentation of this file](deltalistmodel_8cpp.md)


```C++
/***************************************************************************
    deltalistmodel.cpp
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

#include "deltalistmodel.h"

#include <QJsonArray>
#include <QJsonObject>


DeltaListModel::DeltaListModel()
{
}

int DeltaListModel::rowCount( const QModelIndex &parent ) const
{
  if ( parent.isValid() )
  {
    return 0;
  }

  return static_cast<int>( mDeltas.size() );
}

QVariant DeltaListModel::data( const QModelIndex &index, int role ) const
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

QHash<int, QByteArray> DeltaListModel::roleNames() const
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

bool DeltaListModel::isValid() const
{
  return mIsValid;
}

bool DeltaListModel::isRefreshing() const
{
  return mIsRefreshing;
}

QJsonDocument DeltaListModel::json() const
{
  return mJson;
}

QString DeltaListModel::errorString() const
{
  return mErrorString;
}

void DeltaListModel::refresh()
{
  if ( !mDeltas.isEmpty() )
  {
    beginResetModel();
    mDeltas.clear();
    mJson = QJsonDocument();
    endResetModel();
  }

  if ( !mCloudConnection || mCloudProjectId.isEmpty() )
  {
    setIsRefreshing( false );
    return;
  }

  setIsRefreshing( true );
  NetworkReply *deltaStatusReply = mCloudConnection->get( QStringLiteral( "/api/v1/deltas/%1/" ).arg( mCloudProjectId ) );
  connect( deltaStatusReply, &NetworkReply::finished, this, [this, deltaStatusReply]() {
    QNetworkReply *rawReply = deltaStatusReply->currentRawReply();
    deltaStatusReply->deleteLater();

    Q_ASSERT( deltaStatusReply->isFinished() );
    Q_ASSERT( rawReply );

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      setIsRefreshing( false );
      return;
    }

    mJson = QJsonDocument::fromJson( rawReply->readAll() );

    beginResetModel();
    QString errorString;
    bool isValid = false;
    mDeltas = QFieldCloudUtils::parseDeltaJsonDocument( mJson, errorString, isValid );
    endResetModel();

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

    setIsRefreshing( false );
  } );
}

void DeltaListModel::setCloudConnection( QFieldCloudConnection *cloudConnection )
{
  if ( mCloudConnection == cloudConnection )
  {
    return;
  }

  mCloudConnection = cloudConnection;
  emit cloudConnectionChanged();
}

void DeltaListModel::setCloudProjectId( const QString &cloudProjectId )
{
  if ( mCloudProjectId == cloudProjectId )
  {
    return;
  }

  mCloudProjectId = cloudProjectId;
  emit cloudProjectIdChanged();
}

void DeltaListModel::setIsRefreshing( bool isRefreshing )
{
  if ( mIsRefreshing == isRefreshing )
  {
    return;
  }

  mIsRefreshing = isRefreshing;
  emit isRefreshingChanged();
}
```


