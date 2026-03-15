

# File featurelistmodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**featurelistmodel.cpp**](featurelistmodel_8cpp.md)

[Go to the documentation of this file](featurelistmodel_8cpp.md)


```C++
/***************************************************************************
  featurelistmodel.cpp - FeatureListModel

 ---------------------
 begin                : 1.2.2017
 copyright            : (C) 2017 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "featurelistmodel.h"
#include "qgsvectorlayer.h"
#include "stringutils.h"

#include <QRegularExpression>
#include <qgsexpressioncontextutils.h>
#include <qgsproject.h>
#include <qgsvaluerelationfieldformatter.h>


FeatureListModel::FeatureListModel( QObject *parent )
  : QAbstractItemModel( parent )
  , mCurrentLayer( nullptr )
{
  mReloadTimer.setInterval( 200 );
  mReloadTimer.setSingleShot( true );
  connect( &mReloadTimer, &QTimer::timeout, this, &FeatureListModel::gatherFeatureList );
}

FeatureListModel::~FeatureListModel()
{
  cleanupGatherer();
}

void FeatureListModel::cleanupGatherer()
{
  if ( mGatherer )
  {
    disconnect( mGatherer, &QThread::finished, this, &FeatureListModel::processFeatureList );
    connect( mGatherer, &QThread::finished, mGatherer, &QObject::deleteLater );
    mGatherer->stop();
    mGatherer = nullptr;
  }
}

QModelIndex FeatureListModel::index( int row, int column, const QModelIndex &parent ) const
{
  Q_UNUSED( parent )

  return createIndex( row, column, 1000 );
}

QModelIndex FeatureListModel::parent( const QModelIndex &child ) const
{
  Q_UNUSED( child )

  // We live in flat structures here
  return QModelIndex();
}

int FeatureListModel::rowCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent )
  return static_cast<int>( mEntries.size() );
}

int FeatureListModel::columnCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent )
  return 1;
}

QVariant FeatureListModel::data( const QModelIndex &index, int role ) const
{
  if ( index.row() < 0 || index.row() >= mEntries.size() )
  {
    return QVariant();
  }

  switch ( role )
  {
    case Qt::DisplayRole:
    case DisplayStringRole:
      return mEntries.value( index.row() ).displayString;

    case KeyFieldRole:
      return mEntries.value( index.row() ).key;

    case GroupFieldRole:
      return mEntries.value( index.row() ).group;

    case FeatureIdRole:
      return mEntries.value( index.row() ).fid;
  }

  return QVariant();
}

QHash<int, QByteArray> FeatureListModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();

  roles[KeyFieldRole] = "keyFieldValue";
  roles[DisplayStringRole] = "displayString";
  roles[GroupFieldRole] = "groupFieldValue";
  roles[FeatureIdRole] = "featureId";

  return roles;
}

QgsVectorLayer *FeatureListModel::currentLayer() const
{
  return mCurrentLayer.data();
}

void FeatureListModel::setCurrentLayer( QgsVectorLayer *currentLayer )
{
  if ( mCurrentLayer == currentLayer )
  {
    return;
  }

  if ( mCurrentLayer )
  {
    disconnect( mCurrentLayer, &QgsVectorLayer::featureAdded, this, &FeatureListModel::onFeatureAdded );
    disconnect( mCurrentLayer, &QgsVectorLayer::attributeValueChanged, this, &FeatureListModel::onAttributeValueChanged );
    disconnect( mCurrentLayer, &QgsVectorLayer::featureDeleted, this, &FeatureListModel::onFeatureDeleted );
  }

  mCurrentLayer = currentLayer;

  if ( mCurrentLayer )
  {
    connect( mCurrentLayer, &QgsVectorLayer::featureAdded, this, &FeatureListModel::onFeatureAdded );
    connect( mCurrentLayer, &QgsVectorLayer::attributeValueChanged, this, &FeatureListModel::onAttributeValueChanged );
    connect( mCurrentLayer, &QgsVectorLayer::featureDeleted, this, &FeatureListModel::onFeatureDeleted );
  }

  reloadLayer();

  emit currentLayerChanged();
}

QString FeatureListModel::keyField() const
{
  return mKeyField;
}

void FeatureListModel::setKeyField( const QString &keyField )
{
  if ( mKeyField == keyField )
  {
    return;
  }

  mKeyField = keyField;

  reloadLayer();

  emit keyFieldChanged();
}

QString FeatureListModel::displayValueField() const
{
  return mDisplayValueField;
}

void FeatureListModel::setDisplayValueField( const QString &displayValueField )
{
  if ( mDisplayValueField == displayValueField )
  {
    return;
  }

  mDisplayValueField = displayValueField;

  reloadLayer();

  emit displayValueFieldChanged();
}

QString FeatureListModel::groupField() const
{
  return mGroupField;
}

void FeatureListModel::setGroupField( const QString &groupField )
{
  if ( mGroupField == groupField )
  {
    return;
  }

  mGroupField = groupField;

  reloadLayer();

  emit groupFieldChanged();
}

bool FeatureListModel::displayGroupName() const
{
  return mDisplayGroupName;
}

void FeatureListModel::setDisplayGroupName( bool displayGroupName )
{
  if ( mDisplayGroupName == displayGroupName )
  {
    return;
  }

  mDisplayGroupName = displayGroupName;

  emit displayGroupNameChanged();
}

int FeatureListModel::findKey( const QVariant &key ) const
{
  int idx = 0;
  for ( const Entry &entry : mEntries )
  {
    if ( entry.key == key )
    {
      return idx;
    }

    ++idx;
  }

  if ( mAddNull )
  {
    return 0;
  }

  return -1;
}

QList<int> FeatureListModel::findDisplayValueMatches( const QString &filter ) const
{
  QMap<QString, int> matches;
  const QString preparedFilter = filter.trimmed().toLower();
  if ( !filter.trimmed().isEmpty() )
  {
    int idx = 0;
    for ( const Entry &entry : mEntries )
    {
      if ( entry.displayString.trimmed().toLower().startsWith( preparedFilter ) )
      {
        matches.insert( entry.displayString.trimmed().toLower(), idx );
      }
      ++idx;
    }
  }
  return matches.values();
}

void FeatureListModel::onFeatureAdded()
{
  reloadLayer();
}

void FeatureListModel::onAttributeValueChanged( QgsFeatureId, int idx, const QVariant & )
{
  QgsExpressionContext context = mCurrentLayer->createExpressionContext();
  QgsExpression expression( mCurrentLayer->displayExpression() );
  expression.prepare( &context );
  QSet<QString> referencedColumns = expression.referencedColumns();
  referencedColumns << mDisplayValueField;

  if ( referencedColumns.contains( mCurrentLayer->fields().at( idx ).name() ) )
  {
    reloadLayer();
  }
}

void FeatureListModel::onFeatureDeleted()
{
  reloadLayer();
}

QgsFeature FeatureListModel::getFeatureFromKeyValue( const QVariant &value ) const
{
  if ( !mCurrentLayer )
  {
    return QgsFeature();
  }

  QgsFeature feature;
  for ( auto &entry : mEntries )
  {
    if ( entry.key == value )
    {
      feature = mCurrentLayer->getFeature( entry.fid );
    }
  }

  return feature;
}

QgsFeature FeatureListModel::getFeatureById( QgsFeatureId id ) const
{
  if ( !mCurrentLayer )
  {
    return QgsFeature();
  }

  return mCurrentLayer->getFeature( id );
}

void FeatureListModel::gatherFeatureList()
{
  if ( !mCurrentLayer || !mCurrentLayer->isValid() )
  {
    return;
  }

  QgsFeatureRequest request;
  QgsExpressionContext context = mCurrentLayer->createExpressionContext();
  QgsExpression expression( mCurrentLayer->displayExpression() );
  expression.prepare( &context );

  QSet<QString> referencedColumns = expression.referencedColumns();

  if ( !keyField().isNull() )
  {
    referencedColumns << mKeyField;
  }

  if ( !groupField().isNull() )
  {
    referencedColumns << mGroupField;
  }

  referencedColumns << mDisplayValueField;

  QgsFields fields = mCurrentLayer->fields();
  int displayValueIndex = fields.indexOf( mDisplayValueField );

  request.setSubsetOfAttributes( referencedColumns, fields );

  QString fieldDisplayString = displayValueIndex >= 0
                                 ? QgsExpression::quotedColumnRef( mDisplayValueField )
                                 : QStringLiteral( " ( %1 ) " ).arg( mCurrentLayer->displayExpression() );

  QString searchTermExpression;
  if ( !mSearchTerm.isEmpty() )
  {
    QString escapedSearchTerm = QgsExpression::quotedValue( mSearchTerm ).replace( QRegularExpression( QStringLiteral( "^'|'$" ) ), QString( "" ) );
    searchTermExpression = QStringLiteral( " %1 ILIKE '%%2%' " ).arg( fieldDisplayString, escapedSearchTerm );

    const QStringList searchTermParts = escapedSearchTerm.split( QRegularExpression( QStringLiteral( "\\s+" ) ), Qt::SkipEmptyParts );
    for ( const QString &searchTermPart : searchTermParts )
    {
      searchTermExpression += QStringLiteral( " OR %1 ILIKE '%%2%' " ).arg( fieldDisplayString, searchTermPart );
    }
  }

  if ( !searchTermExpression.isEmpty() || !mFilterExpression.isEmpty() )
  {
    QgsExpressionContext filterContext = QgsExpressionContext( QgsExpressionContextUtils::globalProjectLayerScopes( mCurrentLayer ) );

    if ( mAppExpressionContextScopesGenerator )
    {
      QList<QgsExpressionContextScope *> scopes = mAppExpressionContextScopesGenerator->generate();
      while ( !scopes.isEmpty() )
      {
        filterContext.appendScope( scopes.takeFirst() );
      }
    }

    if ( mCurrentFormFeature.isValid() && QgsValueRelationFieldFormatter::expressionRequiresFormScope( mFilterExpression ) )
    {
      filterContext.appendScope( QgsExpressionContextUtils::formScope( mCurrentFormFeature ) );
    }

    request.setExpressionContext( filterContext );
    request.setFilterExpression( mFilterExpression );
    if ( mFilterExpression.isEmpty() )
    {
      request.setFilterExpression( QStringLiteral( " (%1) " ).arg( searchTermExpression ) );
    }
    else if ( searchTermExpression.isEmpty() )
    {
      request.setFilterExpression( mFilterExpression );
    }
    else
    {
      request.setFilterExpression( QStringLiteral( " (%1) AND (%2) " ).arg( mFilterExpression, searchTermExpression ) );
    }
  }

  cleanupGatherer();

  mGatherer = new FeatureExpressionValuesGatherer( mCurrentLayer, fieldDisplayString, request, QStringList() << keyField() << groupField() );
  connect( mGatherer, &QThread::finished, this, &FeatureListModel::processFeatureList );
  mGatherer->start();
}

void FeatureListModel::processFeatureList()
{
  if ( !mGatherer )
  {
    return;
  }

  mEntries.clear();

  QList<Entry> entries;

  if ( mAddNull )
  {
    entries.append( Entry( QStringLiteral( "<i>NULL</i>" ), QVariant(), QVariant(), QgsFeatureId() ) );
  }

  const QVector<FeatureExpressionValuesGatherer::Entry> gatheredEntries = mGatherer->entries();
  mGatherer->deleteLater();
  mGatherer = nullptr;

  for ( const FeatureExpressionValuesGatherer::Entry &gatheredEntry : gatheredEntries )
  {
    Entry entry( gatheredEntry.value, gatheredEntry.identifierFields.at( 0 ), gatheredEntry.identifierFields.at( 1 ), gatheredEntry.featureId );

    if ( !mSearchTerm.isEmpty() )
    {
      entry.fuzzyScore = StringUtils::calcFuzzyScore( entry.displayString, mSearchTerm );
      if ( entry.fuzzyScore == 0 )
      {
        continue;
      }
    }

    entries.append( entry );
  }

  std::sort( entries.begin(), entries.end(), [this]( const Entry &entry1, const Entry &entry2 ) {
    if ( entry1.key.isNull() && !entry2.key.isNull() )
    {
      return true;
    }

    if ( !entry1.key.isNull() && entry2.key.isNull() )
    {
      return false;
    }

    if ( !mGroupField.isEmpty() && entry1.group != entry2.group )
    {
      return entry1.group < entry2.group;
    }

    if ( !mSearchTerm.isEmpty() )
    {
      const bool entry1StartsWithSearchTerm = entry1.displayString.toLower().startsWith( mSearchTerm );
      const bool entry2StartsWithSearchTerm = entry2.displayString.toLower().startsWith( mSearchTerm );
      if ( entry1StartsWithSearchTerm && !entry2StartsWithSearchTerm )
      {
        return true;
      }

      if ( !entry1StartsWithSearchTerm && entry2StartsWithSearchTerm )
      {
        return false;
      }
    }

    if ( mOrderByValue )
    {
      return entry1.displayString.toLower() < entry2.displayString.toLower();
    }

    // Order By Key (as a fallback)
    const bool entry1KeyIsNull = entry1.key.isNull();
    const bool entry2KeyIsNull = entry2.key.isNull();
    if ( entry1KeyIsNull && !entry2KeyIsNull )
    {
      return true;
    }
    else if ( !entry1KeyIsNull && entry2KeyIsNull )
    {
      return false;
    }

    return entry1.key < entry2.key;
  } );

  beginResetModel();
  mEntries = entries;
  endResetModel();
}

FeatureListModel::Entry FeatureListModel::entryFromRow( int row )
{
  return row < 0 || row >= mEntries.size() ? FeatureListModel::Entry() : mEntries.at( row );
}

void FeatureListModel::reloadLayer()
{
  cleanupGatherer();
  mReloadTimer.start();
}

bool FeatureListModel::addNull() const
{
  return mAddNull;
}

void FeatureListModel::setAddNull( bool addNull )
{
  if ( mAddNull == addNull )
  {
    return;
  }

  mAddNull = addNull;
  emit addNullChanged();

  reloadLayer();
}

bool FeatureListModel::orderByValue() const
{
  return mOrderByValue;
}

void FeatureListModel::setOrderByValue( bool orderByValue )
{
  if ( mOrderByValue == orderByValue )
  {
    return;
  }

  mOrderByValue = orderByValue;
  reloadLayer();
  emit orderByValueChanged();
}

QString FeatureListModel::filterExpression() const
{
  return mFilterExpression;
}

void FeatureListModel::setFilterExpression( const QString &filterExpression )
{
  if ( mFilterExpression == filterExpression )
  {
    return;
  }

  mFilterExpression = filterExpression;
  reloadLayer();
  emit filterExpressionChanged();
}

QString FeatureListModel::searchTerm() const
{
  return mSearchTerm;
}

void FeatureListModel::setSearchTerm( const QString &searchTerm )
{
  const QString lowerSearchTerm = searchTerm.toLower();
  if ( mSearchTerm == lowerSearchTerm )
  {
    return;
  }

  mSearchTerm = lowerSearchTerm;
  reloadLayer();
  emit searchTermChanged();
}

QgsFeature FeatureListModel::currentFormFeature() const
{
  return mCurrentFormFeature;
}

void FeatureListModel::setCurrentFormFeature( const QgsFeature &feature )
{
  if ( mCurrentFormFeature == feature )
  {
    return;
  }

  mCurrentFormFeature = feature;

  if ( !mFilterExpression.isEmpty() && QgsValueRelationFieldFormatter::expressionRequiresFormScope( mFilterExpression ) )
  {
    reloadLayer();
  }

  emit currentFormFeatureChanged();
}

AppExpressionContextScopesGenerator *FeatureListModel::appExpressionContextScopesGenerator() const
{
  return mAppExpressionContextScopesGenerator.data();
}

void FeatureListModel::setAppExpressionContextScopesGenerator( AppExpressionContextScopesGenerator *generator )
{
  if ( mAppExpressionContextScopesGenerator == generator )
  {
    return;
  }

  mAppExpressionContextScopesGenerator = generator;

  if ( !mFilterExpression.isEmpty() )
  {
    reloadLayer();
  }

  emit appExpressionContextScopesGeneratorChanged();
}
```


