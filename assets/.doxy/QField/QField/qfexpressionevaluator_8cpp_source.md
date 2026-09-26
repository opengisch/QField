

# File qfexpressionevaluator.cpp

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qfexpressionevaluator.cpp**](qfexpressionevaluator_8cpp.md)

[Go to the documentation of this file](qfexpressionevaluator_8cpp.md)


```C++
/***************************************************************************
 qfexpressionevaluator.cpp - QfExpressionEvaluator
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

#include "qfexpressionevaluator.h"

#include <qgsexpressioncontextutils.h>

QfExpressionEvaluator::QfExpressionEvaluator( QObject *parent )
  : QObject( parent )
{
}

void QfExpressionEvaluator::setMode( Mode mode )
{
  if ( mMode == mode )
    return;

  mMode = mode;
  emit modeChanged();
}

void QfExpressionEvaluator::setExpressionText( const QString &expressionText )
{
  if ( mExpressionText == expressionText )
    return;

  mExpressionText = expressionText;
  emit expressionTextChanged();
}

void QfExpressionEvaluator::setFeature( const QgsFeature &feature )
{
  if ( mFeature == feature )
    return;

  mFeature = feature;
  emit featureChanged();
}

void QfExpressionEvaluator::setLayer( QgsMapLayer *layer )
{
  if ( mLayer == layer )
    return;

  mLayer = layer;
  emit layerChanged();
}

void QfExpressionEvaluator::setProject( QgsProject *project )
{
  if ( mProject == project )
    return;

  mProject = project;
  emit projectChanged();
}

void QfExpressionEvaluator::setMapSettings( QgsQuickMapSettings *mapSettings )
{
  if ( mMapSettings == mapSettings )
    return;

  mMapSettings = mapSettings;
  emit mapSettingsChanged();
}

QfAppExpressionContextScopesGenerator *QfExpressionEvaluator::appExpressionContextScopesGenerator() const
{
  return mAppExpressionContextScopesGenerator.data();
}

void QfExpressionEvaluator::setAppExpressionContextScopesGenerator( QfAppExpressionContextScopesGenerator *generator )
{
  if ( mAppExpressionContextScopesGenerator == generator )
    return;

  mAppExpressionContextScopesGenerator = generator;
  emit appExpressionContextScopesGeneratorChanged();
}

QfAttributeFormModel *QfExpressionEvaluator::attributeFormModel() const
{
  return mAttributeFormModel.data();
}

void QfExpressionEvaluator::setAttributeFormModel( QfAttributeFormModel *attributeFormModel )
{
  if ( mAttributeFormModel == attributeFormModel )
    return;

  mAttributeFormModel = attributeFormModel;
  emit attributeFormModelChanged();
}

void QfExpressionEvaluator::setVariables( const QVariantMap &variables )
{
  if ( mVariables == variables )
    return;

  mVariables = variables;
  emit variablesChanged();
}

QVariant QfExpressionEvaluator::evaluate()
{
  return evaluate( mExpressionText );
}

QVariant QfExpressionEvaluator::evaluate( const QString &expressionText )
{
  if ( expressionText.isEmpty() )
    return QString();

  QgsExpressionContext expressionContext;
  if ( mAttributeFormModel )
  {
    expressionContext = mAttributeFormModel->createExpressionContext();
  }
  else
  {
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
  }

  QVariant value;
  if ( mMode == ExpressionMode )
  {
    QgsExpression expression( expressionText );
    expression.prepare( &expressionContext );
    value = expression.evaluate( &expressionContext );
  }
  else
  {
    value = QgsExpression::replaceExpressionText( expressionText, &expressionContext );
  }

  return value.toString();
}
```


