

# File expressioncalculatorlocatorfilter.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**locator**](dir_952e15d753357bc11da55d627a96f3ed.md) **>** [**expressioncalculatorlocatorfilter.cpp**](expressioncalculatorlocatorfilter_8cpp.md)

[Go to the documentation of this file](expressioncalculatorlocatorfilter_8cpp.md)


```C++
/***************************************************************************
  expressioncalculatorlocatorfilter.cpp

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

#include "expressioncalculatorlocatorfilter.h"
#include "locatormodelsuperbridge.h"
#include "platformutilities.h"

#include <QAction>
#include <QRegularExpression>
#include <qgscoordinatereferencesystemutils.h>
#include <qgscoordinateutils.h>
#include <qgsexpressioncontextutils.h>
#include <qgsfeedback.h>
#include <qgspoint.h>
#include <qgsproject.h>


ExpressionCalculatorLocatorFilter::ExpressionCalculatorLocatorFilter( LocatorModelSuperBridge *locatorBridge, QObject *parent )
  : QgsLocatorFilter( parent )
  , mLocatorBridge( locatorBridge )
{
  setUseWithoutPrefix( false );
}

ExpressionCalculatorLocatorFilter *ExpressionCalculatorLocatorFilter::clone() const
{
  return new ExpressionCalculatorLocatorFilter( mLocatorBridge );
}

void ExpressionCalculatorLocatorFilter::fetchResults( const QString &string, const QgsLocatorContext &, QgsFeedback * )
{
  QgsExpressionContext context;
  context << QgsExpressionContextUtils::globalScope()
          << QgsExpressionContextUtils::projectScope( QgsProject::instance() );

  QString error;
  if ( QgsExpression::checkExpression( string, &context, error ) )
  {
    QgsExpression exp( string );
    const QString resultString = exp.evaluate( &context ).toString();
    if ( !resultString.isEmpty() )
    {
      QgsLocatorResult result;
      result.filter = this;
      result.displayString = tr( "Copy “%1” to clipboard" ).arg( resultString );
      result.setUserData( resultString );
      result.score = 1;
      emit resultFetched( result );
    }
  }

  return;
}

void ExpressionCalculatorLocatorFilter::triggerResult( const QgsLocatorResult &result )
{
  triggerResultFromAction( result, Normal );
}

void ExpressionCalculatorLocatorFilter::triggerResultFromAction( const QgsLocatorResult &result, const int actionId )
{
  switch ( actionId )
  {
    case Normal:
    {
      const QString resultString = result.userData().toString();
      PlatformUtilities::instance()->copyTextToClipboard( resultString );
    }
  }

  return;
}
```


