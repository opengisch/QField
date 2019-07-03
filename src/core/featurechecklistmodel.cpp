#include "featurechecklistmodel.h"

FeatureCheckListModel::FeatureCheckListModel()
{

}

QVariant FeatureCheckListModel::data( const QModelIndex &index, int role ) const
{
  if ( role == CheckedRole )
    return mCheckedEntries.contains( FeatureListModel::data( index, FeatureListModel::KeyFieldRole ).toString() );
  else
    return FeatureListModel::data( index, role );
}

bool FeatureCheckListModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
  if ( data( index, role ) == value )
    return true;

  switch ( role )
  {
    case CheckedRole:
    {
      if ( value.toBool() )
        setChecked( index );
      else
        setUnchecked( index );
      break;
    }
  }
  return true;
}

QHash<int, QByteArray> FeatureCheckListModel::roleNames() const
{
  QHash<int, QByteArray> roles = FeatureListModel::roleNames();

  roles[CheckedRole] = "checked";

  return roles;
}

QString FeatureCheckListModel::attributeValue() const
{
  return mAttributeValue;
}

void FeatureCheckListModel::setAttributeValue( const QString &attributeValue )
{
  if ( mAttributeValue == attributeValue )
    return;

  mAttributeValue = attributeValue;

  //set mCheckedEntries
  emit attributeValueChanged();
}

void FeatureCheckListModel::setChecked( const QModelIndex &index )
{
  mCheckedEntries.append( FeatureListModel::data( index, FeatureListModel::KeyFieldRole ).toString() );
}

void FeatureCheckListModel::setUnchecked( const QModelIndex &index )
{
  mCheckedEntries.removeAll( FeatureListModel::data( index, FeatureListModel::KeyFieldRole ).toString() );
}
