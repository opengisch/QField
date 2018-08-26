

#include <QtTest>


#include <qgslinestring.h>
#include <qgsgeometry.h>
#include <qgspoint.h>
#include <qgspointxy.h>

#include "vertexmodel.h"


class TestVertexModel: public QObject
{
    Q_OBJECT
  private slots:
    void initTestCase()
    {
      mModel = new VertexModel();

      QgsLineString *lineString = new QgsLineString( QVector<QgsPoint>() << QgsPoint( 1, 1 ) << QgsPoint( 2, 1 ) << QgsPoint( 3, 1 ) );
      mLineGeometry = QgsGeometry( lineString );

      mPolygonGeometry = QgsGeometry::fromPolygonXY( QVector<QVector<QgsPointXY>>()
                         <<  ( QVector<QgsPointXY>()
                               << QgsPointXY( 0, 0 )
                               << QgsPointXY( 2, 0 )
                               << QgsPointXY( 2, 2 )
                               << QgsPointXY( 0, 2 )
                               << QgsPointXY( 0, 0 ) ) ) ;
    }

    void canRemoveVertexTest()
    {
      // line
      mModel->setGeometry( mLineGeometry, QgsCoordinateReferenceSystem() );
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
      mModel->setGeometry(mPolygonGeometry, QgsCoordinateReferenceSystem());
      QCOMPARE( mModel->editingMode(), VertexModel::NoEditing );
      QVERIFY( !mModel->canRemoveVertex() );
      mModel->setCurrentVertex( 0 );
      mModel->removeCurrentVertex();
      QCOMPARE( mModel->vertexCount(), 3 );
      QVERIFY( !mModel->canRemoveVertex() );
    }

    void transformTest()
    {
      // TODO
    }

    void returnGeometryTest()
    {
      // TODO
    }

    void xxxTest()
    {
      // TODO
    }

    void yyyTest()
    {
      // TODO
    }

    void zzzTest()
    {
      // TODO
    }

    void cleanupTestCase()
    {
      delete mModel;
    }

  private:
    VertexModel *mModel;
    QgsGeometry mLineGeometry;
    QgsGeometry mPolygonGeometry;
};

QTEST_MAIN( TestVertexModel )
#include "test_vertexmodel.moc"
