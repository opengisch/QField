#include "orderedrelationmodel.h"
#include "qfield_testbase.h"

#include <QtTest>
#include <qgsapplication.h>
#include <qgsproject.h>
#include <qgsrelationmanager.h>
#include <qgsvectorlayer.h>

class TestOrderedRelationModel : public QObject
{
    Q_OBJECT
  private slots:
    void init()
    {
      //create layers
      mReferencingLayer.reset( new QgsVectorLayer( QStringLiteral( "NoGeometry?field=id:integer&field=foreignkey:integer&field=rank:integer" ), QStringLiteral( "king" ), QStringLiteral( "memory" ) ) );
      mReferencingLayer->setDisplayExpression( "name" );
      QVERIFY( mReferencingLayer->isValid() );
      QgsProject::instance()->addMapLayer( mReferencingLayer.get(), false, false );

      mReferencedLayer.reset( new QgsVectorLayer( QStringLiteral( "NoGeometry?field=id:integer" ), QStringLiteral( "land" ), QStringLiteral( "memory" ) ) );
      mReferencedLayer->setDisplayExpression( "name" );
      QVERIFY( mReferencedLayer->isValid() );
      QgsProject::instance()->addMapLayer( mReferencedLayer.get(), false, false );

      //create relations
      mRelation.setId( QStringLiteral( "referenced.referencing" ) );
      mRelation.setName( QStringLiteral( "referenced.referencing" ) );
      mRelation.setReferencingLayer( mReferencingLayer->id() );
      mRelation.setReferencedLayer( mReferencedLayer->id() );
      mRelation.addFieldPair( QStringLiteral( "foreignkey" ), QStringLiteral( "id" ) );
      QVERIFY( mRelation.isValid() );
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
      QCOMPARE( mReferencingLayer->featureCount(), 4L );

      // add features to the referenced layer
      QgsFeature referenced_ft1( mReferencedLayer->fields() );
      referenced_ft1.setAttribute( QStringLiteral( "id" ), 1 );

      mReferencedLayer->startEditing();
      mReferencedLayer->addFeature( referenced_ft1 );
      mReferencedLayer->commitChanges();
      QCOMPARE( mReferencedLayer->featureCount(), 1L );

      mModel = new OrderedRelationModel();
    }

    void cleanup()
    {
      delete mModel;

      QgsProject::instance()->removeMapLayer( mReferencedLayer.get() );
      QgsProject::instance()->removeMapLayer( mReferencingLayer.get() );
    }

    void testDeleteFeature()
    {
      QCOMPARE( mModel->rowCount(), 0 );

      mModel->setRelation( mRelation );
      mModel->setOrderingField( QStringLiteral( "rank" ) );
      mModel->setFeature( mReferencedLayer->getFeature( 1 ) );

      QVERIFY( QSignalSpy( mModel, &ReferencingFeatureListModel::modelUpdated ).wait( 1000 ) );

      QCOMPARE( mModel->rowCount(), 4 );
      QVERIFY( mModel->deleteFeature( 2 ) );
      QVERIFY( QSignalSpy( mModel, &ReferencingFeatureListModel::modelUpdated ).wait( 1000 ) );
      QCOMPARE( mModel->rowCount(), 3 );
      QVERIFY( !mReferencingLayer->getFeature( 2 ).isValid() );
      QCOMPARE( mReferencingLayer->getFeature( 1 ).attribute( QStringLiteral( "rank" ) ).toInt(), 1 );
      QCOMPARE( mReferencingLayer->getFeature( 3 ).attribute( QStringLiteral( "rank" ) ).toInt(), 2 );
      QCOMPARE( mReferencingLayer->getFeature( 4 ).attribute( QStringLiteral( "rank" ) ).toInt(), 3 );
    }

    void testMoveFeature()
    {
      QCOMPARE( mModel->rowCount(), 0 );

      mModel->setRelation( mRelation );
      mModel->setOrderingField( QStringLiteral( "rank" ) );
      mModel->setFeature( mReferencedLayer->getFeature( 1 ) );

      QVERIFY( QSignalSpy( mModel, &ReferencingFeatureListModel::modelUpdated ).wait( 1000 ) );

      QCOMPARE( mModel->rowCount(), 4 );
      // try to move items out of range
      QVERIFY( !mModel->moveItems( 2, 40 ) );
      QCOMPARE( mReferencingLayer->getFeature( 1 ).attribute( QStringLiteral( "rank" ) ).toInt(), 1 );
      QCOMPARE( mReferencingLayer->getFeature( 2 ).attribute( QStringLiteral( "rank" ) ).toInt(), 2 );
      QCOMPARE( mReferencingLayer->getFeature( 3 ).attribute( QStringLiteral( "rank" ) ).toInt(), 3 );
      QCOMPARE( mReferencingLayer->getFeature( 4 ).attribute( QStringLiteral( "rank" ) ).toInt(), 4 );

      // try to move items out of range
      QVERIFY( !mModel->moveItems( -2, 2 ) );
      QCOMPARE( mReferencingLayer->getFeature( 1 ).attribute( QStringLiteral( "rank" ) ).toInt(), 1 );
      QCOMPARE( mReferencingLayer->getFeature( 2 ).attribute( QStringLiteral( "rank" ) ).toInt(), 2 );
      QCOMPARE( mReferencingLayer->getFeature( 3 ).attribute( QStringLiteral( "rank" ) ).toInt(), 3 );
      QCOMPARE( mReferencingLayer->getFeature( 4 ).attribute( QStringLiteral( "rank" ) ).toInt(), 4 );

      QVERIFY( mModel->moveItems( 1, 3 ) );
      QVERIFY( QSignalSpy( mModel, &ReferencingFeatureListModel::modelUpdated ).wait( 1000 ) );
      QCOMPARE( mModel->rowCount(), 4 );
      QCOMPARE( mReferencingLayer->getFeature( 1 ).attribute( QStringLiteral( "rank" ) ).toInt(), 1 );
      QCOMPARE( mReferencingLayer->getFeature( 2 ).attribute( QStringLiteral( "rank" ) ).toInt(), 4 );
      QCOMPARE( mReferencingLayer->getFeature( 3 ).attribute( QStringLiteral( "rank" ) ).toInt(), 2 );
      QCOMPARE( mReferencingLayer->getFeature( 4 ).attribute( QStringLiteral( "rank" ) ).toInt(), 3 );

      // check feature ordering
      QCOMPARE( mModel->data( mModel->index( 0, 0 ), OrderedRelationModel::FeatureIdRole ), 1 );
      QCOMPARE( mModel->data( mModel->index( 1, 0 ), OrderedRelationModel::FeatureIdRole ), 3 );
      QCOMPARE( mModel->data( mModel->index( 2, 0 ), OrderedRelationModel::FeatureIdRole ), 4 );
      QCOMPARE( mModel->data( mModel->index( 3, 0 ), OrderedRelationModel::FeatureIdRole ), 2 );
    }

  private:
    std::unique_ptr<QgsVectorLayer> mReferencedLayer;
    std::unique_ptr<QgsVectorLayer> mReferencingLayer;
    QgsRelation mRelation;
    OrderedRelationModel *mModel;
};

QFIELDTEST_MAIN( TestOrderedRelationModel )
#include "test_orderedrelationmodel.moc"
