
#include <QtTest>
#include <qgsapplication.h>
#include <qgslinestring.h>
#include <qgsgeometry.h>
#include <qgspoint.h>
#include <qgspointxy.h>
#include <qgsmessagelog.h>

#include "qgsquickmapsettings.h"
#include "vertexmodel.h"
#include "qfield_testbase.h"


#define VERIFYNEAR(value,expected,epsilon) { \
    QVERIFY2( qgsDoubleNear( value, expected, epsilon ), QStringLiteral("Expecting %1 got %2 (diff %3 > %4)").arg(expected).arg(value).arg(std::fabs( static_cast< double >( expected ) - value )).arg(epsilon).toUtf8().constData() ); \
  }(void)(0)

namespace QTest
{
  template<>
  char *toString( const QgsPoint &point )
  {
    QByteArray ba = "QgsPoint(" + QByteArray::number( point.x() ) +
                    ", " + QByteArray::number( point.y() ) + ")";
    return qstrdup( ba.data() );
  }
}

class TestVertexModel: public QObject
{
    Q_OBJECT
  private slots:
    void initTestCase()
    {
      connect( QgsApplication::messageLog(), static_cast<void ( QgsMessageLog::* )( const QString &message, const QString &tag, Qgis::MessageLevel level )>( &QgsMessageLog::messageReceived ), this, []( const QString & message, const QString & tag, Qgis::MessageLevel level )
      {
        qWarning() << message;
      } );
      mModel = new VertexModel();

      QgsLineString *lineString = new QgsLineString( QVector<QgsPoint>() << QgsPoint( 0, 0 ) << QgsPoint( 2, 2 ) << QgsPoint( 4, 4 ) );
      mLineGeometry = QgsGeometry( lineString );

      mPolygonGeometry = QgsGeometry::fromPolygonXY( QVector<QVector<QgsPointXY>>()
                         << ( QVector<QgsPointXY>()
                              << QgsPointXY( 0, 0 )
                              << QgsPointXY( 2, 0 )
                              << QgsPointXY( 2, 2 )
                              << QgsPointXY( 0, 2 )
                              << QgsPointXY( 0, 0 ) ) ) ;

      mRingPolygonGeometry = QgsGeometry::fromPolygonXY( QVector<QVector<QgsPointXY>>()
                             << ( QVector<QgsPointXY>()
                                  << QgsPointXY( 0, 0 )
                                  << QgsPointXY( 4, 0 )
                                  << QgsPointXY( 4, 4 )
                                  << QgsPointXY( 0, 4 )
                                  << QgsPointXY( 0, 0 ) )
                             << ( QVector<QgsPointXY>()
                                  << QgsPointXY( 1, 1 )
                                  << QgsPointXY( 3, 1 )
                                  << QgsPointXY( 3, 3 )
                                  << QgsPointXY( 1, 3 )
                                  << QgsPointXY( 1, 1 ) ) ) ;


      mPoint2056Geometry = QgsGeometry::fromPointXY( QgsPointXY( 2500000, 1200000 ) );

      mLine2056Geometry = QgsGeometry::fromPolylineXY( {QgsPointXY( 2500001, 1200001 ), QgsPointXY( 2500002, 1200002 ), QgsPointXY( 2500004, 1200004 )} );
    }

