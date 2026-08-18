/***************************************************************************
  qfpendingdeltamodel.cpp - QfPendingDeltaModel

 ---------------------
 begin                : 15.8.2026
 copyright            : (C) 2026 by Mohsen Dehghanzadeh
 email                : mohsen@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfdeltafilewrapper.h"
#include "qfpendingdeltamodel.h"
#include "utils/qffeatureutils.h"

#include <QJsonArray>
#include <QJsonObject>
#include <qgsexpression.h>
#include <qgsfeatureid.h>
#include <qgsfeaturerequest.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>

#include <algorithm>

QfPendingDeltaModel::QfPendingDeltaModel( QObject *parent )
  : QAbstractListModel( parent )
{
}

QfDeltaFileWrapper *QfPendingDeltaModel::deltaFileWrapper() const
{
  return mDeltaFileWrapper;
}

void QfPendingDeltaModel::setDeltaFileWrapper( QfDeltaFileWrapper *deltaFileWrapper )
{
  if ( mDeltaFileWrapper == deltaFileWrapper )
  {
    return;
  }

  if ( mDeltaFileWrapper )
  {
    disconnect( mDeltaFileWrapper, &QfDeltaFileWrapper::countChanged, this, &QfPendingDeltaModel::refresh );
  }

  mDeltaFileWrapper = deltaFileWrapper;

  if ( mDeltaFileWrapper )
  {
    connect( mDeltaFileWrapper, &QfDeltaFileWrapper::countChanged, this, &QfPendingDeltaModel::refresh );
  }

  refresh();

  emit deltaFileWrapperChanged();
}

int QfPendingDeltaModel::rowCount( const QModelIndex &parent ) const
{
  if ( parent.isValid() )
  {
    return 0;
  }

  return static_cast<int>( mPendingDeltas.size() );
}

QVariant QfPendingDeltaModel::data( const QModelIndex &index, int role ) const
{
  if ( index.row() < 0 || index.row() >= mPendingDeltas.size() )
  {
    return QVariant();
  }

  const PendingDelta &pendingDelta = mPendingDeltas.at( index.row() );
  switch ( static_cast<ColumnRole>( role ) )
  {
    case LayerIdRole:
      return pendingDelta.layerId;
    case MethodRole:
      return pendingDelta.method;
    case FeatureNameRole:
      return pendingDelta.featureName;
    case AttributeChangesRole:
      return QVariant::fromValue( pendingDelta.attributeChanges );
    case HasGeometryChangeRole:
      return pendingDelta.hasGeometryChange;
  }

  return QVariant();
}

QHash<int, QByteArray> QfPendingDeltaModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[LayerIdRole] = "LayerId";
  roles[MethodRole] = "Method";
  roles[FeatureNameRole] = "FeatureName";
  roles[AttributeChangesRole] = "AttributeChanges";
  roles[HasGeometryChangeRole] = "HasGeometryChange";
  return roles;
}

QString QfPendingDeltaModel::layerName( const QString &layerId ) const
{
  return mLayerSummaries.value( layerId ).name;
}

Qgis::GeometryType QfPendingDeltaModel::layerGeometryType( const QString &layerId ) const
{
  return mLayerSummaries.value( layerId ).geometryType;
}

int QfPendingDeltaModel::layerChangesCount( const QString &layerId ) const
{
  return mLayerSummaries.value( layerId ).changesCount;
}

void QfPendingDeltaModel::refresh()
{
  beginResetModel();

  mPendingDeltas.clear();
  mLayerSummaries.clear();

  if ( mDeltaFileWrapper )
  {
    const QJsonArray deltas = mDeltaFileWrapper->deltas();
    for ( const QJsonValue &deltaValue : deltas )
    {
      const QJsonObject delta = deltaValue.toObject();

      PendingDelta pendingDelta;
      pendingDelta.layerId = delta.value( QStringLiteral( "localLayerId" ) ).toString();

      const QString method = delta.value( QStringLiteral( "method" ) ).toString();
      if ( method == QLatin1String( "create" ) )
      {
        pendingDelta.method = CreateMethod;
      }
      else if ( method == QLatin1String( "patch" ) )
      {
        pendingDelta.method = PatchMethod;
      }
      else if ( method == QLatin1String( "delete" ) )
      {
        pendingDelta.method = DeleteMethod;
      }

      QgsVectorLayer *layer = qobject_cast<QgsVectorLayer *>( QgsProject::instance()->mapLayer( pendingDelta.layerId ) );

      LayerSummary &layerSummary = mLayerSummaries[pendingDelta.layerId];
      if ( layerSummary.changesCount == 0 )
      {
        layerSummary.name = delta.value( QStringLiteral( "localLayerName" ) ).toString();
        layerSummary.geometryType = layer ? layer->geometryType() : Qgis::GeometryType::Unknown;
      }
      layerSummary.changesCount++;

      pendingDelta.featureName = featureDisplayName( delta, pendingDelta.method, layer );
      pendingDelta.attributeChanges = attributeChanges( delta, pendingDelta.method );
      pendingDelta.hasGeometryChange = pendingDelta.method == PatchMethod && delta.value( QStringLiteral( "new" ) ).toObject().contains( QStringLiteral( "geometry" ) );

      mPendingDeltas << pendingDelta;
    }

    // keep the rows of a layer next to each other so the view can section them
    std::stable_sort( mPendingDeltas.begin(), mPendingDeltas.end(), [this]( const PendingDelta &first, const PendingDelta &second ) {
      const QString firstName = mLayerSummaries.value( first.layerId ).name;
      const QString secondName = mLayerSummaries.value( second.layerId ).name;
      return firstName != secondName ? firstName < secondName : first.layerId < second.layerId;
    } );
  }

  endResetModel();
}

QString QfPendingDeltaModel::featureDisplayName( const QJsonObject &delta, DeltaMethod method, QgsVectorLayer *layer ) const
{
  const QString localPk = delta.value( QStringLiteral( "localPk" ) ).toString();
  if ( !layer )
  {
    return localPk;
  }

  QgsFeature feature;
  if ( method != DeleteMethod )
  {
    layer->getFeatures( QfDeltaFileWrapper::localPkRequest( layer, localPk ).setLimit( 1 ) ).nextFeature( feature );
  }

  if ( !feature.isValid() )
  {
    // the feature is gone from the layer, rebuild it from the values kept in the delta
    const QgsFields fields = layer->fields();
    const QJsonObject values = delta.value( method == DeleteMethod ? QStringLiteral( "old" ) : QStringLiteral( "new" ) ).toObject();
    const QJsonObject attributes = values.value( QStringLiteral( "attributes" ) ).toObject();

    feature = QgsFeature( fields );
    for ( QJsonObject::const_iterator attribute = attributes.constBegin(); attribute != attributes.constEnd(); ++attribute )
    {
      const int fieldIndex = fields.indexFromName( attribute.key() );
      if ( fieldIndex >= 0 )
      {
        feature.setAttribute( fieldIndex, attribute.value().toVariant() );
      }
    }
  }

  // a rebuilt feature carries no id, which is what the display name falls back to when the display expression yields nothing
  const QString displayName = QfFeatureUtils::displayName( layer, feature );
  return displayName.isEmpty() || displayName == QString::number( FID_NULL ) ? localPk : displayName;
}

QList<QfDeltaAttributeChange> QfPendingDeltaModel::attributeChanges( const QJsonObject &delta, DeltaMethod method ) const
{
  const QJsonObject oldAttributes = delta.value( QStringLiteral( "old" ) ).toObject().value( QStringLiteral( "attributes" ) ).toObject();
  const QJsonObject newAttributes = delta.value( QStringLiteral( "new" ) ).toObject().value( QStringLiteral( "attributes" ) ).toObject();

  const QJsonObject &changedAttributes = method == DeleteMethod ? oldAttributes : newAttributes;

  QList<QfDeltaAttributeChange> changes;
  for ( QJsonObject::const_iterator attribute = changedAttributes.constBegin(); attribute != changedAttributes.constEnd(); ++attribute )
  {
    changes << QfDeltaAttributeChange( attribute.key(), oldAttributes.value( attribute.key() ).toVariant(), newAttributes.value( attribute.key() ).toVariant() );
  }

  return changes;
}
