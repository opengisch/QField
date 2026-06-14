

# File expressioncalculatorlocatorfilter.h

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**locator**](dir_952e15d753357bc11da55d627a96f3ed.md) **>** [**expressioncalculatorlocatorfilter.h**](expressioncalculatorlocatorfilter_8h.md)

[Go to the documentation of this file](expressioncalculatorlocatorfilter_8h.md)


```C++
/***************************************************************************
  expressioncalculatorlocatorfilter.h

 ---------------------
 begin                : 11.04.2023
 copyright            : (C) 2023 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef EXPRESSIONCALCULATORLOCATORFILTER_H
#define EXPRESSIONCALCULATORLOCATORFILTER_H

#include <QObject>
#include <qgsexpressioncontext.h>
#include <qgslocatorfilter.h>


class LocatorModelSuperBridge;

class ExpressionCalculatorLocatorFilter : public QgsLocatorFilter
{
    Q_OBJECT

  public:
    enum ActionOrigin
    {
      Normal,
    };

    explicit ExpressionCalculatorLocatorFilter( LocatorModelSuperBridge *locatorBridge, QObject *parent = nullptr );
    ExpressionCalculatorLocatorFilter *clone() const override;
    QString name() const override { return QStringLiteral( "calculator" ); }
    QString displayName() const override { return tr( "Calculator" ); }
    QString description() const override { return tr( "Returns the value of an expression typed in the search bar." ); }
    Priority priority() const override { return Highest; }
    QString prefix() const override { return QStringLiteral( "=" ); }
    QgsLocatorFilter::Flags flags() const override { return QgsLocatorFilter::FlagFast; }

    void fetchResults( const QString &string, const QgsLocatorContext &context, QgsFeedback *feedback ) override;
    void triggerResult( const QgsLocatorResult &result ) override;
    void triggerResultFromAction( const QgsLocatorResult &result, const int actionId ) override;

  private:
    LocatorModelSuperBridge *mLocatorBridge = nullptr;
};

#endif // EXPRESSIONCALCULATORLOCATORFILTER_H
```


