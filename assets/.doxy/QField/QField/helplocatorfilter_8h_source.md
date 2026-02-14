

# File helplocatorfilter.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**locator**](dir_952e15d753357bc11da55d627a96f3ed.md) **>** [**helplocatorfilter.h**](helplocatorfilter_8h.md)

[Go to the documentation of this file](helplocatorfilter_8h.md)


```C++
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

class HelpLocatorFilter : public QgsLocatorFilter
{
    Q_OBJECT

  public:
    enum ActionOrigin
    {
      Normal,
    };

    explicit HelpLocatorFilter( LocatorModelSuperBridge *locatorBridge, QObject *parent = nullptr );
    HelpLocatorFilter *clone() const override;
    QString name() const override { return QStringLiteral( "optionpages" ); }
    QString displayName() const override { return tr( "QField Documentation" ); }
    QString description() const override { return tr( "Returns QField documentation pages matching terms." ); }
    Priority priority() const override { return Medium; }
    QString prefix() const override { return QStringLiteral( "?" ); }

    void fetchResults( const QString &string, const QgsLocatorContext &context, QgsFeedback *feedback ) override;
    void triggerResult( const QgsLocatorResult &result ) override;
    void triggerResultFromAction( const QgsLocatorResult &result, const int actionId ) override;

  private:
    LocatorModelSuperBridge *mLocatorBridge = nullptr;
};

#endif // HELPLOCATORFILTER_H
```


