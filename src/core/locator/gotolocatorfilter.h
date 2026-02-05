/***************************************************************************
  gotolocatorfilter.h

 ---------------------
 begin                : 01.08.2020
 copyright            : (C) 2020 by Mathieu Pellerin
 email                : nirvn dot asia at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef GOTOLOCATORFILTER_H
#define GOTOLOCATORFILTER_H

#include <QObject>
#include <qgsexpressioncontext.h>
#include <qgslocatorfilter.h>


class LocatorModelSuperBridge;

/**
 * GotoLocatorFilter is a locator filter to search
 * for and display coordinates.
 */
class GotoLocatorFilter : public QgsLocatorFilter
{
    Q_OBJECT

  public:
    //! Origin of the action which triggers the result
    enum ActionOrigin
    {
      Normal,
      Navigation,
    };

    explicit GotoLocatorFilter( LocatorModelSuperBridge *locatorBridge, QObject *parent = nullptr );
    GotoLocatorFilter *clone() const override;
    QString name() const override { return QStringLiteral( "goto" ); }
    //QString displayName() const override { return tr( "Go to coordinate" ); }
    QString displayName() const override { return tr( "좌표로 이동" ); }
    //QString description() const override { return tr( "Returns a point from a pair of X and Y coordinates - or WGS84 latitude and longitude - typed in the search bar." ); }
    QString description() const override { return tr( "검색창에 입력한 X·Y 좌표 또는 WGS84 위도·경도 위치로 이동합니다." ); }
    Priority priority() const override { return Medium; }
    QString prefix() const override { return QStringLiteral( "go" ); }
    QgsLocatorFilter::Flags flags() const override { return QgsLocatorFilter::FlagFast; }

    void fetchResults( const QString &string, const QgsLocatorContext &context, QgsFeedback *feedback ) override;
    void triggerResult( const QgsLocatorResult &result ) override;
    void triggerResultFromAction( const QgsLocatorResult &result, const int actionId ) override;

  private:
    LocatorModelSuperBridge *mLocatorBridge = nullptr;
};

#endif // GOTOLOCATORFILTER_H
