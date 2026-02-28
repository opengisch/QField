

# File attributeformmodel.cpp

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**attributeformmodel.cpp**](attributeformmodel_8cpp.md)

[Go to the documentation of this file](attributeformmodel_8cpp.md)


```C++
/***************************************************************************
  attributeformmodel.cpp - attributeformmodel

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

#include "attributeformmodel.h"
#include "attributeformmodelbase.h"

AttributeFormModel::AttributeFormModel( QObject *parent )
  : QSortFilterProxyModel( parent )
  , mSourceModel( new AttributeFormModelBase( this ) )
{
  setSourceModel( mSourceModel );

  connect( mSourceModel, &AttributeFormModelBase::hasTabsChanged, this, &AttributeFormModel::hasTabsChanged );
  connect( mSourceModel, &AttributeFormModelBase::hasRemembranceChanged, this, &AttributeFormModel::hasRemembranceChanged );
  connect( mSourceModel, &AttributeFormModelBase::hasConstraintsChanged, this, &AttributeFormModel::hasConstraintsChanged );
  connect( mSourceModel, &AttributeFormModelBase::featureModelChanged, this, &AttributeFormModel::featureModelChanged );
  connect( mSourceModel, &AttributeFormModelBase::featureChanged, this, &AttributeFormModel::featureChanged );
  connect( mSourceModel, &AttributeFormModelBase::constraintsHardValidChanged, this, &AttributeFormModel::constraintsHardValidChanged );
  connect( mSourceModel, &AttributeFormModelBase::constraintsSoftValidChanged, this, &AttributeFormModel::constraintsSoftValidChanged );
}

bool AttributeFormModel::hasTabs() const
{
  return mSourceModel->hasTabs();
}

bool AttributeFormModel::hasRemembrance() const
{
  return mSourceModel->hasRemembrance();
}

bool AttributeFormModel::hasConstraints() const
{
  return mSourceModel->hasConstraints();
}

FeatureModel *AttributeFormModel::featureModel() const
{
  return mSourceModel->featureModel();
}

void AttributeFormModel::setFeatureModel( FeatureModel *featureModel )
{
  mSourceModel->setFeatureModel( featureModel );
}

bool AttributeFormModel::constraintsHardValid() const
{
  return mSourceModel->constraintsHardValid();
}

bool AttributeFormModel::constraintsSoftValid() const
{
  return mSourceModel->constraintsSoftValid();
}

bool AttributeFormModel::save()
{
  return mSourceModel->save();
}

bool AttributeFormModel::create()
{
  return mSourceModel->create();
}

bool AttributeFormModel::deleteFeature()
{
  return mSourceModel->deleteFeature();
}

QVariant AttributeFormModel::attribute( const QString &name )
{
  return mSourceModel->attribute( name );
}

bool AttributeFormModel::changeAttribute( const QString &name, const QVariant &value )
{
  return mSourceModel->changeAttribute( name, value );
}

bool AttributeFormModel::changeGeometry( const QgsGeometry &geometry )
{
  return mSourceModel->changeGeometry( geometry );
}

void AttributeFormModel::applyFeatureModel()
{
  return mSourceModel->applyFeatureModel();
}

void AttributeFormModel::applyParentDefaultValues()
{
  return mSourceModel->applyParentDefaultValues();
}

void AttributeFormModel::applyRelationshipDefaultValues()
{
  return mSourceModel->applyRelationshipDefaultValues();
}

void AttributeFormModel::activateAllRememberValues()
{
  return mSourceModel->activateAllRememberValues();
}

void AttributeFormModel::deactivateAllRememberValues()
{
  return mSourceModel->deactivateAllRememberValues();
}

bool AttributeFormModel::filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
{
  return mSourceModel->data( mSourceModel->index( sourceRow, 0, sourceParent ), CurrentlyVisible ).toBool();
}
```


