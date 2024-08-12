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
#include "qgspostgresstringutils.h"
#include "qgsvaluerelationfieldformatter.h"

FeatureCheckListModel::FeatureCheckListModel( QObject *parent )
  : FeatureListModel( parent )
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

QVariant FeatureCheckListModel::attributeValue() const
{
  QVariantList vl;
  //store as QVariantList because the field type supports data structure
  for ( const QString &s : std::as_const( mCheckedEntries ) )
  {
    // Convert to proper type
    const QMetaType::Type type { fkType() };
    switch ( type )
    {
      case QMetaType::Int:
        vl.push_back( s.toInt() );
        break;
      case QMetaType::LongLong:
        vl.push_back( s.toLongLong() );
        break;
      default:
        vl.push_back( s );
        break;
    }
  }

  QVariant value;
  if ( !vl.isEmpty() )
  {
    if ( mAllowMulti )
    {
      if ( mAttributeField.type() == QMetaType::QVariantMap || mAttributeField.type() == QMetaType::QVariantList || mAttributeField.type() == QMetaType::QStringList )
      {
        value = vl;
      }
      else
      {
        //make string
        value = QgsPostgresStringUtils::buildArray( vl );
      }
    }
    else
    {
      value = vl.first();
    }
  }
  return value;
}

void FeatureCheckListModel::setAttributeValue( const QVariant &attributeValue )
{
  QStringList checkedEntries;

  if ( mAllowMulti )
  {
    if ( mAttributeField.type() == QMetaType::QVariantMap || mAttributeField.type() == QMetaType::QVariantList || mAttributeField.type() == QMetaType::QStringList )
    {
      if ( attributeValue.canConvert<QString>() )
      {
        QString value = attributeValue.value<QString>();
        if ( !value.isEmpty() )
        {
          checkedEntries = attributeValue.toStringList();
        }
      }
      else
      {
        checkedEntries = attributeValue.toStringList();
      }
    }
    else
    {
      //store as a formatted string because the fields supports only string
      checkedEntries = QgsValueRelationFieldFormatter::valueToStringList( attributeValue );
    }
  }
  else
  {
    if ( attributeValue.canConvert<QString>() )
    {
      QString value = attributeValue.value<QString>();

      if ( !value.isEmpty() )
        checkedEntries << value;
    }
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

bool FeatureCheckListModel::allowMulti() const
{
  return mAllowMulti;
}

void FeatureCheckListModel::setAllowMulti( bool allowMulti )
{
  if ( mAllowMulti == allowMulti )
    return;

  mAllowMulti = allowMulti;

  emit allowMultiChanged();
}

void FeatureCheckListModel::toggleCheckAll( const bool toggleChecked )
{
  if ( toggleChecked )
  {
    QStringList checkedEntries;

    for ( int i = 0; i < rowCount(); i++ )
      checkedEntries.append( FeatureListModel::data( createIndex( i, 0 ), FeatureListModel::KeyFieldRole ).toString() );

    if ( checkedEntries != mCheckedEntries )
    {
      beginResetModel();
      mCheckedEntries = checkedEntries;
      endResetModel();
    }
  }
  else
  {
    if ( !mCheckedEntries.isEmpty() )
    {
      beginResetModel();
      mCheckedEntries = QStringList();
      endResetModel();
    }
  }
}

void FeatureCheckListModel::setChecked( const QModelIndex &idx )
{
  if ( !mAllowMulti )
  {
    mCheckedEntries.clear();
    emit dataChanged( index( 0, 0, QModelIndex() ), index( rowCount() - 1, 0, QModelIndex() ), QList<int>() << CheckedRole );
  }

  mCheckedEntries.append( FeatureListModel::data( idx, FeatureListModel::KeyFieldRole ).toString() );
  emit dataChanged( idx, idx, QList<int>() << CheckedRole );
  emit listUpdated();
}

void FeatureCheckListModel::setUnchecked( const QModelIndex &idx )
{
  mCheckedEntries.removeAll( FeatureListModel::data( idx, FeatureListModel::KeyFieldRole ).toString() );
  emit dataChanged( idx, idx, QList<int>() << CheckedRole );
  emit listUpdated();
}

QMetaType::Type FeatureCheckListModel::fkType() const
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
  return QMetaType::UnknownType;
}
