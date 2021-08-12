
#include "qfield_testbase.h"
#include "qgsquickmapsettings.h"
#include "vertexmodel.h"

#include <gtest/gtest.h>
#include <qgsapplication.h>
#include <qgsgeometry.h>
#include <qgslinestring.h>
#include <qgsmessagelog.h>
#include <qgspoint.h>
#include <qgspointxy.h>


#define VERIFYNEAR( value, expected, epsilon )                                                                                                                                                                                                  \
  {                                                                                                                                                                                                                                             \
    EXPECT_TRUE( qgsDoubleNear( value, expected, epsilon ) ) << QStringLiteral( "Expecting %1 got %2 (diff %3 > %4)" ).arg( expected ).arg( value ).arg( std::fabs( static_cast<double>( expected ) - value ) ).arg( epsilon ).toUtf8().constData(); \
  }                                                                                                                                                                                                                                             \
  ( void ) ( 0 )

#if 0
namespace QTest
{
  template<>
  char *toString( const QgsPoint &point )
  {
    QByteArray ba = "QgsPoint(" + QByteArray::number( point.x() ) + ", " + QByteArray::number( point.y() ) + ")";
    return qstrdup( ba.data() );
  }
} // namespace QTest
#endif

class TestVertexModel : public QObject, public ::testing::Test
{
    Q_OBJECT
  protected:

    void SetUp()
    {
      QObject::connect( QgsApplication::messageLog(), qOverload<const QString &, const QString &, Qgis::MessageLevel>( &QgsMessageLog::messageReceived ), this, []( const QString & message, const QString & tag, Qgis::MessageLevel level )
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
                              << QgsPointXY( 0, 0 ) ) );

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
                                  << QgsPointXY( 1, 1 ) ) );


      mPoint2056Geometry = QgsGeometry::fromPointXY( QgsPointXY( 2500000, 1200000 ) );

      mLine2056Geometry = QgsGeometry::fromPolylineXY( { QgsPointXY( 2500001, 1200001 ), QgsPointXY( 2500002, 1200002 ), QgsPointXY( 2500004, 1200004 ) } );
    }

    VertexModel *mModel;
    QgsGeometry mLineGeometry;
    QgsGeometry mPolygonGeometry;
    QgsGeometry mRingPolygonGeometry;
    QgsGeometry mPoint2056Geometry;
    QgsGeometry mLine2056Geometry;
};

