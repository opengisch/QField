#include <qgsapplication.h>
#include <qgis.h>

class QFieldTestBase
{
  public:
    QFieldTestBase()
    {
        int argc = 0;
        mApp = qgis::make_unique<QgsApplication>( argc, nullptr, false );

#ifdef ANDROID
      mApp->setPrefixPath( "" QGIS_INSTALL_DIR, true );
      mApp->setPluginPath( QApplication::applicationDirPath() );
      mApp->setPkgDataPath( AndroidPlatformUtilities().packagePath() );
#else
      mApp->setPrefixPath( CMAKE_INSTALL_PREFIX, true );
#endif
      mApp->initQgis();
    }

    ~QFieldTestBase()
    {
        mApp->exitQgis();
    }
  private:
    std::unique_ptr<QgsApplication> mApp;
};
