#include <QtTest>

#include "test_vertexmodel.cpp"
#include "test_referencingfeaturelistmodel.cpp"

int main( int argc, char **argv )
{
  int status = 0;

  auto closer_lambda = []( QgsApplication * app ) { app->exitQgis(); };
  std::unique_ptr<QgsApplication, decltype( closer_lambda )> app( new QgsApplication( argc, nullptr, false ), closer_lambda );

#ifdef ANDROID
  app->setPrefixPath( "" QGIS_INSTALL_DIR, true );
  app->setPluginPath( QApplication::applicationDirPath() );
  app->setPkgDataPath( AndroidPlatformUtilities().packagePath() );
#else
  app->setPrefixPath( CMAKE_INSTALL_PREFIX, true );
#endif
  app->initQgis();

  auto ASSERT_TEST = [&status, argc, argv]( QObject * obj )
  {
    status |= QTest::qExec( obj, argc, argv );
    delete obj;
  };

  ASSERT_TEST( new TestVertexModel() );
  ASSERT_TEST( new TestReferencingFeatureListModel() );

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
