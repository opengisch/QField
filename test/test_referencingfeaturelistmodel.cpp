/***************************************************************************
                        test_referencingfeaturelistmodel.cpp
                        --------------------
  begin                : October 2019
  copyright            : (C) 2019 by David Signer
  email                : david (at) opengis.ch
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
#include "qgsquickmapsettings.h"
#include "referencingfeaturelistmodel.h"

#include <QSignalSpy>
#include <qgsapplication.h>
#include <qgsproject.h>
#include <qgsrelationmanager.h>
#include <qgsvectorlayer.h>

TEST_CASE( "ReferencingFeatureListModel" )
{
  /* TEST PROJECT
      *
      * LAYERS
      * land (id, name, king_id)
      * - 0 Mordor
      * - 1 Gondor
      * - 2 Rohan
      * - 3 Eriador

      * king
      * - 0 Frodo (Gondor, Rohan, Eriador)
      * - 1 Gollum (Mordor)

      * share (id, share, king_id, land_id)
      * - Mordor Frodo 20
      * - Gondor Frodo 40
      * - Rohan  Frodo 60
      * - EriadorFrodo 100
      *
      * - Mordor Gollum 80
      * - Gondor Gollum 60
      * - Rohan  Gollum 40
      *
      * RELATIONS
      * A king can rule several lands, but a land can be ruled by only one king
      * - landhasoneking (land m--1 king)
      * A king can have shares of several lands and a land can be owned by several kings
      * - sharehasoneoner (shares m--1 king)
      * - shareofoneland (shares m--1 land)
      */

  //create layers
  std::unique_ptr<QgsVectorLayer> mL_Land( new QgsVectorLayer( QStringLiteral( "Polygon?crs=EPSG:3857&field=id:int&field=name:string&field=king_id:int" ), QStringLiteral( "land" ), QStringLiteral( "memory" ) ) );
  mL_Land->setDisplayExpression( "name" );
  REQUIRE( mL_Land->isValid() );
  QgsProject::instance()->addMapLayer( mL_Land.get(), false, false );

  std::unique_ptr<QgsVectorLayer> mL_King( new QgsVectorLayer( QStringLiteral( "Point?crs=EPSG:3857&field=id:int&field=name:string" ), QStringLiteral( "king" ), QStringLiteral( "memory" ) ) );
  mL_King->setDisplayExpression( "name" );
  REQUIRE( mL_King->isValid() );
  QgsProject::instance()->addMapLayer( mL_King.get(), false, false );

  std::unique_ptr<QgsVectorLayer> mL_Share( new QgsVectorLayer( QStringLiteral( "Point?crs=EPSG:3857&field=id:integer&field=share:int&field=king_id:int&field=land_id:int" ), QStringLiteral( "share" ), QStringLiteral( "memory" ) ) );
  mL_Share->setDisplayExpression( "share" );
  REQUIRE( mL_Share->isValid() );
  QgsProject::instance()->addMapLayer( mL_Share.get(), false, false );

  //create relations
  QgsRelation mR_Landhasoneking;
  mR_Landhasoneking.setId( QStringLiteral( "land.king" ) );
  mR_Landhasoneking.setName( QStringLiteral( "land.king" ) );
  mR_Landhasoneking.setReferencingLayer( mL_Land->id() );
  mR_Landhasoneking.setReferencedLayer( mL_King->id() );
  mR_Landhasoneking.addFieldPair( QStringLiteral( "king_id" ), QStringLiteral( "id" ) );
  REQUIRE( mR_Landhasoneking.isValid() );
  QgsProject::instance()->relationManager()->addRelation( mR_Landhasoneking );

  QgsRelation mR_Sharehasoneking;
  mR_Sharehasoneking.setId( QStringLiteral( "share.king" ) );
  mR_Sharehasoneking.setName( QStringLiteral( "share.king" ) );
  mR_Sharehasoneking.setReferencingLayer( mL_Share->id() );
  mR_Sharehasoneking.setReferencedLayer( mL_King->id() );
  mR_Sharehasoneking.addFieldPair( QStringLiteral( "king_id" ), QStringLiteral( "id" ) );
  REQUIRE( mR_Sharehasoneking.isValid() );
  QgsProject::instance()->relationManager()->addRelation( mR_Sharehasoneking );

  QgsRelation mR_Shareofoneland;
  mR_Shareofoneland.setId( QStringLiteral( "share.land" ) );
  mR_Shareofoneland.setName( QStringLiteral( "share.land" ) );
  mR_Shareofoneland.setReferencingLayer( mL_Share->id() );
  mR_Shareofoneland.setReferencedLayer( mL_Land->id() );
  mR_Shareofoneland.addFieldPair( QStringLiteral( "land_id" ), QStringLiteral( "id" ) );
  REQUIRE( mR_Shareofoneland.isValid() );
  QgsProject::instance()->relationManager()->addRelation( mR_Shareofoneland );

  // add features on land
  QgsFeature land_ft0( mL_Land->fields() );
  land_ft0.setAttribute( QStringLiteral( "id" ), 0 );
  land_ft0.setAttribute( QStringLiteral( "name" ), "Mordor" );
  land_ft0.setAttribute( QStringLiteral( "king_id" ), 1 );
  QgsFeature land_ft1( mL_Land->fields() );
  land_ft1.setAttribute( QStringLiteral( "id" ), 1 );
  land_ft1.setAttribute( QStringLiteral( "name" ), "Gondor" );
  land_ft1.setAttribute( QStringLiteral( "king_id" ), 0 );
  QgsFeature land_ft2( mL_Land->fields() );
  land_ft2.setAttribute( QStringLiteral( "id" ), 2 );
  land_ft2.setAttribute( QStringLiteral( "name" ), "Rohan" );
  land_ft2.setAttribute( QStringLiteral( "king_id" ), 0 );
  QgsFeature land_ft3( mL_Land->fields() );
  land_ft3.setAttribute( QStringLiteral( "id" ), 3 );
  land_ft3.setAttribute( QStringLiteral( "name" ), "Eriador" );
  land_ft3.setAttribute( QStringLiteral( "king_id" ), 0 );
  mL_Land->startEditing();
  mL_Land->addFeature( land_ft0 );
  mL_Land->addFeature( land_ft1 );
  mL_Land->addFeature( land_ft2 );
  mL_Land->addFeature( land_ft3 );
  mL_Land->commitChanges();
  REQUIRE( mL_Land->featureCount() == 4L );

  // add features on king
  QgsFeature king_ft0( mL_King->fields() );
  king_ft0.setAttribute( QStringLiteral( "id" ), 0 );
  king_ft0.setAttribute( QStringLiteral( "name" ), "Frodo" );
  QgsFeature king_ft1( mL_King->fields() );
  king_ft1.setAttribute( QStringLiteral( "id" ), 1 );
  king_ft1.setAttribute( QStringLiteral( "name" ), "Gollum" );
  mL_King->startEditing();
  mL_King->addFeature( king_ft0 );
  mL_King->addFeature( king_ft1 );
  mL_King->commitChanges();
  REQUIRE( mL_King->featureCount() == 2L );

  // add features on share
  QgsFeature share_ft0( mL_Share->fields() );
  share_ft0.setAttribute( QStringLiteral( "id" ), 0 );
  share_ft0.setAttribute( QStringLiteral( "king_id" ), 0 );
  share_ft0.setAttribute( QStringLiteral( "land_id" ), 0 );
  share_ft0.setAttribute( QStringLiteral( "share" ), 20 );
  QgsFeature share_ft1( mL_Share->fields() );
  share_ft1.setAttribute( QStringLiteral( "id" ), 1 );
  share_ft1.setAttribute( QStringLiteral( "king_id" ), 0 );
  share_ft1.setAttribute( QStringLiteral( "land_id" ), 1 );
  share_ft1.setAttribute( QStringLiteral( "share" ), 40 );
  QgsFeature share_ft2( mL_Share->fields() );
  share_ft2.setAttribute( QStringLiteral( "id" ), 2 );
  share_ft2.setAttribute( QStringLiteral( "king_id" ), 0 );
  share_ft2.setAttribute( QStringLiteral( "land_id" ), 2 );
  share_ft2.setAttribute( QStringLiteral( "share" ), 60 );
  QgsFeature share_ft3( mL_Share->fields() );
  share_ft3.setAttribute( QStringLiteral( "id" ), 3 );
  share_ft3.setAttribute( QStringLiteral( "king_id" ), 0 );
  share_ft3.setAttribute( QStringLiteral( "land_id" ), 3 );
  share_ft3.setAttribute( QStringLiteral( "share" ), 100 );
  QgsFeature share_ft4( mL_Share->fields() );
  share_ft4.setAttribute( QStringLiteral( "id" ), 4 );
  share_ft4.setAttribute( QStringLiteral( "king_id" ), 1 );
  share_ft4.setAttribute( QStringLiteral( "land_id" ), 0 );
  share_ft4.setAttribute( QStringLiteral( "share" ), 80 );
  QgsFeature share_ft5( mL_Share->fields() );
  share_ft5.setAttribute( QStringLiteral( "id" ), 5 );
  share_ft5.setAttribute( QStringLiteral( "king_id" ), 1 );
  share_ft5.setAttribute( QStringLiteral( "land_id" ), 1 );
  share_ft5.setAttribute( QStringLiteral( "share" ), 60 );
  QgsFeature share_ft6( mL_Share->fields() );
  share_ft6.setAttribute( QStringLiteral( "id" ), 6 );
  share_ft6.setAttribute( QStringLiteral( "king_id" ), 1 );
  share_ft6.setAttribute( QStringLiteral( "land_id" ), 2 );
  share_ft6.setAttribute( QStringLiteral( "share" ), 40 );

  mL_Share->startEditing();
  mL_Share->addFeature( share_ft0 );
  mL_Share->addFeature( share_ft1 );
  mL_Share->addFeature( share_ft2 );
  mL_Share->addFeature( share_ft3 );
  mL_Share->addFeature( share_ft4 );
  mL_Share->addFeature( share_ft5 );
  mL_Share->addFeature( share_ft6 );
  mL_Share->commitChanges();
  REQUIRE( mL_Share->featureCount() == 7L );

  std::unique_ptr<ReferencingFeatureListModel> mModel( new ReferencingFeatureListModel() );

  /*
      GetReferencingFeatures
      - load project
      - create model (set relation, set feature)
      - count list / compare list
      - change parent feature
      - count list / compare list
    */
  SECTION( "GetReferencingFeatures" )
  {
    mModel->setRelation( mR_Landhasoneking );
    QSignalSpy( mModel.get(), &ReferencingFeatureListModel::modelUpdated ).wait( 500 );
    mModel->setNmRelation( QgsRelation() );
    QSignalSpy( mModel.get(), &ReferencingFeatureListModel::modelUpdated ).wait( 500 );

    //check out Frodo
    mModel->setFeature( mL_King->getFeature( 1 ) );
    REQUIRE( QSignalSpy( mModel.get(), &ReferencingFeatureListModel::modelUpdated ).wait( 1000 ) );
    //Frodo rules 3 lands (Gondor, Rohan, Eriador)
    REQUIRE( mModel->rowCount() == 3 );

    //check out Gollum
    mModel->setFeature( mL_King->getFeature( 2 ) );
    REQUIRE( QSignalSpy( mModel.get(), &ReferencingFeatureListModel::modelUpdated ).wait( 1000 ) );
    //Gollum rules 1 land (Mordor)
    REQUIRE( mModel->rowCount() == 1 );
  }

  /*
      GetManyToManyReferencedFeatures
      - load project
      - create model (set relation, set nmrelation, set feature)
      - count list / compare list
      - change parent feature
      - count list / compare list
    */
  SECTION( "GetManyToManyReferencedFeatures" )
  {
    mModel->setRelation( mR_Sharehasoneking );
    QSignalSpy( mModel.get(), &ReferencingFeatureListModel::modelUpdated ).wait( 500 );
    mModel->setNmRelation( mR_Shareofoneland );
    QSignalSpy( mModel.get(), &ReferencingFeatureListModel::modelUpdated ).wait( 500 );

    //check out Frodo
    mModel->setFeature( mL_King->getFeature( 1 ) );
    REQUIRE( QSignalSpy( mModel.get(), &ReferencingFeatureListModel::modelUpdated ).wait( 1000 ) );
    //Frodo has shares of 4 lands (Mordor, Gondor, Rohan, Eriador)
    REQUIRE( mModel->rowCount() == 4 );

    //check out Gollum
    mModel->setFeature( mL_King->getFeature( 2 ) );
    REQUIRE( QSignalSpy( mModel.get(), &ReferencingFeatureListModel::modelUpdated ).wait( 1000 ) );
    //Gollum has shares of 3 lands (Mordor, Gondor, Rohan)
    REQUIRE( mModel->rowCount() == 3 );
  }

  /*
      DeleteReferencingFeature
      - load project
      - create model (set relation, set feature)
      - count list / compare list
      - delete child features
      - count list / compare list
    */
  SECTION( "DeleteReferencingFeature" )
  {
    mModel->setNmRelation( QgsRelation() );
    QSignalSpy( mModel.get(), &ReferencingFeatureListModel::modelUpdated ).wait( 500 );
    mModel->setRelation( mR_Landhasoneking );
    QSignalSpy( mModel.get(), &ReferencingFeatureListModel::modelUpdated ).wait( 500 );

    //check out Frodo
    mModel->setFeature( mL_King->getFeature( 1 ) );
    REQUIRE( QSignalSpy( mModel.get(), &ReferencingFeatureListModel::modelUpdated ).wait( 1000 ) );
    //Frodo rules 3 lands (Gondor, Rohan, Eriador)
    REQUIRE( mModel->rowCount() == 3 );

    //check display string of rohan
    QString displayString = mModel->data( mModel->index( 1, 0 ), ReferencingFeatureListModel::DisplayString ).toString();
    REQUIRE( displayString == QStringLiteral( "Gondor" ) );

    //delete Rohan
    mModel->deleteFeature( qvariant_cast<QgsFeature>( mModel->data( mModel->index( 1, 0 ), ReferencingFeatureListModel::ReferencingFeature ) ).id() );
    REQUIRE( QSignalSpy( mModel.get(), &ReferencingFeatureListModel::modelUpdated ).wait( 1000 ) );
    //Frodo rules 2 lands (Gondor, Eriador) no Rohan anymore
    REQUIRE( mModel->rowCount() == 2 );
  }

  /*
      DeleteReferenceToManyToManyReferencedFeature
      - load project
      - create model (set relation, set feature)
      - count list / compare list
      - delete (unlink) child features
      - count list / compare list
    */
  SECTION( "DeleteReferenceToManyToManyReferencedFeature" )
  {
    mModel->setRelation( mR_Sharehasoneking );
    QSignalSpy( mModel.get(), &ReferencingFeatureListModel::modelUpdated ).wait( 500 );
    mModel->setNmRelation( mR_Shareofoneland );
    QSignalSpy( mModel.get(), &ReferencingFeatureListModel::modelUpdated ).wait( 500 );

    //check out Frodo
    mModel->setFeature( mL_King->getFeature( 1 ) );
    REQUIRE( QSignalSpy( mModel.get(), &ReferencingFeatureListModel::modelUpdated ).wait( 1000 ) );
    //Frodo has shares of 4 lands (Mordor, Gondor, Eriador, Rohan)
    REQUIRE( mModel->rowCount() == 4 );

    //check out Gollum
    mModel->setFeature( mL_King->getFeature( 2 ) );
    REQUIRE( QSignalSpy( mModel.get(), &ReferencingFeatureListModel::modelUpdated ).wait( 1000 ) );
    //Gollum has shares of 3 lands (Mordor, Gondor, Rohan)
    REQUIRE( mModel->rowCount() == 3 );

    //check display string of Gollums Mordor share (80)
    QString displayString = mModel->data( mModel->index( 0, 0 ), ReferencingFeatureListModel::DisplayString ).toString();
    REQUIRE( displayString == QStringLiteral( "40" ) );

    //delete Gollums share on Mordor
    mModel->deleteFeature( qvariant_cast<QgsFeature>( mModel->data( mModel->index( 0, 0 ), ReferencingFeatureListModel::ReferencingFeature ) ).id() );
    REQUIRE( QSignalSpy( mModel.get(), &ReferencingFeatureListModel::modelUpdated ).wait( 1000 ) );
    //Gollum has shares of 2 landd (Gondor, Rohan)
    REQUIRE( mModel->rowCount() == 2 );

    //check out Frodo again
    mModel->setFeature( mL_King->getFeature( 1 ) );
    REQUIRE( QSignalSpy( mModel.get(), &ReferencingFeatureListModel::modelUpdated ).wait( 1000 ) );
    //Frodo has still shares of 4 lands (Mordor, Gondor, Eriador, Rohan) because his shares are untouched
    REQUIRE( mModel->rowCount() == 4 );
  }
}
