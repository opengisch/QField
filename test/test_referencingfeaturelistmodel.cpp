
#include <QtTest>
#include <qgsapplication.h>
#include <qgsvectorlayer.h>
#include <qgsproject.h>
#include <qgsrelationmanager.h>

#include "qgsquickmapsettings.h"
#include "referencingfeaturelistmodel.h"

#ifdef ANDROID
#include "androidplatformutilities.h"
#endif

class TestReferencingFeatureListModel: public QObject
{
    Q_OBJECT
  private slots:
    void initTestCase()
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
      mL_Land.reset( new QgsVectorLayer( QStringLiteral( "Polygon?crs=EPSG:3857&field=id:int&field=name:string&field=king_id:int" ), QStringLiteral( "land" ), QStringLiteral( "memory" ) ) );
      mL_Land->setDisplayExpression( "name" );
      QVERIFY( mL_Land->isValid() );
      QgsProject::instance()->addMapLayer( mL_Land.get(), false, false );

      mL_King.reset( new QgsVectorLayer( QStringLiteral( "Point?crs=EPSG:3857&field=id:int&field=name:string" ), QStringLiteral( "king" ), QStringLiteral( "memory" ) ) );
      mL_King->setDisplayExpression( "name" );
      QVERIFY( mL_King->isValid() );
      QgsProject::instance()->addMapLayer( mL_King.get(), false, false );

      mL_Share.reset( new QgsVectorLayer( QStringLiteral( "Point?crs=EPSG:3857&field=id:integer&field=share:int&field=king_id:int&field=land_id:int" ), QStringLiteral( "share" ), QStringLiteral( "memory" ) ) );
      mL_Share->setDisplayExpression( "share" );
      QVERIFY( mL_Share->isValid() );
      QgsProject::instance()->addMapLayer( mL_Share.get(), false, false );

      //create relations
      mR_Landhasoneking.setId( QStringLiteral( "land.king" ) );
      mR_Landhasoneking.setName( QStringLiteral( "land.king" ) );
      mR_Landhasoneking.setReferencingLayer( mL_Land->id() );
      mR_Landhasoneking.setReferencedLayer( mL_King->id() );
      mR_Landhasoneking.addFieldPair( QStringLiteral( "king_id" ), QStringLiteral( "id" ) );
      QVERIFY( mR_Landhasoneking.isValid() );
      QgsProject::instance()->relationManager()->addRelation( mR_Landhasoneking );

      mR_Sharehasoneking.setId( QStringLiteral( "share.king" ) );
      mR_Sharehasoneking.setName( QStringLiteral( "share.king" ) );
      mR_Sharehasoneking.setReferencingLayer( mL_Share->id() );
      mR_Sharehasoneking.setReferencedLayer( mL_King->id() );
      mR_Sharehasoneking.addFieldPair( QStringLiteral( "king_id" ), QStringLiteral( "id" ) );
      QVERIFY( mR_Sharehasoneking.isValid() );
      QgsProject::instance()->relationManager()->addRelation( mR_Sharehasoneking );

      mR_Shareofoneland.setId( QStringLiteral( "share.land" ) );
      mR_Shareofoneland.setName( QStringLiteral( "share.land" ) );
      mR_Shareofoneland.setReferencingLayer( mL_Share->id() );
      mR_Shareofoneland.setReferencedLayer( mL_Land->id() );
      mR_Shareofoneland.addFieldPair( QStringLiteral( "land_id" ), QStringLiteral( "id" ) );
      QVERIFY( mR_Shareofoneland.isValid() );
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
      QCOMPARE( mL_Land->featureCount(), 4 );

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
      QCOMPARE( mL_King->featureCount(), 2 );

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
      QCOMPARE( mL_Share->featureCount(), 7 );

