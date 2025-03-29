/***************************************************************************
  externalstorage.h - ExternalStorage

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

#ifndef EXTERNALSTORAGE_H
#define EXTERNALSTORAGE_H

#include <QObject>
#include <qgsexternalstorage.h>
#include <qgsexternalstorageregistry.h>

/**
 * \ingroup core
 */
class ExternalStorage : public QObject
{
    Q_OBJECT

    Q_PROPERTY( Qgis::ContentStatus status READ status NOTIFY statusChanged )
    Q_PROPERTY( QString type READ type WRITE setType NOTIFY typeChanged )
    Q_PROPERTY( QString lastError READ lastError NOTIFY lastErrorChanged );

    Q_PROPERTY( QString fetchedContent READ fetchedContent NOTIFY fetchedContentChanged )

  public:
    explicit ExternalStorage( QObject *parent = nullptr );

    /**
     * Returns the current status of the external storage object. When a fetch operation has been triggered,
     * the status will reflect the last fetched content operation.
     */
    Qgis::ContentStatus status() const;

    /**
     * Returns the current external storage type string.
     */
    QString type() const;

    /**
     * Sets the current external storage type string. The type string must be tied to an
     * external storage object that was added in the QgsApplication::externalStorageRegistry().
     *
     */
    void setType( const QString &type );

    /**
     * Returns the last error emitted by an external storage operation.
     */
    QString lastError() const;

    /**
     * Returns the file path of a successfully fetched content operation.
     */
    QString fetchedContent() const;

    /**
     * Triggers a fetch operation to download the content from an external storage and
     * make it available locally.
     * \param url the remote URL of the content
     * \param authenticationConfigurationId the authentication configuration ID used to
     * connect to the external storage endpoint
     */
    Q_INVOKABLE void fetch( const QString &url, const QString &authenticationConfigurationId );

  signals:
    void statusChanged();
    void typeChanged();
    void fetchedContentChanged();
    void lastErrorChanged();

  private slots:
    void contentFetched();
    void contentErrorOccurred( const QString &errorString );

  private:
    Qgis::ContentStatus mStatus = Qgis::ContentStatus::NotStarted;
    QgsExternalStorage *mStorage = nullptr;
    QString mLastError;

    QString mFetchUrl;
    std::unique_ptr<QgsExternalStorageFetchedContent> mFetchedContent;
};

#endif // EXTERNALSTORAGE_H