TEST_F( TestVertexModel, Candidates )
{
  mModel->setGeometry( mLineGeometry );
  EXPECT_EQ( mModel->vertexCount(), 7 );
  EXPECT_EQ( mModel->mVertices.at( 0 ).point, QgsPoint( -.5, -.5 ) );
  EXPECT_EQ( mModel->mVertices.at( 2 ).point, QgsPoint( 1, 1 ) );
  EXPECT_EQ( mModel->mVertices.at( 4 ).point, QgsPoint( 3, 3 ) );
  EXPECT_EQ( mModel->mVertices.at( 6 ).point, QgsPoint( 4.5, 4.5 ) );

  mModel->setGeometry( mPolygonGeometry );
  EXPECT_EQ( mModel->vertexCount(), 8 );
  EXPECT_EQ( mModel->mVertices.at( 0 ).point, QgsPoint( 1, 0 ) );
  EXPECT_EQ( mModel->mVertices.at( 1 ).point, QgsPoint( 2, 0 ) );
  EXPECT_EQ( mModel->mVertices.at( 2 ).point, QgsPoint( 2, 1 ) );
  EXPECT_EQ( mModel->mVertices.at( 3 ).point, QgsPoint( 2, 2 ) );
  EXPECT_EQ( mModel->mVertices.at( 4 ).point, QgsPoint( 1, 2 ) );
  EXPECT_EQ( mModel->mVertices.at( 5 ).point, QgsPoint( 0, 2 ) );
  EXPECT_EQ( mModel->mVertices.at( 6 ).point, QgsPoint( 0, 1 ) );
  EXPECT_EQ( mModel->mVertices.at( 7 ).point, QgsPoint( 0, 0 ) );

  mModel->setGeometry( mRingPolygonGeometry );
  EXPECT_EQ( mModel->vertexCount(), 16 );
  EXPECT_EQ( mModel->mVertices.at( 0 ).point, QgsPoint( 2, 0 ) );
  EXPECT_EQ( mModel->mVertices.at( 1 ).point, QgsPoint( 4, 0 ) );
  EXPECT_EQ( mModel->mVertices.at( 2 ).point, QgsPoint( 4, 2 ) );
  EXPECT_EQ( mModel->mVertices.at( 3 ).point, QgsPoint( 4, 4 ) );
  EXPECT_EQ( mModel->mVertices.at( 4 ).point, QgsPoint( 2, 4 ) );
  EXPECT_EQ( mModel->mVertices.at( 5 ).point, QgsPoint( 0, 4 ) );
  EXPECT_EQ( mModel->mVertices.at( 6 ).point, QgsPoint( 0, 2 ) );
  EXPECT_EQ( mModel->mVertices.at( 7 ).point, QgsPoint( 0, 0 ) );
  EXPECT_EQ( mModel->mVertices.at( 8 ).point, QgsPoint( 2, 1 ) );
  EXPECT_EQ( mModel->mVertices.at( 9 ).point, QgsPoint( 3, 1 ) );
  EXPECT_EQ( mModel->mVertices.at( 10 ).point, QgsPoint( 3, 2 ) );
  EXPECT_EQ( mModel->mVertices.at( 11 ).point, QgsPoint( 3, 3 ) );
  EXPECT_EQ( mModel->mVertices.at( 12 ).point, QgsPoint( 2, 3 ) );
  EXPECT_EQ( mModel->mVertices.at( 13 ).point, QgsPoint( 1, 3 ) );
  EXPECT_EQ( mModel->mVertices.at( 14 ).point, QgsPoint( 1, 2 ) );
  EXPECT_EQ( mModel->mVertices.at( 15 ).point, QgsPoint( 1, 1 ) );
}

TEST_F( TestVertexModel, CanRemoveVertex )
{
  // line
  mModel->setGeometry( mLineGeometry );
  EXPECT_TRUE( !mModel->canRemoveVertex() );
  mModel->setEditingMode( VertexModel::EditVertex );
  EXPECT_TRUE( mModel->canRemoveVertex() );

  mModel->setEditingMode( VertexModel::NoEditing );
  EXPECT_EQ( mModel->editingMode(), VertexModel::NoEditing );
  mModel->setCurrentVertex( 1 );
  EXPECT_EQ( mModel->editingMode(), VertexModel::EditVertex );

  EXPECT_EQ( mModel->vertexCount(), 7 );
  mModel->removeCurrentVertex();
  EXPECT_EQ( mModel->vertexCount(), 5 );
  EXPECT_TRUE( !mModel->canRemoveVertex() );

  // polygon
  mModel->setGeometry( mPolygonGeometry );
  EXPECT_EQ( mModel->editingMode(), VertexModel::NoEditing );
  EXPECT_TRUE( !mModel->canRemoveVertex() );
  mModel->setCurrentVertex( 1 );
  EXPECT_EQ( mModel->vertexCount(), 8 );
  mModel->removeCurrentVertex();
  EXPECT_EQ( mModel->vertexCount(), 6 );
  EXPECT_TRUE( !mModel->canRemoveVertex() );
}

