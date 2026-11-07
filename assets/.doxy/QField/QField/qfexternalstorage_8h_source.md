

# File qfexternalstorage.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfexternalstorage.h**](qfexternalstorage_8h.md)

[Go to the documentation of this file](qfexternalstorage_8h.md)


```C++
/***************************************************************************
  qfexternalstorage.h - QfExternalStorage

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

#ifndef QFEXTERNALSTORAGE_H
#define QFEXTERNALSTORAGE_H

#include <QObject>
#include <qgsexternalstorage.h>
#include <qgsexternalstorageregistry.h>

class QfExternalStorage : public QObject
{
    Q_OBJECT

    Q_PROPERTY( Qgis::ContentStatus status READ status NOTIFY statusChanged )
    Q_PROPERTY( QString type READ type WRITE setType NOTIFY typeChanged )
    Q_PROPERTY( QString lastError READ lastError NOTIFY lastErrorChanged )

    Q_PROPERTY( QString fetchedContent READ fetchedContent NOTIFY fetchedContentChanged )

  public:
    explicit QfExternalStorage( QObject *parent = nullptr );

    Qgis::ContentStatus status() const;

    QString type() const;

    void setType( const QString &type );

    QString lastError() const;

    QString fetchedContent() const;

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

#endif // QFEXTERNALSTORAGE_H
```


