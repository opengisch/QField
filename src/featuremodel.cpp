#include "featuremodel.h"

#include "feature.h"

FeatureModel::FeatureModel( QObject *parent ) :
  QAbstractListModel( parent )
{
}

void FeatureModel::setFeature( Feature feature )
{
  beginResetModel();
  mFeature = feature;
  endResetModel();
}

QHash<int, QByteArray> FeatureModel::roleNames() const
{

  QHash<int, QByteArray> roles;
  roles[AttributeName]  = "attributeName";
  roles[AttributeValue] = "attributeValue";

  return roles;
}


int FeatureModel::rowCount( const QModelIndex& parent ) const
{
  Q_UNUSED( parent )
  return mFeature.attributes().count();
}

QVariant FeatureModel::data( const QModelIndex& index, int role ) const
{
  switch ( role )
  {
    case AttributeName:
      return mFeature.fields()->at( index.row() ).name();
      break;

    case AttributeValue:
      return mFeature.attribute( index.row() );
      break;

    case AttributeEditable:
      return false;
      break;
  }

  return QVariant();
}
