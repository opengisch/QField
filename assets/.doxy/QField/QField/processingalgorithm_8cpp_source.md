

# File processingalgorithm.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**processing**](dir_14e4815d6f2d5bd986fc95a1af67243d.md) **>** [**processingalgorithm.cpp**](processingalgorithm_8cpp.md)

[Go to the documentation of this file](processingalgorithm_8cpp.md)


```C++
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
#include <qgsvectorlayerutils.h>


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

  emit idChanged( mAlgorithmId );

  if ( mPreview && mAlgorithm )
  {
    run( true );
  }
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

  if ( mPreview && mAlgorithm )
  {
    run( true );
  }
}

void ProcessingAlgorithm::setInPlaceFeatures( const QList<QgsFeature> &features )
{
  if ( mInPlaceFeatures == features )
  {
    return;
  }

  mInPlaceFeatures = features;

  emit inPlaceFeaturesChanged();

  if ( mPreview && mAlgorithm )
  {
    run( true );
  }
}

void ProcessingAlgorithm::setParameters( const QVariantMap &parameters )
{
  if ( mAlgorithmParameters == parameters )
  {
    return;
  }

  mAlgorithmParameters = parameters;

  emit parametersChanged();

  if ( mPreview && mAlgorithm )
  {
    run( true );
  }
}

void ProcessingAlgorithm::setPreview( bool preview )
{
  if ( mPreview == preview )
  {
    return;
  }

  mPreview = preview;

  emit previewChanged();

  if ( mPreview && mAlgorithm )
  {
    run( true );
  }
  else
  {
    if ( !mPreviewGeometries.isEmpty() )
    {
      mPreviewGeometries.clear();
      emit previewGeometriesChanged();
    }
  }
}

bool ProcessingAlgorithm::run( bool previewMode )
{
  if ( !mAlgorithm )
  {
    return false;
  }

  if ( previewMode )
  {
    mPreviewGeometries.clear();
    emit previewGeometriesChanged();
  }

  QgsProcessingContext context;
  context.setProject( QgsProject::instance() );

  QgsProcessingFeedback feedback;
  context.setFeedback( &feedback );

  QVariantMap parameters = mAlgorithmParameters;

  if ( mInPlaceLayer )
  {
    if ( mInPlaceFeatures.isEmpty() || !mAlgorithm->supportInPlaceEdit( mInPlaceLayer.data() ) )
    {
      return false;
    }

    context.expressionContext().appendScope( mInPlaceLayer->createExpressionContextScope() );

    QStringList featureIds;
    for ( const QgsFeature &feature : mInPlaceFeatures )
    {
      featureIds << QString::number( feature.id() );
    }

    const QgsProcessingFeatureBasedAlgorithm *featureBasedAlgorithm = dynamic_cast<const QgsProcessingFeatureBasedAlgorithm *>( mAlgorithm );
    if ( featureBasedAlgorithm )
    {
      parameters[featureBasedAlgorithm->inputParameterName()] = QgsProcessingFeatureSourceDefinition( mInPlaceLayer->id(),
                                                                                                      false,
                                                                                                      -1,
                                                                                                      Qgis::ProcessingFeatureSourceDefinitionFlags(),
                                                                                                      Qgis::InvalidGeometryCheck(),
                                                                                                      QStringLiteral( "@id IN (%1)" ).arg( featureIds.join( ',' ) ) );
      parameters[QStringLiteral( "OUTPUT" )] = QStringLiteral( "memory:" );

      const bool wasEditing = mInPlaceLayer->editBuffer();
      if ( !previewMode && !wasEditing )
      {
        mInPlaceLayer->startEditing();
      }

      std::unique_ptr<QgsProcessingFeatureBasedAlgorithm> alg;
      alg.reset( static_cast<QgsProcessingFeatureBasedAlgorithm *>( featureBasedAlgorithm->create( { { QStringLiteral( "IN_PLACE" ), true } } ) ) );
      if ( !alg->prepare( parameters, context, &feedback ) )
      {
        return false;
      }

      QgsFeatureList features = mInPlaceFeatures;
      for ( const QgsFeature &feature : features )
      {
        QgsFeature inputFeature = QgsFeature( feature );
        context.expressionContext().setFeature( inputFeature );
        QgsFeatureList outputFeatures = alg->processFeature( inputFeature, context, &feedback );
        outputFeatures = QgsVectorLayerUtils::makeFeaturesCompatible( outputFeatures, mInPlaceLayer.data() );

        if ( previewMode )
        {
          for ( const QgsFeature &outputFeature : outputFeatures )
          {
            mPreviewGeometries << outputFeature.geometry();
          }

          emit previewGeometriesChanged();
        }
        else
        {
          auto updateOriginalFeature = [this, feature]( const QgsFeature &outputFeature ) {
            QgsGeometry outputGeometry = outputFeature.geometry();
            if ( !outputGeometry.equals( feature.geometry() ) )
            {
              mInPlaceLayer->changeGeometry( feature.id(), outputGeometry );
            }
            if ( outputFeature.attributes() != feature.attributes() )
            {
              QgsAttributeMap newAttributes;
              QgsAttributeMap oldAttributes;
              const QgsFields fields = mInPlaceLayer->fields();
              for ( const QgsField &field : fields )
              {
                const int index = fields.indexOf( field.name() );
                if ( outputFeature.attribute( index ) != feature.attribute( index ) )
                {
                  newAttributes[index] = outputFeature.attribute( index );
                  oldAttributes[index] = feature.attribute( index );
                }
              }
              mInPlaceLayer->changeAttributeValues( feature.id(), newAttributes, oldAttributes );
            }
          };

          if ( outputFeatures.isEmpty() )
          {
            // Algorithm deleted the feature, remove from the layer
            mInPlaceLayer->deleteFeature( feature.id() );
          }
          else if ( outputFeatures.size() == 1 )
          {
            // Algorithm modified the feature, adjust accordingly
            updateOriginalFeature( outputFeatures[0] );
          }
          else if ( outputFeatures.size() > 1 )
          {
            QgsFeatureList newFeatures;
            bool originalFeatureUpdated = false;
            for ( QgsFeature &outputFeature : outputFeatures )
            {
              if ( !originalFeatureUpdated )
              {
                updateOriginalFeature( outputFeature );
                originalFeatureUpdated = true;
                continue;
              }
              newFeatures << QgsVectorLayerUtils::createFeature( mInPlaceLayer.data(), outputFeature.geometry(), outputFeature.attributes().toMap(), &context.expressionContext() );
            }
            mInPlaceLayer->addFeatures( newFeatures );
          }
        }
      }

      mInPlaceLayer->commitChanges( !wasEditing );
    }
    else
    {
      parameters[QStringLiteral( "INPUT" )] = QgsProcessingFeatureSourceDefinition( mInPlaceLayer->id(),
                                                                                    false,
                                                                                    -1,
                                                                                    Qgis::ProcessingFeatureSourceDefinitionFlags(),
                                                                                    Qgis::InvalidGeometryCheck(),
                                                                                    QStringLiteral( "@id IN (%1)" ).arg( featureIds.join( ',' ) ) );
      parameters[QStringLiteral( "OUTPUT" )] = QStringLiteral( "memory:" );

      bool ok;
      QVariantMap results = mAlgorithm->run( parameters, context, &feedback, &ok, { { QStringLiteral( "IN_PLACE" ), true } } );
      if ( ok )
      {
        QgsVectorLayer *outputLayer = qobject_cast<QgsVectorLayer *>( QgsProcessingUtils::mapLayerFromString( results[QStringLiteral( "OUTPUT" )].toString(), context ) );
        if ( outputLayer )
        {
          QgsFeatureIterator outputIterator = outputLayer->getFeatures();
          QgsFeature outputFeature;
          QgsFeatureList outputFeatures;
          while ( outputIterator.nextFeature( outputFeature ) )
          {
            outputFeatures << outputFeature;
          }

          if ( previewMode )
          {
            for ( const QgsFeature &previewFeature : outputFeatures )
            {
              mPreviewGeometries << previewFeature.geometry();
            }

            emit previewGeometriesChanged();
          }
          else
          {
            const bool regeneratePrimaryKey = outputLayer->customProperty( QStringLiteral( "OnConvertFormatRegeneratePrimaryKey" ), false ).toBool();

            QgsVectorLayer *inPlaceLayer = mInPlaceLayer.data();
            QgsFeatureIds inPlaceFeatureIds;
            for ( const QgsFeature &feature : mInPlaceFeatures )
            {
              inPlaceFeatureIds << feature.id();
            }

            outputFeatures = QgsVectorLayerUtils::makeFeaturesCompatible( outputFeatures, inPlaceLayer, regeneratePrimaryKey ? QgsFeatureSink::SinkFlag::RegeneratePrimaryKey : QgsFeatureSink::SinkFlags() );

            const bool wasEditing = inPlaceLayer->editBuffer();
            inPlaceLayer->startEditing();
            inPlaceLayer->deleteFeatures( inPlaceFeatureIds );
            inPlaceLayer->addFeatures( outputFeatures );
            inPlaceLayer->commitChanges( !wasEditing );
          }
        }
      }
    }
  }
  else
  {
    // Currently, only in-place algorithms are supported
    return false;
  }

  return false;
}
```


