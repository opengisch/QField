/***************************************************************************
                        test_orderedrelationmodel.cpp
                        --------------------
  begin                : June 2021
  copyright            : (C) 2021 by Ivan Ivanov
  email                : ivan@opengis.ch
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#define QFIELDTEST_MAIN
#include "catch2.h"
#include "orderedrelationmodel.h"

#include <QSignalSpy>
#include <qgsapplication.h>
#include <qgsproject.h>
#include <qgsrelationmanager.h>
#include <qgsvectorlayer.h>

TEST_CASE( "OrderedRelationModel" )
{
  //create layers
  std::unique_ptr<QgsVectorLayer> mReferencingLayer( new QgsVectorLayer( QStringLiteral( "NoGeometry?field=id:integer&field=foreignkey:integer&field=rank:integer" ), QStringLiteral( "king" ), QStringLiteral( "memory" ) ) );
  mReferencingLayer->setDisplayExpression( "name" );
  REQUIRE( mReferencingLayer->isValid() );
  QgsProject::instance()->addMapLayer( mReferencingLayer.get(), false, false );

  std::unique_ptr<QgsVectorLayer> mReferencedLayer( new QgsVectorLayer( QStringLiteral( "NoGeometry?field=id:integer" ), QStringLiteral( "land" ), QStringLiteral( "memory" ) ) );
  mReferencedLayer->setDisplayExpression( "name" );
  REQUIRE( mReferencedLayer->isValid() );
  QgsProject::instance()->addMapLayer( mReferencedLayer.get(), false, false );

  //create relations
  QgsRelation mRelation;
  mRelation.setId( QStringLiteral( "referenced.referencing" ) );
  mRelation.setName( QStringLiteral( "referenced.referencing" ) );
  mRelation.setReferencingLayer( mReferencingLayer->id() );
  mRelation.setReferencedLayer( mReferencedLayer->id() );
  mRelation.addFieldPair( QStringLiteral( "foreignkey" ), QStringLiteral( "id" ) );
  REQUIRE( mRelation.isValid() );
  QgsProject::instance()->relationManager()->addRelation( mRelation );

  // add features to the referencing layer
  QgsFeature referencing_f1( mReferencingLayer->fields() );
  referencing_f1.setAttribute( QStringLiteral( "id" ), 1 );
  referencing_f1.setAttribute( QStringLiteral( "foreignkey" ), 1 );
  referencing_f1.setAttribute( QStringLiteral( "rank" ), 1 );
  QgsFeature referencing_f2( mReferencingLayer->fields() );
  referencing_f2.setAttribute( QStringLiteral( "id" ), 2 );
  referencing_f2.setAttribute( QStringLiteral( "foreignkey" ), 1 );
  referencing_f2.setAttribute( QStringLiteral( "rank" ), 2 );
  QgsFeature referencing_f3( mReferencingLayer->fields() );
  referencing_f3.setAttribute( QStringLiteral( "id" ), 3 );
  referencing_f3.setAttribute( QStringLiteral( "foreignkey" ), 1 );
  referencing_f3.setAttribute( QStringLiteral( "rank" ), 3 );
  QgsFeature referencing_f4( mReferencingLayer->fields() );
  referencing_f4.setAttribute( QStringLiteral( "id" ), 4 );
  referencing_f4.setAttribute( QStringLiteral( "foreignkey" ), 1 );
  referencing_f4.setAttribute( QStringLiteral( "rank" ), 4 );
  mReferencingLayer->startEditing();
  mReferencingLayer->addFeature( referencing_f1 );
  mReferencingLayer->addFeature( referencing_f2 );
  mReferencingLayer->addFeature( referencing_f3 );
  mReferencingLayer->addFeature( referencing_f4 );
  mReferencingLayer->commitChanges();
  REQUIRE( mReferencingLayer->featureCount() == 4L );

  // add features to the referenced layer
  QgsFeature referenced_ft1( mReferencedLayer->fields() );
  referenced_ft1.setAttribute( QStringLiteral( "id" ), 1 );

  mReferencedLayer->startEditing();
  mReferencedLayer->addFeature( referenced_ft1 );
  mReferencedLayer->commitChanges();
  REQUIRE( mReferencedLayer->featureCount() == 1L );

  std::unique_ptr<OrderedRelationModel> mModel( new OrderedRelationModel() );

  SECTION( "DeleteFeature" )
  {
    REQUIRE( mModel->rowCount() == 0 );

    mModel->setRelation( mRelation );
    mModel->setOrderingField( QStringLiteral( "rank" ) );
    mModel->setFeature( mReferencedLayer->getFeature( 1 ) );

    REQUIRE( QSignalSpy( mModel.get(), &ReferencingFeatureListModel::modelUpdated ).wait( 1000 ) );

    REQUIRE( mModel->rowCount() == 4 );
    REQUIRE( mModel->deleteFeature( 2 ) );
    REQUIRE( QSignalSpy( mModel.get(), &ReferencingFeatureListModel::modelUpdated ).wait( 1000 ) );
    REQUIRE( mModel->rowCount() == 3 );
    REQUIRE( !mReferencingLayer->getFeature( 2 ).isValid() );
    REQUIRE( mReferencingLayer->getFeature( 1 ).attribute( QStringLiteral( "rank" ) ).toInt() == 1 );
    REQUIRE( mReferencingLayer->getFeature( 3 ).attribute( QStringLiteral( "rank" ) ).toInt() == 2 );
    REQUIRE( mReferencingLayer->getFeature( 4 ).attribute( QStringLiteral( "rank" ) ).toInt() == 3 );
  }

  SECTION( "MoveFeature" )
  {
    REQUIRE( mModel->rowCount() == 0 );

    mModel->setRelation( mRelation );
    mModel->setOrderingField( QStringLiteral( "rank" ) );
    mModel->setFeature( mReferencedLayer->getFeature( 1 ) );

    REQUIRE( QSignalSpy( mModel.get(), &ReferencingFeatureListModel::modelUpdated ).wait( 1000 ) );

    REQUIRE( mModel->rowCount() == 4 );
    // try to move items out of range
    REQUIRE( !mModel->moveItems( 2, 40 ) );
    REQUIRE( mReferencingLayer->getFeature( 1 ).attribute( QStringLiteral( "rank" ) ).toInt() == 1 );
    REQUIRE( mReferencingLayer->getFeature( 2 ).attribute( QStringLiteral( "rank" ) ).toInt() == 2 );
    REQUIRE( mReferencingLayer->getFeature( 3 ).attribute( QStringLiteral( "rank" ) ).toInt() == 3 );
    REQUIRE( mReferencingLayer->getFeature( 4 ).attribute( QStringLiteral( "rank" ) ).toInt() == 4 );

    // try to move items out of range
    REQUIRE( !mModel->moveItems( -2, 2 ) );
    REQUIRE( mReferencingLayer->getFeature( 1 ).attribute( QStringLiteral( "rank" ) ).toInt() == 1 );
    REQUIRE( mReferencingLayer->getFeature( 2 ).attribute( QStringLiteral( "rank" ) ).toInt() == 2 );
    REQUIRE( mReferencingLayer->getFeature( 3 ).attribute( QStringLiteral( "rank" ) ).toInt() == 3 );
    REQUIRE( mReferencingLayer->getFeature( 4 ).attribute( QStringLiteral( "rank" ) ).toInt() == 4 );

    REQUIRE( mModel->moveItems( 1, 3 ) );
    REQUIRE( QSignalSpy( mModel.get(), &ReferencingFeatureListModel::modelUpdated ).wait( 1000 ) );
    REQUIRE( mModel->rowCount() == 4 );
    REQUIRE( mReferencingLayer->getFeature( 1 ).attribute( QStringLiteral( "rank" ) ).toInt() == 1 );
    REQUIRE( mReferencingLayer->getFeature( 2 ).attribute( QStringLiteral( "rank" ) ).toInt() == 4 );
    REQUIRE( mReferencingLayer->getFeature( 3 ).attribute( QStringLiteral( "rank" ) ).toInt() == 2 );
    REQUIRE( mReferencingLayer->getFeature( 4 ).attribute( QStringLiteral( "rank" ) ).toInt() == 3 );

    // check feature ordering
    REQUIRE( mModel->data( mModel->index( 0, 0 ), OrderedRelationModel::FeatureIdRole ) == 1 );
    REQUIRE( mModel->data( mModel->index( 1, 0 ), OrderedRelationModel::FeatureIdRole ) == 3 );
    REQUIRE( mModel->data( mModel->index( 2, 0 ), OrderedRelationModel::FeatureIdRole ) == 4 );
    REQUIRE( mModel->data( mModel->index( 3, 0 ), OrderedRelationModel::FeatureIdRole ) == 2 );
  }
}
