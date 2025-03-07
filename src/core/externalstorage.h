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

    Q_PROPERTY( QString fetchedContent READ fetchedContent NOTIFY authenticationIdChanged )

  public:
    explicit ExternalStorage( QObject *parent = nullptr );

    Qgis::ContentStatus status() const;

    QString type() const;

    void setType( const QString &type );

    Q_INVOKABLE void fetch( const QString &url, const QString &authenticationId );

    QString fetchedContent() const;

  signals:
    void statusChanged();
    void typeChanged();
    void authenticationIdChanged();
    void fetchedContentChanged();

  private slots:
    void contentFetched();

  private:
    Qgis::ContentStatus mStatus = Qgis::ContentStatus::NotStarted;
    std::unique_ptr<QgsExternalStorage> mStorage;
    QString mAuthenticationId;

    QString mFetchUrl;
    std::unique_ptr<QgsExternalStorageFetchedContent> mFetchedContent;
};

#endif // EXTERNALSTORAGE_H
