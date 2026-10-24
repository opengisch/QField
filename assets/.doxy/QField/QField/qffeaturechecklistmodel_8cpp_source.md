

# File qffeaturechecklistmodel.cpp

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qffeaturechecklistmodel.cpp**](qffeaturechecklistmodel_8cpp.md)

[Go to the documentation of this file](qffeaturechecklistmodel_8cpp.md)


```C++
/***************************************************************************
  qffeaturechecklistmodel.cpp - QfFeatureCheckListModel

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

#include "qffeaturechecklistmodel.h"
#include "qgspostgresstringutils.h"
#include "qgsvaluerelationfieldformatter.h"

QfFeatureCheckListModelBase::QfFeatureCheckListModelBase( QObject *parent )
  : QfFeatureListModel( parent )
{
}

QVariant QfFeatureCheckListModelBase::data( const QModelIndex &index, int role ) const
{
  if ( role == CheckedRole )
  {
    if ( addNull() && index.row() == 0 )
    {
      return mCheckedEntries.isEmpty();
    }

    if ( keyField().isEmpty() )
    {
      return mCheckedEntries.contains( QfFeatureListModel::data( index, QfFeatureListModel::FeatureIdRole ).toString() );
    }

    return mCheckedEntries.contains( QfFeatureListModel::data( index, QfFeatureListModel::KeyFieldRole ).toString() );
  }
  else
  {
    return QfFeatureListModel::data( index, role );
  }
}

bool QfFeatureCheckListModelBase::setData( const QModelIndex &index, const QVariant &value, int role )
{
  if ( data( index, role ) == value )
    return true;

  switch ( role )
  {
    case CheckedRole:
    {
      if ( value.toBool() )
      {
        setChecked( index );
      }
      else
      {
        setUnchecked( index );
      }
      break;
    }
  }
  return true;
}

QHash<int, QByteArray> QfFeatureCheckListModelBase::roleNames() const
{
  QHash<int, QByteArray> roles = QfFeatureListModel::roleNames();

  roles[CheckedRole] = "checked";

  return roles;
}

QVariant QfFeatureCheckListModelBase::attributeValue() const
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
      if ( !mAttributeField.name().isEmpty() && ( mAttributeField.type() == QMetaType::QVariantMap || mAttributeField.type() == QMetaType::QVariantList || mAttributeField.type() == QMetaType::QStringList ) )
      {
        value = vl;
      }
      else
      {
        if ( mAttributeField.name().isEmpty() )
        {
          value = mCheckedEntries;
        }
        else
        {
          //make string
          value = QgsPostgresStringUtils::buildArray( vl );
        }
      }
    }
    else
    {
      value = vl.first();
    }
  }

  return value;
}

void QfFeatureCheckListModelBase::setAttributeValue( const QVariant &attributeValue )
{
  QStringList checkedEntries;

  if ( mAllowMulti )
  {
    if ( mAttributeField.name().isEmpty() || ( mAttributeField.type() == QMetaType::QVariantMap || mAttributeField.type() == QMetaType::QVariantList || mAttributeField.type() == QMetaType::QStringList ) )
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
  {
    return;
  }

  mCheckedEntries = checkedEntries;
  emit dataChanged( index( 0, 0, QModelIndex() ), index( rowCount() - 1, 0, QModelIndex() ), QList<int>() << CheckedRole );

  emit attributeValueChanged();
}

QgsField QfFeatureCheckListModelBase::attributeField() const
{
  return mAttributeField;
}

void QfFeatureCheckListModelBase::setAttributeField( const QgsField &field )
{
  if ( mAttributeField == field )
    return;

  mAttributeField = field;
}

bool QfFeatureCheckListModelBase::allowMulti() const
{
  return mAllowMulti;
}

void QfFeatureCheckListModelBase::setAllowMulti( bool allowMulti )
{
  if ( mAllowMulti == allowMulti )
    return;

  mAllowMulti = allowMulti;

  emit allowMultiChanged();
}

void QfFeatureCheckListModelBase::toggleCheckAll( const bool toggleChecked )
{
  if ( toggleChecked )
  {
    QStringList checkedEntries;
    for ( int i = 0; i < rowCount(); i++ )
    {
      if ( keyField().isEmpty() )
      {
        checkedEntries.append( QfFeatureListModel::data( createIndex( i, 0 ), QfFeatureListModel::FeatureIdRole ).toString() );
      }
      else
      {
        checkedEntries.append( QfFeatureListModel::data( createIndex( i, 0 ), QfFeatureListModel::KeyFieldRole ).toString() );
      }
    }

    if ( checkedEntries != mCheckedEntries )
    {
      mCheckedEntries = checkedEntries;
      emit dataChanged( index( 0, 0, QModelIndex() ), index( rowCount() - 1, 0, QModelIndex() ), QList<int>() << CheckedRole );
    }
  }
  else
  {
    if ( !mCheckedEntries.isEmpty() )
    {
      mCheckedEntries = QStringList();
      emit dataChanged( index( 0, 0, QModelIndex() ), index( rowCount() - 1, 0, QModelIndex() ), QList<int>() << CheckedRole );
    }
  }
}

void QfFeatureCheckListModelBase::setChecked( const QModelIndex &idx )
{
  const bool wasEmpty = mCheckedEntries.isEmpty();
  if ( !mAllowMulti )
  {
    mCheckedEntries.clear();
    emit dataChanged( index( 0, 0, QModelIndex() ), index( rowCount() - 1, 0, QModelIndex() ), QList<int>() << CheckedRole );
  }

  if ( keyField().isEmpty() )
  {
    mCheckedEntries.append( QfFeatureListModel::data( idx, QfFeatureListModel::FeatureIdRole ).toString() );
  }
  else
  {
    mCheckedEntries.append( QfFeatureListModel::data( idx, QfFeatureListModel::KeyFieldRole ).toString() );
  }
  emit dataChanged( idx, idx, QList<int>() << CheckedRole );

  if ( addNull() && wasEmpty )
  {
    QModelIndex nullIdx = index( 0, 0, QModelIndex() );
    emit dataChanged( nullIdx, nullIdx, QList<int>() << CheckedRole );
  }
  emit listUpdated();
}

void QfFeatureCheckListModelBase::setUnchecked( const QModelIndex &idx )
{
  const bool wasEmpty = mCheckedEntries.isEmpty();
  if ( keyField().isEmpty() )
  {
    mCheckedEntries.removeAll( QfFeatureListModel::data( idx, QfFeatureListModel::FeatureIdRole ).toString() );
  }
  else
  {
    mCheckedEntries.removeAll( QfFeatureListModel::data( idx, QfFeatureListModel::KeyFieldRole ).toString() );
  }
  emit dataChanged( idx, idx, QList<int>() << CheckedRole );

  if ( addNull() && !wasEmpty && mCheckedEntries.isEmpty() )
  {
    QModelIndex nullIdx = index( 0, 0, QModelIndex() );
    emit dataChanged( nullIdx, nullIdx, QList<int>() << CheckedRole );
  }
  emit listUpdated();
}

QMetaType::Type QfFeatureCheckListModelBase::fkType() const
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


QfFeatureCheckListModel::QfFeatureCheckListModel( QObject *parent )
  : QSortFilterProxyModel( parent )
  , mSourceModel( new QfFeatureCheckListModelBase( parent ) )
{
  connect( mSourceModel, &QfFeatureListModel::currentLayerChanged, this, &QfFeatureCheckListModel::currentLayerChanged );
  connect( mSourceModel, &QfFeatureListModel::keyFieldChanged, this, &QfFeatureCheckListModel::keyFieldChanged );
  connect( mSourceModel, &QfFeatureListModel::displayValueFieldChanged, this, &QfFeatureCheckListModel::displayValueFieldChanged );
  connect( mSourceModel, &QfFeatureListModel::groupFieldChanged, this, &QfFeatureCheckListModel::groupFieldChanged );
  connect( mSourceModel, &QfFeatureListModel::displayGroupNameChanged, this, &QfFeatureCheckListModel::displayGroupNameChanged );
  connect( mSourceModel, &QfFeatureListModel::orderByValueChanged, this, &QfFeatureCheckListModel::orderByValueChanged );
  connect( mSourceModel, &QfFeatureListModel::orderByFieldChanged, this, &QfFeatureCheckListModel::orderByFieldChanged );
  connect( mSourceModel, &QfFeatureListModel::orderByFieldNameChanged, this, &QfFeatureCheckListModel::orderByFieldNameChanged );
  connect( mSourceModel, &QfFeatureListModel::addNullChanged, this, &QfFeatureCheckListModel::addNullChanged );
  connect( mSourceModel, &QfFeatureListModel::filterExpressionChanged, this, &QfFeatureCheckListModel::filterExpressionChanged );
  connect( mSourceModel, &QfFeatureListModel::searchTermChanged, this, &QfFeatureCheckListModel::searchTermChanged );
  connect( mSourceModel, &QfFeatureListModel::currentFormFeatureChanged, this, &QfFeatureCheckListModel::currentFormFeatureChanged );
  connect( mSourceModel, &QfFeatureListModel::appExpressionContextScopesGeneratorChanged, this, &QfFeatureCheckListModel::appExpressionContextScopesGeneratorChanged );
  connect( mSourceModel, &QfFeatureCheckListModelBase::attributeValueChanged, this, &QfFeatureCheckListModel::attributeValueChanged );
  connect( mSourceModel, &QfFeatureCheckListModelBase::attributeFieldChanged, this, &QfFeatureCheckListModel::attributeFieldChanged );
  connect( mSourceModel, &QfFeatureCheckListModelBase::allowMultiChanged, this, &QfFeatureCheckListModel::allowMultiChanged );
  connect( mSourceModel, &QfFeatureCheckListModelBase::listUpdated, this, &QfFeatureCheckListModel::listUpdated );

  setSourceModel( mSourceModel );
  setDynamicSortFilter( false );
  sort( 0 );
}

QgsFeature QfFeatureCheckListModel::getFeatureFromKeyValue( const QVariant &value ) const
{
  return mSourceModel->getFeatureFromKeyValue( value );
}

QgsFeature QfFeatureCheckListModel::getFeatureById( QgsFeatureId id ) const
{
  return mSourceModel->getFeatureById( id );
}

QgsVectorLayer *QfFeatureCheckListModel::currentLayer() const
{
  return mSourceModel->currentLayer();
}

void QfFeatureCheckListModel::setCurrentLayer( QgsVectorLayer *currentLayer )
{
  mSourceModel->setCurrentLayer( currentLayer );
}

QString QfFeatureCheckListModel::keyField() const
{
  return mSourceModel->keyField();
}

void QfFeatureCheckListModel::setKeyField( const QString &keyField )
{
  mSourceModel->setKeyField( keyField );
}

QString QfFeatureCheckListModel::displayValueField() const
{
  return mSourceModel->displayValueField();
}

void QfFeatureCheckListModel::setDisplayValueField( const QString &displayValueField )
{
  mSourceModel->setDisplayValueField( displayValueField );
}

QString QfFeatureCheckListModel::groupField() const
{
  return mSourceModel->groupField();
}

void QfFeatureCheckListModel::setGroupField( const QString &groupField )
{
  mSourceModel->setGroupField( groupField );
}

bool QfFeatureCheckListModel::displayGroupName() const
{
  return mSourceModel->displayGroupName();
}
void QfFeatureCheckListModel::setDisplayGroupName( bool displayGroupName )
{
  mSourceModel->setDisplayGroupName( displayGroupName );
}

int QfFeatureCheckListModel::findKey( const QVariant &key ) const
{
  const int sourceRow = mSourceModel->findKey( key );
  return mapFromSource( mSourceModel->index( sourceRow, 0, QModelIndex() ) ).row();
}

QList<int> QfFeatureCheckListModel::findDisplayValueMatches( const QString &filter ) const
{
  const QList<int> sourceRows = mSourceModel->findDisplayValueMatches( filter );
  QList<int> rows;
  for ( const int &sourceRow : sourceRows )
  {
    rows << mapFromSource( mSourceModel->index( sourceRow, 0, QModelIndex() ) ).row();
  }
  return rows;
}

bool QfFeatureCheckListModel::orderByValue() const
{
  return mSourceModel->orderByValue();
}

void QfFeatureCheckListModel::setOrderByValue( bool orderByValue )
{
  mSourceModel->setOrderByValue( orderByValue );
}

bool QfFeatureCheckListModel::orderByField() const
{
  return mSourceModel->orderByField();
}

void QfFeatureCheckListModel::setOrderByField( bool orderByField )
{
  mSourceModel->setOrderByField( orderByField );
}

QString QfFeatureCheckListModel::orderByFieldName() const
{
  return mSourceModel->orderByFieldName();
}

void QfFeatureCheckListModel::setOrderByFieldName( const QString &orderByFieldName )
{
  mSourceModel->setOrderByFieldName( orderByFieldName );
}

bool QfFeatureCheckListModel::addNull() const
{
  return mSourceModel->addNull();
}

void QfFeatureCheckListModel::setAddNull( bool addNull )
{
  mSourceModel->setAddNull( addNull );
}

QString QfFeatureCheckListModel::filterExpression() const
{
  return mSourceModel->filterExpression();
}

void QfFeatureCheckListModel::setFilterExpression( const QString &filterExpression )
{
  mSourceModel->setFilterExpression( filterExpression );
}

QgsFeature QfFeatureCheckListModel::currentFormFeature() const
{
  return mSourceModel->currentFormFeature();
}

void QfFeatureCheckListModel::setCurrentFormFeature( const QgsFeature &feature )
{
  mSourceModel->setCurrentFormFeature( feature );
}

QfAppExpressionContextScopesGenerator *QfFeatureCheckListModel::appExpressionContextScopesGenerator() const
{
  return mSourceModel->appExpressionContextScopesGenerator();
}

void QfFeatureCheckListModel::setAppExpressionContextScopesGenerator( QfAppExpressionContextScopesGenerator *generator )
{
  mSourceModel->setAppExpressionContextScopesGenerator( generator );
}

QVariant QfFeatureCheckListModel::attributeValue() const
{
  return mSourceModel->attributeValue();
}

void QfFeatureCheckListModel::setAttributeValue( const QVariant &attributeValue )
{
  mSourceModel->setAttributeValue( attributeValue );
}

QgsField QfFeatureCheckListModel::attributeField() const
{
  return mSourceModel->attributeField();
}

void QfFeatureCheckListModel::setAttributeField( const QgsField &field )
{
  mSourceModel->setAttributeField( field );
}

bool QfFeatureCheckListModel::allowMulti() const
{
  return mSourceModel->allowMulti();
}

void QfFeatureCheckListModel::setAllowMulti( bool allowMulti )
{
  mSourceModel->setAllowMulti( allowMulti );
}

void QfFeatureCheckListModel::toggleCheckAll( bool toggleChecked )
{
  mSourceModel->toggleCheckAll( toggleChecked );
}

QString QfFeatureCheckListModel::searchTerm() const
{
  return mSourceModel->searchTerm();
}

void QfFeatureCheckListModel::setSearchTerm( const QString &searchTerm )
{
  mSourceModel->setSearchTerm( searchTerm );
}

bool QfFeatureCheckListModel::sortCheckedFirst() const
{
  return mSortCheckedFirst;
}

void QfFeatureCheckListModel::setSortCheckedFirst( bool enabled )
{
  if ( mSortCheckedFirst == enabled )
  {
    return;
  }

  mSortCheckedFirst = enabled;
  emit sortCheckedFirstChanged();

  sort( 0 );
}

int QfFeatureCheckListModel::rowCount( const QModelIndex &parent ) const
{
  return QSortFilterProxyModel::rowCount( parent );
}

bool QfFeatureCheckListModel::filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
{
  return true;
}

bool QfFeatureCheckListModel::lessThan( const QModelIndex &left, const QModelIndex &right ) const
{
  if ( addNull() && ( left.row() == 0 || right.row() == 0 ) )
  {
    return left.row() < right.row();
  }

  if ( ( mSourceModel->mSearchTerm.isEmpty() && mSortCheckedFirst ) )
  {
    const QfFeatureListModel::Entry entryLeft = mSourceModel->entryFromRow( left.row() );
    const QfFeatureListModel::Entry entryRight = mSourceModel->entryFromRow( right.row() );

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

    const bool leftItemSelected = sourceModel()->data( left, QfFeatureCheckListModelBase::CheckedRole ).toBool();
    const bool rightItemSelected = sourceModel()->data( right, QfFeatureCheckListModelBase::CheckedRole ).toBool();

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


