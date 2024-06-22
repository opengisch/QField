/***************************************************************************
  processingalgorithmparametersmodel.h - ProcessingAlgorithmParametersModel

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


#include "processingalgorithmparametersmodel.h"

#include <qgsapplication.h>
#include <qgsprocessingalgorithm.h>
#include <qgsprocessingparameters.h>
#include <qgsprocessingregistry.h>


ProcessingAlgorithmParametersModel::ProcessingAlgorithmParametersModel( QObject *parent )
  : QAbstractListModel( parent )
{
}

void ProcessingAlgorithmParametersModel::rebuild()
{
  beginResetModel();
  mParameters.clear();
  mValues.clear();

  if ( mAlgorithm )
  {
    const QgsProcessingAlgorithm *algorithm = QgsApplication::instance()->processingRegistry()->algorithmById( mAlgorithmId );
    for ( const QgsProcessingParameterDefinition *definition : algorithm->parameterDefinitions() )
    {
      mParameters << definition;
      mValues << definition->defaultValue();
    }
  }

  endResetModel();
}

void ProcessingAlgorithmParametersModel::setAlgorithmId( const QString &id )
{
  if ( mAlgorithmId == id )
  {
    return;
  }

  mAlgorithmId = id;
  mAlgorithm = mAlgorithmId.isEmpty() ? QgsApplication::instance()->processingRegistry()->algorithmById( mAlgorithmId ) : nullptr;
  emit algorithmIdChanged();

  rebuild();
}

QString ProcessingAlgorithmParametersModel::algorithmDisplayName() const
{
  return mAlgorithm ? mAlgorithm->displayName() : QString();
}

QString ProcessingAlgorithmParametersModel::algorithmShortDescription() const
{
  return mAlgorithm ? mAlgorithm->shortDescription() : QString();
}

QHash<int, QByteArray> ProcessingAlgorithmParametersModel::roleNames() const
{
  QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
  roles[ParameterTypeRole] = "ParameterType";
  roles[ParameterDescriptionRole] = "ParameterDescription";
  roles[ParameterDefaultValueRole] = "ParameterDefaultValue";
  roles[ParameterValueRole] = "ParameterValue";

  return roles;
}

int ProcessingAlgorithmParametersModel::rowCount( const QModelIndex &parent ) const
{
  if ( !parent.isValid() )
    return mParameters.size();
  else
    return 0;
}

QVariant ProcessingAlgorithmParametersModel::data( const QModelIndex &index, int role ) const
{
  if ( index.row() >= mParameters.size() || index.row() < 0 || !mParameters.at( index.row() ) )
    return QVariant();

  switch ( role )
  {
    case ParameterTypeRole:
      return mParameters.at( index.row() )->type();
    case ParameterDescriptionRole:
      return mParameters.at( index.row() )->description();
    case ParameterDefaultValueRole:
      return mParameters.at( index.row() )->defaultValue();
    case ParameterValueRole:
      return mValues.at( index.row() );
  }

  return QVariant();
}

bool ProcessingAlgorithmParametersModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
  if ( index.row() >= mParameters.size() || index.row() < 0 || !mParameters.at( index.row() ) )
    return false;

  switch ( role )
  {
    case ParameterValueRole:
      mValues[index.row()] = value;
      return true;
  }

  return false;
}
