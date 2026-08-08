

# File qgismobileapp.cpp



[**FileList**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qgismobileapp.cpp**](qgismobileapp_8cpp.md)

[Go to the source code of this file](qgismobileapp_8cpp_source.md)



* `#include <QApplication>`
* `#include <unistd.h>`
* `#include <stdlib.h>`
* `#include "cpl_conv.h"`
* `#include "cpl_string.h"`
* `#include "cpl_vsi.h"`
* `#include "gdal_version.h"`
* `#include "appinterface.h"`
* `#include "barcodeimageprovider.h"`
* `#include "cogoregistry.h"`
* `#include "distancearea.h"`
* `#include "featurehistory.h"`
* `#include "featurelistmodel.h"`
* `#include "featureutils.h"`
* `#include "geometryutils.h"`
* `#include "gnsspositioninformation.h"`
* `#include "identifytool.h"`
* `#include "layerobserver.h"`
* `#include "layerresolver.h"`
* `#include "layertreemapcanvasbridge.h"`
* `#include "layertreemodel.h"`
* `#include "layerutils.h"`
* `#include "legendimageprovider.h"`
* `#include "linepolygonshape.h"`
* `#include "localfilesimageprovider.h"`
* `#include "locatormodelsuperbridge.h"`
* `#include "messagelogmodel.h"`
* `#include "nearfieldreader.h"`
* `#include "ntripsourcetablefetcher.h"`
* `#include "platformutilities.h"`
* `#include "pluginmodel.h"`
* `#include "positioningsource.h"`
* `#include "printlayoutlistmodel.h"`
* `#include "processingalgorithm.h"`
* `#include "processingalgorithmsmodel.h"`
* `#include "projectinfo.h"`
* `#include "projectsimageprovider.h"`
* `#include "qfield.h"`
* `#include "qfield3dqmlregistration.h"`
* `#include "qfieldappqmlregistration.h"`
* `#include "qfieldcloudconnection.h"`
* `#include "qfieldcloudproject.h"`
* `#include "qfieldcloudprojectsmodel.h"`
* `#include "qfieldcloudutils.h"`
* `#include "qfieldcoreqmlregistration.h"`
* `#include "qfieldguiqmlregistration.h"`
* `#include "qfieldurlhandler.h"`
* `#include "qgismobileapp.h"`
* `#include "qgsproviderregistry.h"`
* `#include "qgsprovidersublayerdetails.h"`
* `#include "qgsquickmapcanvasmap.h"`
* `#include "qgsquickmapsettings.h"`
* `#include "quick3dterrainprovider.h"`
* `#include "recentprojectlistmodel.h"`
* `#include "referencingfeaturelistmodel.h"`
* `#include "rubberbandmodel.h"`
* `#include "rubberbandshape.h"`
* `#include "scalebarmeasurement.h"`
* `#include "sensorlistmodel.h"`
* `#include "snappingresult.h"`
* `#include "snappingutils.h"`
* `#include "trackingmodel.h"`
* `#include "vertexmodel.h"`
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
* `#include <QStyleHints>`
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