      mModel = new ReferencingFeatureListModel();
    }

    /*
      testGetReferencingFeatures
      - load project
      - create model (set relation, set feature)
      - count list / compare list
      - change parent feature
      - count list / compare list
    */
    void testGetReferencingFeatures()
    {
      mModel->setRelation( mR_Landhasoneking );
      QSignalSpy( mModel, &ReferencingFeatureListModel::modelUpdated ).wait( 500 );
      mModel->setNmRelation( QgsRelation() );
      QSignalSpy( mModel, &ReferencingFeatureListModel::modelUpdated ).wait( 500 );

      //check out Frodo
      mModel->setFeature( mL_King->getFeature( 1 ) );
      QVERIFY( QSignalSpy( mModel, &ReferencingFeatureListModel::modelUpdated ).wait( 1000 ) );
      //Frodo rules 3 lands (Gondor, Rohan, Eriador)
      QCOMPARE( mModel->rowCount(), 3 );

      //check out Gollum
      mModel->setFeature( mL_King->getFeature( 2 ) );
      QVERIFY( QSignalSpy( mModel, &ReferencingFeatureListModel::modelUpdated ).wait( 1000 ) );
      //Gollum rules 1 land (Mordor)
      QCOMPARE( mModel->rowCount(), 1 );
    }

    /*
      testGetManyToManyReferencedFeatures
      - load project
      - create model (set relation, set nmrelation, set feature)
      - count list / compare list
      - change parent feature
      - count list / compare list
    */
    void testGetManyToManyReferencedFeatures()
    {
      mModel->setRelation( mR_Sharehasoneking );
      QSignalSpy( mModel, &ReferencingFeatureListModel::modelUpdated ).wait( 500 );
      mModel->setNmRelation( mR_Shareofoneland );
      QSignalSpy( mModel, &ReferencingFeatureListModel::modelUpdated ).wait( 500 );

      //check out Frodo
      mModel->setFeature( mL_King->getFeature( 1 ) );
      QVERIFY( QSignalSpy( mModel, &ReferencingFeatureListModel::modelUpdated ).wait( 1000 ) );
      //Frodo has shares of 4 lands (Mordor, Gondor, Rohan, Eriador)
      QCOMPARE( mModel->rowCount(), 4 );

      //check out Gollum
      mModel->setFeature( mL_King->getFeature( 2 ) );
      QVERIFY( QSignalSpy( mModel, &ReferencingFeatureListModel::modelUpdated ).wait( 1000 ) );
      //Gollum has shares of 3 lands (Mordor, Gondor, Rohan)
      QCOMPARE( mModel->rowCount(), 3 );
    }

    /*
      testDeleteReferencingFeature
      - load project
      - create model (set relation, set feature)
      - count list / compare list
      - delete child features
      - count list / compare list
    */
    void testDeleteReferencingFeature()
    {
      mModel->setNmRelation( QgsRelation() );
      QSignalSpy( mModel, &ReferencingFeatureListModel::modelUpdated ).wait( 500 );
      mModel->setRelation( mR_Landhasoneking );
      QSignalSpy( mModel, &ReferencingFeatureListModel::modelUpdated ).wait( 500 );

      //check out Frodo
      mModel->setFeature( mL_King->getFeature( 1 ) );
      QVERIFY( QSignalSpy( mModel, &ReferencingFeatureListModel::modelUpdated ).wait( 1000 ) );
      //Frodo rules 3 lands (Gondor, Rohan, Eriador)
      QCOMPARE( mModel->rowCount(), 3 );

      //check display string of rohan
      QString displayString = mModel->data( mModel->index( 1, 0 ), ReferencingFeatureListModel::DisplayString ).toString();
      QCOMPARE( displayString, QStringLiteral( "Rohan" ) );

      //delete Rohan
      mModel->deleteFeature( qvariant_cast<QgsFeature>( mModel->data( mModel->index( 1, 0 ), ReferencingFeatureListModel::ReferencingFeature ) ).id() );
      QVERIFY( QSignalSpy( mModel, &ReferencingFeatureListModel::modelUpdated ).wait( 1000 ) );
      //Frodo rules 2 lands (Gondor, Eriador) no Rohan anymore
      QCOMPARE( mModel->rowCount(), 2 );

    }

    /*
      testDdeleteReferenceToManyToManyReferencedFeature
      - load project
      - create model (set relation, set feature)
      - count list / compare list
      - delete (unlink) child features
      - count list / compare list
    */
    void testDeleteReferenceToManyToManyReferencedFeature()
    {
      mModel->setRelation( mR_Sharehasoneking );
      QSignalSpy( mModel, &ReferencingFeatureListModel::modelUpdated ).wait( 500 );
      mModel->setNmRelation( mR_Shareofoneland );
      QSignalSpy( mModel, &ReferencingFeatureListModel::modelUpdated ).wait( 500 );

      //check out Frodo
      mModel->setFeature( mL_King->getFeature( 1 ) );
      QVERIFY( QSignalSpy( mModel, &ReferencingFeatureListModel::modelUpdated ).wait( 1000 ) );
      //Frodo has shares of 4 lands (Mordor, Gondor, Eriador, Rohan)
      QCOMPARE( mModel->rowCount(), 4 );

      //check out Gollum
      mModel->setFeature( mL_King->getFeature( 2 ) );
      QVERIFY( QSignalSpy( mModel, &ReferencingFeatureListModel::modelUpdated ).wait( 1000 ) );
      //Gollum has shares of 3 lands (Mordor, Gondor, Rohan)
      QCOMPARE( mModel->rowCount(), 3 );

      //check display string of Gollums Mordor share (80)
      QString displayString = mModel->data( mModel->index( 0, 0 ), ReferencingFeatureListModel::DisplayString ).toString();
      QCOMPARE( displayString, QStringLiteral( "80" ) );

      //delete Gollums share on Mordor
      mModel->deleteFeature( qvariant_cast<QgsFeature>( mModel->data( mModel->index( 0, 0 ), ReferencingFeatureListModel::ReferencingFeature ) ).id() );
      QVERIFY( QSignalSpy( mModel, &ReferencingFeatureListModel::modelUpdated ).wait( 1000 ) );
      //Gollum has shares of 2 landd (Gondor, Rohan)
      QCOMPARE( mModel->rowCount(), 2 );

      //check out Frodo again
      mModel->setFeature( mL_King->getFeature( 1 ) );
      QVERIFY( QSignalSpy( mModel, &ReferencingFeatureListModel::modelUpdated ).wait( 1000 ) );
      //Frodo has still shares of 4 lands (Mordor, Gondor, Eriador, Rohan) because his shares are untouched
      QCOMPARE( mModel->rowCount(), 4 );
    }

    void cleanupTestCase()
    {
      delete mModel;

      QgsProject::instance()->removeMapLayer( mL_Land.get() );
      QgsProject::instance()->removeMapLayer( mL_King.get() );
      QgsProject::instance()->removeMapLayer( mL_Share.get() );
    }

  private:
    std::unique_ptr<QgsVectorLayer> mL_Land;
    std::unique_ptr<QgsVectorLayer> mL_King;
    std::unique_ptr<QgsVectorLayer> mL_Share;
    QgsRelation mR_Landhasoneking;
    QgsRelation mR_Sharehasoneking;
    QgsRelation mR_Shareofoneland;
    ReferencingFeatureListModel *mModel;

};

//QTEST_MAIN( TestReferencingFeatureListModel )
#include "test_referencingfeaturelistmodel.moc"
