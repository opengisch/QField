/***************************************************************************
              featurelistmodelhighlight.cpp
               ----------------------------------------------------
              date                 : 9.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias.kuhn (at) opengis.ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "featurelistmodelhighlight.h"

FeatureListModelHighlight::FeatureListModelHighlight( QObject *parent )
  : QObject( parent )
  , mMapCanvas( 0 )
  , mModel( 0 )
  , mSelection( 0 )
{
  connect( this, SIGNAL( mapCanvasChanged() ), this, SLOT( onDataChanged() ) );
  connect( this, SIGNAL( modelChanged() ), this, SLOT( onDataChanged() ) );
  connect( this, SIGNAL( selectionChanged() ), this, SLOT( onDataChanged() ) );
}

void FeatureListModelHighlight::setSelection( FeatureListModelSelection* selection )
{
  if ( selection != mSelection )
  {
    disconnect( this, SLOT( onSelectionChanged() ) );
    mSelection = selection;
    connect( selection, SIGNAL( selectionChanged() ), this, SLOT( onSelectionChanged() ) );
    emit selectionChanged();
  }
}

FeatureListModelSelection* FeatureListModelHighlight::selection() const
{
  return mSelection;
}

void FeatureListModelHighlight::onDataChanged()
{
  if ( mModel )
  {
    connect( mModel, SIGNAL( modelReset() ), this, SLOT( onModelDataChanged() ) );
  }
}

void FeatureListModelHighlight::onModelDataChanged()
{
  qDeleteAll( mHighlights );
  mHighlights.clear();

  if ( mMapCanvas )
  {
    int count = mModel->rowCount( QModelIndex() );

    for ( int i = 0; i < count; ++i )
    {
      QModelIndex index = mModel->index( i, 0, QModelIndex() );
      const Feature& feature = mModel->data( index, FeatureListModel::FeatureRole ).value<Feature>();
      QgsHighlight* h = new QgsHighlight( mMapCanvas->mapCanvas(), feature.qgsFeature(), feature.layer() );
      if ( mSelection && mSelection->selection() == i )
      {
        h->setColor( mSelectionColor );
      }
      else
      {
        h->setColor( mColor );
      }
      h->show();
      mHighlights.append( h );
    }
  }
}

void FeatureListModelHighlight::onSelectionChanged()
{
  int i = 0;

  Q_FOREACH( QgsHighlight* h, mHighlights )
  {
    if ( i == mSelection->selection() )
    {
      h->setColor( mSelectionColor );
      h->update();
    }
    else
    {
      h->setColor( mColor );
      h->update();
    }
    ++i;
  }
}
