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

ProcessingAlgorithmsProxyModel::ProcessingAlgorithmsProxyModel( QObject *parent )
  : QSortFilterProxyModel( parent )
  , mModel( new ProcessingAlgorithmsModel( parent ) )
{
  setSourceModel( mModel );
  setDynamicSortFilter( true );
  setSortLocaleAware( true );
  setFilterCaseSensitivity( Qt::CaseInsensitive );
  sort( 0 );
}

void ProcessingAlgorithmsProxyModel::rebuild()
{
  mModel->rebuild();
}

void ProcessingAlgorithmsProxyModel::setFilters( ProcessingAlgorithmsProxyModel::Filters filters )
{
  if ( mFilters == filters )
  {
    return;
  }

  mFilters = filters;
  emit filtersChanged();

  invalidateFilter();
}

void ProcessingAlgorithmsProxyModel::setInPlaceLayer( QgsVectorLayer *layer )
{
  if ( mInPlaceLayer.data() == layer )
  {
    return;
  }

  mInPlaceLayer = layer;
  emit inPlaceLayerChanged();

  invalidateFilter();
}

bool ProcessingAlgorithmsProxyModel::lessThan( const QModelIndex &sourceLeft, const QModelIndex &sourceRight ) const
{
  QString left = mModel->data( sourceLeft, ProcessingAlgorithmsModel::AlgorithmGroupRole ).toString();
  QString right = mModel->data( sourceLeft, ProcessingAlgorithmsModel::AlgorithmGroupRole ).toString();
  int compare = QString::localeAwareCompare( left, right );
  if ( compare != 0 )
  {
    return compare < 0;
  }

  left = mModel->data( sourceLeft, ProcessingAlgorithmsModel::AlgorithmNameRole ).toString();
  right = mModel->data( sourceLeft, ProcessingAlgorithmsModel::AlgorithmNameRole ).toString();
  compare = QString::localeAwareCompare( left, right );
  return compare < 0;
}

bool ProcessingAlgorithmsProxyModel::filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
{
  QModelIndex sourceIndex = mModel->index( sourceRow, 0, sourceParent );
  if ( mFilters & Filter::InPlaceFilter )
  {
    const bool supportsInPlace = mModel->data( sourceIndex, ProcessingAlgorithmsModel::AlgorithmFlagsRole ).toInt() & static_cast<int>( Qgis::ProcessingAlgorithmFlag::SupportsInPlaceEdits );
    if ( !supportsInPlace )
      return false;
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

ProcessingAlgorithmsModel::ProcessingAlgorithmsModel( QObject *parent )
  : QAbstractListModel( parent )
{
  static std::once_flag initialized;
  std::call_once( initialized, [] { QgsApplication::processingRegistry()->addProvider( new QgsNativeAlgorithms( QgsApplication::processingRegistry() ) ); } );
  rebuild();
}

void ProcessingAlgorithmsModel::rebuild()
{
  beginResetModel();
  mAlgorithms.clear();

  const QList<QgsProcessingProvider *> providers = QgsApplication::processingRegistry()->providers();
  for ( QgsProcessingProvider *provider : providers )
  {
    qDebug() << provider->id();
    addProvider( provider );
  }

  endResetModel();
}

void ProcessingAlgorithmsModel::addProvider( QgsProcessingProvider *provider )
{
  if ( !provider )
  {
    return;
  }

  connect( provider, &QgsProcessingProvider::algorithmsLoaded, this, &ProcessingAlgorithmsModel::rebuild, Qt::UniqueConnection );

  const QList<const QgsProcessingAlgorithm *> algorithms = provider->algorithms();
  for ( const QgsProcessingAlgorithm *algorithm : algorithms )
  {
    const static QStringList sSupportedAlgorithms = { QStringLiteral( "native:orthogonalize" ), QStringLiteral( "native:rotatefeatures" ) };
    if ( !sSupportedAlgorithms.contains( algorithm->id() ) )
    {
      continue;
    }

    mAlgorithms << AlgorithmItem( algorithm );
  }
}

const QgsProcessingAlgorithm *ProcessingAlgorithmsModel::algorithmForIndex( const QModelIndex &index ) const
{
  if ( index.row() >= mAlgorithms.size() || index.row() < 0 )
  {
    return nullptr;
  }

  return mAlgorithms.value( index.row() ).algorithm();
}

QHash<int, QByteArray> ProcessingAlgorithmsModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
  roles[AlgorithmIdRole] = "AlgorithmId";
  roles[AlgorithmGroupRole] = "AlgorithmGroup";
  roles[AlgorithmNameRole] = "AlgorithmName";
  roles[AlgorithmSvgIconRole] = "AlgorithmSvgIcon";
  roles[AlgorithmFlagsRole] = "AlgorithmFlags";

  return roles;
}

int ProcessingAlgorithmsModel::rowCount( const QModelIndex &parent ) const
{
  if ( !parent.isValid() )
    return mAlgorithms.size();
  else
    return 0;
}

QVariant ProcessingAlgorithmsModel::data( const QModelIndex &index, int role ) const
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
  }

  return QVariant();
}