    void testCandidates()
    {
      mModel->setGeometry( mLineGeometry );
      QCOMPARE( mModel->vertexCount(), 7 );
      QCOMPARE( mModel->mVertices.at( 0 ).point, QgsPoint( -.5, -.5 ) );
      QCOMPARE( mModel->mVertices.at( 2 ).point, QgsPoint( 1, 1 ) );
      QCOMPARE( mModel->mVertices.at( 4 ).point, QgsPoint( 3, 3 ) );
      QCOMPARE( mModel->mVertices.at( 6 ).point, QgsPoint( 4.5, 4.5 ) );

      mModel->setGeometry( mPolygonGeometry );
      QCOMPARE( mModel->vertexCount(), 8 );
      QCOMPARE( mModel->mVertices.at( 0 ).point, QgsPoint( 1, 0 ) );
      QCOMPARE( mModel->mVertices.at( 1 ).point, QgsPoint( 2, 0 ) );
      QCOMPARE( mModel->mVertices.at( 2 ).point, QgsPoint( 2, 1 ) );
      QCOMPARE( mModel->mVertices.at( 3 ).point, QgsPoint( 2, 2 ) );
      QCOMPARE( mModel->mVertices.at( 4 ).point, QgsPoint( 1, 2 ) );
      QCOMPARE( mModel->mVertices.at( 5 ).point, QgsPoint( 0, 2 ) );
      QCOMPARE( mModel->mVertices.at( 6 ).point, QgsPoint( 0, 1 ) );
      QCOMPARE( mModel->mVertices.at( 7 ).point, QgsPoint( 0, 0 ) );

      mModel->setGeometry( mRingPolygonGeometry );
      QCOMPARE( mModel->vertexCount(), 16 );
      QCOMPARE( mModel->mVertices.at( 0 ).point, QgsPoint( 2, 0 ) );
      QCOMPARE( mModel->mVertices.at( 1 ).point, QgsPoint( 4, 0 ) );
      QCOMPARE( mModel->mVertices.at( 2 ).point, QgsPoint( 4, 2 ) );
      QCOMPARE( mModel->mVertices.at( 3 ).point, QgsPoint( 4, 4 ) );
      QCOMPARE( mModel->mVertices.at( 4 ).point, QgsPoint( 2, 4 ) );
      QCOMPARE( mModel->mVertices.at( 5 ).point, QgsPoint( 0, 4 ) );
      QCOMPARE( mModel->mVertices.at( 6 ).point, QgsPoint( 0, 2 ) );
      QCOMPARE( mModel->mVertices.at( 7 ).point, QgsPoint( 0, 0 ) );
      QCOMPARE( mModel->mVertices.at( 8 ).point, QgsPoint( 2, 1 ) );
      QCOMPARE( mModel->mVertices.at( 9 ).point, QgsPoint( 3, 1 ) );
      QCOMPARE( mModel->mVertices.at( 10 ).point, QgsPoint( 3, 2 ) );
      QCOMPARE( mModel->mVertices.at( 11 ).point, QgsPoint( 3, 3 ) );
      QCOMPARE( mModel->mVertices.at( 12 ).point, QgsPoint( 2, 3 ) );
      QCOMPARE( mModel->mVertices.at( 13 ).point, QgsPoint( 1, 3 ) );
      QCOMPARE( mModel->mVertices.at( 14 ).point, QgsPoint( 1, 2 ) );
      QCOMPARE( mModel->mVertices.at( 15 ).point, QgsPoint( 1, 1 ) );
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
      mModel->setCurrentVertex( 1 );
      QCOMPARE( mModel->editingMode(), VertexModel::EditVertex );

      QCOMPARE( mModel->vertexCount(), 7 );
      mModel->removeCurrentVertex();
      QCOMPARE( mModel->vertexCount(), 5 );
      QVERIFY( !mModel->canRemoveVertex() );

      // polygon
      mModel->setGeometry( mPolygonGeometry );
      QCOMPARE( mModel->editingMode(), VertexModel::NoEditing );
      QVERIFY( !mModel->canRemoveVertex() );
      mModel->setCurrentVertex( 1 );
      QCOMPARE( mModel->vertexCount(), 8 );
      mModel->removeCurrentVertex();
      QCOMPARE( mModel->vertexCount(), 6 );
      QVERIFY( !mModel->canRemoveVertex() );
    }

