

# File expressionevaluator.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**expressionevaluator.cpp**](expressionevaluator_8cpp.md)

[Go to the documentation of this file](expressionevaluator_8cpp.md)


```C++
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

#include <qgsexpressioncontextutils.h>

ExpressionEvaluator::ExpressionEvaluator( QObject *parent )
  : QObject( parent )
{
}

void ExpressionEvaluator::setMode( Mode mode )
{
  if ( mMode == mode )
    return;

  mMode = mode;
  emit modeChanged();
}

void ExpressionEvaluator::setExpressionText( const QString &expressionText )
{
  if ( mExpressionText == expressionText )
    return;

  mExpressionText = expressionText;
  emit expressionTextChanged();
}

void ExpressionEvaluator::setFeature( const QgsFeature &feature )
{
  if ( mFeature == feature )
    return;

  mFeature = feature;
  emit featureChanged();
}

void ExpressionEvaluator::setLayer( QgsMapLayer *layer )
{
  if ( mLayer == layer )
    return;

  mLayer = layer;
  emit layerChanged();
}

void ExpressionEvaluator::setProject( QgsProject *project )
{
  if ( mProject == project )
    return;

  mProject = project;
  emit projectChanged();
}

void ExpressionEvaluator::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
    return;

  mMapSettings = mapSettings;
  emit mapSettingsChanged();
}

AppExpressionContextScopesGenerator *ExpressionEvaluator::appExpressionContextScopesGenerator() const
{
  return mAppExpressionContextScopesGenerator.data();
}

void ExpressionEvaluator::setAppExpressionContextScopesGenerator( AppExpressionContextScopesGenerator *generator )
{
  if ( mAppExpressionContextScopesGenerator == generator )
    return;

  mAppExpressionContextScopesGenerator = generator;
  emit appExpressionContextScopesGeneratorChanged();
}

void ExpressionEvaluator::setVariables( const QVariantMap &variables )
{
  if ( mVariables == variables )
    return;

  mVariables = variables;
  emit variablesChanged();
}

QVariant ExpressionEvaluator::evaluate()
{
  if ( mExpressionText.isEmpty() )
    return QString();

  QgsExpressionContext expressionContext;
  expressionContext << QgsExpressionContextUtils::globalScope();

  if ( mAppExpressionContextScopesGenerator )
  {
    QList<QgsExpressionContextScope *> scopes = mAppExpressionContextScopesGenerator->generate();
    while ( !scopes.isEmpty() )
    {
      expressionContext << scopes.takeFirst();
    }
  }
  if ( mMapSettings )
  {
    expressionContext << QgsExpressionContextUtils::mapSettingsScope( mMapSettings->mapSettings() );
  }
  if ( mProject )
  {
    expressionContext << QgsExpressionContextUtils::projectScope( mProject );
  }
  if ( mLayer )
  {
    expressionContext << QgsExpressionContextUtils::layerScope( mLayer );
  }
  if ( mFeature.isValid() )
  {
    expressionContext.setFeature( mFeature );
  }
  if ( !mVariables.isEmpty() )
  {
    QgsExpressionContextScope *scope = new QgsExpressionContextScope();
    for ( auto it = mVariables.constKeyValueBegin(); it != mVariables.constKeyValueEnd(); ++it )
    {
      scope->addVariable( QgsExpressionContextScope::StaticVariable( it->first, it->second, true, true ) );
    }
    expressionContext << scope;
  }

  QVariant value;
  if ( mMode == ExpressionMode )
  {
    QgsExpression expression( mExpressionText );
    expression.prepare( &expressionContext );
    value = expression.evaluate( &expressionContext );
  }
  else
  {
    value = QgsExpression::replaceExpressionText( mExpressionText, &expressionContext );
  }

  return value.toString();
}
```


