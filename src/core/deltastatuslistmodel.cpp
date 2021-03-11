/***************************************************************************
    deltastatuslistmodel.cpp
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

#include "deltastatuslistmodel.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>


DeltaStatusListModel::DeltaStatusListModel()
{}

DeltaStatusListModel::DeltaStatusListModel( QJsonDocument deltasStatusList )
  : mJson( deltasStatusList )
{
  if ( ! mJson.isArray() )
  {
    mIsValid = false;
    mErrorString = QStringLiteral( "Expected the json document to be an array of delta status" );
    return;
  }

  const QJsonArray deltas = mJson.array();
  QString combinedOutput;

  for ( const QJsonValue &delta : deltas )
  {
    if ( ! delta.isObject() )
    {
      mIsValid = false;
      mErrorString = QStringLiteral( "Expected all array elements to be an object, but the element at #%1 is not" ).arg( mDeltas.size() );
      return;
    }

    const QJsonObject deltaObject = delta.toObject();
    const QStringList requiredKeys({"id", "deltafile_id", "created_at", "updated_at", "status"});
    for ( const QString &requiredKey : requiredKeys )
    {
      if ( deltaObject.value( requiredKey ).isNull() || deltaObject.value( requiredKey ).isUndefined() )
      {
        mIsValid = false;
        mErrorString = QStringLiteral( "Expected all array elements to be an object containing a key \"%1\", but the element at #%2 is not" ).arg( requiredKey ).arg( mDeltas.size() );
        return;
      }
    }

    DeltaStatus deltaStatus;
    deltaStatus.output = deltaObject.value( QStringLiteral( "output" ) ).toString();

    const QString statusString = deltaObject.value( QStringLiteral( "status" ) ).toString();
    if ( statusString == QStringLiteral( "STATUS_APPLIED" ) )
      deltaStatus.status = AppliedStatus;
    else if ( statusString == QStringLiteral( "STATUS_CONFLICT" ) )
      deltaStatus.status = ConflictStatus;
    else if ( statusString == QStringLiteral( "STATUS_NOT_APPLIED" ) )
      deltaStatus.status = NotAppliedStatus;
    else if ( statusString == QStringLiteral( "STATUS_PENDING" ) )
      deltaStatus.status = PendingStatus;
    else if ( statusString == QStringLiteral( "STATUS_BUSY" ) )
      deltaStatus.status = BusyStatus;
    else if ( statusString == QStringLiteral( "STATUS_ERROR" ) )
      deltaStatus.status = ErrorStatus;
    else
    {
      mIsValid = false;
      mErrorString = QStringLiteral( "Unrecognized status \"%1\" for $%2" ).arg( statusString, mDeltas.size() );
      return;
    }

    deltaStatus.id = QUuid( deltaObject.value( QStringLiteral( "id" ) ).toString() );
    deltaStatus.deltafileId = QUuid( deltaObject.value( QStringLiteral( "deltafile_id" ) ).toString() );
    deltaStatus.createdAt = deltaObject.value( QStringLiteral( "created_at" ) ).toString();
    deltaStatus.updatedAt = deltaObject.value( QStringLiteral( "updated_at" ) ).toString();

    mIsValid = true;
    mDeltas.append( deltaStatus );
  }
}

int DeltaStatusListModel::rowCount( const QModelIndex &parent ) const
{
  if ( !parent.isValid() )
    return mDeltas.size();
  else
    return 0;
}

QVariant DeltaStatusListModel::data( const QModelIndex &index, int role ) const
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

QHash<int, QByteArray> DeltaStatusListModel::roleNames() const
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

bool DeltaStatusListModel::isValid() const
{
  return mIsValid;
}

QJsonDocument DeltaStatusListModel::json() const
{
  return mJson;
}

QString DeltaStatusListModel::errorString() const
{
  return mErrorString;
}

bool DeltaStatusListModel::allHaveFinalStatus() const
{
  bool isFinalForAll = false;

  for ( const DeltaStatus &delta : mDeltas )
  {
    switch ( delta.status )
    {
      case AppliedStatus:
      case NotAppliedStatus:
      case ConflictStatus:
      case ErrorStatus:
        isFinalForAll = true;
        break;
      case PendingStatus:
      case BusyStatus:
        isFinalForAll = false;
        break;
    }

    if ( ! isFinalForAll )
      break;
  }

  return isFinalForAll;
}

QString DeltaStatusListModel::combinedOutput() const
{
  QString output;

  for ( const DeltaStatus &delta : mDeltas )
  {
    output += delta.output;
    output += QStringLiteral( "\n" );
  }

  return output;
}
