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


/**
 * \brief A class containing information on a QField blog news item
 * \ingroup core
 */
class QfNewsItem
{
    Q_GADGET

    Q_PROPERTY( QString title MEMBER title )
    Q_PROPERTY( QString description MEMBER description )
    Q_PROPERTY( QDateTime publicationDate MEMBER publicationDate )
    Q_PROPERTY( QUrl imageUrl MEMBER imageUrl )
    Q_PROPERTY( QUrl url MEMBER url )

  public:
    bool operator==( const QfNewsItem &other ) const
    {
      return title == other.title && description == other.description && publicationDate == other.publicationDate && imageUrl == other.imageUrl && url == other.url;
    }
    bool operator!=( const QfNewsItem &other ) const { return !operator==( other ); }

    //! The news item title
    QString title;
    //! The news item short plain text description
    QString description;
    //! The news item publication date
    QDateTime publicationDate;
    //! The news item illustration image URL
    QUrl imageUrl;
    //! The news item blog post URL, also used to uniquely identify the item
    QUrl url;
};

Q_DECLARE_METATYPE( QfNewsItem )


/**
 * \brief A class fetching the QField blog feed and exposing news items
 * published since the user enabled news, minus those already dismissed.
 * \ingroup core
 */
class QfNewsParser : public QObject
{
    Q_OBJECT

    Q_PROPERTY( QUrl feedUrl READ feedUrl WRITE setFeedUrl NOTIFY feedUrlChanged )
    Q_PROPERTY( bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged )
    Q_PROPERTY( bool isFetching READ isFetching NOTIFY isFetchingChanged )
    Q_PROPERTY( QList<QfNewsItem> items READ items NOTIFY itemsChanged )

  public:
    //! The news parser constructor
    explicit QfNewsParser( QObject *parent = nullptr );

    //! Returns the RSS feed URL, defaults to the QField blog feed
    QUrl feedUrl() const { return mFeedUrl; }

    //! Sets the RSS feed URL
    void setFeedUrl( const QUrl &url );

    /**
     * Returns TRUE if the user has opted into receiving news.
     */
    bool enabled() const;

    /**
     * Sets whether the user has opted into receiving news. Enabling news
     * records the current time, only news published after it will be shown.
     */
    void setEnabled( bool enabled );

    //! Returns TRUE while the blog feed is being fetched
    bool isFetching() const { return mIsFetching; }

    //! Returns the undismissed news items published since news were enabled, most recent first
    QList<QfNewsItem> items() const { return mItems; }

    /**
     * Fetches the blog feed and refreshes the news items.
     * \note Nothing will be fetched when news are disabled.
     */
    Q_INVOKABLE void fetch();

    /**
     * Dismisses the news item matching a given \a url, it will no longer be part of the news items.
     */
    Q_INVOKABLE void dismiss( const QUrl &url );

    /**
     * Parses an RSS feed \a data content and returns its news items.
     */
    static QList<QfNewsItem> parseFeed( const QByteArray &data );

  signals:
    //! Emitted when the RSS feed URL has changed
    void feedUrlChanged();

    //! Emitted when news have been enabled or disabled
    void enabledChanged();

    //! Emitted when a fetching operation has begun or ended
    void isFetchingChanged();

    //! Emitted when the list of news items has changed
    void itemsChanged();

  private:
    void updateItems( const QList<QfNewsItem> &feedItems );

    QUrl mFeedUrl = QUrl( QStringLiteral( "https://qfield.org/blog/feed.xml" ) );
    bool mIsFetching = false;
    QList<QfNewsItem> mItems;
};

#endif // QFNEWSPARSER_H