TEST_F( TestVertexModel, AddVertex )
{
  mModel->setGeometry( mPolygonGeometry );
  EXPECT_EQ( mModel->vertexCount(), 8 );
  mModel->setEditingMode( VertexModel::AddVertex );
  mModel->setCurrentVertex( 0 );
  mModel->setCurrentPoint( QgsPoint( -3, 0 ) );
  EXPECT_EQ( mModel->vertexCount(), 10 );

  mModel->setGeometry( mLineGeometry );
  mModel->setEditingMode( VertexModel::AddVertex );
  mModel->setCurrentVertex( 2 );
  EXPECT_EQ( mModel->mCurrentIndex, 2 );
  EXPECT_TRUE( mModel->canPreviousVertex() );
  mModel->previous();
  EXPECT_TRUE( !mModel->canPreviousVertex() );
  EXPECT_EQ( mModel->mCurrentIndex, 0 );
  mModel->next();
  EXPECT_EQ( mModel->mCurrentIndex, 2 );

  mModel->setGeometry( mLineGeometry );
  mModel->setEditingMode( VertexModel::AddVertex );
  mModel->setCurrentVertex( 0 );
  EXPECT_EQ( mModel->mCurrentIndex, 0 );
  EXPECT_EQ( mModel->currentPoint(), QgsPoint( -.5, -.5 ) );
  mModel->next();
  EXPECT_EQ( mModel->mCurrentIndex, 2 );
  EXPECT_EQ( mModel->currentPoint(), QgsPoint( 1, 1 ) );
  mModel->next();
  EXPECT_EQ( mModel->mCurrentIndex, 4 );
  EXPECT_TRUE( mModel->canNextVertex() );
  mModel->next();
  EXPECT_EQ( mModel->mCurrentIndex, 6 );
  EXPECT_TRUE( !mModel->canNextVertex() );
  mModel->previous();
  EXPECT_EQ( mModel->mCurrentIndex, 4 );
}

TEST_F( TestVertexModel, EditingMode )
{
  mModel->setGeometry( mRingPolygonGeometry );
  EXPECT_EQ( mModel->editingMode(), VertexModel::NoEditing );
  EXPECT_EQ( mModel->currentVertexIndex(), -1 );
  mModel->setEditingMode( VertexModel::AddVertex );
  EXPECT_EQ( mModel->currentVertexIndex(), 0 );
  EXPECT_EQ( mModel->mVertices.at( 0 ).currentVertex, true );
  mModel->setCurrentPoint( QgsPoint( 1, 0 ) );
  EXPECT_EQ( mModel->currentVertexIndex(), 1 );
  EXPECT_EQ( mModel->currentPoint(), QgsPoint( 1, 0 ) );
  EXPECT_EQ( mModel->mVertices.at( 1 ).point, QgsPoint( 1, 0 ) );
}

TEST_F( TestVertexModel, Transform )
{
  QgsQuickMapSettings mapSettings;
  mapSettings.setDestinationCrs( QgsCoordinateReferenceSystem::fromEpsgId( 4326 ) );
  mModel->setMapSettings( &mapSettings );
  EXPECT_EQ( mModel->mapSettings()->destinationCrs().authid(), QStringLiteral( "EPSG:4326" ) );
  mModel->setGeometry( mPoint2056Geometry );
  mModel->setCrs( QgsCoordinateReferenceSystem::fromEpsgId( 2056 ) );
  const auto &point = mModel->vertex( 0 ).point;

  VERIFYNEAR( point.y(), 46.9435, 0.001 );
  VERIFYNEAR( point.x(), 6.12514, 0.001 );
}

TEST_F( TestVertexModel, SelectVertexAtPosition )
{
  QgsQuickMapSettings mapSettings;
  mapSettings.setDestinationCrs( QgsCoordinateReferenceSystem::fromEpsgId( 4326 ) );
  // mapSettings.setExtent(  );
  mModel->setMapSettings( &mapSettings );

  mModel->setCrs( QgsCoordinateReferenceSystem::fromEpsgId( 2056 ) );
  mModel->setGeometry( mLine2056Geometry );
  EXPECT_EQ( mModel->mCurrentIndex, -1 );

  mModel->selectVertexAtPosition( QgsPoint( 6.12515656, 46.943546146 ), 10 );
  EXPECT_EQ( mModel->mCurrentIndex, 1 );

  EXPECT_EQ( mModel->editingMode(), VertexModel::EditVertex );

  mModel->setEditingMode( VertexModel::AddVertex );
  EXPECT_EQ( mModel->mCurrentIndex, 2 );

  // selecting a candidate will make it a vertex
  EXPECT_EQ( mModel->mVertices.count(), 7 );
  mModel->selectVertexAtPosition( QgsPoint( 6.12515333, 46.94354385 ), 10 );
  EXPECT_EQ( mModel->editingMode(), VertexModel::EditVertex );
  EXPECT_EQ( mModel->mVertices.count(), 9 );
}
#include "test_vertexmodel.moc"
