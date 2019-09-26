
#include <QtTest>
#include <qgsapplication.h>
#include <qgslinestring.h>
#include <qgsgeometry.h>
#include <qgspoint.h>
#include <qgspointxy.h>

#include "qgsquickmapsettings.h"
#include "vertexmodel.h"

#ifdef ANDROID
#include "androidplatformutilities.h"
#endif

class TestVertexModel: public QObject
{
    Q_OBJECT
  private slots:
    void initTestCase()
    {
      mModel = new VertexModel();

      QgsLineString *lineString = new QgsLineString( QVector<QgsPoint>() << QgsPoint( 0, 1 ) << QgsPoint( 2, 3 ) << QgsPoint( 4, 3 ) );
      mLineGeometry = QgsGeometry( lineString );

      mPolygonGeometry = QgsGeometry::fromPolygonXY( QVector<QVector<QgsPointXY>>()
                         << ( QVector<QgsPointXY>()
                              << QgsPointXY( 0, 0 )
                              << QgsPointXY( 2, 0 )
                              << QgsPointXY( 2, 2 )
                              << QgsPointXY( 0, 2 )
                              << QgsPointXY( 0, 0 ) ) ) ;

      mPoint2056Geometry = QgsGeometry::fromPointXY( QgsPointXY( 2500000, 1200000 ) );
    }

    void canRemoveVertexTest()
    {
      // line
      mModel->setGeometry( mLineGeometry );
      QVERIFY( !mModel->canRemoveVertex() );
      mModel->setEditingMode( VertexModel::EditVertex );
      QVERIFY( mModel->canRemoveVertex() );

      mModel->setEditingMode( VertexModel::NoEditing );
      QCOMPARE( mModel->editingMode(), VertexModel::NoEditing );
      mModel->setCurrentVertex( 0 );
      QCOMPARE( mModel->editingMode(), VertexModel::EditVertex );

      QCOMPARE( mModel->vertexCount(), 3 );
      mModel->removeCurrentVertex();
      QCOMPARE( mModel->vertexCount(), 2 );
      QVERIFY( !mModel->canRemoveVertex() );

      // polygon
      mModel->setGeometry( mPolygonGeometry );
      QCOMPARE( mModel->editingMode(), VertexModel::NoEditing );
      QVERIFY( !mModel->canRemoveVertex() );
      mModel->setCurrentVertex( 0 );
      mModel->removeCurrentVertex();
      QCOMPARE( mModel->vertexCount(), 3 );
      QVERIFY( !mModel->canRemoveVertex() );
    }

    void addVertexTest()
    {
      mModel->setGeometry( mPolygonGeometry );
      QCOMPARE( mModel->vertexCount(), 4 );
      mModel->setEditingMode( VertexModel::AddVertex );
      QCOMPARE( mModel->vertexCount(), 5 );

      mModel->setGeometry( mLineGeometry );
      mModel->setEditingMode( VertexModel::AddVertex );
      QCOMPARE( mModel->mCurrentIndex, 1 );
      QVERIFY( mModel->canPreviousVertex() );
      mModel->previous();
      QVERIFY( !mModel->canPreviousVertex() );
      QCOMPARE( mModel->mCurrentIndex, 0 );
      mModel->next();
      QCOMPARE( mModel->mCurrentIndex, 1 );

      mModel->setGeometry( mLineGeometry );
      mModel->setEditingMode( VertexModel::AddVertex );
      QCOMPARE( mModel->mCurrentIndex, 1 );
      QCOMPARE( mModel->currentPoint(), QgsPoint( 1, 2 ) );
      mModel->next();
      QCOMPARE( mModel->mCurrentIndex, 2 );
      QCOMPARE( mModel->currentPoint(), QgsPoint( 3, 3 ) );
      mModel->next();
      QCOMPARE( mModel->mCurrentIndex, 3 );
      QCOMPARE( mModel->currentPoint().x(), 4.5 );
      QCOMPARE( mModel->currentPoint().y(), 3.0 );
      QCOMPARE( mModel->currentPoint(), QgsPoint( 4.5, 3 ) );
      QVERIFY( !mModel->canNextVertex() );
      mModel->next();
      QCOMPARE( mModel->mCurrentIndex, 3 );
      mModel->previous();
      QCOMPARE( mModel->mCurrentIndex, 2 );
      QCOMPARE( mModel->currentPoint(), QgsPoint( 3, 3 ) );
      mModel->previous();
      QCOMPARE( mModel->mCurrentIndex, 1 );
      QCOMPARE( mModel->currentPoint().x(), 1.0 );
    }

    void transformTest()
    {
      QgsQuickMapSettings *mapSettings = new QgsQuickMapSettings();
      mapSettings->setDestinationCrs( QgsCoordinateReferenceSystem::fromEpsgId( 21781 ) );
      mModel->setMapSettings( mapSettings );
      QCOMPARE( mModel->mapSettings()->destinationCrs().authid(), QStringLiteral( "EPSG:21781" ) );
      mModel->setGeometry( mPoint2056Geometry );
      mModel->setCrs( QgsCoordinateReferenceSystem::fromEpsgId( 2056 ) );
      QVERIFY( std::abs( mModel->item( 0 )->data( VertexModel::PointRole ).value<QgsPoint>().y() - 200000 ) < .1 );
      QVERIFY( std::abs( mModel->item( 0 )->data( VertexModel::PointRole ).value<QgsPoint>().x() - 500000 ) < .1 );

      delete mapSettings;
    }

    void returnGeometryTest()
    {
      // TODO, also test when adding vertex mode
    }



    void cleanupTestCase()
    {
      delete mModel;
    }

  private:
    VertexModel *mModel;
    QgsGeometry mLineGeometry;
    QgsGeometry mPolygonGeometry;
    QgsGeometry mPoint2056Geometry;
};

//QTEST_MAIN( TestVertexModel )
#include "test_vertexmodel.moc"
