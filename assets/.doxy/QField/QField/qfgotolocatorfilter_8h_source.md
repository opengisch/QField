

# File qfgotolocatorfilter.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**locator**](dir_952e15d753357bc11da55d627a96f3ed.md) **>** [**qfgotolocatorfilter.h**](qfgotolocatorfilter_8h.md)

[Go to the documentation of this file](qfgotolocatorfilter_8h.md)


```C++
/***************************************************************************
  qfgotolocatorfilter.h

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


#ifndef QFGOTOLOCATORFILTER_H
#define QFGOTOLOCATORFILTER_H

#include <QObject>
#include <qgsexpressioncontext.h>
#include <qgslocatorfilter.h>


class QfLocatorModelSuperBridge;

class QfGotoLocatorFilter : public QgsLocatorFilter
{
    Q_OBJECT

  public:
    enum ActionOrigin
    {
      Normal,
      QfNavigation,
    };

    explicit QfGotoLocatorFilter( QfLocatorModelSuperBridge *locatorBridge, QObject *parent = nullptr );
    QfGotoLocatorFilter *clone() const override;
    QString name() const override { return QStringLiteral( "goto" ); }
    QString displayName() const override { return tr( "Go to coordinate" ); }
    QString description() const override { return tr( "Returns a point from a pair of X and Y coordinates - or WGS84 latitude and longitude - typed in the search bar." ); }
    Priority priority() const override { return Medium; }
    QString prefix() const override { return QStringLiteral( "go" ); }
    QgsLocatorFilter::Flags flags() const override { return QgsLocatorFilter::FlagFast; }

    void fetchResults( const QString &string, const QgsLocatorContext &context, QgsFeedback *feedback ) override;
    void triggerResult( const QgsLocatorResult &result ) override;
    void triggerResultFromAction( const QgsLocatorResult &result, const int actionId ) override;

  private:
    QfLocatorModelSuperBridge *mLocatorBridge = nullptr;
};

#endif // QFGOTOLOCATORFILTER_H
```


