#include "featurechecklistmodel.h"
#include "qgsvaluerelationfieldformatter.h"
#include <QDebug>

FeatureCheckListModel::FeatureCheckListModel()
{

}

QVariant FeatureCheckListModel::data( const QModelIndex &index, int role ) const
{
  if ( role == CheckedRole )
  {
    if ( mCheckedEntries.contains( FeatureListModel::data( index, FeatureListModel::KeyFieldRole ).toString() ) )
    {

      qDebug() << "List contains : " <<  FeatureListModel::data( index, FeatureListModel::KeyFieldRole ).toString();
    }
    else
    {

      qDebug() << "List NOT contains : " <<  FeatureListModel::data( index, FeatureListModel::KeyFieldRole ).toString();
    }

    return mCheckedEntries.contains( FeatureListModel::data( index, FeatureListModel::KeyFieldRole ).toString() );
  }
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

  qDebug() << "Get data from list: " <<  mCheckedEntries.join( QStringLiteral( ", " ) ).prepend( '{' ).append( '}' );
  return mCheckedEntries.join( QStringLiteral( ", " ) ).prepend( '{' ).append( '}' );
}

void FeatureCheckListModel::setAttributeValue( const QString &attributeValue )
{
  //needs JSON
  if ( mCheckedEntries == QgsValueRelationFieldFormatter::valueToStringList( attributeValue ) )
    return;

  qDebug() << "Set data to list: " << attributeValue;

  beginResetModel();
  mCheckedEntries = QgsValueRelationFieldFormatter::valueToStringList( attributeValue );
  endResetModel();
  qDebug() << "Model resetted 1";

  //nobody listening on that at the moment
  emit attributeValueChanged();
}

void FeatureCheckListModel::setChecked( const QModelIndex &index )
{
  beginResetModel();
  mCheckedEntries.append( FeatureListModel::data( index, FeatureListModel::KeyFieldRole ).toString() );
  endResetModel();
  qDebug() << "Model resetted 1";

  emit listUpdated();
}

void FeatureCheckListModel::setUnchecked( const QModelIndex &index )
{
  beginResetModel();
  mCheckedEntries.removeAll( FeatureListModel::data( index, FeatureListModel::KeyFieldRole ).toString() );
  endResetModel();
  qDebug() << "Model resetted 1";

  emit listUpdated();
}
