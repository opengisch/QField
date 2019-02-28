#include "referencedfeaturelistmodel.h"

ReferencedFeatureListModel::ReferencedFeatureListModel(QObject *parent)
  : QStandardItemModel(parent)
{
}

QHash<int, QByteArray> ReferencedFeatureListModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();

  roles[DisplayString] = "displayString";
  roles[FeatureId] = "featureId";

  return roles;
}

QModelIndex ReferencedFeatureListModel::index(int row, int column, const QModelIndex &parent) const
{
  Q_UNUSED( column )
  Q_UNUSED( parent )

  return createIndex( row, column, 1000 );
}

QModelIndex ReferencedFeatureListModel::parent(const QModelIndex &index) const
{
  Q_UNUSED( index )

  return QModelIndex();
}

int ReferencedFeatureListModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED( parent )
  return mEntries.size();
}

int ReferencedFeatureListModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED( parent )
  return 1;
}

QVariant ReferencedFeatureListModel::data( const QModelIndex &index, int role ) const
{
  if ( role == DisplayString )
    return mEntries.value( index.row() ).displayString;
  if ( role == FeatureId )
    return mEntries.value( index.row() ).featureId;

  return QVariant();
}

void ReferencedFeatureListModel::setFeatureId(const QgsFeatureId &featureId)
{
  mFeatureId = featureId;
  feedTheModel(mRelation,mFeatureId);
}

QgsFeatureId ReferencedFeatureListModel::featureId() const
{
  return mFeatureId;
}

void ReferencedFeatureListModel::setRelation(const QgsRelation &relation)
{
  mRelation = relation;
  feedTheModel(mRelation,mFeatureId);
}

QgsRelation ReferencedFeatureListModel::relation() const
{
  return mRelation;
}

AttributeFormModel *ReferencedFeatureListModel::attributeFormModel() const
{
  return mAttributeFormModel;
}

void ReferencedFeatureListModel::setAttributeFormModel( AttributeFormModel *attributeFormModel )
{
  if ( mAttributeFormModel == attributeFormModel )
    return;

  if ( mAttributeFormModel )
  {
    disconnect( mAttributeFormModel, &AttributeFormModel::loadRelationData, this, &ReferencedFeatureListModel::feedTheModel );
  }

  mAttributeFormModel = attributeFormModel;
  connect( mAttributeFormModel, &AttributeFormModel::loadRelationData, this, &ReferencedFeatureListModel::feedTheModel );
}

void ReferencedFeatureListModel::feedTheModel(QgsRelation relation, QgsFeatureId featureId)
{
  if( !relation.isValid() || featureId<0 )
    return;
  QgsFeatureIterator relatedFeaturesIt = relation.getRelatedFeatures( relation.referencedLayer()->getFeature( featureId ) );
  QgsExpressionContext context = relation.referencingLayer()->createExpressionContext();
  QgsExpression expression( relation.referencingLayer()->displayExpression() );

  QgsFeature childFeature;
  while ( relatedFeaturesIt.nextFeature( childFeature ) )
  {
   context.setFeature( childFeature );
   mEntries.append( Entry( expression.evaluate( &context ).toString(), childFeature.id() ) );
  }
}
