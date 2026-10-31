

# File qfattributeformmodel.cpp

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qfattributeformmodel.cpp**](qfattributeformmodel_8cpp.md)

[Go to the documentation of this file](qfattributeformmodel_8cpp.md)


```C++
/***************************************************************************
  qfattributeformmodel.cpp - attributeformmodel

 ---------------------
 begin                : 22.9.2016
 copyright            : (C) 2016 by Matthias Kuhn
 email                : matthias@opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qfattributeformmodel.h"
#include "qfattributeformmodelbase.h"

QfAttributeFormModel::QfAttributeFormModel( QObject *parent )
  : QSortFilterProxyModel( parent )
  , mSourceModel( new QfAttributeFormModelBase( this ) )
{
  setSourceModel( mSourceModel );

  connect( mSourceModel, &QfAttributeFormModelBase::isWizardChanged, this, &QfAttributeFormModel::isWizardChanged );
  connect( mSourceModel, &QfAttributeFormModelBase::hasTabsChanged, this, &QfAttributeFormModel::hasTabsChanged );
  connect( mSourceModel, &QfAttributeFormModelBase::hasRemembranceChanged, this, &QfAttributeFormModel::hasRemembranceChanged );
  connect( mSourceModel, &QfAttributeFormModelBase::hasConstraintsChanged, this, &QfAttributeFormModel::hasConstraintsChanged );
  connect( mSourceModel, &QfAttributeFormModelBase::featureModelChanged, this, &QfAttributeFormModel::featureModelChanged );
  connect( mSourceModel, &QfAttributeFormModelBase::constraintsHardValidChanged, this, &QfAttributeFormModel::constraintsHardValidChanged );
  connect( mSourceModel, &QfAttributeFormModelBase::constraintsSoftValidChanged, this, &QfAttributeFormModel::constraintsSoftValidChanged );
}

bool QfAttributeFormModel::isWizard() const
{
  return mSourceModel->isWizard();
}

bool QfAttributeFormModel::hasTabs() const
{
  return mSourceModel->hasTabs();
}

bool QfAttributeFormModel::hasRemembrance() const
{
  return mSourceModel->hasRemembrance();
}

bool QfAttributeFormModel::hasConstraints() const
{
  return mSourceModel->hasConstraints();
}

QfFeatureModel *QfAttributeFormModel::featureModel() const
{
  return mSourceModel->featureModel();
}

void QfAttributeFormModel::setFeatureModel( QfFeatureModel *featureModel )
{
  mSourceModel->setFeatureModel( featureModel );
}

bool QfAttributeFormModel::constraintsHardValid() const
{
  return mSourceModel->constraintsHardValid();
}

bool QfAttributeFormModel::constraintsSoftValid() const
{
  return mSourceModel->constraintsSoftValid();
}

bool QfAttributeFormModel::save()
{
  return mSourceModel->save();
}

bool QfAttributeFormModel::create()
{
  return mSourceModel->create();
}

bool QfAttributeFormModel::deleteFeature()
{
  return mSourceModel->deleteFeature();
}

QVariant QfAttributeFormModel::attribute( const QString &name )
{
  return mSourceModel->attribute( name );
}

bool QfAttributeFormModel::changeAttribute( const QString &name, const QVariant &value )
{
  return mSourceModel->changeAttribute( name, value );
}

bool QfAttributeFormModel::changeGeometry( const QgsGeometry &geometry )
{
  return mSourceModel->changeGeometry( geometry );
}

void QfAttributeFormModel::applyFeatureModel()
{
  return mSourceModel->applyFeatureModel();
}

void QfAttributeFormModel::applyParentDefaultValues()
{
  return mSourceModel->applyParentDefaultValues();
}

void QfAttributeFormModel::applyRelationshipDefaultValues()
{
  return mSourceModel->applyRelationshipDefaultValues();
}

void QfAttributeFormModel::activateAllRememberValues()
{
  return mSourceModel->activateAllRememberValues();
}

void QfAttributeFormModel::deactivateAllRememberValues()
{
  return mSourceModel->deactivateAllRememberValues();
}

QgsExpressionContext QfAttributeFormModel::createExpressionContext() const
{
  return mSourceModel->createExpressionContext();
}

bool QfAttributeFormModel::filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
{
  return mSourceModel->data( mSourceModel->index( sourceRow, 0, sourceParent ), CurrentlyVisible ).toBool();
}
```


