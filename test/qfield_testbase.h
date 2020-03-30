#include <qgis.h>
#include <qgsapplication.h>

#define QFIELDTEST_MAIN( TestObject )                \
  QT_BEGIN_NAMESPACE                                 \
  QTEST_ADD_GPU_BLACKLIST_SUPPORT_DEFS               \
  QT_END_NAMESPACE                                   \
  int main( int argc, char *argv[] )                 \
  {                                                  \
    QgsApplication app( argc, argv, false );         \
    app.init();                                      \
    app.setPrefixPath( CMAKE_INSTALL_PREFIX, true ); \
    app.initQgis();                                  \
    app.setAttribute( Qt::AA_Use96Dpi, true );       \
    QTEST_DISABLE_KEYPAD_NAVIGATION                  \
    QTEST_ADD_GPU_BLACKLIST_SUPPORT                  \
    TestObject tc;                                   \
    QTEST_SET_MAIN_SOURCE_PATH                       \
    return QTest::qExec( &tc, argc, argv );          \
    app.exitQgis();                                  \
  }
