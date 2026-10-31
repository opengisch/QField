

# File qfnewsparser.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfnewsparser.h**](qfnewsparser_8h.md)

[Go to the documentation of this file](qfnewsparser_8h.md)


```C++
/***************************************************************************
                        qfnewsparser.h
                        --------------
  begin                : October 2026
  copyright            : (C) 2026 by Kaustuv Pokharel
  email                : kaustuv@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QFNEWSPARSER_H
#define QFNEWSPARSER_H

#include <QDateTime>
#include <QObject>
#include <QUrl>


class QfNewsItem
{
    Q_GADGET

    Q_PROPERTY( QString uuid MEMBER uuid )
    Q_PROPERTY( QString title MEMBER title )
    Q_PROPERTY( QString description MEMBER description )
    Q_PROPERTY( QDateTime publicationDate MEMBER publicationDate )
    Q_PROPERTY( QUrl imageUrl MEMBER imageUrl )
    Q_PROPERTY( QUrl url MEMBER url )

  public:
    bool operator==( const QfNewsItem &other ) const
    {
      return uuid == other.uuid && title == other.title && description == other.description && publicationDate == other.publicationDate && imageUrl == other.imageUrl && url == other.url;
    }
    bool operator!=( const QfNewsItem &other ) const { return !operator==( other ); }

    QString uuid;
    QString title;
    QString description;
    QDateTime publicationDate;
    QUrl imageUrl;
    QUrl url;
};

Q_DECLARE_METATYPE( QfNewsItem )



class QfNewsParser : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QUrl feedUrl READ feedUrl WRITE setFeedUrl NOTIFY feedUrlChanged )
    Q_PROPERTY( bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged )
    Q_PROPERTY( bool isFetching READ isFetching NOTIFY isFetchingChanged )
    Q_PROPERTY( QList<QfNewsItem> items READ items NOTIFY itemsChanged )

  public:
    explicit QfNewsParser( QObject *parent = nullptr );

    QUrl feedUrl() const { return mFeedUrl; }

    void setFeedUrl( const QUrl &url );

    bool enabled() const;

    void setEnabled( bool enabled );

    bool isFetching() const { return mIsFetching; }

    QList<QfNewsItem> items() const { return mItems; }

    Q_INVOKABLE void fetch();

    Q_INVOKABLE void dismiss( const QString &uuid );

    static QList<QfNewsItem> parseFeed( const QByteArray &data );

    static inline const QString DISMISSED_UUIDS_SETTINGS_KEY = QStringLiteral( "/QField/News/DismissedUuids" );

  signals:
    void feedUrlChanged();

    void enabledChanged();

    void isFetchingChanged();

    void itemsChanged();

  private:
    static constexpr int MAXIMUM_AGE_DAYS = 45;

    QString cacheFilePath() const;
    void restoreItemsFromCache();

    void clearItems();
    void updateItems( const QList<QfNewsItem> &feedItems );

    bool mEnabled = false;
    QUrl mFeedUrl;
    bool mIsFetching = false;
    QList<QfNewsItem> mItems;
};

#endif // QFNEWSPARSER_H
```


