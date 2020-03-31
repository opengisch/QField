/***************************************************************************
 expressionevaluator.cpp - ExpressionEvaluator
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

#include "expressionevaluator.h"
#include "qgsproject.h"
#include "qgsexpressioncontextutils.h"

ExpressionEvaluator::ExpressionEvaluator( QObject *parent )
  : QObject( parent )
{
}

void ExpressionEvaluator::setExpressionText( const QString &expressionText )
{
  mExpressionText = expressionText;
  emit expressionTextChanged( mExpressionText );
}

void ExpressionEvaluator::setFeature( const QgsFeature &feature )
{
  mFeature = feature;
  emit featureChanged( mFeature );
}

void ExpressionEvaluator::setLayer( QgsMapLayer *layer )
{
  mLayer = layer;
  emit layerChanged( mLayer );
}

QVariant ExpressionEvaluator::evaluate()
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
