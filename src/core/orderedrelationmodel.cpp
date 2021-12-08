/***************************************************************************
  orderedrelationmodel.h - OrderedRelationModel

 ---------------------
 begin                : Jun 2021
 copyright            : (C) 2021 by Ivan Ivanov
 email                : ivan (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "fieldexpressionvaluesgatherer.h"
#include "orderedrelationmodel.h"
#include "qfield_core_export.h"
#include "qgsexpressioncontextutils.h"
#include "qgsfeature.h"
#include "qgsfeaturerequest.h"
#include "qgsmessagelog.h"
#include "qgsproject.h"
#include "qgsrelation.h"
#include "qgsvectorlayer.h"

#include <QAbstractTableModel>

OrderedRelationModel::OrderedRelationModel( QObject *parent )
  : ReferencingFeatureListModel( parent )
{
}

QString OrderedRelationModel::orderingField() const
{
  return mOrderingField;
}

void OrderedRelationModel::setOrderingField( const QString &orderingField )
{
  if ( mOrderingField == orderingField )
    return;

  mOrderingField = orderingField;
  reload();
  emit orderingFieldChanged();
}

QString OrderedRelationModel::imagePath() const
{
  return mImagePath;
}

void OrderedRelationModel::setImagePath( const QString &imagePath )
{
  if ( mImagePath == imagePath )
    return;

  mImagePath = imagePath;
  reload();
  emit imagePathChanged();
}

QString OrderedRelationModel::description() const
{
  return mDescription;
}

void OrderedRelationModel::setDescription( const QString &description )
{
  if ( mDescription == description )
    return;

  mDescription = description;
  reload();
  emit descriptionChanged();
}

QHash<int, QByteArray> OrderedRelationModel::roleNames() const
{
  QHash<int, QByteArray> roles = ReferencingFeatureListModel::roleNames();

  roles[OrderedRelationModel::ImagePathRole] = "ImagePath";
  roles[OrderedRelationModel::DescriptionRole] = "Description";
  roles[OrderedRelationModel::FeatureIdRole] = "FeatureId";

  return roles;
}

QVariant OrderedRelationModel::data( const QModelIndex &index, int role ) const
{
  QVariant result;

  if ( index.row() < 0 || index.row() >= rowCount( QModelIndex() ) )
    return QVariant();

  QgsExpression expr;
  QgsExpressionContext context;
  context.appendScopes( QgsExpressionContextUtils::globalProjectLayerScopes( mRelation.referencingLayer() ) );
  context.setFeature( mEntries.at( index.row() ).referencingFeature );

  switch ( role )
  {
    case ImagePathRole:
      expr = QgsExpression( mImagePath );
      result = expr.evaluate( &context );
      return result;
    case DescriptionRole:
      expr = QgsExpression( mDescription );
      result = expr.evaluate( &context );
      return result;
    case FeatureIdRole:
      return mEntries[index.row()].referencingFeature.id();
  }

  return ReferencingFeatureListModel::data( index, role );
}

void OrderedRelationModel::onViewCurrentFeatureChanged( int index )
{
  if ( index < 0 || index >= rowCount( QModelIndex() ) )
    return;

  emit currentFeatureChanged( mEntries[index].referencingFeature );
}

bool OrderedRelationModel::moveItems( const int fromIdx, const int toIdx )
{
  if ( fromIdx == toIdx )
    return false;

  if ( !mRelation.isValid() )
    return false;

  QgsVectorLayer *referencingLayer = mRelation.referencingLayer();
  int orderingFieldIdx = referencingLayer->fields().indexFromName( mOrderingField );

  if ( orderingFieldIdx == -1 )
    return false;

  int startIdx = std::min( fromIdx, toIdx );
  int endIdx = std::max( fromIdx, toIdx );
  int delta = fromIdx > toIdx ? 1 : -1;

  if ( startIdx < 0 || endIdx >= mEntries.count() )
    return false;

  if ( !referencingLayer->startEditing() )
  {
    QgsMessageLog::logMessage( tr( "Cannot start editing" ), "QField", Qgis::Critical );
    return false;
  }

  for ( int i = startIdx; i <= endIdx; i++ )
  {
    int newIdx = ( i == fromIdx )
                   ? toIdx + 1
                   : mEntries[i].referencingFeature.attribute( orderingFieldIdx ).toInt() + delta;
    bool isSuccess = referencingLayer->changeAttributeValue( mEntries[i].referencingFeature.id(), orderingFieldIdx, newIdx );

    if ( !isSuccess )
    {
      if ( referencingLayer->rollBack() )
        QgsMessageLog::logMessage( tr( "Cannot rollback layer changes in layer %1" ).arg( referencingLayer->name() ), "QField", Qgis::Critical );

      emit failedReorder();
      return false;
    }
  }

  if ( !referencingLayer->commitChanges() )
  {
    QgsMessageLog::logMessage( tr( "Cannot commit layer changes in layer %1." ).arg( referencingLayer->name() ), "QField", Qgis::Critical );

    if ( !referencingLayer->rollBack() )
      QgsMessageLog::logMessage( tr( "Cannot rollback layer changes in layer %1" ).arg( referencingLayer->name() ), "QField", Qgis::Critical );

    return false;
  }

  reload();

  return true;
}

bool OrderedRelationModel::beforeDeleteFeature( QgsVectorLayer *referencingLayer, QgsFeatureId referencingFeatureId )
{
  int orderingFieldIdx = referencingLayer->fields().indexFromName( mOrderingField );

  if ( orderingFieldIdx == -1 )
    return false;

  // first try to update the ordering the of the related features
  bool isSuccess = false;
  int ordering = -1;
  for ( const Entry &entry : std::as_const( mEntries ) )
  {
    // find the deleted feature
    if ( entry.referencingFeature.id() == referencingFeatureId )
    {
      ordering = entry.referencingFeature.attribute( mOrderingField ).toInt();
      continue;
    }

    // skip until the deleted feature is looped and the ordering has an absolute value
    if ( ordering == -1 )
      continue;

    Q_ASSERT( ordering <= entry.referencingFeature.attribute( orderingFieldIdx ).toInt() );

    int newOrdering = ordering;
    isSuccess = referencingLayer->changeAttributeValue( entry.referencingFeature.id(), orderingFieldIdx, newOrdering );

    if ( !isSuccess )
    {
      QgsMessageLog::logMessage( tr( "Cannot update features ordering" ), "QField", Qgis::Critical );

      if ( !referencingLayer->rollBack() )
        QgsMessageLog::logMessage( tr( "Cannot rollback layer changes in layer %1" ).arg( referencingLayer->name() ), "QField", Qgis::Critical );

      return false;
    }

    // increment the absolute value of the ordering for the next element
    ordering++;
  }

  return true;
}

void OrderedRelationModel::sortEntries()
{
  std::sort( mEntries.begin(), mEntries.end(), [=]( const Entry &e1, const Entry &e2 ) {
    return e1.referencingFeature.attribute( mOrderingField ).toInt() < e2.referencingFeature.attribute( mOrderingField ).toInt();
  } );
}
