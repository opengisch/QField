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
#include "vertexmodel.h"

#include <QGeoPositionInfoSource>
#include <QMutex>
#include <qgscurvepolygon.h>
#include <qgsgeometrycollection.h>
#include <qgsgeometryoptions.h>
#include <qgsmessagelog.h>
#include <qgsproject.h>
#include <qgsrelationmanager.h>
#include <qgsvectorlayer.h>
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
      ( *sRememberings )[mLayer].rememberedAttributes.fill( false, layer->fields().size() );
    }
  }
  emit currentLayerChanged();
}

QgsVectorLayer *FeatureModel::layer() const
{
  return mLayer.data();
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
  const auto fieldPairs = mLinkedRelation.fieldPairs();
  for ( QgsRelation::FieldPair fieldPair : fieldPairs )
  {
    mFeature.setAttribute( mFeature.fieldNameIndex( fieldPair.first ), linkedParentFeature().attribute( fieldPair.second ) );
    mLinkedAttributeIndexes.append( mFeature.fieldNameIndex( fieldPair.first ) );
  }
  endResetModel();

  emit featureChanged();
}

bool FeatureModel::positionLocked() const
{
  return mPositionLocked;
}

void FeatureModel::setPositionLocked( bool positionLocked )
{
  if ( mPositionLocked == positionLocked )
    return;

  mPositionLocked = positionLocked;

  emit positionLockedChanged();
}

void FeatureModel::setCloudUserInformation( const CloudUserInformation &cloudUserInformation )
{
  mCloudUserInformation = cloudUserInformation;

  emit cloudUserInformationChanged();
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

  return mFeature.attributes().count();
}

