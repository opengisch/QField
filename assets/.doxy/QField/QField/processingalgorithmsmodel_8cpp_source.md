

# File processingalgorithmsmodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**processing**](dir_14e4815d6f2d5bd986fc95a1af67243d.md) **>** [**processingalgorithmsmodel.cpp**](processingalgorithmsmodel_8cpp.md)

[Go to the documentation of this file](processingalgorithmsmodel_8cpp.md)


```C++
/***************************************************************************
  processingalgorithmsmodel.h - ProcessingAlgorithmsModel

 ---------------------
 begin                : 18.06.2024
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


#include "processingalgorithmsmodel.h"

#include <qgsapplication.h>
#include <qgsnativealgorithms.h>
#include <qgsprocessingalgorithm.h>
#include <qgsprocessingprovider.h>
#include <qgsprocessingregistry.h>
#include <qgsvectorlayer.h>

#include <mutex>


ProcessingAlgorithmsModel::ProcessingAlgorithmsModel( QObject *parent )
  : QSortFilterProxyModel( parent )
  , mModel( new ProcessingAlgorithmsModelBase( parent ) )
{
  setSourceModel( mModel );
  setDynamicSortFilter( true );
  setSortLocaleAware( true );
  setFilterCaseSensitivity( Qt::CaseInsensitive );
  sort( 0 );
}

void ProcessingAlgorithmsModel::rebuild()
{
  mModel->rebuild();
}

void ProcessingAlgorithmsModel::setFilters( ProcessingAlgorithmsModel::Filters filters )
{
  if ( mFilters == filters )
  {
    return;
  }

  mFilters = filters;
  emit filtersChanged();

  invalidateFilter();
}

void ProcessingAlgorithmsModel::setInPlaceLayer( QgsVectorLayer *layer )
{
  if ( mInPlaceLayer.data() == layer )
  {
    return;
  }

  mInPlaceLayer = layer;
  emit inPlaceLayerChanged();

  invalidateFilter();
}

bool ProcessingAlgorithmsModel::lessThan( const QModelIndex &sourceLeft, const QModelIndex &sourceRight ) const
{
  QString left = mModel->data( sourceLeft, ProcessingAlgorithmsModelBase::AlgorithmGroupRole ).toString();
  QString right = mModel->data( sourceRight, ProcessingAlgorithmsModelBase::AlgorithmGroupRole ).toString();
  int compare = QString::localeAwareCompare( left, right );
  if ( compare != 0 )
  {
    return compare < 0;
  }

  left = mModel->data( sourceLeft, ProcessingAlgorithmsModelBase::AlgorithmNameRole ).toString();
  right = mModel->data( sourceRight, ProcessingAlgorithmsModelBase::AlgorithmNameRole ).toString();
  compare = QString::localeAwareCompare( left, right );
  return compare < 0;
}

bool ProcessingAlgorithmsModel::filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
{
  QModelIndex sourceIndex = mModel->index( sourceRow, 0, sourceParent );
  if ( mFilters & Filter::InPlaceFilter )
  {
    const bool supportsInPlace = mModel->data( sourceIndex, ProcessingAlgorithmsModelBase::AlgorithmFlagsRole ).toInt() & static_cast<int>( Qgis::ProcessingAlgorithmFlag::SupportsInPlaceEdits );
    if ( !supportsInPlace )
      return false;
  }
  if ( mFilters & Filter::FavoriteFilter )
  {
    if ( !mModel->data( sourceIndex, ProcessingAlgorithmsModelBase::AlgorithmFavoriteRole ).toBool() )
    {
      return false;
    }
  }
  if ( mInPlaceLayer )
  {
    const QgsProcessingAlgorithm *algorithm = mModel->algorithmForIndex( sourceIndex );
    if ( algorithm && !algorithm->supportInPlaceEdit( mInPlaceLayer.data() ) )
    {
      return false;
    }
  }

  return true;
}


ProcessingAlgorithmsModelBase::ProcessingAlgorithmsModelBase( QObject *parent )
  : QAbstractListModel( parent )
{
  static std::once_flag initialized;
  std::call_once( initialized, [] { QgsApplication::processingRegistry()->addProvider( new QgsNativeAlgorithms( QgsApplication::processingRegistry() ) ); } );

  QSettings settings;
  if ( !settings.value( QStringLiteral( "processing/favoritesInitialized" ), false ).toBool() )
  {
    settings.setValue( QStringLiteral( "processing/favorites/%1" ).arg( QStringLiteral( "native:orthogonalize" ) ), true );
    settings.setValue( QStringLiteral( "processing/favorites/%1" ).arg( QStringLiteral( "native:rotatefeatures" ) ), true );
    settings.setValue( QStringLiteral( "processing/favorites/%1" ).arg( QStringLiteral( "native:smoothgeometry" ) ), true );
    settings.setValue( QStringLiteral( "processing/favoritesInitialized" ), true );
  }

  rebuild();
}

void ProcessingAlgorithmsModelBase::rebuild()
{
  beginResetModel();
  mAlgorithms.clear();

  const QList<QgsProcessingProvider *> providers = QgsApplication::processingRegistry()->providers();
  for ( QgsProcessingProvider *provider : providers )
  {
    addProvider( provider );
  }

  endResetModel();
}

void ProcessingAlgorithmsModelBase::addProvider( QgsProcessingProvider *provider )
{
  if ( !provider )
  {
    return;
  }

  connect( provider, &QgsProcessingProvider::algorithmsLoaded, this, &ProcessingAlgorithmsModelBase::rebuild, Qt::UniqueConnection );

  const QList<const QgsProcessingAlgorithm *> algorithms = provider->algorithms();
  for ( const QgsProcessingAlgorithm *algorithm : algorithms )
  {
    const static QStringList sSupportedParameters = { QStringLiteral( "number" ), QStringLiteral( "area" ), QStringLiteral( "distance" ), QStringLiteral( "enum" ), QStringLiteral( "boolean" ), QStringLiteral( "sink" ), QStringLiteral( "source" ) };
    const QgsProcessingFeatureBasedAlgorithm *featureBasedAlgorithm = dynamic_cast<const QgsProcessingFeatureBasedAlgorithm *>( algorithm );
    if ( algorithm->flags() & Qgis::ProcessingAlgorithmFlag::SupportsInPlaceEdits )
    {
      bool isSupported = true;
      for ( const QgsProcessingParameterDefinition *parameter : algorithm->parameterDefinitions() )
      {
        if ( !( parameter->flags() & Qgis::ProcessingParameterFlag::Optional ) && !sSupportedParameters.contains( parameter->type() ) )
        {
          isSupported = false;
          break;
        }

        if ( parameter->type() == QStringLiteral( "source" ) && featureBasedAlgorithm && parameter->name() != featureBasedAlgorithm->inputParameterName() )
        {
          isSupported = false;
          break;
        }
      }
      if ( !isSupported )
      {
        continue;
      }

      mAlgorithms << AlgorithmItem( algorithm );
    }
  }
}

const QgsProcessingAlgorithm *ProcessingAlgorithmsModelBase::algorithmForIndex( const QModelIndex &index ) const
{
  if ( index.row() >= mAlgorithms.size() || index.row() < 0 )
  {
    return nullptr;
  }

  return mAlgorithms.value( index.row() ).algorithm();
}

QHash<int, QByteArray> ProcessingAlgorithmsModelBase::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
  roles[AlgorithmIdRole] = "AlgorithmId";
  roles[AlgorithmGroupRole] = "AlgorithmGroup";
  roles[AlgorithmNameRole] = "AlgorithmName";
  roles[AlgorithmSvgIconRole] = "AlgorithmSvgIcon";
  roles[AlgorithmFlagsRole] = "AlgorithmFlags";
  roles[AlgorithmFavoriteRole] = "AlgorithmFavorite";

  return roles;
}

int ProcessingAlgorithmsModelBase::rowCount( const QModelIndex &parent ) const
{
  return !parent.isValid() ? static_cast<int>( mAlgorithms.size() ) : 0;
}

QVariant ProcessingAlgorithmsModelBase::data( const QModelIndex &index, int role ) const
{
  if ( index.row() >= mAlgorithms.size() || index.row() < 0 || !mAlgorithms.at( index.row() ).algorithm() )
    return QVariant();

  switch ( role )
  {
    case AlgorithmIdRole:
      return mAlgorithms.at( index.row() ).algorithm()->id();
    case AlgorithmGroupRole:
      return mAlgorithms.at( index.row() ).algorithm()->group();
    case AlgorithmNameRole:
      return mAlgorithms.at( index.row() ).algorithm()->displayName();
    case AlgorithmSvgIconRole:
      return mAlgorithms.at( index.row() ).algorithm()->svgIconPath();
    case AlgorithmFlagsRole:
      return static_cast<int>( mAlgorithms.at( index.row() ).algorithm()->flags() );
    case AlgorithmFavoriteRole:
      QSettings settings;
      return settings.value( QStringLiteral( "processing/favorites/%1" ).arg( mAlgorithms.at( index.row() ).algorithm()->id() ), false ).toBool();
  }

  return QVariant();
}

bool ProcessingAlgorithmsModelBase::setData( const QModelIndex &index, const QVariant &value, int role )
{
  if ( index.row() >= mAlgorithms.size() || index.row() < 0 || !mAlgorithms.at( index.row() ).algorithm() )
    return false;

  switch ( role )
  {
    case AlgorithmFavoriteRole:
      QSettings settings;
      settings.setValue( QStringLiteral( "processing/favorites/%1" ).arg( mAlgorithms.at( index.row() ).algorithm()->id() ), value.toBool() );
      emit dataChanged( index, index, QList<int>() << role );
      return true;
  }

  return false;
}
```


