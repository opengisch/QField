#include <QtTest>

#include "test_vertexmodel.cpp"
#include "test_referencingfeaturelistmodel.cpp"

int main( int argc, char **argv )
{
  int status = 0;

  auto ASSERT_TEST = [&status, argc, argv]( QObject * obj )
  {
    status |= QTest::qExec( obj, argc, argv );
    delete obj;
  };

  ASSERT_TEST( new TestReferencingFeatureListModel() );
  ASSERT_TEST( new TestVertexModel() );

  /*
  {
     TestVertexModel testVertexModel;
     status |= QTest::qExec(&testVertexModel, argc, argv);
  }
  {
     TestReferencingFeatureListModel testReferencingFeatureListModel;
     status |= QTest::qExec(&testReferencingFeatureListModel, argc, argv);
  }
  */
  return status;
}
