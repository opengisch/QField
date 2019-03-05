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
  feedTheModel();
}

QgsFeatureId ReferencedFeatureListModel::featureId() const
{
  return mFeatureId;
}

void ReferencedFeatureListModel::setRelation(const QgsRelation &relation)
{
  mRelation = relation;
  feedTheModel();
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
  mAttributeFormModel = attributeFormModel;
  connect( mAttributeFormModel, &AttributeFormModel::setRelationFeatureId, this, &ReferencedFeatureListModel::elSloto );
}

void ReferencedFeatureListModel::feedTheModel()
{
  if( !mRelation.isValid() || mFeatureId<0 )
    return;
  mEntries.clear();
  QgsFeatureIterator relatedFeaturesIt = mRelation.getRelatedFeatures( mRelation.referencedLayer()->getFeature( mFeatureId ) );
  QgsExpressionContext context = mRelation.referencingLayer()->createExpressionContext();
  QgsExpression expression( mRelation.referencingLayer()->displayExpression() );

  beginResetModel();
  QgsFeature childFeature;
  while ( relatedFeaturesIt.nextFeature( childFeature ) )
  {
   context.setFeature( childFeature );
   mEntries.append( Entry( expression.evaluate( &context ).toString(), childFeature.id() ) );
  }
  endResetModel();
}

void ReferencedFeatureListModel::elSloto(QgsFeatureId featureId)
{
  setFeatureId( featureId );
}
