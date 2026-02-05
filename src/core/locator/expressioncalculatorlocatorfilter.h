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

/**
 * ExpressionCalculatorLocatorFilter is a locator filter to type in expressions
 * and copy their returned value.
 */
class ExpressionCalculatorLocatorFilter : public QgsLocatorFilter
{
    Q_OBJECT

  public:
    //! Origin of the action which triggers the result
    enum ActionOrigin
    {
      Normal,
    };

    explicit ExpressionCalculatorLocatorFilter( LocatorModelSuperBridge *locatorBridge, QObject *parent = nullptr );
    ExpressionCalculatorLocatorFilter *clone() const override;
    QString name() const override { return QStringLiteral( "calculator" ); }
    //QString displayName() const override { return tr( "Calculator" ); }
    QString displayName() const override { return tr( "계산기" ); }
    //QString description() const override { return tr( "Returns the value of an expression typed in the search bar." ); }
    QString description() const override { return tr( "검색창에 입력한 수식을 계산해 결과를 보여줍니다." ); }
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