    void addVertexTest()
    {
      mModel->setGeometry( mPolygonGeometry );
      QCOMPARE( mModel->vertexCount(), 8 );
      mModel->setEditingMode( VertexModel::AddVertex );
      mModel->setCurrentVertex( 0 );
      mModel->setCurrentPoint( QgsPoint( -3, 0 ) );
      QCOMPARE( mModel->vertexCount(), 10 );

      mModel->setGeometry( mLineGeometry );
      mModel->setEditingMode( VertexModel::AddVertex );
      mModel->setCurrentVertex( 2 );
      QCOMPARE( mModel->mCurrentIndex, 2 );
      QVERIFY( mModel->canPreviousVertex() );
      mModel->previous();
      QVERIFY( !mModel->canPreviousVertex() );
      QCOMPARE( mModel->mCurrentIndex, 0 );
      mModel->next();
      QCOMPARE( mModel->mCurrentIndex, 2 );

      mModel->setGeometry( mLineGeometry );
      mModel->setEditingMode( VertexModel::AddVertex );
      mModel->setCurrentVertex( 0 );
      QCOMPARE( mModel->mCurrentIndex, 0 );
      QCOMPARE( mModel->currentPoint(), QgsPoint( -.5, -.5 ) );
      mModel->next();
      QCOMPARE( mModel->mCurrentIndex, 2 );
      QCOMPARE( mModel->currentPoint(), QgsPoint( 1, 1 ) );
      mModel->next();
      QCOMPARE( mModel->mCurrentIndex, 4 );
      QVERIFY( mModel->canNextVertex() );
      mModel->next();
      QCOMPARE( mModel->mCurrentIndex, 6 );
      QVERIFY( !mModel->canNextVertex() );
      mModel->previous();
      QCOMPARE( mModel->mCurrentIndex, 4 );
    }

    void editingModeTest()
    {
      mModel->setGeometry( mRingPolygonGeometry );
      QCOMPARE( mModel->editingMode(), VertexModel::NoEditing );
      QCOMPARE( mModel->currentVertexIndex(), -1 );
      mModel->setEditingMode( VertexModel::AddVertex );
      QCOMPARE( mModel->currentVertexIndex(), 0 );
      QCOMPARE( mModel->mVertices.at( 0 ).currentVertex, true );
      mModel->setCurrentPoint( QgsPoint( 1, 0 ) );
      QCOMPARE( mModel->currentVertexIndex(), 1 );
      QCOMPARE( mModel->currentPoint(), QgsPoint( 1, 0 ) );
      QCOMPARE( mModel->mVertices.at( 1 ).point, QgsPoint( 1, 0 ) );
    }

    void transformTest()
    {
      QgsQuickMapSettings mapSettings;
      mapSettings.setDestinationCrs( QgsCoordinateReferenceSystem::fromEpsgId( 4326 ) );
      mModel->setMapSettings( &mapSettings );
      QCOMPARE( mModel->mapSettings()->destinationCrs().authid(), QStringLiteral( "EPSG:4326" ) );
      mModel->setGeometry( mPoint2056Geometry );
      mModel->setCrs( QgsCoordinateReferenceSystem::fromEpsgId( 2056 ) );
      const auto &point = mModel->vertex( 0 ).point;

      VERIFYNEAR( point.y(), 46.9435, 0.001 );
      VERIFYNEAR( point.x(), 6.12514, 0.001 );
    }

    void selectVertexAtPositionTest()
    {
      QgsQuickMapSettings mapSettings;
      mapSettings.setDestinationCrs( QgsCoordinateReferenceSystem::fromEpsgId( 4326 ) );
      // mapSettings.setExtent(  );
      mModel->setMapSettings( &mapSettings );

      mModel->setCrs( QgsCoordinateReferenceSystem::fromEpsgId( 2056 ) );
      mModel->setGeometry( mLine2056Geometry );
      QCOMPARE( mModel->mCurrentIndex, -1 );

      mModel->selectVertexAtPosition( QgsPoint( 6.12515656, 46.943546146 ), 10 );
      QCOMPARE( mModel->mCurrentIndex, 1 );

      QCOMPARE( mModel->editingMode(), VertexModel::EditVertex );

      mModel->setEditingMode( VertexModel::AddVertex );
      QCOMPARE( mModel->mCurrentIndex, 2 );

      // selecting a candidate will make it a vertex
      QCOMPARE( mModel->mVertices.count(), 7 );
      mModel->selectVertexAtPosition( QgsPoint( 6.12515333, 46.94354385 ), 10 );
      QCOMPARE( mModel->editingMode(), VertexModel::EditVertex );
      QCOMPARE( mModel->mVertices.count(), 9 );
    }

    void cleanupTestCase()
    {
      delete mModel;
    }

  private:
    VertexModel *mModel;
    QgsGeometry mLineGeometry;
    QgsGeometry mPolygonGeometry;
    QgsGeometry mRingPolygonGeometry;
    QgsGeometry mPoint2056Geometry;
    QgsGeometry mLine2056Geometry;
};

QFIELDTEST_MAIN( TestVertexModel )
#include "test_vertexmodel.moc"
