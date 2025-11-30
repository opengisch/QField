

# File bookmarklocatorfilter.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**locator**](dir_952e15d753357bc11da55d627a96f3ed.md) **>** [**bookmarklocatorfilter.h**](bookmarklocatorfilter_8h.md)

[Go to the documentation of this file](bookmarklocatorfilter_8h.md)


```C++
/***************************************************************************
  bookmarklocatorfilter.h

 ---------------------
 begin                : 11.01.2022
 copyright            : (C) 2022 by Mathieu Pellerin
 email                : mathieu at opengis dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef BOOKMARKLOCATORFILTER_H
#define BOOKMARKLOCATORFILTER_H

#include <QObject>
#include <qgslocatorfilter.h>


class LocatorModelSuperBridge;

class BookmarkLocatorFilter : public QgsLocatorFilter
{
    Q_OBJECT

  public:
    enum ActionOrigin
    {
      Normal,
    };

    explicit BookmarkLocatorFilter( LocatorModelSuperBridge *locatorBridge, QObject *parent = nullptr );
    BookmarkLocatorFilter *clone() const override;
    QString name() const override { return QStringLiteral( "bookmarks" ); }
    QString displayName() const override { return tr( "Spatial bookmarks" ); }
    QString description() const override { return tr( "Returns a list of user and currently open project bookmarks with matching names." ); }
    Priority priority() const override { return Highest; }
    QString prefix() const override { return QStringLiteral( "b" ); }
    QgsLocatorFilter::Flags flags() const override { return QgsLocatorFilter::FlagFast; }

    void fetchResults( const QString &string, const QgsLocatorContext &context, QgsFeedback *feedback ) override;
    void triggerResult( const QgsLocatorResult &result ) override;
    void triggerResultFromAction( const QgsLocatorResult &result, const int actionId ) override;

  private:
    LocatorModelSuperBridge *mLocatorBridge = nullptr;
};

#endif // BOOKMARKLOCATORFILTER_H
```


