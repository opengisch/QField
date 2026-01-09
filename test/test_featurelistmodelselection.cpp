/***************************************************************************
                        test_featurelistmodelselection
                        --------------------
  begin                : January 2026
  copyright            : (C) 2026 by Kaustuv Pokharel
  email                : kaustuv@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "catch2.h"
#include "featurelistmodelselection.h"
#include "multifeaturelistmodel.h"

#include <QCoreApplication>
#include <QElapsedTimer>
#include <QEventLoop>
#include <QSignalSpy>
#include <qgsgeometry.h>
#include <qgsvectorlayer.h>

#include <memory>

TEST_CASE( "FeatureListModelSelection default state is safe" )
{
  FeatureListModelSelection selection;

  REQUIRE( selection.model() == nullptr );
  REQUIRE( selection.focusedItem() == -1 );
  REQUIRE( selection.focusedLayer() == nullptr );

  const QgsFeature f = selection.focusedFeature();
  REQUIRE_FALSE( f.isValid() );
  REQUIRE( selection.focusedGeometry().isEmpty() );

  selection.clear();
  REQUIRE( selection.focusedItem() == -1 );
}

TEST_CASE( "FeatureListModelSelection behaviour" )
{
  if ( !QCoreApplication::instance() )
  {
    static int argc = 1;
    static char argv0[] = "qfield-tests";
    static char *argv[] = { argv0, nullptr };
    static QCoreApplication app( argc, argv );
    Q_UNUSED( app );
  }

  static const int maximumWaitTimeMilliseconds = 2000;

  std::unique_ptr<QgsVectorLayer> layer = std::make_unique<QgsVectorLayer>(
    QStringLiteral( "Point?crs=epsg:4326&field=name:string" ),
    QStringLiteral( "test" ),
    QStringLiteral( "memory" ) );

  REQUIRE( layer->isValid() );
  layer->setDisplayExpression( QStringLiteral( "\"name\"" ) );

  const int nameFieldIndex = layer->fields().indexFromName( QStringLiteral( "name" ) );
  REQUIRE( nameFieldIndex >= 0 );

  layer->startEditing();

  QgsFeature featureA( layer->fields() );
  featureA.setAttribute( QStringLiteral( "name" ), QStringLiteral( "A" ) );
  featureA.setGeometry( QgsGeometry::fromPointXY( QgsPointXY( 0, 0 ) ) );
  REQUIRE( layer->addFeature( featureA ) );

  QgsFeature featureB( layer->fields() );
  featureB.setAttribute( QStringLiteral( "name" ), QStringLiteral( "B" ) );
  featureB.setGeometry( QgsGeometry::fromPointXY( QgsPointXY( 1, 1 ) ) );
  REQUIRE( layer->addFeature( featureB ) );

  layer->commitChanges();

  MultiFeatureListModel model;
  QMap<QgsVectorLayer *, QgsFeatureRequest> requests;
  requests.insert( layer.get(), QgsFeatureRequest() );
  model.setFeatures( requests );

  // Wait for model population
  {
    QElapsedTimer timer;
    timer.start();
    while ( model.rowCount() != 2 && timer.elapsed() < maximumWaitTimeMilliseconds )
    {
      QCoreApplication::processEvents( QEventLoop::AllEvents, 50 );
    }
    REQUIRE( model.rowCount() == 2 );
  }

  // Find row for "A"
  int rowA = -1;
  for ( int row = 0; row < model.rowCount(); ++row )
  {
    const QModelIndex idx = model.index( row, 0 );
    if ( model.data( idx, Qt::DisplayRole ).toString() == QStringLiteral( "A" ) )
    {
      rowA = row;
      break;
    }
  }
  REQUIRE( rowA >= 0 );

  const QModelIndex idxA = model.index( rowA, 0 );
  const QgsFeatureId fidA = model.data( idxA, MultiFeatureListModel::FeatureIdRole ).value<QgsFeatureId>();
  REQUIRE( fidA >= 0 );

  FeatureListModelSelection selection;

  // check setModel resets
  selection.setFocusedItem( 0 );
  REQUIRE( selection.focusedItem() == 0 );
  selection.setModel( &model );
  REQUIRE( selection.focusedItem() == -1 );

  // Focused properties expose data from the underlying model
  selection.setFocusedItem( rowA );
  REQUIRE( selection.focusedLayer() == layer.get() );

  {
    const QgsFeature focusedFeature = selection.focusedFeature();
    REQUIRE( focusedFeature.isValid() );
    REQUIRE( focusedFeature.attribute( QStringLiteral( "name" ) ).toString() == QStringLiteral( "A" ) );
  }

  // toggleSelectedItem forwards to the model and emits signal
  {
    QSignalSpy selectedSpy( &selection, &FeatureListModelSelection::selectedFeaturesChanged );

    selection.toggleSelectedItem( rowA );

    QElapsedTimer timer;
    timer.start();
    while ( model.selectedCount() != 1 && timer.elapsed() < maximumWaitTimeMilliseconds )
    {
      QCoreApplication::processEvents( QEventLoop::AllEvents, 50 );
    }

    REQUIRE( model.selectedCount() == 1 );
    REQUIRE( selectedSpy.count() == 1 );

    model.clearSelection();
  }

  // Feature edit updates list display name, and focusedFeature reflects it, no caching in selection
  {
    layer->startEditing();
    REQUIRE( layer->changeAttributeValue( fidA, nameFieldIndex, QVariant( QStringLiteral( "A updated" ) ) ) );
    layer->commitChanges();

    int rowUpdated = -1;

    QElapsedTimer timer;
    timer.start();
    while ( rowUpdated < 0 && timer.elapsed() < maximumWaitTimeMilliseconds )
    {
      for ( int row = 0; row < model.rowCount(); ++row )
      {
        const QModelIndex idx = model.index( row, 0 );
        if ( model.data( idx, Qt::DisplayRole ).toString() == QStringLiteral( "A updated" ) )
        {
          rowUpdated = row;
          break;
        }
      }

      if ( rowUpdated < 0 )
        QCoreApplication::processEvents( QEventLoop::AllEvents, 50 );
    }

    REQUIRE( rowUpdated >= 0 );

    selection.setFocusedItem( rowUpdated );

    const QgsFeature focusedFeature = selection.focusedFeature();
    REQUIRE( focusedFeature.isValid() );
    REQUIRE( focusedFeature.attribute( QStringLiteral( "name" ) ).toString() == QStringLiteral( "A updated" ) );
  }

  // Out of range focus is safe so invalid model index -> default or empty results
  {
    selection.setFocusedItem( 999 );

    REQUIRE( selection.focusedLayer() == nullptr );

    const QgsFeature f = selection.focusedFeature();
    REQUIRE_FALSE( f.isValid() );
    REQUIRE( selection.focusedGeometry().isEmpty() );
  }

  selection.clear();
  REQUIRE( selection.focusedItem() == -1 );
}