QVariant FeatureModel::data( const QModelIndex &index, int role ) const
{
  if ( index.row() < 0 )
    return QVariant();

  switch ( role )
  {
    case AttributeName:
      return mLayer->attributeDisplayName( index.row() );

    case AttributeValue:
      return mFeature.attribute( index.row() );

    case Field:
      return mLayer->fields().at( index.row() );

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
      QgsField fld = mLayer->fields().at( index.row() );

      if ( !fld.convertCompatible( val ) )
      {
        QgsMessageLog::logMessage( tr( "Value \"%1\" %4 could not be converted to a compatible value for field %2(%3)." ).arg( value.toString(), fld.name(), fld.typeName(), value.isNull() ? "NULL" : "NOT NULL" ) );
        return false;
      }

      bool success = mFeature.setAttribute( index.row(), val );
      if ( success )
      {
        emit dataChanged( index, index, QVector<int>() << role );
        // emit a feature changed signal so the attribute form's currentFeature has up-to-date values
        emit featureChanged();
      }
      return success;
    }

    case RememberAttribute:
    {
      QMutex *mutex = sMutex;
      QMutexLocker locker( mutex );
      ( *sRememberings )[mLayer].rememberedAttributes[index.row()] = value.toBool();
      emit dataChanged( index, index, QVector<int>() << role );
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

void FeatureModel::updateDefaultValues()
{
  if ( !mLayer )
    return;

  QgsExpressionContext expressionContext = mLayer->createExpressionContext();
  if ( mPositionInformation.isValid() )
  {
    expressionContext << ExpressionContextUtils::positionScope( mPositionInformation, mPositionLocked );
  }
  if ( mTopSnappingResult.isValid() )
  {
    expressionContext << ExpressionContextUtils::mapToolCaptureScope( mTopSnappingResult );
  }
  expressionContext << ExpressionContextUtils::cloudUserScope( mCloudUserInformation );
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

bool FeatureModel::save()
{
  if ( !mLayer )
    return false;

  bool rv = true;

  if ( !startEditing() )
  {
    rv = false;
  }

  switch ( mModelMode )
  {
    case SingleFeatureModel:
    {
      // We take charge of default values that are set to be applied on feature update to take into account positioning and cloud context
      updateDefaultValues();

      QgsFeature feat = mFeature;
      if ( !mLayer->updateFeature( feat, true ) )
        QgsMessageLog::logMessage( tr( "Cannot update feature" ), QStringLiteral( "QField" ), Qgis::Warning );

      if ( mProject && mProject->topologicalEditing() )
      {
        applyVertexModelTopography();
      }

      rv &= commit();

      if ( rv )
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
      break;
    }

    case MultiFeatureModel:
    {
      for ( QgsFeature &feature : mFeatures )
      {
        for ( int i = 0; i < mFeature.attributes().count(); i++ )
        {
          if ( !mAttributesAllowEdit[i] )
            continue;

          feature.setAttribute( i, mFeature.attributes().at( i ) );
        }

        if ( !mLayer->updateFeature( feature ) )
        {
          QgsMessageLog::logMessage( tr( "Cannot update feature" ), QStringLiteral( "QField" ), Qgis::Warning );
        }
      }
      rv &= commit();
    }
  }

  return rv;
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

  return mLayer->editFormConfig().suppress() == QgsEditFormConfig::FeatureFormSuppress::SuppressOn;
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

  QgsExpressionContext expressionContext = mLayer->createExpressionContext();
  if ( mPositionInformation.isValid() )
  {
    expressionContext << ExpressionContextUtils::positionScope( mPositionInformation, mPositionLocked );
  }

  //set snapping_results to ExpressionScope...
  if ( mTopSnappingResult.isValid() )
    expressionContext << ExpressionContextUtils::mapToolCaptureScope( mTopSnappingResult );

  expressionContext << ExpressionContextUtils::cloudUserScope( mCloudUserInformation );
  expressionContext.setFeature( mFeature );

  QgsFields fields = mLayer->fields();

  beginResetModel();
  for ( int i = 0; i < fields.count(); ++i )
  {
    //if the value does not need to be remembered and it's not prefilled by the linked parent feature
    if ( !sRememberings->value( mLayer ).rememberedAttributes.at( i ) && !mLinkedAttributeIndexes.contains( i ) )
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
      else if ( !partialReset )
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
  endResetModel();
}

void FeatureModel::applyGeometry()
{
  QString error;
  QgsGeometry geometry = mGeometry->asQgsGeometry();

  if ( QgsWkbTypes::geometryType( geometry.wkbType() ) == QgsWkbTypes::PolygonGeometry )
  {
    if ( !geometry.isGeosValid() )
    {
      // Remove any invalid intersection in polygon geometry
      QgsGeometry sanitizedGeometry;
      if ( QgsGeometryCollection *collection = qgsgeometry_cast<QgsGeometryCollection *>( geometry.get() ) )
      {
        QgsGeometryPartIterator parts = collection->parts();
        while ( parts.hasNext() )
        {
          QgsGeometry part( parts.next() );
          sanitizedGeometry.addPart( part.buffer( 0.0, 5 ).constGet()->clone(), QgsWkbTypes::PolygonGeometry );
        }
      }
      else if ( QgsCurvePolygon *polygon = qgsgeometry_cast<QgsCurvePolygon *>( geometry.get() ) )
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
#if _QGIS_VERSION_INT >= 32500
        case Qgis::AvoidIntersectionsMode::AvoidIntersectionsCurrentLayer:
#else
        case QgsProject::AvoidIntersectionsMode::AvoidIntersectionsCurrentLayer:
#endif
          intersectionLayers.append( mLayer );
          break;
#if _QGIS_VERSION_INT >= 32500
        case Qgis::AvoidIntersectionsMode::AvoidIntersectionsLayers:
#else
        case QgsProject::AvoidIntersectionsMode::AvoidIntersectionsLayers:
#endif
          intersectionLayers = QgsProject::instance()->avoidIntersectionsLayers();
          break;
#if _QGIS_VERSION_INT >= 32500
        case Qgis::AvoidIntersectionsMode::AllowIntersections:
#else
        case QgsProject::AvoidIntersectionsMode::AllowIntersections:
#endif
          break;
      }
      if ( !intersectionLayers.isEmpty() )
      {
        QHash<QgsVectorLayer *, QSet<QgsFeatureId>> ignoredFeature;
        if ( mFeature.id() != FID_NULL )
        {
          ignoredFeature.insert( mLayer, QSet<QgsFeatureId>() << mFeature.id() );
        }
        geometry.avoidIntersections( intersectionLayers, ignoredFeature );
      }
    }
  }

  if ( geometry.wkbType() != QgsWkbTypes::Unknown && mLayer && mLayer->geometryOptions()->geometryPrecision() == 0.0 )
  {
    // Still do a bit of node cleanup
    QgsGeometry deduplicatedGeometry = geometry;
    deduplicatedGeometry.removeDuplicateNodes( 0.00000001 );
    if ( deduplicatedGeometry.constGet()->isValid( error ) )
      geometry = deduplicatedGeometry;
  }

  mFeature.setGeometry( geometry );
}

void FeatureModel::removeLayer( QObject *layer )
{
  sRememberings->remove( static_cast<QgsVectorLayer *>( layer ) );
}

bool FeatureModel::create()
{
  if ( !mLayer )
    return false;

  if ( !startEditing() )
  {
    QgsMessageLog::logMessage( tr( "Cannot start editing on layer \"%1\" to create feature %2" ).arg( mLayer->name() ).arg( mFeature.id() ), QStringLiteral( "QField" ), Qgis::Critical );
    return false;
  }

  // The connection below will be triggered when the new feature is committed and will provide
  // the saved feature ID needed to fetch the saved feature back from the data provider
  QgsFeatureId createdFeatureId;
  QMetaObject::Connection connection = connect( mLayer, &QgsVectorLayer::featureAdded, this, [&createdFeatureId]( QgsFeatureId fid ) { createdFeatureId = fid; } );

  bool isSuccess = true;
  QgsFeature createdFeature = QgsVectorLayerUtils::createFeature( mLayer, mFeature.geometry(), mFeature.attributes().toMap() );
  if ( mLayer->addFeature( createdFeature ) )
  {
    if ( mProject && mProject->topologicalEditing() )
      mLayer->addTopologicalPoints( mFeature.geometry() );

    if ( commit() )
    {
      QgsFeature feat;
      if ( mLayer->getFeatures( QgsFeatureRequest().setFilterFid( createdFeatureId ) ).nextFeature( feat ) )
      {
        setFeature( feat );
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

  disconnect( connection );
  return isSuccess;
}

bool FeatureModel::deleteFeature()
{
  return LayerUtils::deleteFeature( mProject, mLayer, mFeature.id(), false );
}

bool FeatureModel::commit()
{
  if ( !mLayer->commitChanges() )
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

bool FeatureModel::startEditing()
{
  // Already an edit session active
  if ( mLayer->editBuffer() )
    return true;

  if ( !mLayer->startEditing() )
  {
    QgsMessageLog::logMessage( tr( "Cannot start editing" ), QStringLiteral( "QField" ), Qgis::Warning );
    return false;
  }

  return true;
}

GnssPositionInformation FeatureModel::positionInformation() const
{
  return mPositionInformation;
}

void FeatureModel::setPositionInformation( const GnssPositionInformation &positionInformation )
{
  mPositionInformation = positionInformation;
  emit positionInformationChanged();
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

void FeatureModel::applyGeometryToVertexModel()
{
  if ( !mVertexModel )
    return;

  mVertexModel->setGeometry( mFeature.geometry() );
}

// a filter to gather all matches at the same place
// taken from QGIS' qgsvectortool.cpp
class MatchCollectingFilter : public QgsPointLocator::MatchFilter
{
  public:
    QList<QgsPointLocator::Match> matches;

    MatchCollectingFilter() {}

    bool acceptMatch( const QgsPointLocator::Match &match ) override
    {
      if ( match.distance() > 0 )
        return false;

      // there may be multiple points at the same location, but we get only one
      // result... the locator API needs a new method verticesInRect()
      QgsFeature f;
      match.layer()->getFeatures( QgsFeatureRequest( match.featureId() ).setNoAttributes() ).nextFeature( f );
      QgsGeometry matchGeom = f.geometry();
      bool isPolygon = matchGeom.type() == QgsWkbTypes::PolygonGeometry;
      QgsVertexId polygonRingVid;
      QgsVertexId vid;
      QgsPoint pt;
      while ( matchGeom.constGet()->nextVertex( vid, pt ) )
      {
        int vindex = matchGeom.vertexNrFromVertexId( vid );
        if ( pt.x() == match.point().x() && pt.y() == match.point().y() )
        {
          if ( isPolygon )
          {
            // for polygons we need to handle the case where the first vertex is matching because the
            // last point will have the same coordinates and we would have a duplicate match which
            // would make subsequent code behave incorrectly (topology editing mode would add a single
            // vertex twice)
            if ( vid.vertex == 0 )
            {
              polygonRingVid = vid;
            }
            else if ( vid.ringEqual( polygonRingVid ) && vid.vertex == matchGeom.constGet()->vertexCount( vid.part, vid.ring ) - 1 )
            {
              continue;
            }
          }

          QgsPointLocator::Match extra_match( match.type(), match.layer(), match.featureId(),
                                              0, match.point(), vindex );
          matches.append( extra_match );
        }
      }
      return true;
    }
};

void FeatureModel::applyVertexModelTopography()
{
  if ( !mVertexModel )
    return;

  const QVector<QPair<QgsPoint, QgsPoint>> pointsMoved = mVertexModel->verticesMoved();
  const QVector<QgsPoint> pointsDeleted = mVertexModel->verticesDeleted();

  const QVector<QgsVectorLayer *> vectorLayers = mProject ? mProject->layers<QgsVectorLayer *>() : QVector<QgsVectorLayer *>() << mLayer;
  for ( auto vectorLayer : vectorLayers )
  {
    if ( vectorLayer->readOnly() )
      continue;

    if ( vectorLayer != mLayer )
    {
      vectorLayer->startEditing();
    }

    QgsPointLocator loc( vectorLayer );
    for ( const auto &point : pointsMoved )
    {
      MatchCollectingFilter filter;
      loc.nearestVertex( point.first, 0, &filter );
      for ( int i = 0; i < filter.matches.size(); i++ )
      {
        vectorLayer->moveVertex( point.second, filter.matches.at( i ).featureId(), filter.matches.at( i ).vertexIndex() );
      }
    }

    for ( const auto &point : pointsDeleted )
    {
      MatchCollectingFilter filter;
      loc.nearestVertex( point, 0, &filter );
      for ( int i = 0; i < filter.matches.size(); i++ )
      {
        vectorLayer->deleteVertex( filter.matches.at( i ).featureId(), filter.matches.at( i ).vertexIndex() );
      }
    }

    vectorLayer->addTopologicalPoints( mFeature.geometry() );
    if ( vectorLayer != mLayer )
    {
      vectorLayer->commitChanges( true );
    }
  }
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
