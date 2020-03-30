/***************************************************************************
                            featuremodel.cpp
                              -------------------
              begin                : 10.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias (at) opengis.ch
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
#include "featuremodel.h"
#include "vertexmodel.h"

#include <QGeoPositionInfoSource>
#include <qgsmessagelog.h>
#include <qgsproject.h>
#include <qgsrelationmanager.h>
#include <qgsvectorlayer.h>


FeatureModel::FeatureModel( QObject *parent )
  : QAbstractListModel( parent )
{
  connect( this, &FeatureModel::modelReset, this, &FeatureModel::featureChanged );
}

void FeatureModel::setFeature( const QgsFeature &feature )
{
  beginResetModel();
  mFeature = feature;
  endResetModel();
  emit featureChanged();
}

void FeatureModel::setCurrentLayer( QgsVectorLayer *layer )
{
  if ( layer == mLayer )
    return;

  //remember the last feature of the old layer
  if ( mRememberings.contains( mLayer ) )
  {
    mRememberings[mLayer].rememberedFeature = mFeature;
  }

  mLayer = layer;

  connect( mLayer, &QgsVectorLayer::destroyed, this, &FeatureModel::removeLayer, Qt::UniqueConnection );

  if ( mLayer )
  {
    //load remember values or create new entry
    if ( mRememberings.contains( mLayer ) )
    {
      mFeature = mRememberings[mLayer].rememberedFeature;
    }
    else
    {
      mFeature = QgsFeature( mLayer->fields() );
      mRememberings[mLayer].rememberedAttributes.fill( false, layer->fields().size() );
    }
  }
  emit currentLayerChanged();
}

QgsVectorLayer *FeatureModel::layer() const
{
  return mLayer;
}

VertexModel *FeatureModel::vertexModel()
{
  return mVertexModel;
}

void FeatureModel::setVertexModel( VertexModel *model )
{
  if ( model == mVertexModel )
    return;

  mVertexModel = model;
  emit vertexModelChanged();
}

QgsFeature FeatureModel::feature() const
{
  return mFeature;
}

void FeatureModel::setLinkedFeatureValues()
{
  beginResetModel();
  mLinkedAttributeIndexes.clear();
  const auto fieldPairs = mLinkedRelation.fieldPairs();
  for ( QgsRelation::FieldPair fieldPair : fieldPairs )
  {
    mFeature.setAttribute( mFeature.fieldNameIndex( fieldPair.first ), linkedParentFeature().attribute( fieldPair.second ) );
    mLinkedAttributeIndexes.append( mFeature.fieldNameIndex( fieldPair.first ) );
  }
  endResetModel();

  emit featureChanged();
}

void FeatureModel::setLinkedParentFeature( const QgsFeature &feature )
{
  if ( mLinkedParentFeature == feature )
    return;

  mLinkedParentFeature = feature;

  if ( mLinkedRelation.isValid() )
    setLinkedFeatureValues();
}

QgsFeature FeatureModel::linkedParentFeature() const
{
  return mLinkedParentFeature;
}

void FeatureModel::setLinkedRelation( const QgsRelation &relation )
{
  mLinkedRelation = relation;

  if ( mLinkedParentFeature.isValid() )
    setLinkedFeatureValues();
}

QgsRelation FeatureModel::linkedRelation() const
{
  return mLinkedRelation;
}

QHash<int, QByteArray> FeatureModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
  roles[AttributeName] = "AttributeName";
  roles[AttributeValue] = "AttributeValue";
  roles[Field] = "Field";
  roles[RememberAttribute] = "RememberAttribute";
  roles[LinkedAttribute] = "LinkedAttribute";

  return roles;
}


int FeatureModel::rowCount( const QModelIndex &parent ) const
{
  if ( parent.isValid() )
    return 0;
  else
    return mFeature.attributes().count();
}

QVariant FeatureModel::data( const QModelIndex &index, int role ) const
{
  switch ( role )
  {
    case AttributeName:
      return mLayer->attributeDisplayName( index.row() );

    case AttributeValue:
      return mFeature.attribute( index.row() );

    case Field:
      return mLayer->fields().at( index.row() );

    case RememberAttribute:
      return mRememberings[mLayer].rememberedAttributes.at( index.row() );

    case LinkedAttribute:
      return mLinkedAttributeIndexes.contains( index.row() );
  }

  return QVariant();
}

bool FeatureModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
  if ( data( index, role ) == value )
    return true;

  switch ( role )
  {
    case AttributeValue:
    {
      QVariant val( value );
      QgsField fld = mFeature.fields().at( index.row() );

      if ( !fld.convertCompatible( val ) )
      {
        QgsMessageLog::logMessage( tr( "Value \"%1\" %4 could not be converted to a compatible value for field %2(%3)." ).arg( value.toString(), fld.name(), fld.typeName(), value.isNull() ? "NULL" : "NOT NULL" ) );
        return false;
      }
      bool success = mFeature.setAttribute( index.row(), val );
      if ( success )
        emit dataChanged( index, index, QVector<int>() << role );
      return success;
      break;
    }

    case RememberAttribute:
    {
      mRememberings[mLayer].rememberedAttributes[index.row()] = value.toBool();
      emit dataChanged( index, index, QVector<int>() << role );
      break;
    }
  }

  return false;
}

bool FeatureModel::save()
{
  if ( !mLayer )
    return false;

  bool rv = true;

  if ( !startEditing() )
  {
    rv = false;
  }

  QgsFeature feat = mFeature;
  if ( !mLayer->updateFeature( feat ) )
    QgsMessageLog::logMessage( tr( "Cannot update feature" ), "QField", Qgis::Warning );
  rv = commit();

  if ( rv )
  {
    QgsFeature feat;
    if ( mLayer->getFeatures( QgsFeatureRequest().setFilterFid( mFeature.id() ) ).nextFeature( feat ) )
      setFeature( feat );
    else
      QgsMessageLog::logMessage( tr( "Feature %1 could not be fetched after commit" ).arg( mFeature.id() ), "QField", Qgis::Warning );
  }
  return rv;
}

void FeatureModel::reset()
{
  if ( !mLayer )
    return;

  mLayer->rollBack();
}

bool FeatureModel::suppressFeatureForm() const
{
  if ( !mLayer )
    return false;

  return mLayer->editFormConfig().suppress();
}

void FeatureModel::resetAttributes()
{
  if ( !mLayer )
    return;

  QgsExpressionContext expressionContext = mLayer->createExpressionContext();
  if ( mPositionSource )
    expressionContext << ExpressionContextUtils::positionScope( mPositionSource.get() );

  //set snapping_results to ExpressionScope...
  if ( mTopSnappingResult.isValid() )
    expressionContext << ExpressionContextUtils::mapToolCaptureScope( mTopSnappingResult );

  expressionContext.setFeature( mFeature );

  QgsFields fields = mLayer->fields();

  beginResetModel();
  for ( int i = 0; i < fields.count(); ++i )
  {
    //if the value does not need to be remembered and it's not prefilled by the linked parent feature
    if ( !mRememberings[mLayer].rememberedAttributes.at( i ) && !mLinkedAttributeIndexes.contains( i ) )
    {
      if ( fields.at( i ).defaultValueDefinition().isValid() )
      {
        QgsExpression exp( fields.at( i ).defaultValueDefinition().expression() );
        exp.prepare( &expressionContext );
        if ( exp.hasParserError() )
          QgsMessageLog::logMessage( tr( "Default value expression for %1:%2 has parser error: %3" ).arg( mLayer->name(), fields.at( i ).name(), exp.parserErrorString() ), QStringLiteral( "QField" ) );

        QVariant value = exp.evaluate( &expressionContext );

        if ( exp.hasEvalError() )
          QgsMessageLog::logMessage( tr( "Default value expression for %1:%2 has evaluation error: %3" ).arg( mLayer->name(), fields.at( i ).name(), exp.evalErrorString() ), QStringLiteral( "QField" ) );


        mFeature.setAttribute( i, value );
      }
      else
      {
        mFeature.setAttribute( i, QVariant() );
      }
    }
  }
  endResetModel();
}

void FeatureModel::applyGeometry()
{
  mFeature.setGeometry( mGeometry->asQgsGeometry() );
}

void FeatureModel::removeLayer( QObject *layer )
{
  mRememberings.remove( static_cast<QgsVectorLayer *>( layer ) );
}

void FeatureModel::featureAdded( QgsFeatureId fid )
{
  setFeature( mLayer->getFeature( fid ) );
}

void FeatureModel::create()
{
  if ( !mLayer )
    return;

  startEditing();
  connect( mLayer, &QgsVectorLayer::featureAdded, this, &FeatureModel::featureAdded );
  if ( !mLayer->addFeature( mFeature ) )
  {
    QgsMessageLog::logMessage( tr( "Feature could not be added" ), "QField", Qgis::Critical );
  }

  if ( commit() )
  {
    QgsFeature feat;
    if ( mLayer->getFeatures( QgsFeatureRequest().setFilterFid( mFeature.id() ) ).nextFeature( feat ) )
      setFeature( feat );
    else
      QgsMessageLog::logMessage( tr( "Feature %1 could not be fetched after commit" ).arg( mFeature.id() ), "QField", Qgis::Warning );
  }
  disconnect( mLayer, &QgsVectorLayer::featureAdded, this, &FeatureModel::featureAdded );
}

void FeatureModel::deleteFeature()
{
  startEditing();

  //delete child features in case of compositions
  const QList<QgsRelation> referencingRelations = QgsProject::instance()->relationManager()->referencedRelations( mLayer );
  for ( const QgsRelation &referencingRelation : referencingRelations )
  {
    if ( referencingRelation.strength() == QgsRelation::Composition )
    {
      QgsVectorLayer *childLayer = referencingRelation.referencingLayer();
      childLayer->startEditing();
      QgsFeatureIterator relatedFeaturesIt = referencingRelation.getRelatedFeatures( mFeature );
      QgsFeature childFeature;
      while ( relatedFeaturesIt.nextFeature( childFeature ) )
      {
        childLayer->deleteFeature( childFeature.id() );
      }
      childLayer->commitChanges();
    }
  }

  //delete parent
  mLayer->deleteFeature( mFeature.id() );
  commit();
}

bool FeatureModel::commit()
{
  if ( !mLayer->commitChanges() )
  {
    QgsMessageLog::logMessage( tr( "Could not save changes. Rolling back." ), "QField", Qgis::Critical );
    mLayer->rollBack();
    return false;
  }
  else
  {
    return true;
  }
}

bool FeatureModel::startEditing()
{
  // Already an edit session active
  if ( mLayer->editBuffer() )
    return true;

  if ( !mLayer->startEditing() )
  {
    QgsMessageLog::logMessage( tr( "Cannot start editing" ), "QField", Qgis::Warning );
    return false;
  }
  else
  {
    return true;
  }
}

QString FeatureModel::positionSourceName() const
{
  return mPositionSource ? mPositionSource->sourceName() : QString();
}

void FeatureModel::setPositionSourceName( const QString &positionSourceName )
{
  if ( mPositionSource && mPositionSource->sourceName() == positionSourceName )
    return;

  mPositionSource.reset( QGeoPositionInfoSource::createSource( positionSourceName, this ) );
  emit positionSourceChanged();
}

SnappingResult FeatureModel::topSnappingResult() const
{
  return mTopSnappingResult;
}

void FeatureModel::setTopSnappingResult( const SnappingResult &topSnappingResult )
{
  mTopSnappingResult = topSnappingResult;
}

void FeatureModel::applyVertexModelToGeometry()
{
  if ( !mVertexModel )
    return;

  mFeature.setGeometry( mVertexModel->geometry() );
}

QVector<bool> FeatureModel::rememberedAttributes() const
{
  return mRememberings[mLayer].rememberedAttributes;
}
