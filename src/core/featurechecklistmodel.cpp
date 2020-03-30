/***************************************************************************
  featurechecklistmodel.cpp - FeatureCheckListModel

 ---------------------
 begin                : August 2019
 copyright            : (C) 2019 by David Signer
 email                : david (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "featurechecklistmodel.h"
#include "qgsmessagelog.h"
#include "qgsvaluerelationfieldformatter.h"
#if VERSION_INT >= 30600
#include "qgspostgresstringutils.h"
#endif

FeatureCheckListModel::FeatureCheckListModel()
{
}

QVariant FeatureCheckListModel::data( const QModelIndex &index, int role ) const
{
  if ( role == CheckedRole )
  {
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

  QVariantList vl;
  //store as QVariantList because the field type supports data structure
  for ( const QString &s : qgis::as_const( mCheckedEntries ) )
  {
    // Convert to proper type
    const QVariant::Type type {fkType()};
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

  if ( mAttributeField.type() == QVariant::Map || mAttributeField.type() == QVariant::List )
  {
    value = vl;
  }
  else
  {
    //make string
#if VERSION_INT >= 30600
    value = QgsPostgresStringUtils::buildArray( vl );
#else
    QgsMessageLog::logMessage( tr( "Storing of value relation widget checklists not available for Android 5" ), "QField", Qgis::Critical );
#endif
  }

  return value;
}

void FeatureCheckListModel::setAttributeValue( const QVariant &attributeValue )
{
  QStringList checkedEntries;

  if ( mAttributeField.type() == QVariant::Map )
  {
    //store as QVariantList because the field type supports data structure
    checkedEntries = attributeValue.toStringList();
  }
  else
  {
    //store as a formatted string because the fields supports only string
    checkedEntries = QgsValueRelationFieldFormatter::valueToStringList( attributeValue );
  }

  if ( mCheckedEntries == checkedEntries )
    return;

  beginResetModel();
  mCheckedEntries = checkedEntries;
  endResetModel();

  emit attributeValueChanged();
}

QgsField FeatureCheckListModel::attributeField() const
{
  return mAttributeField;
}

void FeatureCheckListModel::setAttributeField( const QgsField &field )
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

  emit listUpdated();
}

void FeatureCheckListModel::setUnchecked( const QModelIndex &index )
{
  beginResetModel();
  mCheckedEntries.removeAll( FeatureListModel::data( index, FeatureListModel::KeyFieldRole ).toString() );
  endResetModel();

  emit listUpdated();
}


QVariant::Type FeatureCheckListModel::fkType() const
{
  if ( currentLayer() )
  {
    QgsFields fields = currentLayer()->fields();
    int idx {fields.indexOf( keyField() )};
    if ( idx >= 0 )
    {
      return fields.at( idx ).type();
    }
  }
  return QVariant::Type::Invalid;
}
