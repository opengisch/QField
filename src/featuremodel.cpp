/***************************************************************************
                            featuremodel.cpp
                              -------------------
              begin                : 10.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "featuremodel.h"

#include <qgsmessagelog.h>
#include <qgsvectorlayer.h>

FeatureModel::FeatureModel( QObject *parent )
  : QAbstractListModel( parent )
{
  connect( this, &FeatureModel::modelReset, this, &FeatureModel::featureChanged );
}

void FeatureModel::setFeature( const QgsFeature& feature )
{
  if ( mFeature == feature )
    return;

  beginResetModel();
  mFeature = feature;
  endResetModel();
  emit featureChanged();
}

void FeatureModel::setLayer( QgsVectorLayer* layer )
{
  if ( layer == mLayer )
    return;

  mLayer = layer;
  if ( mLayer )
    mFeature = QgsFeature( mLayer->fields() );

  emit layerChanged();
}

QgsVectorLayer* FeatureModel::layer() const
{
  return mLayer;
}

QgsFeature FeatureModel::feature() const
{
  return mFeature;
}

QHash<int, QByteArray> FeatureModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
  roles[AttributeName]  = "AttributeName";
  roles[AttributeValue] = "AttributeValue";
  roles[Field] = "Field";

  return roles;
}


int FeatureModel::rowCount( const QModelIndex& parent ) const
{
  if ( parent.isValid() )
    return 0;
  else
    return mFeature.attributes().count();
}

QVariant FeatureModel::data( const QModelIndex& index, int role ) const
{
  switch ( role )
  {
    case AttributeName:
      return mLayer->attributeDisplayName( index.row() );
      break;

    case AttributeValue:
      return mFeature.attribute( index.row() );
      break;

    case Field:
      return mLayer->fields().at( index.row() );
      break;
  }

  return QVariant();
}

bool FeatureModel::setAttribute( int fieldIndex, const QVariant& value )
{
  QVariant val = value;
  QgsField fld = mFeature.fields().at( fieldIndex );

  if ( !fld.convertCompatible( val ) )
  {
    QgsMessageLog::logMessage( tr( "Value \"%1\" %4 could not be converted to a compatible value for field %2(%3)." ).arg( value.toString(), fld.name(), fld.typeName(), value.isNull() ? "NULL" : "NOT NULL" ) );
    return false;
  }
  return mFeature.setAttribute( fieldIndex, val );
}

bool FeatureModel::save()
{
  if ( !mLayer )
    return false;

  bool rv = true;

  if ( !startEditing() )
  {
    rv = false;
  }

  QgsFeature feat = mFeature;
  if ( !mLayer->updateFeature( feat ) )
    QgsMessageLog::logMessage( tr( "Cannot update feature" ), "QField", QgsMessageLog::WARNING );
  rv = commit();

  if ( rv )
  {
    QgsFeature feat;
    if ( mLayer->getFeatures( QgsFeatureRequest().setFilterFid( mFeature.id() ) ).nextFeature( feat ) )
      setFeature( feat );
    else
      QgsMessageLog::logMessage( tr( "Feature %1 could not be fetched after commit" ).arg( mFeature.id() ), "QField", QgsMessageLog::WARNING );
  }
  return rv;
}

void FeatureModel::reset()
{
  if ( !mLayer )
    return;

  mLayer->rollBack();
}

bool FeatureModel::suppressFeatureForm() const
{
  if ( !mLayer )
    return false;

  return mLayer->editFormConfig().suppress();
}

void FeatureModel::resetAttributes()
{
  if ( !mLayer )
    return;

  beginResetModel();
  for ( int i = 0; i < mLayer->fields().count(); ++i )
  {
    mFeature.setAttribute( i, QVariant() );
  }
  endResetModel();
}

void FeatureModel::applyGeometry()
{
  mFeature.setGeometry( mGeometry->asQgsGeometry() );
}

void FeatureModel::create()
{
  if ( !mLayer )
    return;

  startEditing();
  if ( !mLayer->addFeature( mFeature ) )
  {
    QgsMessageLog::logMessage( tr( "Feature could not be added" ), "QField", QgsMessageLog::CRITICAL );
  }
  commit();
}

bool FeatureModel::commit()
{
  if ( !mLayer->commitChanges() )
  {
    QgsMessageLog::logMessage( tr( "Could not save changes. Rolling back." ), "QField", QgsMessageLog::CRITICAL );
    mLayer->rollBack();
    return false;
  }
  else
  {
    return true;
  }
}

bool FeatureModel::startEditing()
{
  // Already an edit session active
  if ( mLayer->editBuffer() )
    return true;

  if ( !mLayer->startEditing() )
  {
    QgsMessageLog::logMessage( tr( "Cannot start editing" ), "QField", QgsMessageLog::WARNING );
    return false;
  }
  else
  {
    return true;
  }
}
