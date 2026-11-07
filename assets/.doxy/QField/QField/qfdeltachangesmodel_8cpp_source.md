

# File qfdeltachangesmodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldcloud**](dir_d27ac98b23548f5810403c734ce9817b.md) **>** [**qfdeltachangesmodel.cpp**](qfdeltachangesmodel_8cpp.md)

[Go to the documentation of this file](qfdeltachangesmodel_8cpp.md)


```C++
/***************************************************************************
  qfdeltachangesmodel.cpp - QfDeltaChangesModel

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

#include "qfdeltachangesmodel.h"
#include "qfdeltafilewrapper.h"
#include "qffeatureutils.h"

#include <QJsonObject>
#include <qgsfeatureid.h>
#include <qgsfeatureiterator.h>
#include <qgsfeaturerequest.h>
#include <qgsproject.h>
#include <qgsvectorlayer.h>

#include <algorithm>

QfDeltaChangesModel::QfDeltaChangesModel( QObject *parent )
  : QAbstractListModel( parent )
{
}

QfDeltaFileWrapper *QfDeltaChangesModel::deltaFileWrapper() const
{
  return mDeltaFileWrapper;
}

void QfDeltaChangesModel::setDeltaFileWrapper( QfDeltaFileWrapper *deltaFileWrapper )
{
  if ( mDeltaFileWrapper == deltaFileWrapper )
  {
    return;
  }

  if ( mDeltaFileWrapper )
  {
    disconnect( mDeltaFileWrapper, &QfDeltaFileWrapper::countChanged, this, &QfDeltaChangesModel::refresh );
  }

  mDeltaFileWrapper = deltaFileWrapper;

  if ( mDeltaFileWrapper )
  {
    connect( mDeltaFileWrapper, &QfDeltaFileWrapper::countChanged, this, &QfDeltaChangesModel::refresh );
  }

  beginResetModel();
  mLayerSummaries.clear();
  mDeltaChanges.clear();
  endResetModel();

  emit deltaFileWrapperChanged();

  refresh();
}

int QfDeltaChangesModel::rowCount( const QModelIndex &parent ) const
{
  if ( parent.isValid() )
  {
    return 0;
  }

  return static_cast<int>( mDeltaChanges.size() );
}

QVariant QfDeltaChangesModel::data( const QModelIndex &index, int role ) const
{
  if ( index.row() < 0 || index.row() >= mDeltaChanges.size() )
  {
    return QVariant();
  }

  const DeltaChange &deltaChange = mDeltaChanges.at( index.row() );
  switch ( static_cast<ColumnRole>( role ) )
  {
    case LayerIdRole:
      return deltaChange.layerId;
    case MethodRole:
      return deltaChange.method;
    case FeatureNameRole:
      return deltaChange.featureName;
    case AttributeChangesRole:
      return QVariant::fromValue( deltaChange.attributeChanges );
    case HasGeometryChangeRole:
      return deltaChange.hasGeometryChange;
  }

  return QVariant();
}

QHash<int, QByteArray> QfDeltaChangesModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[LayerIdRole] = "LayerId";
  roles[MethodRole] = "Method";
  roles[FeatureNameRole] = "FeatureName";
  roles[AttributeChangesRole] = "AttributeChanges";
  roles[HasGeometryChangeRole] = "HasGeometryChange";
  return roles;
}

QString QfDeltaChangesModel::layerName( const QString &layerId ) const
{
  return mLayerSummaries.value( layerId ).name;
}

Qgis::GeometryType QfDeltaChangesModel::layerGeometryType( const QString &layerId ) const
{
  return mLayerSummaries.value( layerId ).geometryType;
}

int QfDeltaChangesModel::layerChangesCount( const QString &layerId ) const
{
  return static_cast<int>( mLayerSummaries.value( layerId ).changes.size() );
}

QfDeltaChangesModel::DeltaMethod QfDeltaChangesModel::deltaMethod( const QJsonObject &delta )
{
  const QString method = delta.value( QStringLiteral( "method" ) ).toString();
  if ( method == QLatin1String( "create" ) )
  {
    return CreateMethod;
  }
  else if ( method == QLatin1String( "patch" ) )
  {
    return PatchMethod;
  }
  else if ( method == QLatin1String( "delete" ) )
  {
    return DeleteMethod;
  }

  return UnknownMethod;
}

void QfDeltaChangesModel::refresh()
{
  QHash<QString, QJsonArray> deltasByLayerId;
  QHash<QString, QString> layerNames;

  if ( mDeltaFileWrapper )
  {
    const QJsonArray deltas = mDeltaFileWrapper->deltas();
    for ( const auto &deltaValue : deltas )
    {
      const QJsonObject delta = deltaValue.toObject();
      const QString layerId = delta.value( QStringLiteral( "localLayerId" ) ).toString();

      deltasByLayerId[layerId].append( delta );
      layerNames.insert( layerId, delta.value( QStringLiteral( "localLayerName" ) ).toString() );
    }
  }

  QHash<QString, LayerSummary> layerSummaries;
  for ( QHash<QString, QJsonArray>::const_iterator it = deltasByLayerId.constBegin(); it != deltasByLayerId.constEnd(); ++it )
  {
    // the feature names cost a layer request, keep the layers whose deltas did not move
    const LayerSummary knownSummary = mLayerSummaries.value( it.key() );
    if ( knownSummary.deltas == it.value() )
    {
      layerSummaries.insert( it.key(), knownSummary );
      continue;
    }

    QgsVectorLayer *layer = qobject_cast<QgsVectorLayer *>( QgsProject::instance()->mapLayer( it.key() ) );

    LayerSummary layerSummary;
    layerSummary.name = layerNames.value( it.key() );
    layerSummary.geometryType = layer ? layer->geometryType() : Qgis::GeometryType::Unknown;
    layerSummary.deltas = it.value();
    layerSummary.changes = layerChanges( it.key(), it.value(), layer );

    layerSummaries.insert( it.key(), layerSummary );
  }

  // keep the rows of a layer next to each other so the view can section them
  QStringList layerIds = layerSummaries.keys();
  std::sort( layerIds.begin(), layerIds.end(), [&layerSummaries]( const QString &first, const QString &second ) {
    const QString firstName = layerSummaries.value( first ).name;
    const QString secondName = layerSummaries.value( second ).name;
    return firstName != secondName ? firstName < secondName : first < second;
  } );

  QList<DeltaChange> deltaChanges;
  for ( const QString &layerId : std::as_const( layerIds ) )
  {
    deltaChanges << layerSummaries.value( layerId ).changes;
  }

  beginResetModel();
  mLayerSummaries = layerSummaries;
  mDeltaChanges = deltaChanges;
  endResetModel();
}

QList<QfDeltaChangesModel::DeltaChange> QfDeltaChangesModel::layerChanges( const QString &layerId, const QJsonArray &deltas, QgsVectorLayer *layer ) const
{
  const QHash<QString, QString> displayNames = featureDisplayNames( layer, deltas );

  QList<DeltaChange> changes;
  changes.reserve( deltas.size() );
  for ( const auto &deltaValue : deltas )
  {
    const QJsonObject delta = deltaValue.toObject();
    const QString localPk = delta.value( QStringLiteral( "localPk" ) ).toString();

    DeltaChange change;
    change.layerId = layerId;
    change.method = deltaMethod( delta );
    change.featureName = displayNames.value( localPk, localPk );
    change.attributeChanges = attributeChanges( delta, change.method );
    change.hasGeometryChange = change.method == PatchMethod && delta.value( QStringLiteral( "new" ) ).toObject().contains( QStringLiteral( "geometry" ) );

    changes << change;
  }

  return changes;
}

QHash<QString, QString> QfDeltaChangesModel::featureDisplayNames( QgsVectorLayer *layer, const QJsonArray &deltas ) const
{
  QHash<QString, QString> displayNames;
  if ( !layer )
  {
    return displayNames;
  }

  QStringList remainingPks;
  QHash<QString, QJsonObject> deltaByLocalPk;
  for ( const auto &deltaValue : deltas )
  {
    const QJsonObject delta = deltaValue.toObject();
    const QString localPk = delta.value( QStringLiteral( "localPk" ) ).toString();

    deltaByLocalPk.insert( localPk, delta );
    if ( deltaMethod( delta ) != DeleteMethod )
    {
      remainingPks << localPk;
    }
  }

  const QString localPkFieldName = QfDeltaFileWrapper::getLocalPkAttribute( layer ).second;
  if ( !remainingPks.isEmpty() && !localPkFieldName.isEmpty() )
  {
    QgsFeature feature;
    QgsFeatureIterator features = layer->getFeatures( QfDeltaFileWrapper::localPkRequest( layer, remainingPks ) );
    while ( features.nextFeature( feature ) )
    {
      const QString displayName = QfFeatureUtils::displayName( layer, feature );
      if ( !displayName.isEmpty() )
      {
        displayNames.insert( feature.attribute( localPkFieldName ).toString(), displayName );
      }
    }
  }

  // the features gone from the layer are named after the values their delta kept of them
  const QgsFields fields = layer->fields();
  for ( QHash<QString, QJsonObject>::const_iterator it = deltaByLocalPk.constBegin(); it != deltaByLocalPk.constEnd(); ++it )
  {
    if ( displayNames.contains( it.key() ) )
    {
      continue;
    }

    const QJsonObject values = it.value().value( deltaMethod( it.value() ) == DeleteMethod ? QStringLiteral( "old" ) : QStringLiteral( "new" ) ).toObject();
    const QJsonObject attributes = values.value( QStringLiteral( "attributes" ) ).toObject();

    QgsFeature feature( fields );
    for ( QJsonObject::const_iterator attribute = attributes.constBegin(); attribute != attributes.constEnd(); ++attribute )
    {
      const int fieldIndex = fields.indexFromName( attribute.key() );
      if ( fieldIndex >= 0 )
      {
        feature.setAttribute( fieldIndex, attribute.value().toVariant() );
      }
    }

    // a rebuilt feature carries no id, the fallback of a display expression yielding nothing
    const QString displayName = QfFeatureUtils::displayName( layer, feature );
    if ( !displayName.isEmpty() && displayName != QString::number( FID_NULL ) )
    {
      displayNames.insert( it.key(), displayName );
    }
  }

  return displayNames;
}

QList<QfDeltaAttributeChange> QfDeltaChangesModel::attributeChanges( const QJsonObject &delta, DeltaMethod method ) const
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
```


