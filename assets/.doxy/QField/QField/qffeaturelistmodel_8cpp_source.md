

# File qffeaturelistmodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qffeaturelistmodel.cpp**](qffeaturelistmodel_8cpp.md)

[Go to the documentation of this file](qffeaturelistmodel_8cpp.md)


```C++
/***************************************************************************
  qffeaturelistmodel.cpp - QfFeatureListModel

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


#include "qffeaturelistmodel.h"
#include "qfstringutils.h"
#include "qgsvectorlayer.h"

#include <QRegularExpression>
#include <qgsexpressioncontextutils.h>
#include <qgsproject.h>
#include <qgsstringutils.h>
#include <qgsvaluerelationfieldformatter.h>


QfFeatureListModel::QfFeatureListModel( QObject *parent )
  : QAbstractItemModel( parent )
  , mCurrentLayer( nullptr )
{
  mReloadTimer.setInterval( 200 );
  mReloadTimer.setSingleShot( true );
  connect( &mReloadTimer, &QTimer::timeout, this, &QfFeatureListModel::gatherFeatureList );
}

QfFeatureListModel::~QfFeatureListModel()
{
  cleanupGatherer();
}

void QfFeatureListModel::cleanupGatherer()
{
  if ( mGatherer )
  {
    disconnect( mGatherer, &QThread::finished, this, &QfFeatureListModel::processFeatureList );
    connect( mGatherer, &QThread::finished, mGatherer, &QObject::deleteLater );
    mGatherer->stop();
    mGatherer = nullptr;
  }
}

QModelIndex QfFeatureListModel::index( int row, int column, const QModelIndex &parent ) const
{
  Q_UNUSED( parent )

  return createIndex( row, column, 1000 );
}

QModelIndex QfFeatureListModel::parent( const QModelIndex &child ) const
{
  Q_UNUSED( child )

  // We live in flat structures here
  return QModelIndex();
}

int QfFeatureListModel::rowCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent )
  return static_cast<int>( mEntries.size() );
}

int QfFeatureListModel::columnCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent )
  return 1;
}

QVariant QfFeatureListModel::data( const QModelIndex &index, int role ) const
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

QHash<int, QByteArray> QfFeatureListModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();

  roles[KeyFieldRole] = "keyFieldValue";
  roles[DisplayStringRole] = "displayString";
  roles[GroupFieldRole] = "groupFieldValue";
  roles[FeatureIdRole] = "featureId";

  return roles;
}

QgsVectorLayer *QfFeatureListModel::currentLayer() const
{
  return mCurrentLayer.data();
}

void QfFeatureListModel::setCurrentLayer( QgsVectorLayer *currentLayer )
{
  if ( mCurrentLayer == currentLayer )
  {
    return;
  }

  if ( mCurrentLayer )
  {
    disconnect( mCurrentLayer, &QgsVectorLayer::featureAdded, this, &QfFeatureListModel::onFeatureAdded );
    disconnect( mCurrentLayer, &QgsVectorLayer::attributeValueChanged, this, &QfFeatureListModel::onAttributeValueChanged );
    disconnect( mCurrentLayer, &QgsVectorLayer::featureDeleted, this, &QfFeatureListModel::onFeatureDeleted );
    disconnect( mCurrentLayer, &QgsVectorLayer::dataChanged, this, &QfFeatureListModel::reloadLayer );
  }

  mCurrentLayer = currentLayer;

  if ( mCurrentLayer )
  {
    connect( mCurrentLayer, &QgsVectorLayer::featureAdded, this, &QfFeatureListModel::onFeatureAdded );
    connect( mCurrentLayer, &QgsVectorLayer::attributeValueChanged, this, &QfFeatureListModel::onAttributeValueChanged );
    connect( mCurrentLayer, &QgsVectorLayer::featureDeleted, this, &QfFeatureListModel::onFeatureDeleted );
    connect( mCurrentLayer, &QgsVectorLayer::dataChanged, this, &QfFeatureListModel::reloadLayer );
  }

  reloadLayer();

  emit currentLayerChanged();
}

QString QfFeatureListModel::keyField() const
{
  return mKeyField;
}

void QfFeatureListModel::setKeyField( const QString &keyField )
{
  if ( mKeyField == keyField )
  {
    return;
  }

  mKeyField = keyField;

  reloadLayer();

  emit keyFieldChanged();
}

QString QfFeatureListModel::displayValueField() const
{
  return mDisplayValueField;
}

void QfFeatureListModel::setDisplayValueField( const QString &displayValueField )
{
  if ( mDisplayValueField == displayValueField )
  {
    return;
  }

  mDisplayValueField = displayValueField;

  reloadLayer();

  emit displayValueFieldChanged();
}

QString QfFeatureListModel::groupField() const
{
  return mGroupField;
}

void QfFeatureListModel::setGroupField( const QString &groupField )
{
  if ( mGroupField == groupField )
  {
    return;
  }

  mGroupField = groupField;

  reloadLayer();

  emit groupFieldChanged();
}

bool QfFeatureListModel::displayGroupName() const
{
  return mDisplayGroupName;
}

void QfFeatureListModel::setDisplayGroupName( bool displayGroupName )
{
  if ( mDisplayGroupName == displayGroupName )
  {
    return;
  }

  mDisplayGroupName = displayGroupName;

  emit displayGroupNameChanged();
}

int QfFeatureListModel::findKey( const QVariant &key ) const
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

QList<int> QfFeatureListModel::findDisplayValueMatches( const QString &filter ) const
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

void QfFeatureListModel::onFeatureAdded()
{
  reloadLayer();
}

void QfFeatureListModel::onAttributeValueChanged( QgsFeatureId, int idx, const QVariant & )
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

void QfFeatureListModel::onFeatureDeleted()
{
  reloadLayer();
}

QgsFeature QfFeatureListModel::getFeatureFromKeyValue( const QVariant &value ) const
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

QgsFeature QfFeatureListModel::getFeatureById( QgsFeatureId id ) const
{
  if ( !mCurrentLayer )
  {
    return QgsFeature();
  }

  return mCurrentLayer->getFeature( id );
}

void QfFeatureListModel::gatherFeatureList()
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

  if ( orderByField() && !orderByFieldName().isNull() )
  {
    referencedColumns << mOrderByFieldName;
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
    QString escapedSearchTerm = QgsExpression::quotedValue( QgsStringUtils::unaccent( mSearchTerm ) ).replace( QRegularExpression( QStringLiteral( "^'|'$" ) ), QString( "" ) );
    searchTermExpression = QStringLiteral( " unaccent( %1 ) ILIKE '%%2%' " ).arg( fieldDisplayString, escapedSearchTerm );

    QStringList searchTermParts = escapedSearchTerm.split( QRegularExpression( QStringLiteral( "\\s+" ) ), Qt::SkipEmptyParts );
    if ( !searchTermParts.isEmpty() )
    {
      for ( QString &searchTermPart : searchTermParts )
      {
        searchTermPart = QStringLiteral( "unaccent( %1 ) ILIKE '%%2%' " ).arg( fieldDisplayString, searchTermPart );
      }
      searchTermExpression += QStringLiteral( "OR (%2) " ).arg( searchTermParts.join( QStringLiteral( " AND " ) ) );
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

  mGatherer = new QfFeatureExpressionValuesGatherer( mCurrentLayer, fieldDisplayString, request, QStringList() << keyField() << groupField() << orderByFieldName() );
  connect( mGatherer, &QThread::finished, this, &QfFeatureListModel::processFeatureList );
  mGatherer->start();
}

void QfFeatureListModel::processFeatureList()
{
  if ( !mGatherer )
  {
    return;
  }

  mEntries.clear();

  QList<Entry> entries;

  if ( mAddNull )
  {
    entries.append( Entry( QStringLiteral( "<i>NULL</i>" ), QVariant(), QVariant(), QgsFeatureId(), QString() ) );
  }

  const QVector<QfFeatureExpressionValuesGatherer::Entry> gatheredEntries = mGatherer->entries();
  mGatherer->deleteLater();
  mGatherer = nullptr;

  for ( const QfFeatureExpressionValuesGatherer::Entry &gatheredEntry : gatheredEntries )
  {
    Entry entry( gatheredEntry.value, gatheredEntry.identifierFields.at( 0 ), gatheredEntry.identifierFields.at( 1 ), gatheredEntry.featureId, gatheredEntry.identifierFields.at( 2 ).toString() );

    if ( !mSearchTerm.isEmpty() )
    {
      entry.fuzzyScore = QfStringUtils::calcFuzzyScore( entry.displayString, mSearchTerm );
      if ( qgsDoubleNear( entry.fuzzyScore, 0.0 ) )
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

      return entry1.fuzzyScore > entry2.fuzzyScore;
    }

    if ( mOrderByValue )
    {
      return entry1.displayString.toLower() < entry2.displayString.toLower();
    }

    if ( mOrderByField && !mOrderByFieldName.isEmpty() )
    {
      return entry1.orderByValue < entry2.orderByValue;
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

QfFeatureListModel::Entry QfFeatureListModel::entryFromRow( int row )
{
  return row < 0 || row >= mEntries.size() ? QfFeatureListModel::Entry() : mEntries.at( row );
}

void QfFeatureListModel::reloadLayer()
{
  cleanupGatherer();
  mReloadTimer.start();
}

bool QfFeatureListModel::addNull() const
{
  return mAddNull;
}

void QfFeatureListModel::setAddNull( bool addNull )
{
  if ( mAddNull == addNull )
  {
    return;
  }

  mAddNull = addNull;
  emit addNullChanged();

  reloadLayer();
}

bool QfFeatureListModel::orderByValue() const
{
  return mOrderByValue;
}

void QfFeatureListModel::setOrderByValue( bool orderByValue )
{
  if ( mOrderByValue == orderByValue )
  {
    return;
  }

  mOrderByValue = orderByValue;
  reloadLayer();
  emit orderByValueChanged();
}

bool QfFeatureListModel::orderByField() const
{
  return mOrderByField;
}

void QfFeatureListModel::setOrderByField( bool orderByField )
{
  if ( mOrderByField == orderByField )
  {
    return;
  }

  mOrderByField = orderByField;
  reloadLayer();
  emit orderByFieldChanged();
}

QString QfFeatureListModel::orderByFieldName() const
{
  return mOrderByFieldName;
}

void QfFeatureListModel::setOrderByFieldName( const QString &orderByFieldName )
{
  if ( mOrderByFieldName == orderByFieldName )
  {
    return;
  }

  mOrderByFieldName = orderByFieldName;
  reloadLayer();
  emit orderByFieldNameChanged();
}

QString QfFeatureListModel::filterExpression() const
{
  return mFilterExpression;
}

void QfFeatureListModel::setFilterExpression( const QString &filterExpression )
{
  if ( mFilterExpression == filterExpression )
  {
    return;
  }

  mFilterExpression = filterExpression;
  reloadLayer();
  emit filterExpressionChanged();
}

QString QfFeatureListModel::searchTerm() const
{
  return mSearchTerm;
}

void QfFeatureListModel::setSearchTerm( const QString &searchTerm )
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

QgsFeature QfFeatureListModel::currentFormFeature() const
{
  return mCurrentFormFeature;
}

void QfFeatureListModel::setCurrentFormFeature( const QgsFeature &feature )
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

QfAppExpressionContextScopesGenerator *QfFeatureListModel::appExpressionContextScopesGenerator() const
{
  return mAppExpressionContextScopesGenerator.data();
}

void QfFeatureListModel::setAppExpressionContextScopesGenerator( QfAppExpressionContextScopesGenerator *generator )
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


