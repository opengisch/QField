#include "referencedfeaturelistmodel.h"

ReferencedFeatureListModel::ReferencedFeatureListModel(QObject *parent)
  : QAbstractItemModel(parent)
{
  //evtl. reload functionality in case there have been changes...
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
  if ( role == Qt::DisplayRole )
    return mEntries.value( index.row() ).displayString;

  return QVariant();
}

void ReferencedFeatureListModel::setFeatureId(const QgsFeatureId &featureId)
{
  mFeatureId = featureId;
}

QgsFeatureId ReferencedFeatureListModel::featureId() const
{
  return mFeatureId;
}

void ReferencedFeatureListModel::setRelation(const QgsRelation &relation)
{
  mRelation = relation;
}

QgsRelation ReferencedFeatureListModel::relation() const
{
  return mRelation;
}

void ReferencedFeatureListModel::feedTheModel()
{
   QgsFeatureIterator relatedFeaturesIt = mRelation.getRelatedFeatures( mRelation.referencedLayer()->getFeature( mFeatureId ) );
   QgsExpressionContext context = mRelation.referencingLayer()->createExpressionContext();
   QgsExpression expression( mRelation.referencingLayer()->displayExpression() );

   QgsFeature childFeature;
   while ( relatedFeaturesIt.nextFeature( childFeature ) )
   {
     context.setFeature( childFeature );
     mEntries.append( Entry( expression.evaluate( &context ).toString(), childFeature.id() ) );
   }
}
