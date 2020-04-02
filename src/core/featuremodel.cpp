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

#include "featuremodel.h"
#include "expressioncontextutils.h"
#include "vertexmodel.h"

#include <qgsproject.h>
#include <qgsmessagelog.h>
#include <qgsvectorlayer.h>
#include <QGeoPositionInfoSource>
#include <qgsrelationmanager.h>

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
  roles[AttributeName]  = "AttributeName";
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
      mRememberings[mLayer].rememberedAttributes[ index.row() ] = value.toBool();
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

  if ( QgsProject::instance()->topologicalEditing() )
  {
    applyVertexModelToLayerTopography();
    mLayer->addTopologicalPoints( feat.geometry() );
  }

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
    if ( !mRememberings[mLayer].rememberedAttributes.at( i ) &&
         !mLinkedAttributeIndexes.contains( i ) )
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

bool FeatureModel::featureExists() const
{
  QgsFeature feature;
  mLayer->getFeatures( QgsFeatureRequest().setFilterFid( mFeature.id() ) ).nextFeature( feature );
  return feature.isValid();
}

void FeatureModel::applyGeometry()
{
  QgsGeometry geometry = mGeometry->asQgsGeometry();

  QList<QgsVectorLayer *> intersectionLayers;
  switch ( QgsProject::instance()->avoidIntersectionsMode() )
  {
    case QgsProject::AvoidIntersectionsMode::AvoidIntersectionsCurrentLayer:
      intersectionLayers.append( mLayer );
      break;
    case QgsProject::AvoidIntersectionsMode::AvoidIntersectionsLayers:
      intersectionLayers = QgsProject::instance()->avoidIntersectionsLayers();
      break;
    case QgsProject::AvoidIntersectionsMode::AllowIntersections:
      break;
  }
  if ( !intersectionLayers.isEmpty() && geometry.type() == QgsWkbTypes::PolygonGeometry )
  {
    geometry.avoidIntersections( intersectionLayers );
  }

  mFeature.setGeometry( geometry  );
}

void FeatureModel::removeLayer( QObject *layer )
{
  mRememberings.remove( static_cast< QgsVectorLayer * >( layer ) );
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

  if ( QgsProject::instance()->topologicalEditing() )
    mLayer->addTopologicalPoints( mFeature.geometry() );

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
      QgsGeometry matchGeom  = f.geometry();
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

void FeatureModel::applyVertexModelToLayerTopography()
{
  if ( !mVertexModel )
    return;

  QgsPointLocator *loc = new QgsPointLocator( mLayer );
  const QVector<QPair<QgsPoint,QgsPoint>> pointsMoved = mVertexModel->verticesMoved();
  for ( const auto &point : pointsMoved )
  {
    MatchCollectingFilter filter;
    loc->nearestVertex( point.first, 0, &filter );
    for ( int i = 0; i < filter.matches.size(); i++ )
    {
      mLayer->moveVertex( point.second, filter.matches.at( i ).featureId(), filter.matches.at( i ).vertexIndex() );
    }
  }

  const QVector<QgsPoint> pointsDeleted = mVertexModel->verticesDeleted();
  for ( const auto &point : pointsDeleted )
  {
    MatchCollectingFilter filter;
    loc->nearestVertex( point, 0, &filter );
    for ( int i = 0; i < filter.matches.size(); i++ )
    {
      mLayer->deleteVertex( filter.matches.at( i ).featureId(), filter.matches.at( i ).vertexIndex() );
    }
  }
}

QVector<bool> FeatureModel::rememberedAttributes() const
{
  return mRememberings[mLayer].rememberedAttributes;
}

void FeatureModel::updateRubberband() const
{
  mGeometry->updateRubberband( mFeature.geometry() );
}
