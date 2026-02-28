

# File featuremodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**featuremodel.cpp**](featuremodel_8cpp.md)

[Go to the documentation of this file](featuremodel_8cpp.md)


```C++
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
#include "layerutils.h"
#include "qgsquickmapsettings.h"
#include "vertexmodel.h"

#include <QJSValue>
#include <QMutex>
#include <qgscurvepolygon.h>
#include <qgsexpressioncontextutils.h>
#include <qgsgeometrycollection.h>
#include <qgsgeometryoptions.h>
#include <qgsmessagelog.h>
#include <qgsproject.h>
#include <qgsrelationmanager.h>
#include <qgsvectorlayer.h>
#include <qgsvectorlayereditutils.h>
#include <qgsvectorlayerutils.h>

typedef QMap<QgsVectorLayer *, FeatureModel::RememberValues> Rememberings;
Q_GLOBAL_STATIC( Rememberings, sRememberings )
Q_GLOBAL_STATIC( QMutex, sMutex )

FeatureModel::FeatureModel( QObject *parent )
  : QAbstractListModel( parent )
{
  connect( this, &FeatureModel::modelReset, this, &FeatureModel::featureChanged );
}

void FeatureModel::setModelMode( const ModelModes mode )
{
  if ( mModelMode == mode )
    return;

  beginResetModel();
  mModelMode = mode;
  emit modelModeChanged();
  endResetModel();
}

FeatureModel::ModelModes FeatureModel::modelMode() const
{
  return mModelMode;
}

void FeatureModel::setFeature( const QgsFeature &feature )
{
  if ( mModelMode != SingleFeatureModel || feature == mFeature )
    return;

  beginResetModel();
  mFeature = feature;
  mFeatures.clear();
  mAttributesAllowEdit.clear();
  emit featureChanged();
  endResetModel();

  updatePermissions();
}

void FeatureModel::setFeatures( const QList<QgsFeature> &features )
{
  if ( mModelMode != MultiFeatureModel )
    return;

  beginResetModel();
  if ( !features.isEmpty() )
  {
    mFeatures = features;
    mAttributesAllowEdit.clear();

    mFeature = mFeatures.at( 0 );
    for ( int i = 0; i < mFeature.attributes().count(); i++ )
    {
      bool equalValue = true;
      struct AttributeNotEqual
      {
          const int attributeIndex;
          const QgsFeature &feature;
          AttributeNotEqual( const QgsFeature &feature, int attributeIndex )
            : attributeIndex( attributeIndex ), feature( feature ) {}
          bool operator()( const QgsFeature &f ) const { return f.attributes().size() > attributeIndex && feature.attributes().at( attributeIndex ) != f.attributes().at( attributeIndex ); }
      };

      if ( std::any_of( mFeatures.begin(), mFeatures.end(), AttributeNotEqual( mFeature, i ) ) )
      {
        mFeature.setAttribute( i, QVariant() );
        equalValue = false;
      }
      mAttributesAllowEdit << equalValue;
    }
  }
  else
  {
    mFeatures.clear();
    mAttributesAllowEdit.clear();

    mFeature = QgsFeature();
  }
  emit featuresChanged();
  endResetModel();
}

void FeatureModel::setCurrentLayer( QgsVectorLayer *layer )
{
  if ( layer == mLayer )
    return;

  mLayer = layer;

  if ( mLayer )
  {
    connect( mLayer, &QgsVectorLayer::destroyed, this, &FeatureModel::removeLayer, Qt::UniqueConnection );

    //load remember values or create new entry
    if ( sRememberings->contains( mLayer ) )
    {
      mFeature = sRememberings->value( mLayer ).rememberedFeature;
    }
    else
    {
      mFeature = QgsFeature( mLayer->fields() );
      QMutex *mutex = sMutex;
      QMutexLocker locker( mutex );
      ( *sRememberings )[mLayer].rememberedFeature = mFeature;
      ( *sRememberings )[mLayer].rememberedAttributes.reserve( layer->fields().size() );
      const QgsEditFormConfig config = mLayer->editFormConfig();
      for ( int i = 0; i < layer->fields().size(); i++ )
      {
#if _QGIS_VERSION_INT >= 39900
        ( *sRememberings )[mLayer].rememberedAttributes << ( config.reuseLastValuePolicy( i ) == Qgis::AttributeFormReuseLastValuePolicy::AllowedDefaultOn );
#else
        ( *sRememberings )[mLayer].rememberedAttributes << config.reuseLastValue( i );
#endif
      }
    }

    if ( mLayer->customPropertyKeys().contains( QStringLiteral( "is_geometry_locked" ) ) )
    {
      // Compatibility with old QFieldSync versions prior to geometry lock being broken down into individual permissions
      const bool isLocked = mLayer->customProperty( QStringLiteral( "QFieldSync/is_geometry_locked" ), false ).toBool();
      const bool isLockedExpressionActive = mLayer->customProperty( QStringLiteral( "QFieldSync/is_geometry_locked_expression_active" ), false ).toBool();
      const QString lockedExpression = isLockedExpressionActive ? mLayer->customProperty( QStringLiteral( "QFieldSync/geometry_locked_expression" ), false ).toString().trimmed() : QString();

      mFeatureAdditionLockedByDefault = isLocked;
      mFeatureAdditionLockedExpression = lockedExpression;
      mAttributeEditingLockedByDefault = false;
      mAttributeEditingLockedExpression.clear();
      mGeometryEditingLockedByDefault = isLocked;
      mGeometryEditingLockedExpression = lockedExpression;
      mFeatureDeletionLockedByDefault = isLocked;
      mFeatureDeletionLockedExpression = lockedExpression;
    }
    else
    {
      mFeatureAdditionLockedByDefault = mLayer->customProperty( QStringLiteral( "QFieldSync/is_feature_addition_locked" ), false ).toBool();
      const bool featureAdditionLockedExpressionActive = mLayer->customProperty( QStringLiteral( "QFieldSync/is_feature_addition_locked_expression_active" ), false ).toBool();
      if ( featureAdditionLockedExpressionActive )
      {
        mFeatureAdditionLockedExpression = mLayer->customProperty( QStringLiteral( "QFieldSync/feature_addition_locked_expression" ), QString() ).toString().trimmed();
      }
      else
      {
        mFeatureAdditionLockedExpression.clear();
      }
      mAttributeEditingLockedByDefault = mLayer->customProperty( QStringLiteral( "QFieldSync/is_attribute_editing_locked" ), false ).toBool();
      const bool attributeEditingLockedExpressionActive = mLayer->customProperty( QStringLiteral( "QFieldSync/is_attribute_editing_locked_expression_active" ), false ).toBool();
      if ( attributeEditingLockedExpressionActive )
      {
        mAttributeEditingLockedExpression = mLayer->customProperty( QStringLiteral( "QFieldSync/attribute_editing_locked_expression" ), QString() ).toString().trimmed();
      }
      else
      {
        mAttributeEditingLockedExpression.clear();
      }
      mGeometryEditingLockedByDefault = mLayer->customProperty( QStringLiteral( "QFieldSync/is_geometry_editing_locked" ), false ).toBool();
      const bool geometryEditingLockedExpressionActive = mLayer->customProperty( QStringLiteral( "QFieldSync/is_geometry_editing_locked_expression_active" ), false ).toBool();
      if ( geometryEditingLockedExpressionActive )
      {
        mGeometryEditingLockedExpression = mLayer->customProperty( QStringLiteral( "QFieldSync/geometry_editing_locked_expression" ), QString() ).toString().trimmed();
      }
      else
      {
        mGeometryEditingLockedExpression.clear();
      }
      mFeatureDeletionLockedByDefault = mLayer->customProperty( QStringLiteral( "QFieldSync/is_feature_deletion_locked" ), false ).toBool();
      const bool featureDeletionLockedExpressionActive = mLayer->customProperty( QStringLiteral( "QFieldSync/is_feature_deletion_locked_expression_active" ), false ).toBool();
      if ( featureDeletionLockedExpressionActive )
      {
        mFeatureDeletionLockedExpression = mLayer->customProperty( QStringLiteral( "QFieldSync/feature_deletion_locked_expression" ), QString() ).toString().trimmed();
      }
      else
      {
        mFeatureDeletionLockedExpression.clear();
      }
    }
  }
  else
  {
    mFeatureAdditionLockedByDefault = false;
    mFeatureAdditionLockedExpression.clear();
    mAttributeEditingLockedByDefault = false;
    mAttributeEditingLockedExpression.clear();
    mGeometryEditingLockedByDefault = false;
    mGeometryEditingLockedExpression.clear();
    mFeatureDeletionLockedByDefault = false;
    mFeatureDeletionLockedExpression.clear();
  }

  updatePermissions();
  emit currentLayerChanged();
}

QgsVectorLayer *FeatureModel::layer() const
{
  return mLayer.data();
}

AppExpressionContextScopesGenerator *FeatureModel::appExpressionContextScopesGenerator() const
{
  return mAppExpressionContextScopesGenerator.data();
}

void FeatureModel::setAppExpressionContextScopesGenerator( AppExpressionContextScopesGenerator *generator )
{
  if ( mAppExpressionContextScopesGenerator == generator )
    return;

  mAppExpressionContextScopesGenerator = generator;
  emit appExpressionContextScopesGeneratorChanged();
}

Geometry *FeatureModel::geometry()
{
  return mGeometry;
}

void FeatureModel::setGeometry( Geometry *geometry )
{
  if ( mGeometry == geometry )
    return;

  mGeometry = geometry;
  emit geometryChanged();
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

QList<QgsFeature> FeatureModel::features() const
{
  return mFeatures;
}

void FeatureModel::setLinkedFeatureValues()
{
  beginResetModel();
  mLinkedAttributeIndexes.clear();

  const bool parentFeatureIsNew = std::numeric_limits<QgsFeatureId>::min() == mLinkedParentFeature.id();
  const QList<QgsRelation::FieldPair> fieldPairs = mLinkedRelation.fieldPairs();
  for ( QgsRelation::FieldPair fieldPair : fieldPairs )
  {
    mFeature.setAttribute( fieldPair.first, linkedParentFeature().attribute( fieldPair.second ) );
    if ( parentFeatureIsNew && mLinkedRelation.referencedLayer() && mLinkedRelation.referencedLayer()->dataProvider() )
    {
      if ( mFeature.attribute( fieldPair.first ).toString() == mLinkedRelation.referencedLayer()->dataProvider()->defaultValueClause( mLinkedParentFeature.fieldNameIndex( fieldPair.second ) ) )
      {
        mFeature.setAttribute( fieldPair.first, QVariant() );
      }
    }
    mLinkedAttributeIndexes.append( mFeature.fieldNameIndex( fieldPair.first ) );
  }

  switch ( mLinkedRelation.type() )
  {
    case Qgis::RelationshipType::Generated:
    {
      const QgsPolymorphicRelation linkedPolymorphicRelation = mLinkedRelation.polymorphicRelation();
      mFeature.setAttribute( linkedPolymorphicRelation.referencedLayerField(), linkedPolymorphicRelation.layerRepresentation( mLinkedRelation.referencedLayer() ) );
      mLinkedAttributeIndexes.append( mFeature.fieldNameIndex( linkedPolymorphicRelation.referencedLayerField() ) );
      break;
    }

    case Qgis::RelationshipType::Normal:
      break;
  }

  endResetModel();
  emit featureChanged();
}

void FeatureModel::setLinkedParentFeature( const QgsFeature &feature )
{
  if ( mLinkedParentFeature == feature )
    return;

  mLinkedParentFeature = feature;
  emit linkedParentFeatureChanged();

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

QString FeatureModel::linkedRelationOrderingField() const
{
  return mLinkedRelationOrderingField;
}

void FeatureModel::setLinkedRelationOrderingField( const QString &orderingField )
{
  if ( orderingField == mLinkedRelationOrderingField )
    return;

  mLinkedRelationOrderingField = orderingField;

  emit linkedRelationOrderingFieldChanged();
}

QHash<int, QByteArray> FeatureModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
  roles[AttributeName] = "AttributeName";
  roles[AttributeValue] = "AttributeValue";
  roles[Field] = "Field";
  roles[RememberAttribute] = "RememberAttribute";
  roles[LinkedAttribute] = "LinkedAttribute";
  roles[AttributeAllowEdit] = "AttributeAllowEdit";

  return roles;
}


int FeatureModel::rowCount( const QModelIndex &parent ) const
{
  if ( parent.isValid() )
    return 0;

  return static_cast<int>( mFeature.attributes().count() );
}

QVariant FeatureModel::data( const QModelIndex &index, int role ) const
{
  if ( index.row() < 0 )
    return QVariant();

  switch ( role )
  {
    case AttributeName:
      return mLayer ? mLayer->attributeDisplayName( index.row() ) : mFeature.fields().at( index.row() ).name();

    case AttributeValue:
      return mFeature.attribute( index.row() );

    case Field:
      return mLayer ? mLayer->fields().at( index.row() ) : mFeature.fields().at( index.row() );

    case RememberAttribute:
      return sRememberings->value( mLayer ).rememberedAttributes.at( index.row() );

    case LinkedAttribute:
      return mLinkedAttributeIndexes.contains( index.row() );

    case AttributeAllowEdit:
      if ( mModelMode == MultiFeatureModel )
      {
        return index.row() < mAttributesAllowEdit.count() ? mAttributesAllowEdit.at( index.row() ) : false;
      }
      return true;
  }

  return QVariant();
}

bool FeatureModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
  if ( index.row() < 0 )
    return false;

  if ( qgsVariantEqual( data( index, role ), value ) )
    return true;

  switch ( role )
  {
    case AttributeValue:
    {
      QVariant val( value );
      const QgsField field = mLayer ? mLayer->fields().at( index.row() ) : mFeature.fields().at( index.row() );

      // Objects and arrays coming from the QML realm are QJSValue objects, convert to QVariant
      if ( val.canConvert<QJSValue>() )
      {
        val = val.value<QJSValue>().toVariant();
      }

      if ( !field.convertCompatible( val ) )
      {
        QgsMessageLog::logMessage( tr( "Value \"%1\" %4 could not be converted to a compatible value for field %2(%3)." ).arg( value.toString(), field.name(), field.typeName(), value.isNull() ? "NULL" : "NOT NULL" ) );
        return false;
      }

      bool success = mFeature.setAttribute( index.row(), val );
      if ( success )
      {
        emit dataChanged( index, index, QVector<int>() << role );
        // emit a feature changed signal so the attribute form's currentFeature has up-to-date values
        emit featureChanged();
        updatePermissions();
      }
      return success;
    }

    case RememberAttribute:
    {
      if ( mLayer )
      {
        QMutex *mutex = sMutex;
        QMutexLocker locker( mutex );
        ( *sRememberings )[mLayer].rememberedAttributes[index.row()] = value.toBool();

        QgsEditFormConfig config = mLayer->editFormConfig();
#if _QGIS_VERSION_INT >= 39900
        if ( config.reuseLastValuePolicy( index.row() ) == Qgis::AttributeFormReuseLastValuePolicy::NotAllowed )
        {
          return false;
        }
        config.setReuseLastValuePolicy( index.row(), value.toBool() ? Qgis::AttributeFormReuseLastValuePolicy::AllowedDefaultOn : Qgis::AttributeFormReuseLastValuePolicy::AllowedDefaultOff );
#else
        config.setReuseLastValue( index.row(), value.toBool() );
#endif
        mLayer->setEditFormConfig( config );

        emit dataChanged( index, index, QVector<int>() << role );
      }
      break;
    }

    case AttributeAllowEdit:
    {
      if ( mModelMode == MultiFeatureModel )
      {
        mAttributesAllowEdit[index.row()] = value.toBool();
        emit dataChanged( index, index, QVector<int>() << role );
      }
      break;
    }
  }

  return false;
}

void FeatureModel::updatePermissions()
{
  const bool readOnly = mLayer ? mLayer->readOnly() : false;

  bool featureAdditionLocked = mFeatureAdditionLockedByDefault || readOnly;
  bool attributeEditingLocked = mAttributeEditingLockedByDefault || readOnly;
  bool geometryEditingLocked = mGeometryEditingLockedByDefault || readOnly;
  bool featureDeletionLocked = mFeatureDeletionLockedByDefault || readOnly;

  if ( mLayer && !mLayer->readOnly() )
  {
    if ( !mFeatureAdditionLockedExpression.isEmpty() || !mAttributeEditingLockedExpression.isEmpty() || !mGeometryEditingLockedExpression.isEmpty() || !mFeatureDeletionLockedExpression.isEmpty() )
    {
      QgsExpressionContext expressionContext = createExpressionContext();
      expressionContext.setFeature( mFeature );

      if ( !mFeatureAdditionLockedExpression.isEmpty() )
      {
        QgsExpression expression( mFeatureAdditionLockedExpression );
        expression.prepare( &expressionContext );
        featureAdditionLocked = expression.evaluate( &expressionContext ).toBool();
      }
      if ( !mAttributeEditingLockedExpression.isEmpty() )
      {
        QgsExpression expression( mAttributeEditingLockedExpression );
        expression.prepare( &expressionContext );
        attributeEditingLocked = expression.evaluate( &expressionContext ).toBool();
      }
      if ( !mGeometryEditingLockedExpression.isEmpty() )
      {
        QgsExpression expression( mGeometryEditingLockedExpression );
        expression.prepare( &expressionContext );
        geometryEditingLocked = expression.evaluate( &expressionContext ).toBool();
      }
      if ( !mFeatureDeletionLockedExpression.isEmpty() )
      {
        QgsExpression expression( mFeatureDeletionLockedExpression );
        expression.prepare( &expressionContext );
        featureDeletionLocked = expression.evaluate( &expressionContext ).toBool();
      }
    }
  }

  if ( mFeatureAdditionLocked != featureAdditionLocked )
  {
    mFeatureAdditionLocked = featureAdditionLocked;
    emit featureAdditionLockedChanged();
  }
  if ( mAttributeEditingLocked != attributeEditingLocked )
  {
    mAttributeEditingLocked = attributeEditingLocked;
    emit attributeEditingLockedChanged();
  }
  if ( mGeometryEditingLocked != geometryEditingLocked )
  {
    mGeometryEditingLocked = geometryEditingLocked;
    emit geometryEditingLockedChanged();
  }
  if ( mFeatureDeletionLocked != featureDeletionLocked )
  {
    mFeatureDeletionLocked = featureDeletionLocked;
    emit featureDeletionLockedChanged();
  }
}

void FeatureModel::updateDefaultValues()
{
  if ( !mLayer )
    return;

  QgsExpressionContext expressionContext = createExpressionContext();
  expressionContext.setFeature( mFeature );

  QgsFields fields = mLayer->fields();
  for ( int i = 0; i < fields.count(); ++i )
  {
    if ( fields.at( i ).defaultValueDefinition().isValid() && fields.at( i ).defaultValueDefinition().applyOnUpdate() )
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
  }
}

QgsExpressionContext FeatureModel::createExpressionContext() const
{
  QgsExpressionContext expressionContext;
  if ( mLayer )
  {
    expressionContext = mLayer->createExpressionContext();
  }

  if ( mAppExpressionContextScopesGenerator )
  {
    QList<QgsExpressionContextScope *> scopes = mAppExpressionContextScopesGenerator->generate();
    while ( !scopes.isEmpty() )
    {
      expressionContext << scopes.takeFirst();
    }
  }

  if ( mTopSnappingResult.isValid() )
  {
    expressionContext << ExpressionContextUtils::mapToolCaptureScope( mTopSnappingResult );
  }

  if ( mLinkedParentFeature.isValid() )
  {
    expressionContext << QgsExpressionContextUtils::parentFormScope( mLinkedParentFeature );
  }

  return expressionContext;
}

bool FeatureModel::changeGeometry( const QgsGeometry &geometry )
{
  if ( !mLayer )
  {
    return false;
  }


  if ( geometry.wkbType() != mLayer->wkbType() || mModelMode != SingleFeatureModel )
  {
    return false;
  }

  mFeature.setGeometry( geometry );
  emit featureChanged();
  updatePermissions();
  return true;
}

bool FeatureModel::save( bool flushBuffer )
{
  if ( !mLayer )
  {
    return false;
  }

  bool isSuccess = true;

  if ( mBatchMode )
  {
    // We take charge of default values that are set to be applied on feature update to take into account positioning and cloud context
    updateDefaultValues();

    QgsFeature temporaryFeature = mFeature;
    isSuccess = !mLayer->updateFeature( temporaryFeature, true );
  }
  else
  {
    const bool wasEditing = isEditing();
    flushBuffer = flushBuffer || !wasEditing;

    if ( !startEditing() )
    {
      isSuccess = false;
    }

    switch ( mModelMode )
    {
      case SingleFeatureModel:
      {
        // We take charge of default values that are set to be applied on feature update to take into account positioning and cloud context
        updateDefaultValues();

        QgsGeometry temporaryGeometry = mFeature.geometry();
        QgsAttributeMap temporaryAttributeMap = mFeature.attributes().toMap();
        bool changed = false;
        changed = mLayer->changeGeometry( mFeature.id(), temporaryGeometry, true );
        changed |= mLayer->changeAttributeValues( mFeature.id(), temporaryAttributeMap, QgsAttributeMap(), true );
        if ( !changed )
        {
          QgsMessageLog::logMessage( tr( "Cannot update feature" ), QStringLiteral( "QField" ), Qgis::Warning );
        }

        if ( flushBuffer )
        {
          isSuccess &= commit( !wasEditing );
          if ( isSuccess && !wasEditing )
          {
            QgsFeature modifiedFeature;
            if ( mLayer->getFeatures( QgsFeatureRequest().setFilterFid( mFeature.id() ) ).nextFeature( modifiedFeature ) )
            {
              if ( modifiedFeature != mFeature )
              {
                setFeature( modifiedFeature );
              }
              else
              {
                emit featureUpdated();
              }
            }
            else
            {
              QgsMessageLog::logMessage( tr( "Feature %1 could not be fetched after commit" ).arg( mFeature.id() ), QStringLiteral( "QField" ), Qgis::Warning );
            }
          }
        }
        break;
      }

      case MultiFeatureModel:
      {
        // We need to copy these members as the first feature updated triggers a refresh of the selected features, leading to changes in feature model members
        const QgsFeature referenceFeature = mFeature;
        const QList<bool> attributesAllowEdit = mAttributesAllowEdit;
        QList<QgsFeature> features = mFeatures;
        for ( QgsFeature &feature : features )
        {
          for ( int i = 0; i < referenceFeature.attributes().count(); i++ )
          {
            if ( !attributesAllowEdit[i] )
              continue;

            feature.setAttribute( i, referenceFeature.attributes().at( i ) );
          }

          QgsExpressionContext expressionContext = createExpressionContext();
          expressionContext.setFeature( mFeature );

          QgsFields fields = mLayer->fields();
          for ( int i = 0; i < fields.count(); ++i )
          {
            if ( fields.at( i ).defaultValueDefinition().isValid() && fields.at( i ).defaultValueDefinition().applyOnUpdate() )
            {
              QgsExpression exp( fields.at( i ).defaultValueDefinition().expression() );
              exp.prepare( &expressionContext );
              if ( exp.hasParserError() )
                QgsMessageLog::logMessage( tr( "Default value expression for %1:%2 has parser error: %3" ).arg( mLayer->name(), fields.at( i ).name(), exp.parserErrorString() ), QStringLiteral( "QField" ) );

              QVariant value = exp.evaluate( &expressionContext );
              if ( exp.hasEvalError() )
                QgsMessageLog::logMessage( tr( "Default value expression for %1:%2 has evaluation error: %3" ).arg( mLayer->name(), fields.at( i ).name(), exp.evalErrorString() ), QStringLiteral( "QField" ) );

              feature.setAttribute( i, value );
            }
          }

          if ( !mLayer->updateFeature( feature, true ) )
          {
            QgsMessageLog::logMessage( tr( "Cannot update feature" ), QStringLiteral( "QField" ), Qgis::Warning );
          }
        }

        if ( flushBuffer )
        {
          isSuccess &= commit( !wasEditing );
        }
      }
    }
  }

  return isSuccess;
}

void FeatureModel::reset()
{
  if ( !mLayer )
    return;

  mLayer->rollBack();
}

void FeatureModel::refresh()
{
  if ( !mLayer || FID_IS_NEW( mFeature.id() ) )
    return;

  QgsFeature feat;
  if ( mLayer->getFeatures( QgsFeatureRequest().setFilterFid( mFeature.id() ) ).nextFeature( feat ) )
  {
    setFeature( feat );
  }
}

bool FeatureModel::suppressFeatureForm() const
{
  if ( !mLayer )
    return false;

  return mLayer->editFormConfig().suppress() == Qgis::AttributeFormSuppression::On;
}

void FeatureModel::resetFeature()
{
  if ( !mLayer )
    return;

  if ( sRememberings->contains( mLayer ) )
  {
    QMutex *mutex = sMutex;
    QMutexLocker locker( mutex );
    ( *sRememberings )[mLayer].rememberedFeature = mFeature;
  }

  mFeature = QgsFeature( mLayer->fields() );
}

void FeatureModel::resetFeatureId()
{
  if ( !mLayer )
    return;

  if ( sRememberings->contains( mLayer ) )
  {
    QMutex *mutex = sMutex;
    QMutexLocker locker( mutex );
    ( *sRememberings )[mLayer].rememberedFeature = mFeature;
  }

  mFeature.setId( FID_NULL );
}

void FeatureModel::resetAttributes( bool partialReset )
{
  if ( !mLayer )
    return;

  QgsFields fields = mLayer->fields();
  beginResetModel();
  for ( int i = 0; i < fields.count(); ++i )
  {
    //if the value does not need to be remembered and it's not prefilled by the linked parent feature
    if ( !sRememberings->value( mLayer ).rememberedAttributes.at( i ) && !mLinkedAttributeIndexes.contains( i ) )
    {
      if ( !partialReset || fields.at( i ).defaultValueDefinition().isValid() )
      {
        mFeature.setAttribute( i, QVariant() );
      }
    }
    else if ( sRememberings->value( mLayer ).rememberedAttributes.at( i ) )
    {
      mFeature.setAttribute( i, sRememberings->value( mLayer ).rememberedFeature.attribute( i ) );
    }

    if ( mLinkedRelationOrderingField == fields.at( i ).name() && mLinkedRelation.isValid() && mLinkedParentFeature.isValid() )
    {
      int maxOrdering = 0;

      QgsFeatureIterator relatedFeaturesIt = mLinkedRelation.getRelatedFeatures( mLinkedParentFeature );
      QgsFeature siblingFeature;

      while ( relatedFeaturesIt.nextFeature( siblingFeature ) )
      {
        int siblingOrdering = siblingFeature.attribute( mLinkedRelationOrderingField ).toInt();
        if ( siblingOrdering > maxOrdering )
          maxOrdering = siblingOrdering;
      }

      maxOrdering += 1;

      mFeature.setAttribute( i, maxOrdering );
    }
  }

  // Reset default values
  QgsExpressionContext expressionContext = createExpressionContext();
  expressionContext.setFeature( mFeature );
  mFeature = QgsVectorLayerUtils::createFeature( mLayer, mFeature.geometry(), mFeature.attributes().toMap(), &expressionContext );
  endResetModel();

  updatePermissions();
}

bool FeatureModel::updateAttributesFromFeature( const QgsFeature &feature )
{
  bool updated = false;
  QList<int> primaryKeyAttributes;
  if ( mLayer )
  {
    primaryKeyAttributes << mLayer->primaryKeyAttributes();
    const QString sourcePrimaryKeys = mLayer->customProperty( QStringLiteral( "QFieldSync/sourceDataPrimaryKeys" ) ).toString();
    if ( !sourcePrimaryKeys.isEmpty() && mFeature.fields().lookupField( sourcePrimaryKeys ) >= 0 )
    {
      primaryKeyAttributes << mFeature.fields().lookupField( sourcePrimaryKeys );
    }
  }
  const QgsFields fields = feature.fields();
  for ( int i = 0; i < fields.size(); i++ )
  {
    const int idx = mFeature.fields().lookupField( fields[i].name() );
    if ( idx >= 0 )
    {
      if ( primaryKeyAttributes.contains( idx ) )
      {
        continue;
      }

      const QgsField field = mFeature.fields()[idx];
      if ( !field.defaultValueDefinition().expression().isEmpty() && !field.defaultValueDefinition().applyOnUpdate() )
      {
        // Skip attributes that have default value set only once on feature creation to avoid overriding generated UUIDs
        continue;
      }

      if ( mLayer )
      {
        if ( mLayer->editFormConfig().readOnly( idx ) )
        {
          // Skip attributes that are marked as read-only
          continue;
        }

        QgsProperty property = mLayer->editFormConfig().dataDefinedFieldProperties( field.name() ).property( QgsEditFormConfig::DataDefinedProperty::Editable );
        if ( property.isActive() )
        {
          QgsExpression expression( property.asExpression() );
          QgsExpressionContext expressionContext = createExpressionContext();
          expressionContext.setFeature( mFeature );
          expression.prepare( &expressionContext );
          if ( !expression.evaluate( &expressionContext ).toBool() )
          {
            // Skip attributes who's data-defined editable property returns false
            continue;
          }
        }
      }

      if ( setData( index( idx ), feature.attributes()[i], AttributeValue ) )
      {
        updated = true;
      }
    }
  }
  return updated;
}

void FeatureModel::applyGeometry( bool fromVertexModel )
{
  if ( ( !fromVertexModel && !mGeometry ) || ( fromVertexModel && !mVertexModel ) )
  {
    return;
  }

  const bool wasEditing = mLayer->editBuffer();
  const bool requiresEditing = ( mProject && mProject->topologicalEditing() );
  if ( !wasEditing && requiresEditing )
  {
    mLayer->startEditing();
  }

  QString error;
  QgsGeometry geometry = fromVertexModel ? mVertexModel->geometry() : mGeometry->asQgsGeometry();
  QgsFeatureIds modifiedFeatureIds;
  if ( mProject && mProject->topologicalEditing() && fromVertexModel )
  {
    // Overall geometry topology not applied until we conduct further modifications, including overlap avoidance
    modifiedFeatureIds = applyVertexModelTopography();
  }

  switch ( QgsWkbTypes::geometryType( geometry.wkbType() ) )
  {
    case Qgis::GeometryType::Polygon:
      if ( !geometry.isGeosValid() )
      {
        // Remove any invalid intersection in polygon geometry
        QgsGeometry sanitizedGeometry;
        if ( const QgsGeometryCollection *collection = qgsgeometry_cast<const QgsGeometryCollection *>( geometry.constGet() ) )
        {
          QgsGeometryConstPartIterator parts = collection->parts();
          while ( parts.hasNext() )
          {
            QgsGeometry part( parts.next()->clone() );
            sanitizedGeometry.addPartV2( part.buffer( 0.0, 5 ).constGet()->clone(), Qgis::WkbType ::Polygon );
          }
        }
        else if ( qgsgeometry_cast<const QgsCurvePolygon *>( geometry.constGet() ) )
        {
          sanitizedGeometry = geometry.buffer( 0.0, 5 );
        }

        if ( !sanitizedGeometry.isNull() && sanitizedGeometry.constGet()->isValid( error ) )
        {
          geometry = sanitizedGeometry;
        }

        // PSA: calling makeValid() wipes out M values
        geometry = geometry.makeValid();
      }

      if ( mProject )
      {
        QList<QgsVectorLayer *> intersectionLayers;
        switch ( mProject->avoidIntersectionsMode() )
        {
          case Qgis::AvoidIntersectionsMode::AvoidIntersectionsCurrentLayer:
            intersectionLayers.append( mLayer );
            break;
          case Qgis::AvoidIntersectionsMode::AvoidIntersectionsLayers:
            intersectionLayers = QgsProject::instance()->avoidIntersectionsLayers();
            break;
          case Qgis::AvoidIntersectionsMode::AllowIntersections:
            break;
        }
        if ( !intersectionLayers.isEmpty() )
        {
          if ( mFeature.id() != FID_NULL )
          {
            if ( !modifiedFeatureIds.contains( mFeature.id() ) )
            {
              modifiedFeatureIds << mFeature.id();
            }
          }

          if ( fromVertexModel && !modifiedFeatureIds.isEmpty() )
          {
            if ( !modifiedFeatureIds.contains( mFeature.id() ) || modifiedFeatureIds.size() >= 2 )
            {
              if ( mFeature.id() != FID_NULL )
              {
                modifiedFeatureIds.remove( mFeature.id() );
              }

              QgsFeature modifiedFeature;
              const QgsFeatureRequest request = QgsFeatureRequest().setFilterFids( modifiedFeatureIds );
              QgsFeatureIterator it = mLayer->getFeatures( modifiedFeatureIds );
              while ( it.nextFeature( modifiedFeature ) )
              {
                QgsGeometry modifiedGeometry = modifiedFeature.geometry();
                if ( !modifiedGeometry.isGeosValid() )
                {
                  // PSA: calling makeValid() wipes out M values
                  modifiedGeometry = modifiedGeometry.makeValid();
                }
                QHash<QgsVectorLayer *, QSet<QgsFeatureId>> ignoredFeature;
                ignoredFeature.insert( mLayer, QSet<QgsFeatureId>() << modifiedFeature.id() << mFeature.id() );
                Qgis::GeometryOperationResult result = modifiedGeometry.avoidIntersectionsV2( intersectionLayers, ignoredFeature );
                if ( result != Qgis::GeometryOperationResult::NothingHappened )
                {
                  if ( QgsWkbTypes::isMultiType( modifiedGeometry.wkbType() ) && !QgsWkbTypes::isMultiType( mLayer->wkbType() ) )
                  {
                    QVector<QgsGeometry> modifiedGeometryCollection = modifiedGeometry.asGeometryCollection();
                    QVector<QgsGeometry>::iterator largestModifiedFeature = std::max_element( modifiedGeometryCollection.begin(), modifiedGeometryCollection.end(), []( const QgsGeometry &a, const QgsGeometry &b ) -> bool {
                      return a.area() < b.area();
                    } );
                    modifiedGeometry = *largestModifiedFeature;
                    modifiedGeometryCollection.erase( largestModifiedFeature );
                    for ( QgsGeometry &modifiedGeometryFromCollection : modifiedGeometryCollection )
                    {
                      QgsFeature newFeature = QgsVectorLayerUtils::createFeature( mLayer, modifiedGeometryFromCollection, modifiedFeature.attributes().toMap() );
                      const QString sourcePrimaryKeys = mLayer->customProperty( QStringLiteral( "QFieldSync/sourceDataPrimaryKeys" ) ).toString();
                      if ( !sourcePrimaryKeys.isEmpty() && mLayer->fields().lookupField( sourcePrimaryKeys ) >= 0 )
                      {
                        const int sourcePrimaryKeysIndex = mLayer->fields().lookupField( sourcePrimaryKeys );
                        if ( !mLayer->fields().at( sourcePrimaryKeysIndex ).defaultValueDefinition().isValid() )
                        {
                          newFeature.setAttribute( mLayer->fields().lookupField( sourcePrimaryKeys ), QVariant() );
                        }
                      }
                      mLayer->addFeature( newFeature );
                    }
                  }
                  mLayer->changeGeometry( modifiedFeature.id(), modifiedGeometry );
                }
              }
            }
          }

          QHash<QgsVectorLayer *, QSet<QgsFeatureId>> ignoredFeature;
          ignoredFeature.insert( mLayer, QSet<QgsFeatureId>() << mFeature.id() );
          geometry.avoidIntersectionsV2( intersectionLayers, ignoredFeature );
        }
      }
      break;

    case Qgis::GeometryType::Line:
    case Qgis::GeometryType::Point:
    case Qgis::GeometryType::Unknown:
    case Qgis::GeometryType::Null:
      break;
  }

  if ( geometry.wkbType() != Qgis::WkbType::Unknown && mLayer && mLayer->geometryOptions()->geometryPrecision() == 0.0 )
  {
    // Still do a bit of node cleanup
    QgsGeometry deduplicatedGeometry = geometry;
    deduplicatedGeometry.removeDuplicateNodes( 0.00000001 );
    if ( deduplicatedGeometry.constGet()->isValid( error ) )
      geometry = deduplicatedGeometry;
  }

  if ( mProject && mProject->topologicalEditing() )
  {
    applyGeometryTopography( geometry );
  }

  if ( requiresEditing )
  {
    mLayer->commitChanges( !wasEditing );
  }

  mFeature.setGeometry( geometry );
}

void FeatureModel::removeLayer( QObject *layer )
{
  sRememberings->remove( static_cast<QgsVectorLayer *>( layer ) );
}

void FeatureModel::setBatchMode( bool batchMode )
{
  if ( mBatchMode == batchMode )
    return;

  mBatchMode = batchMode;

  if ( mLayer )
  {
    if ( mBatchMode )
    {
      mBatchModeWasEditing = isEditing();
      if ( !mBatchModeWasEditing )
      {
        mLayer->startEditing();
      }
    }
    else
    {
      mLayer->commitChanges( !mBatchModeWasEditing );
    }
  }

  emit batchModeChanged();
}

bool FeatureModel::create( bool flushBuffer )
{
  if ( !mLayer || mFeatureAdditionLocked )
    return false;

  bool isSuccess = true;

  // The connection below will be triggered when the new feature is committed and will provide
  // the saved feature ID needed to fetch the saved feature back from the data provider
  QgsFeatureId createdFeatureId;
  QMetaObject::Connection connection = connect( mLayer, &QgsVectorLayer::featureAdded, this, [&createdFeatureId]( QgsFeatureId fid ) { createdFeatureId = fid; } );

  if ( mBatchMode )
  {
    isSuccess = mLayer->addFeature( mFeature, QgsFeatureSink::FastInsert );
    if ( isSuccess )
    {
      mFeature.setId( createdFeatureId );
    }
  }
  else
  {
    const bool wasEditing = isEditing();

    if ( !wasEditing && !startEditing() )
    {
      QgsMessageLog::logMessage( tr( "Cannot start editing on layer \"%1\" to create feature %2" ).arg( mLayer->name() ).arg( mFeature.id() ), QStringLiteral( "QField" ), Qgis::Critical );
      return false;
    }

    bool hasRelations = false;
    QList<QPair<QgsRelation, QgsFeatureRequest>> revisitRelations;
    if ( mProject )
    {
      // Gather any relationship children which would have relied on an auto-generated field value
      const QList<QgsRelation> relations = mProject->relationManager()->referencedRelations( mLayer );
      hasRelations = !relations.isEmpty();
      QgsFeature temporaryFeature = mFeature;
      for ( const QgsRelation &relation : relations )
      {
        const QgsAttributeList rereferencedFields = relation.referencedFields();
        bool needsRevisit = false;
        for ( const int fieldIndex : rereferencedFields )
        {
          if ( mLayer->dataProvider() && !mLayer->dataProvider()->defaultValueClause( fieldIndex ).isEmpty() )
          {
            temporaryFeature.setAttribute( fieldIndex, QVariant() );
            needsRevisit = true;
          }
        }
        if ( needsRevisit )
        {
          revisitRelations << qMakePair( relation, relation.getRelatedFeaturesRequest( temporaryFeature ) );
        }
      }
    }

    flushBuffer = flushBuffer || !wasEditing || hasRelations;
    if ( mLayer->addFeature( mFeature ) )
    {
      if ( mProject && mProject->topologicalEditing() && !mFeature.geometry().isEmpty() )
      {
        applyGeometryTopography( mFeature.geometry() );
      }

      if ( flushBuffer )
      {
        if ( commit( !wasEditing ) )
        {
          QgsFeature feat;
          if ( mLayer->getFeatures( QgsFeatureRequest().setFilterFid( createdFeatureId ) ).nextFeature( feat ) )
          {
            setFeature( feat );

            if ( hasRelations )
            {
              // Revisit relations in need of attribute updates
              for ( const QPair<QgsRelation, QgsFeatureRequest> &revisitRelation : std::as_const( revisitRelations ) )
              {
                const QList<QgsRelation::FieldPair> fieldPairs = revisitRelation.first.fieldPairs();
                revisitRelation.first.referencingLayer()->startEditing();
                QgsFeatureIterator it = revisitRelation.first.referencingLayer()->getFeatures( revisitRelation.second );
                QgsFeature childFeature;
                while ( it.nextFeature( childFeature ) )
                {
                  for ( const QgsRelation::FieldPair &fieldPair : fieldPairs )
                  {
                    childFeature.setAttribute( fieldPair.referencingField(), feat.attribute( fieldPair.referencedField() ) );
                  }
                  revisitRelation.first.referencingLayer()->updateFeature( childFeature );
                }
                revisitRelation.first.referencingLayer()->commitChanges();
              }

              // We need to update default values after creation to insure expression relying on relation children compute properly
              updateDefaultValues();
              save();
            }
          }
          else
          {
            QgsMessageLog::logMessage( tr( "Layer \"%1\" has been commited but the newly created feature %2 could not be fetched" ).arg( mLayer->name() ).arg( mFeature.id() ), QStringLiteral( "QField" ), Qgis::Critical );
            isSuccess = false;
          }
        }
        else
        {
          const QString msgs = mLayer->commitErrors().join( QStringLiteral( "\n" ) );
          QgsMessageLog::logMessage( tr( "Layer \"%1\" cannot be commited with the newly created feature %2. Reason:\n%3" ).arg( mLayer->name() ).arg( mFeature.id() ).arg( msgs ), QStringLiteral( "QField" ), Qgis::Critical );
          isSuccess = false;
        }
      }
    }
    else
    {
      QgsMessageLog::logMessage( tr( "Feature %2 could not be added in layer \"%1\"" ).arg( mLayer->name() ).arg( mFeature.id() ), QStringLiteral( "QField" ), Qgis::Critical );
      isSuccess = false;
    }

    if ( isSuccess && sRememberings->contains( mLayer ) )
    {
      QMutex *mutex = sMutex;
      QMutexLocker locker( mutex );
      ( *sRememberings )[mLayer].rememberedFeature = mFeature;
    }
  }

  disconnect( connection );

  return isSuccess;
}

bool FeatureModel::deleteFeature()
{
  return LayerUtils::deleteFeature( mProject, mLayer, mFeature.id(), true );
}

bool FeatureModel::commit( bool stopEditing )
{
  if ( !mLayer->commitChanges( stopEditing ) )
  {
    QgsMessageLog::logMessage( tr( "Could not save changes. Rolling back." ), QStringLiteral( "QField" ), Qgis::Critical );
    mLayer->rollBack();
    return false;
  }
  else
  {
    return true;
  }
}

bool FeatureModel::isEditing() const
{
  return mLayer->editBuffer();
}

bool FeatureModel::startEditing()
{
  // Already an edit session active
  if ( isEditing() )
  {
    return true;
  }

  if ( !mLayer->startEditing() )
  {
    QgsMessageLog::logMessage( tr( "Cannot start editing" ), QStringLiteral( "QField" ), Qgis::Warning );
    return false;
  }

  return true;
}

SnappingResult FeatureModel::topSnappingResult() const
{
  return mTopSnappingResult;
}

void FeatureModel::setTopSnappingResult( const SnappingResult &topSnappingResult )
{
  mTopSnappingResult = topSnappingResult;
}

void FeatureModel::applyGeometryToVertexModel()
{
  if ( !mVertexModel )
    return;

  mVertexModel->setGeometry( mFeature.geometry() );
}

void FeatureModel::applyGeometryTopography( const QgsGeometry &geometry )
{
  const double searchRadius = mLayer ? QgsVectorLayerEditUtils::getTopologicalSearchRadius( mLayer ) : 0.0;
  QgsRectangle bbox = geometry.boundingBox();
  bbox.grow( searchRadius );

  QgsFeature dummyFeature;
  const QgsFeatureRequest request = QgsFeatureRequest().setNoAttributes().setFlags( Qgis::FeatureRequestFlag::NoGeometry ).setLimit( 1 ).setFilterRect( bbox );
  const QVector<QgsVectorLayer *> vectorLayers = mProject->layers<QgsVectorLayer *>();
  for ( QgsVectorLayer *vectorLayer : vectorLayers )
  {
    if ( vectorLayer->readOnly() || !vectorLayer->isSpatial() )
      continue;

    if ( vectorLayer->customProperty( QStringLiteral( "QFieldSync/is_geometry_locked" ), false ).toBool() || vectorLayer->customProperty( QStringLiteral( "QFieldSync/is_geometry_locked_expression_active" ), false ).toBool() || vectorLayer->customProperty( QStringLiteral( "QFieldSync/is_geometry_editing_locked" ), false ).toBool() || vectorLayer->customProperty( QStringLiteral( "QFieldSync/is_geometry_editing_locked_expression_active" ), false ).toBool() )
      continue;

    if ( vectorLayer != mLayer )
    {
      if ( !vectorLayer->getFeatures( request ).nextFeature( dummyFeature ) )
        continue;
    }

    const bool wasEditing = vectorLayer->editBuffer();
    if ( vectorLayer != mLayer && !wasEditing )
    {
      vectorLayer->startEditing();
    }

    vectorLayer->addTopologicalPoints( geometry );

    if ( vectorLayer != mLayer )
    {
      vectorLayer->commitChanges( !wasEditing );
    }
  }
}

QgsFeatureIds FeatureModel::applyVertexModelTopography()
{
  QgsFeatureIds modifiedFeatureIds;
  if ( !mVertexModel )
  {
    return modifiedFeatureIds;
  }

  const QVector<QgsPoint> pointsAdded = mVertexModel->verticesAdded();
  const QVector<QPair<QgsPoint, QgsPoint>> pointsMoved = mVertexModel->verticesMoved();
  const QVector<QgsPoint> pointsDeleted = mVertexModel->verticesDeleted();
  QList<VertexModel::VertexChange> history = mVertexModel->history( true );

  QgsRectangle bbox;
  const double searchRadius = mLayer ? QgsVectorLayerEditUtils::getTopologicalSearchRadius( mLayer ) : 0.0;
  for ( const auto &point : pointsAdded )
  {
    QgsRectangle pointBbox = point.boundingBox();
    pointBbox.grow( searchRadius );
    bbox.combineExtentWith( pointBbox );
  }
  for ( const auto &point : pointsMoved )
  {
    QgsRectangle pointBbox = point.first.boundingBox();
    pointBbox.grow( searchRadius );
    bbox.combineExtentWith( pointBbox );
  }
  for ( const auto &point : pointsDeleted )
  {
    QgsRectangle pointBbox = point.boundingBox();
    pointBbox.grow( searchRadius );
    bbox.combineExtentWith( pointBbox );
  }

  QMetaObject::Connection connection = connect( mLayer, &QgsVectorLayer::geometryChanged, this, [&modifiedFeatureIds]( QgsFeatureId fid, const QgsGeometry & ) {
    if ( !modifiedFeatureIds.contains( fid ) )
    {
      modifiedFeatureIds << fid;
    }
  } );

  QgsFeature dummyFeature;
  const QgsFeatureRequest request = QgsFeatureRequest().setNoAttributes().setFlags( Qgis::FeatureRequestFlag::NoGeometry ).setLimit( 1 ).setFilterRect( bbox );
  const QVector<QgsVectorLayer *> vectorLayers = mProject ? mProject->layers<QgsVectorLayer *>() : QVector<QgsVectorLayer *>() << mLayer;
  for ( QgsVectorLayer *vectorLayer : vectorLayers )
  {
    if ( vectorLayer->readOnly() || !vectorLayer->isSpatial() )
      continue;

    if ( vectorLayer->customProperty( QStringLiteral( "QFieldSync/is_geometry_locked" ), false ).toBool() || vectorLayer->customProperty( QStringLiteral( "QFieldSync/is_geometry_locked_expression_active" ), false ).toBool() || vectorLayer->customProperty( QStringLiteral( "QFieldSync/is_geometry_editing_locked" ), false ).toBool() || vectorLayer->customProperty( QStringLiteral( "QFieldSync/is_geometry_editing_locked_expression_active" ), false ).toBool() )
      continue;

    if ( vectorLayer != mLayer )
    {
      if ( !vectorLayer->getFeatures( request ).nextFeature( dummyFeature ) )
        continue;
    }

    const bool wasEditing = vectorLayer->editBuffer();
    if ( !wasEditing )
    {
      vectorLayer->startEditing();
    }

    QgsPointLocator loc( vectorLayer );
    const double searchTolerance = QgsTolerance::vertexSearchRadius( vectorLayer, mVertexModel->mapSettings()->mapSettings() );
    for ( const VertexModel::VertexChange &change : history )
    {
      switch ( change.type )
      {
        case VertexModel::VertexAddition:
        {
          vectorLayer->addTopologicalPoints( change.vertex.point );
          break;
        }

        case VertexModel::VertexMove:
        {
          QgsPointLocator::MatchList matches = loc.verticesInRect( change.vertex.originalPoint, searchTolerance );
          for ( int i = 0; i < matches.size(); i++ )
          {
            vectorLayer->moveVertex( change.vertex.point, matches.at( i ).featureId(), matches.at( i ).vertexIndex() );
          }
          break;
        }

        case VertexModel::VertexDeletion:
        {
          QgsPointLocator::MatchList matches = loc.verticesInRect( change.vertex.point, searchTolerance );
          for ( int i = 0; i < matches.size(); i++ )
          {
            vectorLayer->deleteVertex( matches.at( i ).featureId(), matches.at( i ).vertexIndex() );
          }
          break;
        }

        case VertexModel::NoChange:
          break;
      }
    }

    if ( vectorLayer != mLayer )
    {
      vectorLayer->commitChanges( !wasEditing );
    }
  }

  disconnect( connection );

  return modifiedFeatureIds;
}

QVector<bool> FeatureModel::rememberedAttributes() const
{
  return sRememberings->value( mLayer ).rememberedAttributes;
}

void FeatureModel::updateRubberband() const
{
  mGeometry->updateRubberband( mFeature.geometry() );
}

void FeatureModel::setProject( QgsProject *project )
{
  if ( mProject == project )
    return;

  mProject = project;

  emit projectChanged();
}
```


