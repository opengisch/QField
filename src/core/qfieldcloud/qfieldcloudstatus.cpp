/***************************************************************************
    qfieldcloudstatus.cpp
    ---------------------
    begin                : February 2026
    copyright            : (C) 2024 by Mohsen Dehghanzadeh
    email                : mohsen@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "networkmanager.h"
#include "networkreply.h"
#include "qfieldcloudstatus.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>


QFieldCloudStatus::QFieldCloudStatus( QObject *parent )
  : QObject( parent )
{
  mRefreshTimer.setInterval( 5 * 60 * 1000 ); // 5 minutes
  mRefreshTimer.setSingleShot( false );
  connect( &mRefreshTimer, &QTimer::timeout, this, &QFieldCloudStatus::fetchStatus );
}

QString QFieldCloudStatus::url() const
{
  return mUrl;
}

void QFieldCloudStatus::setUrl( const QString &url )
{
  if ( url == mUrl )
  {
    return;
  }

  mUrl = url;
  emit urlChanged();

  if ( !mSimulating )
  {
    // Reset state when URL changes
    mHasProblem = false;
    mStatusMessage.clear();
    mDetailsMessage.clear();
    mStatusPageUrl.clear();
    emit statusUpdated();
  }

  if ( !mUrl.isEmpty() && !mSimulating )
  {
    fetchStatus();
    mRefreshTimer.start();
  }
  else if ( mUrl.isEmpty() )
  {
    mRefreshTimer.stop();
  }
}

bool QFieldCloudStatus::hasProblem() const
{
  return mHasProblem;
}

QString QFieldCloudStatus::statusMessage() const
{
  return mStatusMessage;
}

QString QFieldCloudStatus::detailsMessage() const
{
  return mDetailsMessage;
}

QString QFieldCloudStatus::statusPageUrl() const
{
  return mStatusPageUrl;
}

void QFieldCloudStatus::refresh()
{
  fetchStatus();
}

void QFieldCloudStatus::simulateStatus( const QString &scenario )
{
  mStatusPageUrl = QStringLiteral( "https://status.qfield.cloud/" );
  mRefreshTimer.stop();

  // Disconnect any in-flight request so its response won't overwrite the simulation
  if ( mPendingReply )
  {
    disconnect( mPendingReply, nullptr, this, nullptr );
    mPendingReply->deleteLater();
    mPendingReply = nullptr;
  }

  if ( scenario == QStringLiteral( "degraded" ) )
  {
    mSimulating = true;
    mHasProblem = true;
    mStatusMessage = tr( "QFieldCloud service is degraded" );
    mDetailsMessage = tr( "Database or storage services are not operating normally." );
  }
  else if ( scenario == QStringLiteral( "incident" ) )
  {
    mSimulating = true;
    mHasProblem = true;
    mStatusMessage = tr( "There is an ongoing incident" );
    mDetailsMessage = QStringLiteral( "We are currently investigating increased error rates on project synchronization. Some users may experience timeouts." );
  }
  else if ( scenario == QStringLiteral( "maintenance" ) )
  {
    mSimulating = true;
    mHasProblem = true;
    mStatusMessage = tr( "QFieldCloud is under maintenance" );
    mDetailsMessage = QStringLiteral( "Scheduled database migration. Service may be intermittently unavailable." );
  }
  else if ( scenario == QStringLiteral( "full" ) )
  {
    mSimulating = true;
    mHasProblem = true;
    mStatusMessage = tr( "QFieldCloud service is degraded" ) + QStringLiteral( ". " ) + tr( "There is an ongoing incident" ) + QStringLiteral( ". " ) + tr( "QFieldCloud is under maintenance" );
    mDetailsMessage = QStringLiteral( "Major outage affecting all services.\n\nEmergency maintenance in progress." );
  }
  else
  {
    // "ok" or anything else: clear the simulated state
    mSimulating = false;
    mHasProblem = false;
    mStatusMessage.clear();
    mDetailsMessage.clear();
  }

  emit statusUpdated();
}

void QFieldCloudStatus::fetchStatus()
{
  if ( mUrl.isEmpty() )
  {
    return;
  }

  if ( mPendingReply )
  {
    return;
  }

  QNetworkRequest request;
  request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );
  request.setAttribute( QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::RedirectPolicy::NoLessSafeRedirectPolicy );

  QUrl requestUrl( mUrl + QStringLiteral( "/api/v1/status/" ) );
  request.setUrl( requestUrl );

  mPendingReply = NetworkManager::get( request );

  connect( mPendingReply, &NetworkReply::finished, this, [this]() {
    if ( !mPendingReply )
    {
      return;
    }
    QNetworkReply *rawReply = mPendingReply->currentRawReply();
    if ( rawReply->error() == QNetworkReply::NoError && !mSimulating )
    {
      parseStatusResponse( rawReply->readAll() );
    }
    // On network error, silently ignore — do not infer service status from network errors
    mPendingReply->deleteLater();
    mPendingReply = nullptr;
  } );
}

void QFieldCloudStatus::parseStatusResponse( const QByteArray &data )
{
  QJsonParseError jsonError;
  const QJsonDocument doc = QJsonDocument::fromJson( data, &jsonError );

  if ( jsonError.error != QJsonParseError::NoError || !doc.isObject() )
  {
    return;
  }

  const QJsonObject obj = doc.object();

  mDatabaseStatus = obj.value( QStringLiteral( "database" ) ).toString();
  mStorageStatus = obj.value( QStringLiteral( "storage" ) ).toString();
  mStatusPageUrl = obj.value( QStringLiteral( "status_page_url" ) ).toString();
  mIncidentMessage = obj.value( QStringLiteral( "incident_message" ) ).toString();
  mIncidentTimestamp = obj.value( QStringLiteral( "incident_timestamp_utc" ) ).toString();
  mMaintenanceMessage = obj.value( QStringLiteral( "maintenance_message" ) ).toString();
  mMaintenanceStartTimestamp = obj.value( QStringLiteral( "maintenance_start_timestamp_utc" ) ).toString();
  mMaintenanceEndTimestamp = obj.value( QStringLiteral( "maintenance_end_timestamp_utc" ) ).toString();

  // Determine if there is a problem
  const bool databaseDegraded = !mDatabaseStatus.isEmpty() && mDatabaseStatus != QStringLiteral( "ok" );
  const bool storageDegraded = !mStorageStatus.isEmpty() && mStorageStatus != QStringLiteral( "ok" );
  const bool hasIncident = !mIncidentTimestamp.isEmpty();
  const bool hasMaintenance = !mMaintenanceMessage.isEmpty();

  mHasProblem = databaseDegraded || storageDegraded || hasIncident || hasMaintenance;

  // Build status message
  if ( mHasProblem )
  {
    QStringList messages;

    if ( databaseDegraded || storageDegraded )
    {
      messages << tr( "QFieldCloud service is degraded" );
    }

    if ( hasIncident )
    {
      messages << tr( "There is an ongoing incident" );
    }

    if ( hasMaintenance )
    {
      messages << tr( "QFieldCloud is under maintenance" );
    }

    mStatusMessage = messages.join( QStringLiteral( ". " ) );

    // Build details message
    QStringList details;
    if ( !mIncidentMessage.isEmpty() )
    {
      details << mIncidentMessage;
    }
    if ( !mMaintenanceMessage.isEmpty() )
    {
      details << mMaintenanceMessage;
    }
    mDetailsMessage = details.join( QStringLiteral( "\n\n" ) );
  }
  else
  {
    mStatusMessage.clear();
    mDetailsMessage.clear();
  }

  emit statusUpdated();
}
