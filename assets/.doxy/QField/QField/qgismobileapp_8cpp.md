

# File qgismobileapp.cpp



[**FileList**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qgismobileapp.cpp**](qgismobileapp_8cpp.md)

[Go to the source code of this file](qgismobileapp_8cpp_source.md)



* `#include <QApplication>`
* `#include <unistd.h>`
* `#include <stdlib.h>`
* `#include "cpl_conv.h"`
* `#include "cpl_string.h"`
* `#include "cpl_vsi.h"`
* `#include "qf3d.h"`
* `#include "qf3dterrainprovider.h"`
* `#include "qfapp.h"`
* `#include "qfappinterface.h"`
* `#include "qfbarcodeimageprovider.h"`
* `#include "qfcloudconnection.h"`
* `#include "qfcloudproject.h"`
* `#include "qfcloudprojectsmodel.h"`
* `#include "qfcloudutils.h"`
* `#include "qfcogoregistry.h"`
* `#include "qfcore.h"`
* `#include "qfdistancearea.h"`
* `#include "qffeaturehistory.h"`
* `#include "qffeaturelistmodel.h"`
* `#include "qffeatureutils.h"`
* `#include "qfgeometryutils.h"`
* `#include "qfgnsspositioninformation.h"`
* `#include "qfgui.h"`
* `#include "qfidentifytool.h"`
* `#include "qfield.h"`
* `#include "qflayerobserver.h"`
* `#include "qflayerresolver.h"`
* `#include "qflayertreemapcanvasbridge.h"`
* `#include "qflayertreemodel.h"`
* `#include "qflayerutils.h"`
* `#include "qflegendimageprovider.h"`
* `#include "qflinepolygonshape.h"`
* `#include "qflocalfilesimageprovider.h"`
* `#include "qflocatormodelsuperbridge.h"`
* `#include "qfmessagelogmodel.h"`
* `#include "qfntripsourcetablefetcher.h"`
* `#include "qfplatformutilities.h"`
* `#include "qfpluginmodel.h"`
* `#include "qfpositioningsource.h"`
* `#include "qfprintlayoutlistmodel.h"`
* `#include "qfprocessingalgorithm.h"`
* `#include "qfprocessingalgorithmsmodel.h"`
* `#include "qfprojectinfo.h"`
* `#include "qfprojectsimageprovider.h"`
* `#include "qfrecentprojectlistmodel.h"`
* `#include "qfreferencingfeaturelistmodel.h"`
* `#include "qfrubberbandmodel.h"`
* `#include "qfrubberbandshape.h"`
* `#include "qfscalebarmeasurement.h"`
* `#include "qfsensorlistmodel.h"`
* `#include "qfsnappingresult.h"`
* `#include "qfsnappingutils.h"`
* `#include "qftrackingmodel.h"`
* `#include "qfurlhandler.h"`
* `#include "qfvertexmodel.h"`
* `#include "qgismobileapp.h"`
* `#include "qgsproviderregistry.h"`
* `#include "qgsprovidersublayerdetails.h"`
* `#include "qgsquickmapcanvasmap.h"`
* `#include "qgsquickmapsettings.h"`
* `#include <QDateTime>`
* `#include <QDesktopServices>`
* `#include <QFileInfo>`
* `#include <QFontDatabase>`
* `#include <QPalette>`
* `#include <QPermissions>`
* `#include <QQmlFileSelector>`
* `#include <QResource>`
* `#include <QScreen>`
* `#include <QSslConfiguration>`
* `#include <QtQml/QQmlApplicationEngine>`
* `#include <QtQml/QQmlContext>`
* `#include <QtQml/QQmlEngine>`
* `#include <qgsauthmanager.h>`
* `#include <qgsbilinearrasterresampler.h>`
* `#include <qgscoordinatereferencesystem.h>`
* `#include <qgsexpressionfunction.h>`
* `#include <qgsfeature.h>`
* `#include <qgsfield.h>`
* `#include <qgsfieldconstraints.h>`
* `#include <qgsfontmanager.h>`
* `#include <qgsgeopackageprojectstorage.h>`
* `#include <qgslayertree.h>`
* `#include <qgslayertreemodel.h>`
* `#include <qgslayertreeregistrybridge.h>`
* `#include <qgslayoutatlas.h>`
* `#include <qgslayoutexporter.h>`
* `#include <qgslayoutitemlabel.h>`
* `#include <qgslayoutitemmap.h>`
* `#include <qgslayoutmanager.h>`
* `#include <qgslayoutpagecollection.h>`
* `#include <qgslocalizeddatapathregistry.h>`
* `#include <qgslocator.h>`
* `#include <qgsmaplayer.h>`
* `#include <qgsmaplayerstyle.h>`
* `#include <qgsmessagelog.h>`
* `#include <qgsnetworkaccessmanager.h>`
* `#include <qgsofflineediting.h>`
* `#include <qgsprintlayout.h>`
* `#include <qgsproject.h>`
* `#include <qgsprojectelevationproperties.h>`
* `#include <qgsprojectstorage.h>`
* `#include <qgsprojectstorageregistry.h>`
* `#include <qgsprojectstylesettings.h>`
* `#include <qgsprojectviewsettings.h>`
* `#include <qgsrasterlayer.h>`
* `#include <qgsrasterresamplefilter.h>`
* `#include <qgsscreenproperties.h>`
* `#include <qgssettingsregistrycore.h>`
* `#include <qgssinglesymbolrenderer.h>`
* `#include <qgstemporalutils.h>`
* `#include <qgsterrainprovider.h>`
* `#include <qgsunittypes.h>`
* `#include <qgsvectorlayer.h>`
* `#include <qgsvectorlayertemporalproperties.h>`
* `#include <qgsvectortilelayer.h>`
































































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**CPL\_SUPRESS\_CPLUSPLUS**](qgismobileapp_8cpp.md#define-cpl_supress_cplusplus)  <br> |
| define  | [**QUOTE**](qgismobileapp_8cpp.md#define-quote) (string) `\_QUOTE( string )`<br> |
| define  | [**\_QUOTE**](qgismobileapp_8cpp.md#define-_quote) (string) `#string`<br> |

## Macro Definition Documentation





### define CPL\_SUPRESS\_CPLUSPLUS 

```C++
#define CPL_SUPRESS_CPLUSPLUS 
```




<hr>



### define QUOTE 

```C++
#define QUOTE (
    string
) `_QUOTE( string )`
```




<hr>



### define \_QUOTE 

```C++
#define _QUOTE (
    string
) `#string`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/app/qgismobileapp.cpp`

