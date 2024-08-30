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

#include "expressioncontextutils.h"
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

void ExpressionEvaluator::setPositionInformation( const GnssPositionInformation &positionInformation )
{
  mPositionInformation = positionInformation;
  emit positionInformationChanged();
}

void ExpressionEvaluator::setCloudUserInformation( const CloudUserInformation &cloudUserInformation )
{
  mCloudUserInformation = cloudUserInformation;
  emit cloudUserInformationChanged();
}

QVariant ExpressionEvaluator::evaluate()
{
  if ( mExpressionText.isEmpty() )
    return QString();

  QgsExpressionContext expressionContext;
  expressionContext << QgsExpressionContextUtils::globalScope();

  if ( mPositionInformation.isValid() )
  {
    expressionContext << ExpressionContextUtils::positionScope( mPositionInformation, false );
  }
  if ( !mCloudUserInformation.username.isEmpty() )
  {
    expressionContext << ExpressionContextUtils::cloudUserScope( mCloudUserInformation );
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
