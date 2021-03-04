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

#include <qgslocatorfilter.h>
#include <qgsexpressioncontext.h>


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
      AddFeature
    };

    explicit GotoLocatorFilter( LocatorModelSuperBridge *locatorBridge, QObject *parent = nullptr );
    GotoLocatorFilter *clone() const override;
    QString name() const override { return QStringLiteral( "goto" ); }
    QString displayName() const override { return tr( "Go to coordinate" ); }
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
