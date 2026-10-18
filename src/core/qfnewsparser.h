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
 * \brief A class containing information on a news item
 * \ingroup core
 */
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

    //! The news item unique identifier
    QString uuid;
    //! The news item title
    QString title;
    //! The news item short plain text description
    QString description;
    //! The news item publication date
    QDateTime publicationDate;
    //! The news item illustration image URL
    QUrl imageUrl;
    //! The news item URL
    QUrl url;
};

Q_DECLARE_METATYPE( QfNewsItem )


/**
 * \brief A class fetching RSS feeds and generating news items
 * published within the last MAXIMUM_AGE_DAYS days, minus those already dismissed.
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

    //! Returns the RSS feed URL
    QUrl feedUrl() const { return mFeedUrl; }

    /**
   * Sets the RSS feed URL. News items are cleared and, if available, restored
   * from the last successful fetch of that feed until the next fetch() call.
   */
    void setFeedUrl( const QUrl &url );

    /**
   * Returns TRUE if the user has opted into receiving news.
   */
    bool enabled() const;

    /**
   * Sets whether the user has opted into receiving news.
   */
    void setEnabled( bool enabled );

    //! Returns TRUE while the RSS feed is being fetched
    bool isFetching() const { return mIsFetching; }

    //! Returns the undismissed news items published within the last MAXIMUM_AGE_DAYS days, most recent first
    QList<QfNewsItem> items() const { return mItems; }

    /**
   * Fetches the RSS feed and refreshes the news items.
   * \note Nothing will be fetched when news are disabled.
   */
    Q_INVOKABLE void fetch();

    /**
   * Dismisses the news item matching a given \a uuid, it will no longer be part of the news items.
   */
    Q_INVOKABLE void dismiss( const QString &uuid );

    /**
   * Parses an RSS feed \a data content and returns its news items.
   */
    static QList<QfNewsItem> parseFeed( const QByteArray &data );

    //! Dismissed news items settings key (exposed for testing purpose)
    static inline const QString DISMISSED_UUIDS_SETTINGS_KEY = QStringLiteral( "/QField/News/DismissedUuids" );

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
    //! News items older than this number of days are no longer shown
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
