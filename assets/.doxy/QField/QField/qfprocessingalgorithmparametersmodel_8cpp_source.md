

# File qfprocessingalgorithmparametersmodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**processing**](dir_14e4815d6f2d5bd986fc95a1af67243d.md) **>** [**qfprocessingalgorithmparametersmodel.cpp**](qfprocessingalgorithmparametersmodel_8cpp.md)

[Go to the documentation of this file](qfprocessingalgorithmparametersmodel_8cpp.md)


```C++
/***************************************************************************
  qfprocessingalgorithmparametersmodel.h - QfProcessingAlgorithmParametersModel

 ---------------------
 begin                : 21.06.2024
 copyright            : (C) 2024 by Mathieu Pellerin
 email                : mathieu at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "qfprocessingalgorithmparametersmodel.h"

#include <qgsapplication.h>
#include <qgscoordinatereferencesystem.h>
#include <qgsprocessingalgorithm.h>
#include <qgsprocessingparameters.h>
#include <qgsprocessingregistry.h>
#include <qgsunittypes.h>

QfProcessingAlgorithmParametersModel::QfProcessingAlgorithmParametersModel( QObject *parent )
  : QSortFilterProxyModel( parent )
  , mModel( new QfProcessingAlgorithmParametersModelBase( parent ) )
{
  setSourceModel( mModel );
  connect( mModel, &QfProcessingAlgorithmParametersModelBase::algorithmIdChanged, this, &QfProcessingAlgorithmParametersModel::algorithmIdChanged );
  connect( mModel, &QfProcessingAlgorithmParametersModelBase::inPlaceLayerChanged, this, &QfProcessingAlgorithmParametersModel::inPlaceLayerChanged );
  connect( mModel, &QfProcessingAlgorithmParametersModelBase::parametersChanged, this, &QfProcessingAlgorithmParametersModel::parametersChanged );
}

void QfProcessingAlgorithmParametersModel::setFilters( QfProcessingAlgorithmParametersModel::Filters filters )
{
  if ( mFilters == filters )
  {
    return;
  }

  beginFilterChange();
  mFilters = filters;
  endFilterChange( QSortFilterProxyModel::Direction::Rows );

  emit filtersChanged();
}

QString QfProcessingAlgorithmParametersModel::algorithmId() const
{
  return mModel->algorithmId();
}

void QfProcessingAlgorithmParametersModel::setAlgorithmId( const QString &id )
{
  mModel->setAlgorithmId( id );
}

QgsVectorLayer *QfProcessingAlgorithmParametersModel::inPlaceLayer() const
{
  return mModel->inPlaceLayer();
}

void QfProcessingAlgorithmParametersModel::setInPlaceLayer( QgsVectorLayer *layer )
{
  mModel->setInPlaceLayer( layer );
}

bool QfProcessingAlgorithmParametersModel::isValid() const
{
  return mModel->isValid();
}

bool QfProcessingAlgorithmParametersModel::hasParameters() const
{
  return mModel->hasParameters();
}

bool QfProcessingAlgorithmParametersModel::hasAdvancedParameters() const
{
  return mModel->hasAdvancedParameters();
}

QString QfProcessingAlgorithmParametersModel::algorithmDisplayName() const
{
  return mModel->algorithmDisplayName();
}

QString QfProcessingAlgorithmParametersModel::algorithmShortHelp() const
{
  return mModel->algorithmShortHelp();
}

QVariantMap QfProcessingAlgorithmParametersModel::parameters()
{
  return mModel->parameters();
}

void QfProcessingAlgorithmParametersModel::setParameters( const QVariantMap &parameters )
{
  mModel->setParameters( parameters );
}

bool QfProcessingAlgorithmParametersModel::filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
{
  QModelIndex sourceIndex = mModel->index( sourceRow, 0, sourceParent );
  if ( ( mFilters & Filter::GeneralParameterFilter ) && ( mFilters & Filter::AdvancedParameterFilter ) )
  {
    return true;
  }
  else if ( mFilters & Filter::GeneralParameterFilter )
  {
    const bool advancedParameter = mModel->data( sourceIndex, QfProcessingAlgorithmParametersModelBase::ParameterFlagsRole ).toInt() & static_cast<int>( Qgis::ProcessingParameterFlag::Advanced );
    if ( advancedParameter )
      return false;
  }
  else if ( mFilters & Filter::AdvancedParameterFilter )
  {
    const bool advancedParameter = mModel->data( sourceIndex, QfProcessingAlgorithmParametersModelBase::ParameterFlagsRole ).toInt() & static_cast<int>( Qgis::ProcessingParameterFlag::Advanced );
    if ( !advancedParameter )
      return false;
  }

  return true;
}


QfProcessingAlgorithmParametersModelBase::QfProcessingAlgorithmParametersModelBase( QObject *parent )
  : QAbstractListModel( parent )
{
}

void QfProcessingAlgorithmParametersModelBase::rebuild()
{
  beginResetModel();
  mHasAdvancedParameters = false;
  mParameters.clear();
  mValues.clear();

  if ( mAlgorithm )
  {
    const static QStringList sSupportedParameters = { QStringLiteral( "number" ), QStringLiteral( "area" ), QStringLiteral( "distance" ), QStringLiteral( "enum" ), QStringLiteral( "boolean" ), QStringLiteral( "source" ) };
    const QgsProcessingAlgorithm *algorithm = QgsApplication::instance()->processingRegistry()->algorithmById( mAlgorithmId );
    for ( const QgsProcessingParameterDefinition *definition : algorithm->parameterDefinitions() )
    {
      if ( sSupportedParameters.contains( definition->type() ) )
      {
        if ( definition->flags() & Qgis::ProcessingParameterFlag::Advanced )
        {
          mHasAdvancedParameters = true;
        }

        if ( definition->type() != QStringLiteral( "source" ) || definition->name() != "INPUT" )
        {
          mParameters << definition;
          mValues << definition->defaultValue();
        }
      }
    }
  }

  endResetModel();
}

void QfProcessingAlgorithmParametersModelBase::setAlgorithmId( const QString &id )
{
  if ( mAlgorithmId == id )
  {
    return;
  }

  mAlgorithmId = id;
  mAlgorithm = !mAlgorithmId.isEmpty() ? QgsApplication::instance()->processingRegistry()->algorithmById( mAlgorithmId ) : nullptr;

  rebuild();

  emit algorithmIdChanged( mAlgorithmId );
  emit parametersChanged();
}

void QfProcessingAlgorithmParametersModelBase::setInPlaceLayer( QgsVectorLayer *layer )
{
  if ( mInPlaceLayer == layer )
  {
    return;
  }

  mInPlaceLayer = layer;

  rebuild();

  emit inPlaceLayerChanged();
  emit parametersChanged();
}

QString QfProcessingAlgorithmParametersModelBase::algorithmDisplayName() const
{
  return mAlgorithm ? mAlgorithm->displayName() : QString();
}

QString QfProcessingAlgorithmParametersModelBase::algorithmShortHelp() const
{
  return mAlgorithm ? mAlgorithm->shortHelpString() : QString();
}

QVariantMap QfProcessingAlgorithmParametersModelBase::parameters()
{
  QVariantMap parameters;
  for ( int i = 0; i < mParameters.size(); i++ )
  {
    parameters[mParameters.at( i )->name()] = mValues.at( i );
  }
  return parameters;
}

void QfProcessingAlgorithmParametersModelBase::setParameters( const QVariantMap &parameters )
{
  for ( int i = 0; i < mParameters.size(); i++ )
  {
    if ( parameters.contains( mParameters.at( i )->name() ) )
    {
      mValues[i] = parameters.value( mParameters.at( i )->name() );
    }
  }

  emit parametersChanged();
}

QHash<int, QByteArray> QfProcessingAlgorithmParametersModelBase::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
  roles[ParameterTypeRole] = "ParameterType";
  roles[ParameterDescriptionRole] = "ParameterDescription";
  roles[ParameterFlagsRole] = "ParameterFlags";
  roles[ParameterDefaultValueRole] = "ParameterDefaultValue";
  roles[ParameterValueRole] = "ParameterValue";
  roles[ParameterConfigurationRole] = "ParameterConfiguration";

  return roles;
}

int QfProcessingAlgorithmParametersModelBase::rowCount( const QModelIndex &parent ) const
{
  return !parent.isValid() ? static_cast<int>( mParameters.size() ) : 0;
}

QVariant QfProcessingAlgorithmParametersModelBase::data( const QModelIndex &index, int role ) const
{
  if ( index.row() >= mParameters.size() || index.row() < 0 || !mParameters.at( index.row() ) )
    return QVariant();

  switch ( role )
  {
    case ParameterTypeRole:
      return mParameters.at( index.row() )->type();
    case ParameterDescriptionRole:
      return mParameters.at( index.row() )->description();
    case ParameterFlagsRole:
      return static_cast<int>( mParameters.at( index.row() )->flags() );
    case ParameterDefaultValueRole:
      return mParameters.at( index.row() )->defaultValue();
    case ParameterValueRole:
      return mValues.at( index.row() );
    case ParameterConfigurationRole:
      QVariantMap configuration;
      if ( mParameters.at( index.row() )->type() == QStringLiteral( "area" ) )
      {
        const QgsProcessingParameterArea *parameterArea = dynamic_cast<const QgsProcessingParameterArea *>( mParameters.at( index.row() ) );
        configuration["minimum"] = parameterArea->minimum();
        configuration["maximum"] = parameterArea->maximum();
        configuration["areaUnit"] = static_cast<int>( ( mInPlaceLayer ? QgsUnitTypes::distanceToAreaUnit( mInPlaceLayer->crs().mapUnits() ) : Qgis::AreaUnit::Unknown ) );
      }
      else if ( mParameters.at( index.row() )->type() == QStringLiteral( "distance" ) )
      {
        const QgsProcessingParameterDistance *parameterDistance = dynamic_cast<const QgsProcessingParameterDistance *>( mParameters.at( index.row() ) );
        configuration["minimum"] = parameterDistance->minimum();
        configuration["maximum"] = parameterDistance->maximum();
        configuration["distanceUnit"] = static_cast<int>( ( mInPlaceLayer ? mInPlaceLayer->crs().mapUnits() : Qgis::DistanceUnit::Unknown ) );
      }
      else if ( mParameters.at( index.row() )->type() == QStringLiteral( "number" ) )
      {
        const QgsProcessingParameterNumber *parameterNumber = dynamic_cast<const QgsProcessingParameterNumber *>( mParameters.at( index.row() ) );
        configuration["minimum"] = parameterNumber->minimum();
        configuration["maximum"] = parameterNumber->maximum();
        configuration["dataType"] = static_cast<int>( parameterNumber->dataType() );
      }
      else if ( mParameters.at( index.row() )->type() == QStringLiteral( "enum" ) )
      {
        const QgsProcessingParameterEnum *parameterEnum = dynamic_cast<const QgsProcessingParameterEnum *>( mParameters.at( index.row() ) );
        configuration["options"] = parameterEnum->options();
      }
      else if ( mParameters.at( index.row() )->type() == QStringLiteral( "source" ) )
      {
        const QgsProcessingParameterFeatureSource *parameterFeatureSource = dynamic_cast<const QgsProcessingParameterFeatureSource *>( mParameters.at( index.row() ) );
        const QMap<QString, QgsMapLayer *> mapLayers = QgsProject::instance()->mapLayers();
        QVariantList supportedLayers;
        QVariantMap supportedLayer;
        supportedLayer["id"] = QString();
        supportedLayer["name"] = QString();
        supportedLayers << supportedLayer;
        for ( auto it = mapLayers.begin(); it != mapLayers.end(); ++it )
        {
          if ( QgsVectorLayer *vectorLayer = dynamic_cast<QgsVectorLayer *>( it.value() ) )
          {
            if ( parameterFeatureSource->dataTypes().isEmpty() || parameterFeatureSource->dataTypes().contains( static_cast<int>( vectorLayer->geometryType() ) ) )
            {
              supportedLayer["id"] = vectorLayer->id();
              supportedLayer["name"] = vectorLayer->name();
              supportedLayers << supportedLayer;
            }
          }
        }
        configuration["layers"] = supportedLayers;
      }
      return configuration;
  }

  return QVariant();
}

bool QfProcessingAlgorithmParametersModelBase::setData( const QModelIndex &index, const QVariant &value, int role )
{
  if ( index.row() >= mParameters.size() || index.row() < 0 || !mParameters.at( index.row() ) )
    return false;

  switch ( role )
  {
    case ParameterValueRole:
      if ( mValues[index.row()] != value )
      {
        mValues[index.row()] = value;
        emit dataChanged( index, index, QList<int>() << ParameterValueRole );
        emit parametersChanged();
      }
      return true;
  }

  return false;
}
```


