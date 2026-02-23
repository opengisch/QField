/***************************************************************************
    qfieldcloudstatus.h
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

#ifndef QFIELDCLOUDSTATUS_H
#define QFIELDCLOUDSTATUS_H

#include <QObject>
#include <QTimer>

class NetworkReply;

/**
 * \ingroup core
 *
 * QFieldCloudStatus fetches the QFieldCloud service status endpoint
 * and exposes the health state of the service to QML.
 *
 * It periodically polls the /api/v1/status/ endpoint and provides
 * properties to determine whether the service is degraded, has ongoing
 * incidents, or is under maintenance.
 */
class QFieldCloudStatus : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QString url READ url WRITE setUrl NOTIFY urlChanged )
    Q_PROPERTY( bool hasProblem READ hasProblem NOTIFY statusUpdated )
    Q_PROPERTY( QString statusMessage READ statusMessage NOTIFY statusUpdated )
    Q_PROPERTY( QString detailsMessage READ detailsMessage NOTIFY statusUpdated )
    Q_PROPERTY( QString statusPageUrl READ statusPageUrl NOTIFY statusUpdated )

  public:
    explicit QFieldCloudStatus( QObject *parent = nullptr );

    /**
     * Returns the base URL of the QFieldCloud server.
     */
    QString url() const;

    /**
     * Sets the base URL of the QFieldCloud server and triggers a status fetch.
     */
    void setUrl( const QString &url );

    /**
     * Returns true if the service has any problem (degraded, incident, or maintenance).
     */
    bool hasProblem() const;

    /**
     * Returns a short user-facing message summarizing the service status.
     */
    QString statusMessage() const;

    /**
     * Returns additional details (incident/maintenance message) if available.
     */
    QString detailsMessage() const;

    /**
     * Returns the URL to the status page (e.g. https://status.qfield.cloud/).
     */
    QString statusPageUrl() const;

    /**
     * Manually triggers a status fetch.
     */
    Q_INVOKABLE void refresh();

  signals:
    void urlChanged();
    void statusUpdated();

  private:
    void fetchStatus();
    void parseStatusResponse( const QByteArray &data );

    QString mUrl;
    QTimer mRefreshTimer;

    QString mStatusPageUrl;
    QString mIncidentMessage;
    QString mMaintenanceMessage;

    bool mHasProblem = false;
    QString mStatusMessage;
    QString mDetailsMessage;

    NetworkReply *mPendingReply = nullptr;
};

#endif // QFIELDCLOUDSTATUS_H
