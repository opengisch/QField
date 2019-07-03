#include "featurechecklistmodel.h"
#include "qgsvaluerelationfieldformatter.h"

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
  //needs JSON
  return mCheckedEntries.join( QStringLiteral( ", " ) ).prepend( '{' ).append( '}' );
}

void FeatureCheckListModel::setAttributeValue( const QString &attributeValue )
{
  //needs JSON
  if ( mCheckedEntries == QgsValueRelationFieldFormatter::valueToStringList( attributeValue ) )
    return;

  mCheckedEntries = QgsValueRelationFieldFormatter::valueToStringList( attributeValue );

  //nobody listening on that at the moment
  emit attributeValueChanged();
}

void FeatureCheckListModel::setChecked( const QModelIndex &index )
{
  mCheckedEntries.append( FeatureListModel::data( index, FeatureListModel::KeyFieldRole ).toString() );
  emit listUpdated();
}

void FeatureCheckListModel::setUnchecked( const QModelIndex &index )
{
  mCheckedEntries.removeAll( FeatureListModel::data( index, FeatureListModel::KeyFieldRole ).toString() );
  emit listUpdated();
}
