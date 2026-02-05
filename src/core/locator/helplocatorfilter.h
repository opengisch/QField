/***************************************************************************
  helplocatorfilter.h

 ---------------------
 begin                : 02.07.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : mathieu at opengis dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef HELPLOCATORFILTER_H
#define HELPLOCATORFILTER_H

#include <QObject>
#include <qgslocatorfilter.h>


class LocatorModelSuperBridge;

/**
 * HelpLocatorFilter is a locator filter to search
 * for and display QField documentation pages.
 */
class HelpLocatorFilter : public QgsLocatorFilter
{
    Q_OBJECT

  public:
    //! Origin of the action which triggers the result
    enum ActionOrigin
    {
      Normal,
    };

    explicit HelpLocatorFilter( LocatorModelSuperBridge *locatorBridge, QObject *parent = nullptr );
    HelpLocatorFilter *clone() const override;
    QString name() const override { return QStringLiteral( "optionpages" ); }
    //QString displayName() const override { return tr( "QField Documentation" ); }
    QString displayName() const override { return tr( "QField 문서" ); }
    //QString description() const override { return tr( "Returns QField documentation pages matching terms." ); }
    QString description() const override { return tr( "검색어에 해당하는 QField 문서 페이지를 제공합니다." ); }
    Priority priority() const override { return Medium; }
    QString prefix() const override { return QStringLiteral( "?" ); }

    void fetchResults( const QString &string, const QgsLocatorContext &context, QgsFeedback *feedback ) override;
    void triggerResult( const QgsLocatorResult &result ) override;
    void triggerResultFromAction( const QgsLocatorResult &result, const int actionId ) override;

  private:
    LocatorModelSuperBridge *mLocatorBridge = nullptr;
};

#endif // HELPLOCATORFILTER_H
