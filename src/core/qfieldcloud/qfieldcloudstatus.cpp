/***************************************************************************
    qfieldcloudstatus.cpp
    ---------------------
    begin                : February 2026
    copyright            : (C) 2026 by Mohsen Dehghanzadeh
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
#include <qgsmessagelog.h>


QFieldCloudStatus::QFieldCloudStatus( QObject *parent )
  : QObject( parent )
{
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

  // Reset state when URL changes
  mHasProblem = false;
  mStatusMessage.clear();
  mDetailsMessage.clear();
  mStatusPageUrl.clear();
  emit statusUpdated();

  if ( !mUrl.isEmpty() )
  {
    mRefreshTimer.setInterval( 5 * 60 * 1000 );
    fetchStatus();
    mRefreshTimer.start();
  }
  else
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
    if ( rawReply->error() == QNetworkReply::NoError )
    {
      parseStatusResponse( rawReply->readAll() );
    }
    else
    {
      QgsMessageLog::logMessage( QStringLiteral( "QFieldCloud status check failed: %1" ).arg( rawReply->errorString() ), QStringLiteral( "QFieldCloud" ), Qgis::MessageLevel::Warning );
    }
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

  const QString databaseStatus = obj.value( QStringLiteral( "database" ) ).toString();
  const QString storageStatus = obj.value( QStringLiteral( "storage" ) ).toString();
  mStatusPageUrl = obj.value( QStringLiteral( "status_page_url" ) ).toString();
  mIncidentMessage = obj.value( QStringLiteral( "incident_message" ) ).toString();
  const QString incidentTimestamp = obj.value( QStringLiteral( "incident_timestamp_utc" ) ).toString();
  mMaintenanceMessage = obj.value( QStringLiteral( "maintenance_message" ) ).toString();

  // Determine if there is a problem
  const bool databaseDegraded = !databaseStatus.isEmpty() && databaseStatus != QStringLiteral( "ok" );
  const bool storageDegraded = !storageStatus.isEmpty() && storageStatus != QStringLiteral( "ok" );
  const bool hasIncident = !incidentTimestamp.isEmpty();
  const bool hasMaintenance = !mMaintenanceMessage.isEmpty();

  mHasProblem = databaseDegraded || storageDegraded || hasIncident || hasMaintenance;

  // Adjust polling interval: faster when there's a problem, slower when ok
  mRefreshTimer.setInterval( mHasProblem ? 30 * 1000 : 5 * 60 * 1000 );

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
