

# File featurechecklistmodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**featurechecklistmodel.cpp**](featurechecklistmodel_8cpp.md)

[Go to the documentation of this file](featurechecklistmodel_8cpp.md)


```C++
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

FeatureCheckListModelBase::FeatureCheckListModelBase( QObject *parent )
  : FeatureListModel( parent )
{
}

QVariant FeatureCheckListModelBase::data( const QModelIndex &index, int role ) const
{
  if ( role == CheckedRole )
  {
    if ( addNull() && index.row() == 0 )
    {
      return mCheckedEntries.isEmpty();
    }
    return mCheckedEntries.contains( FeatureListModel::data( index, FeatureListModel::KeyFieldRole ).toString() );
  }
  else
  {
    return FeatureListModel::data( index, role );
  }
}

bool FeatureCheckListModelBase::setData( const QModelIndex &index, const QVariant &value, int role )
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

QHash<int, QByteArray> FeatureCheckListModelBase::roleNames() const
{
  QHash<int, QByteArray> roles = FeatureListModel::roleNames();

  roles[CheckedRole] = "checked";

  return roles;
}

QVariant FeatureCheckListModelBase::attributeValue() const
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

void FeatureCheckListModelBase::setAttributeValue( const QVariant &attributeValue )
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

QgsField FeatureCheckListModelBase::attributeField() const
{
  return mAttributeField;
}

void FeatureCheckListModelBase::setAttributeField( const QgsField &field )
{
  if ( mAttributeField == field )
    return;

  mAttributeField = field;
}

bool FeatureCheckListModelBase::allowMulti() const
{
  return mAllowMulti;
}

void FeatureCheckListModelBase::setAllowMulti( bool allowMulti )
{
  if ( mAllowMulti == allowMulti )
    return;

  mAllowMulti = allowMulti;

  emit allowMultiChanged();
}

void FeatureCheckListModelBase::toggleCheckAll( const bool toggleChecked )
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

void FeatureCheckListModelBase::setChecked( const QModelIndex &idx )
{
  const bool wasEmpty = mCheckedEntries.isEmpty();
  if ( !mAllowMulti )
  {
    mCheckedEntries.clear();
    emit dataChanged( index( 0, 0, QModelIndex() ), index( rowCount() - 1, 0, QModelIndex() ), QList<int>() << CheckedRole );
  }

  mCheckedEntries.append( FeatureListModel::data( idx, FeatureListModel::KeyFieldRole ).toString() );
  emit dataChanged( idx, idx, QList<int>() << CheckedRole );
  if ( addNull() && wasEmpty )
  {
    QModelIndex nullIdx = index( 0, 0, QModelIndex() );
    emit dataChanged( nullIdx, nullIdx, QList<int>() << CheckedRole );
  }
  emit listUpdated();
}

void FeatureCheckListModelBase::setUnchecked( const QModelIndex &idx )
{
  const bool wasEmpty = mCheckedEntries.isEmpty();
  mCheckedEntries.removeAll( FeatureListModel::data( idx, FeatureListModel::KeyFieldRole ).toString() );
  emit dataChanged( idx, idx, QList<int>() << CheckedRole );
  if ( addNull() && !wasEmpty && mCheckedEntries.isEmpty() )
  {
    QModelIndex nullIdx = index( 0, 0, QModelIndex() );
    emit dataChanged( nullIdx, nullIdx, QList<int>() << CheckedRole );
  }
  emit listUpdated();
}

QMetaType::Type FeatureCheckListModelBase::fkType() const
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


FeatureCheckListModel::FeatureCheckListModel( QObject *parent )
  : QSortFilterProxyModel( parent )
  , mSourceModel( new FeatureCheckListModelBase( parent ) )
{
  connect( mSourceModel, &FeatureListModel::currentLayerChanged, this, &FeatureCheckListModel::currentLayerChanged );
  connect( mSourceModel, &FeatureListModel::keyFieldChanged, this, &FeatureCheckListModel::keyFieldChanged );
  connect( mSourceModel, &FeatureListModel::displayValueFieldChanged, this, &FeatureCheckListModel::displayValueFieldChanged );
  connect( mSourceModel, &FeatureListModel::groupFieldChanged, this, &FeatureCheckListModel::groupFieldChanged );
  connect( mSourceModel, &FeatureListModel::displayGroupNameChanged, this, &FeatureCheckListModel::displayGroupNameChanged );
  connect( mSourceModel, &FeatureListModel::orderByValueChanged, this, &FeatureCheckListModel::orderByValueChanged );
  connect( mSourceModel, &FeatureListModel::addNullChanged, this, &FeatureCheckListModel::addNullChanged );
  connect( mSourceModel, &FeatureListModel::filterExpressionChanged, this, &FeatureCheckListModel::filterExpressionChanged );
  connect( mSourceModel, &FeatureListModel::searchTermChanged, this, &FeatureCheckListModel::searchTermChanged );
  connect( mSourceModel, &FeatureListModel::currentFormFeatureChanged, this, &FeatureCheckListModel::currentFormFeatureChanged );
  connect( mSourceModel, &FeatureListModel::appExpressionContextScopesGeneratorChanged, this, &FeatureCheckListModel::appExpressionContextScopesGeneratorChanged );
  connect( mSourceModel, &FeatureCheckListModelBase::attributeValueChanged, this, &FeatureCheckListModel::attributeValueChanged );
  connect( mSourceModel, &FeatureCheckListModelBase::attributeFieldChanged, this, &FeatureCheckListModel::attributeFieldChanged );
  connect( mSourceModel, &FeatureCheckListModelBase::allowMultiChanged, this, &FeatureCheckListModel::allowMultiChanged );
  connect( mSourceModel, &FeatureCheckListModelBase::listUpdated, this, &FeatureCheckListModel::listUpdated );

  setSourceModel( mSourceModel );
  setDynamicSortFilter( false );
  sort( 0 );
}

QgsFeature FeatureCheckListModel::getFeatureFromKeyValue( const QVariant &value ) const
{
  return mSourceModel->getFeatureFromKeyValue( value );
}

QgsFeature FeatureCheckListModel::getFeatureById( QgsFeatureId id ) const
{
  return mSourceModel->getFeatureById( id );
}

QgsVectorLayer *FeatureCheckListModel::currentLayer() const
{
  return mSourceModel->currentLayer();
}

void FeatureCheckListModel::setCurrentLayer( QgsVectorLayer *currentLayer )
{
  mSourceModel->setCurrentLayer( currentLayer );
}

QString FeatureCheckListModel::keyField() const
{
  return mSourceModel->keyField();
}

void FeatureCheckListModel::setKeyField( const QString &keyField )
{
  mSourceModel->setKeyField( keyField );
}

QString FeatureCheckListModel::displayValueField() const
{
  return mSourceModel->displayValueField();
}

void FeatureCheckListModel::setDisplayValueField( const QString &displayValueField )
{
  mSourceModel->setDisplayValueField( displayValueField );
}

QString FeatureCheckListModel::groupField() const
{
  return mSourceModel->groupField();
}

void FeatureCheckListModel::setGroupField( const QString &groupField )
{
  mSourceModel->setGroupField( groupField );
}

bool FeatureCheckListModel::displayGroupName() const
{
  return mSourceModel->displayGroupName();
}
void FeatureCheckListModel::setDisplayGroupName( bool displayGroupName )
{
  mSourceModel->setDisplayGroupName( displayGroupName );
}

int FeatureCheckListModel::findKey( const QVariant &key ) const
{
  const int sourceRow = mSourceModel->findKey( key );
  return mapFromSource( mSourceModel->index( sourceRow, 0, QModelIndex() ) ).row();
}

QList<int> FeatureCheckListModel::findDisplayValueMatches( const QString &filter ) const
{
  const QList<int> sourceRows = mSourceModel->findDisplayValueMatches( filter );
  QList<int> rows;
  for ( const int &sourceRow : sourceRows )
  {
    rows << mapFromSource( mSourceModel->index( sourceRow, 0, QModelIndex() ) ).row();
  }
  return rows;
}

bool FeatureCheckListModel::orderByValue() const
{
  return mSourceModel->orderByValue();
}

void FeatureCheckListModel::setOrderByValue( bool orderByValue )
{
  mSourceModel->setOrderByValue( orderByValue );
}

bool FeatureCheckListModel::addNull() const
{
  return mSourceModel->addNull();
}

void FeatureCheckListModel::setAddNull( bool addNull )
{
  mSourceModel->setAddNull( addNull );
}

QString FeatureCheckListModel::filterExpression() const
{
  return mSourceModel->filterExpression();
}

void FeatureCheckListModel::setFilterExpression( const QString &filterExpression )
{
  mSourceModel->setFilterExpression( filterExpression );
}

QgsFeature FeatureCheckListModel::currentFormFeature() const
{
  return mSourceModel->currentFormFeature();
}

void FeatureCheckListModel::setCurrentFormFeature( const QgsFeature &feature )
{
  mSourceModel->setCurrentFormFeature( feature );
}

AppExpressionContextScopesGenerator *FeatureCheckListModel::appExpressionContextScopesGenerator() const
{
  return mSourceModel->appExpressionContextScopesGenerator();
}

void FeatureCheckListModel::setAppExpressionContextScopesGenerator( AppExpressionContextScopesGenerator *generator )
{
  mSourceModel->setAppExpressionContextScopesGenerator( generator );
}

QVariant FeatureCheckListModel::attributeValue() const
{
  return mSourceModel->attributeValue();
}

void FeatureCheckListModel::setAttributeValue( const QVariant &attributeValue )
{
  mSourceModel->setAttributeValue( attributeValue );
}

QgsField FeatureCheckListModel::attributeField() const
{
  return mSourceModel->attributeField();
}

void FeatureCheckListModel::setAttributeField( const QgsField &field )
{
  mSourceModel->setAttributeField( field );
}

bool FeatureCheckListModel::allowMulti() const
{
  return mSourceModel->allowMulti();
}

void FeatureCheckListModel::setAllowMulti( bool allowMulti )
{
  mSourceModel->setAllowMulti( allowMulti );
}

void FeatureCheckListModel::toggleCheckAll( bool toggleChecked )
{
  mSourceModel->toggleCheckAll( toggleChecked );
}

QString FeatureCheckListModel::searchTerm() const
{
  return mSourceModel->searchTerm();
}

void FeatureCheckListModel::setSearchTerm( const QString &searchTerm )
{
  mSourceModel->setSearchTerm( searchTerm );
}

bool FeatureCheckListModel::sortCheckedFirst() const
{
  return mSortCheckedFirst;
}

void FeatureCheckListModel::setSortCheckedFirst( bool enabled )
{
  if ( mSortCheckedFirst == enabled )
  {
    return;
  }

  mSortCheckedFirst = enabled;
  emit sortCheckedFirstChanged();

  sort( 0 );
}

bool FeatureCheckListModel::filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
{
  return true;
}

bool FeatureCheckListModel::lessThan( const QModelIndex &left, const QModelIndex &right ) const
{
  if ( addNull() && ( left.row() == 0 || right.row() == 0 ) )
  {
    return left.row() < right.row();
  }

  if ( ( mSourceModel->mSearchTerm.isEmpty() && mSortCheckedFirst ) )
  {
    const FeatureListModel::Entry entryLeft = mSourceModel->entryFromRow( left.row() );
    const FeatureListModel::Entry entryRight = mSourceModel->entryFromRow( right.row() );

    if ( !groupField().isEmpty() )
    {
      const bool entryLeftGroupIsNull = entryLeft.group.isNull();
      const bool entryRightGroupIsNull = entryRight.group.isNull();
      if ( entryLeftGroupIsNull && !entryRightGroupIsNull )
      {
        return true;
      }
      else if ( !entryLeftGroupIsNull && entryRightGroupIsNull )
      {
        return false;
      }
      else if ( entryLeft.group != entryRight.group )
      {
        return entryLeft.group < entryRight.group;
      }
    }

    const bool leftItemSelected = sourceModel()->data( left, FeatureCheckListModelBase::CheckedRole ).toBool();
    const bool rightItemSelected = sourceModel()->data( right, FeatureCheckListModelBase::CheckedRole ).toBool();

    if ( rightItemSelected && !leftItemSelected )
    {
      return false;
    }
    else if ( !rightItemSelected && leftItemSelected )
    {
      return true;
    }
  }

  return false;
}
```


