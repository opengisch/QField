#include "featurechecklistmodel.h"
#include "qgsvaluerelationfieldformatter.h"
#include "qgspostgresstringutils.h"
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

QVariant FeatureCheckListModel::attributeValue() const
{
  QVariant value;

  qDebug() << "Get data from list: " <<  mCheckedEntries.join( QStringLiteral( ", " ) ).prepend( '{' ).append( '}' );

  QVariantList vl;
  //store as QVariantList because the field type supports data structure
  for ( const QString &s : qgis::as_const( mCheckedEntries ) )
  {
    // Convert to proper type
    const QVariant::Type type { fkType() };
    switch ( type )
    {
      case QVariant::Type::Int:
        vl.push_back( s.toInt() );
        break;
      case QVariant::Type::LongLong:
        vl.push_back( s.toLongLong() );
        break;
      default:
        vl.push_back( s );
        break;
    }
  }

  if ( mAttributeField.type() == QVariant::Map ||
       mAttributeField.type() == QVariant::List )
  {
    value = vl;
  }
  else
  {
    //make string
    value = QgsPostgresStringUtils::buildArray( vl );
  }

  return value;
}

void FeatureCheckListModel::setAttributeValue( const QVariant &attributeValue )
{
  QStringList checkedEntries;

  if ( mAttributeField.type() == QVariant::Map )
  {
    //because of json it's stored as QVariantList
    checkedEntries = attributeValue.toStringList();
  }
  else
  {
    //because of hstore it's stored as QString
    checkedEntries = QgsValueRelationFieldFormatter::valueToStringList( attributeValue );
  }

  if ( mCheckedEntries == checkedEntries )
    return;

  qDebug() << "Set data to list: " << attributeValue.toString();

  beginResetModel();
  mCheckedEntries = checkedEntries;
  endResetModel();
  qDebug() << "Model resetted 1";

  //nobody listening on that at the moment
  emit attributeValueChanged();
}

QgsField FeatureCheckListModel::attributeField() const
{
  return mAttributeField;
}

void FeatureCheckListModel::setAttributeField( QgsField field )
{
  if ( mAttributeField == field )
    return;

  mAttributeField = field;
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


QVariant::Type FeatureCheckListModel::fkType() const
{
  if ( currentLayer() )
  {
    QgsFields fields = currentLayer()->fields();
    int idx { fields.indexOf( keyField() ) };
    if ( idx >= 0 )
    {
      return fields.at( idx ).type();
    }
  }
  return QVariant::Type::Invalid;
}
