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

#include <mutex>

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

QHash<int, QByteArray> ProcessingAlgorithmsModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
  roles[AlgorithmGroupRole] = "AlgorithmGroup";
  roles[AlgorithmNameRole] = "AlgorithmName";
  roles[AlgorithmIconRole] = "AlgorithmIcon";

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
    case AlgorithmGroupRole:
      return mAlgorithms.at( index.row() ).algorithm()->group();
    case AlgorithmNameRole:
      return mAlgorithms.at( index.row() ).algorithm()->displayName();
    case AlgorithmIconRole:
      return mAlgorithms.at( index.row() ).algorithm()->icon();
  }

  return QVariant();
}
