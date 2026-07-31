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


DeltaListModel::DeltaListModel( QJsonDocument deltasStatusList, QObject *parent )
  : mJson( deltasStatusList )
  , QAbstractListModel( parent )
{
  if ( !mJson.isEmpty() )
  {
    parseJson();
  }
}

void DeltaListModel::parseJson()
{
  if ( !mJson.isArray() )
  {
    mIsValid = false;
    mErrorString = tr( "Expected the json document to be an array of delta status" );
    return;
  }

  beginResetModel();

  const QJsonArray deltas = mJson.array();
  for ( const QJsonValue deltaJson : deltas )
  {
    if ( !deltaJson.isObject() )
    {
      mIsValid = false;
      mErrorString = tr( "Expected all array elements to be an object, but the element at #%1 is not" ).arg( mDeltas.size() );
      break;
      ;
    }

    const QJsonObject deltaObject = deltaJson.toObject();
    const QStringList requiredKeys( { "id", "deltafile_id", "created_at", "updated_at", "status" } );
    auto match = std::find_if( requiredKeys.begin(), requiredKeys.end(), [&deltaObject]( const QString &key ) {
      return deltaObject.value( key ).isNull() || deltaObject.value( key ).isUndefined();
    } );
    if ( match != requiredKeys.end() )
    {
      mIsValid = false;
      mErrorString = tr( "Expected all array elements to be an object containing a key \"%1\", but the element at #%2 is not" ).arg( *match ).arg( mDeltas.size() );
      break;
    }

    Delta delta;
    delta.output = deltaObject.value( QStringLiteral( "output" ) ).toString();

    const QString statusString = deltaObject.value( QStringLiteral( "status" ) ).toString();
    if ( statusString == QStringLiteral( "STATUS_APPLIED" ) )
      delta.status = AppliedStatus;
    else if ( statusString == QStringLiteral( "STATUS_CONFLICT" ) )
      delta.status = ConflictStatus;
    else if ( statusString == QStringLiteral( "STATUS_NOT_APPLIED" ) )
      delta.status = NotAppliedStatus;
    else if ( statusString == QStringLiteral( "STATUS_PENDING" ) )
      delta.status = PendingStatus;
    else if ( statusString == QStringLiteral( "STATUS_BUSY" ) )
      delta.status = BusyStatus;
    else if ( statusString == QStringLiteral( "STATUS_ERROR" ) )
      delta.status = ErrorStatus;
    else if ( statusString == QStringLiteral( "STATUS_IGNORED" ) )
      delta.status = IgnoredStatus;
    else if ( statusString == QStringLiteral( "STATUS_UNPERMITTED" ) )
      delta.status = UnpermittedStatus;
    else
    {
      mIsValid = false;
      mErrorString = tr( "Unrecognized status \"%1\" for $%2" ).arg( statusString, QString::number( mDeltas.size() ) );
      return;
    }

    delta.id = QUuid( deltaObject.value( QStringLiteral( "id" ) ).toString() );
    delta.deltafileId = QUuid( deltaObject.value( QStringLiteral( "deltafile_id" ) ).toString() );
    delta.createdAt = deltaObject.value( QStringLiteral( "created_at" ) ).toString();
    delta.updatedAt = deltaObject.value( QStringLiteral( "updated_at" ) ).toString();

    mIsValid = true;
    mDeltas.append( delta );
  }

  endResetModel();
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
    case CreatedAtRole:
      return mDeltas.at( index.row() ).createdAt;
    case UpdatedAtRole:
      return mDeltas.at( index.row() ).updatedAt;
    case StatusRole:
      return mDeltas.at( index.row() ).status;
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
  roles[CreatedAtRole] = "CreatedAt";
  roles[UpdatedAtRole] = "UpdatedAt";
  roles[StatusRole] = "Status";
  roles[OutputRole] = "Output";
  return roles;
}

bool DeltaListModel::isValid() const
{
  return mIsValid;
}

QJsonDocument DeltaListModel::json() const
{
  return mJson;
}

QString DeltaListModel::errorString() const
{
  return mErrorString;
}

bool DeltaListModel::allHaveFinalStatus() const
{
  bool isFinalForAll = false;

  for ( const Delta &delta : mDeltas )
  {
    switch ( delta.status )
    {
      case AppliedStatus:
      case NotAppliedStatus:
      case ConflictStatus:
      case ErrorStatus:
      case UnpermittedStatus:
      case IgnoredStatus:
        isFinalForAll = true;
        break;
      case PendingStatus:
      case BusyStatus:
        isFinalForAll = false;
        break;
    }

    if ( !isFinalForAll )
      break;
  }

  return isFinalForAll;
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

  if ( mCloudProjectId.isEmpty() )
  {
    return;
  }

  NetworkReply *deltaStatusReply = mCloudConnection->get( QStringLiteral( "/api/v1/deltas/%1/" ).arg( mCloudProjectId ) );
  connect( deltaStatusReply, &NetworkReply::finished, this, [this, deltaStatusReply]() {
    QNetworkReply *rawReply = deltaStatusReply->currentRawReply();
    deltaStatusReply->deleteLater();

    Q_ASSERT( deltaStatusReply->isFinished() );
    Q_ASSERT( rawReply );

    if ( rawReply->error() != QNetworkReply::NoError )
    {
      return;
    }

    mJson = QJsonDocument::fromJson( rawReply->readAll() );
    parseJson();
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
