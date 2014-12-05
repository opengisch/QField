#include "featurelistmodel.h"

#include <qgsvectorlayer.h>
#include <QDebug>

FeatureListModel::FeatureListModel( QObject *parent )
  :  QAbstractItemModel( parent )
  , mCount( 0 )
{
}

FeatureListModel::FeatureListModel( QList<QgsMapToolIdentify::IdentifyResult> features, QObject* parent )
  : QAbstractItemModel( parent )
  , mCount( 0 )
{
  setFeatures( features );
}

void FeatureListModel::setFeatures( const QList<QgsMapToolIdentify::IdentifyResult>& results )
{
  beginResetModel();

  Q_FOREACH( QVector<Feature*> f, mFeatures )
  qDeleteAll( f );
  mCount = 0;

  mFeatures.clear();
  mLayers.clear();

  QSet<QgsMapLayer*> layers;

  Q_FOREACH( const QgsMapToolIdentify::IdentifyResult& res, results )
  {
    layers.insert( res.mLayer );
    Feature* f = new Feature( res.mFeature, qobject_cast<QgsVectorLayer*>( res.mLayer ) );
    mFeatures[res.mLayer].append( f );
    mCount++;
  }

  mLayers = layers.toList();

  endResetModel();
}

QHash<int, QByteArray> FeatureListModel::roleNames() const
{
  QHash<int, QByteArray> roleNames;

  roleNames[Qt::DisplayRole] = "display";
  roleNames[FeatureIdRole] = "featureId";
  roleNames[FeatureRole] = "feature";

  return roleNames;
}

QModelIndex FeatureListModel::index( int row, int column, const QModelIndex& parent ) const
{
  if ( row < 0 )
    return QModelIndex();

  if ( parent.internalPointer() == 0 )
  {
    return createIndex( row, column, mLayers.at( row ) );
  }
  else
  {
    QgsMapLayer* layer = static_cast<QgsMapLayer*>( parent.internalPointer() );
    return createIndex( row, column, mFeatures.value( layer ).at( row ) );
  }
}

QModelIndex FeatureListModel::parent( const QModelIndex& child ) const
{
  if ( mLayers.contains( static_cast<QgsMapLayer*>( child.internalPointer() ) ) )
  {
    return QModelIndex();
  }
  else
  {
    const Feature* feature = static_cast<Feature*>( child.internalPointer() );
    QgsVectorLayer* layer = feature->layer();
    return createIndex( mLayers.indexOf( layer ), 0, layer );
  }
}

int FeatureListModel::rowCount( const QModelIndex& parent ) const
{
  if ( parent.isValid() )
  {
    QgsMapLayer* layer = static_cast<QgsMapLayer*>( parent.internalPointer() );
    Q_ASSERT( layer );
    return mFeatures.value( layer ).count();
  }
  else
  {
    return mLayers.count();
  }
}

int FeatureListModel::columnCount( const QModelIndex& parent ) const
{
  Q_UNUSED( parent )
  return 1;
}

QVariant FeatureListModel::data( const QModelIndex& index, int role ) const
{
  QgsMapLayer* layer;

  if ( index.parent().internalPointer() != 0 )
  {
    // it's a feature
    Feature* feature = toFeature( index );
    layer = feature->layer();
    QgsVectorLayer* vLayer = qobject_cast<QgsVectorLayer*>( layer );

    switch( role )
    {
      case FeatureIdRole:
        return feature->id();

      case FeatureRole:
        return QVariant::fromValue<Feature>( *feature );

      case Qt::DisplayRole:
        return feature->displayText();
    }
  }
  else
  {
    layer = toLayer( index );
    // it's a layer
    switch( role )
    {
      case FeatureIdRole:
      case FeatureRole:
        return QVariant();

      case Qt::DisplayRole:
        return layer->name();
    }
  }

  return QVariant();
}
