/***************************************************************************
 expressionutils.cpp - ExpressionUtils
                              -------------------
 begin                : January 2020
 copyright            : (C) 2020 by David Signer
 email                : david (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "expressionutils.h"
#include "qgsproject.h"
#include "qgsexpressioncontextutils.h"

ExpressionUtils::ExpressionUtils( QObject *parent )
  : QObject( parent )
{
}

QString ExpressionUtils::evaluate()
{
  if ( !mFeature.isValid() || !mLayer || mExpressionText.isEmpty() )
    return QString();

  QgsExpression exp( mExpressionText );
  QgsExpressionContext expressionContext;
  expressionContext.setFeature( mFeature );
  expressionContext << QgsExpressionContextUtils::globalScope()
                    << QgsExpressionContextUtils::projectScope( QgsProject::instance() )
                    << QgsExpressionContextUtils::layerScope( mLayer );

  QVariant value = exp.evaluate( &expressionContext );
  return value.toString();
}
