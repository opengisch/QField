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

/**
 * GotoLocatorFilter is a locator filter to search
 * for and display coordinates.
 */
class BookmarkLocatorFilter : public QgsLocatorFilter
{
    Q_OBJECT

  public:
    //! Origin of the action which triggers the result
    enum ActionOrigin
    {
      Normal,
    };

    explicit BookmarkLocatorFilter( LocatorModelSuperBridge *locatorBridge, QObject *parent = nullptr );
    BookmarkLocatorFilter *clone() const override;
    QString name() const override { return QStringLiteral( "bookmarks" ); }
    //QString displayName() const override { return tr( "Spatial bookmarks" ); }
    QString displayName() const override { return tr( "공간 북마크" ); }
    //QString description() const override { return tr( "Returns a list of user and currently open project bookmarks with matching names." ); }
    QString description() const override { return tr( "이름이 같은 사용자 북마크와 현재 프로젝트의 북마크를 찾아 목록으로 보여줍니다." ); }
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
