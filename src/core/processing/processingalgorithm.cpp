/***************************************************************************
  processingalgorithm.cpp - ProcessingAlgorithm

 ---------------------
 begin                : 22.06.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "processingalgorithm.h"

#include <qgsapplication.h>
#include <qgsprocessingalgorithm.h>
#include <qgsprocessingprovider.h>
#include <qgsprocessingregistry.h>
#include <qgsvectorlayer.h>


ProcessingAlgorithm::ProcessingAlgorithm( QObject *parent )
  : QObject( parent )
{
}

void ProcessingAlgorithm::setId( const QString &id )
{
  if ( mAlgorithmId == id )
  {
    return;
  }

  mAlgorithmId = id;
  mAlgorithm = !mAlgorithmId.isEmpty() ? QgsApplication::instance()->processingRegistry()->algorithmById( mAlgorithmId ) : nullptr;

  if ( mAlgorithmParametersModel )
  {
    mAlgorithmParametersModel->setAlgorithmId( id );
  }

  emit idChanged( mAlgorithmId );
}

QString ProcessingAlgorithm::displayName() const
{
  return mAlgorithm ? mAlgorithm->displayName() : QString();
}

QString ProcessingAlgorithm::shortHelp() const
{
  return mAlgorithm ? mAlgorithm->shortHelpString() : QString();
}

void ProcessingAlgorithm::setInPlaceLayer( QgsVectorLayer *layer )
{
  if ( mInPlaceLayer.data() == layer )
  {
    return;
  }

  mInPlaceLayer = layer;
  emit inPlaceLayerChanged();
}

void ProcessingAlgorithm::setParametersModel( ProcessingAlgorithmParametersModel *parametersModel )
{
  if ( mAlgorithmParametersModel == parametersModel )
  {
    return;
  }

  if ( mAlgorithmParametersModel )
  {
    disconnect( mAlgorithmParametersModel, &ProcessingAlgorithmParametersModel::algorithmIdChanged, this, &ProcessingAlgorithm::setId );
  }

  mAlgorithmParametersModel = parametersModel;

  if ( mAlgorithmParametersModel )
  {
    mAlgorithmParametersModel->setAlgorithmId( mAlgorithmId );
    connect( mAlgorithmParametersModel, &ProcessingAlgorithmParametersModel::algorithmIdChanged, this, &ProcessingAlgorithm::setId );
  }

  emit parametersModelChanged();
}
