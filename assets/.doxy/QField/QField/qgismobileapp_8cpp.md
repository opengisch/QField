

# File qgismobileapp.cpp



[**FileList**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qgismobileapp.cpp**](qgismobileapp_8cpp.md)

[Go to the source code of this file](qgismobileapp_8cpp_source.md)



* `#include <QApplication>`
* `#include <unistd.h>`
* `#include <stdlib.h>`
* `#include "cpl_conv.h"`
* `#include "cpl_string.h"`
* `#include "cpl_vsi.h"`
* `#include "gdal_version.h"`
* `#include "appexpressioncontextscopesgenerator.h"`
* `#include "appinterface.h"`
* `#include "attributeformmodel.h"`
* `#include "audiorecorder.h"`
* `#include "badlayerhandler.h"`
* `#include "barcodedecoder.h"`
* `#include "barcodeimageprovider.h"`
* `#include "changelogcontents.h"`
* `#include "cogoexecutor.h"`
* `#include "cogooperation.h"`
* `#include "cogooperationsmodel.h"`
* `#include "cogoregistry.h"`
* `#include "coordinatereferencesystemutils.h"`
* `#include "deltafilewrapper.h"`
* `#include "deltalistmodel.h"`
* `#include "digitizinglogger.h"`
* `#include "distancearea.h"`
* `#include "drawingcanvas.h"`
* `#include "expressioncontextutils.h"`
* `#include "expressionevaluator.h"`
* `#include "expressionvariablemodel.h"`
* `#include "externalstorage.h"`
* `#include "featurechecklistmodel.h"`
* `#include "featurehistory.h"`
* `#include "featurelistextentcontroller.h"`
* `#include "featurelistmodel.h"`
* `#include "featurelistmodelselection.h"`
* `#include "featuremodel.h"`
* `#include "featureutils.h"`
* `#include "fileutils.h"`
* `#include "focusstack.h"`
* `#include "geofencer.h"`
* `#include "geometry.h"`
* `#include "geometryeditorsmodel.h"`
* `#include "geometryutils.h"`
* `#include "gnsspositioninformation.h"`
* `#include "gridmodel.h"`
* `#include "identifytool.h"`
* `#include "layerobserver.h"`
* `#include "layerresolver.h"`
* `#include "layertreemapcanvasbridge.h"`
* `#include "layertreemodel.h"`
* `#include "layerutils.h"`
* `#include "legendimageprovider.h"`
* `#include "linepolygonshape.h"`
* `#include "localfilesimageprovider.h"`
* `#include "localfilesmodel.h"`
* `#include "locatormodelsuperbridge.h"`
* `#include "maplayermodel.h"`
* `#include "maptoscreen.h"`
* `#include "messagelogmodel.h"`
* `#include "navigation.h"`
* `#include "navigationmodel.h"`
* `#include "nearfieldreader.h"`
* `#include "orderedrelationmodel.h"`
* `#include "parametizedimage.h"`
* `#include "permissions.h"`
* `#include "platformutilities.h"`
* `#include "pluginmodel.h"`
* `#include "positioning.h"`
* `#include "positioningdevicemodel.h"`
* `#include "positioninginformationmodel.h"`
* `#include "positioningutils.h"`
* `#include "printlayoutlistmodel.h"`
* `#include "processingalgorithm.h"`
* `#include "processingalgorithmparametersmodel.h"`
* `#include "processingalgorithmsmodel.h"`
* `#include "processingutils.h"`
* `#include "projectinfo.h"`
* `#include "projectsimageprovider.h"`
* `#include "projectsource.h"`
* `#include "projectutils.h"`
* `#include "qfield.h"`
* `#include "qfieldcloudconnection.h"`
* `#include "qfieldcloudproject.h"`
* `#include "qfieldcloudprojectsmodel.h"`
* `#include "qfieldcloudutils.h"`
* `#include "qfieldlocatorfilter.h"`
* `#include "qfieldurlhandler.h"`
* `#include "qgismobileapp.h"`
* `#include "qgsgeometrywrapper.h"`
* `#include "qgsproviderregistry.h"`
* `#include "qgsprovidersublayerdetails.h"`
* `#include "qgsquickcoordinatetransformer.h"`
* `#include "qgsquickelevationprofilecanvas.h"`
* `#include "qgsquickmapcanvasmap.h"`
* `#include "qgsquickmapsettings.h"`
* `#include "qgsquickmaptransform.h"`
* `#include "quick3dmaptexturedata.h"`
* `#include "quick3drubberbandgeometry.h"`
* `#include "quick3dterraingeometry.h"`
* `#include "quick3dterrainprovider.h"`
* `#include "recentprojectlistmodel.h"`
* `#include "referencingfeaturelistmodel.h"`
* `#include "relationutils.h"`
* `#include "resourcesource.h"`
* `#include "rubberbandmodel.h"`
* `#include "rubberbandshape.h"`
* `#include "scalebarmeasurement.h"`
* `#include "sensorlistmodel.h"`
* `#include "snappingresult.h"`
* `#include "snappingutils.h"`
* `#include "stringutils.h"`
* `#include "submodel.h"`
* `#include "theme.h"`
* `#include "trackingmodel.h"`
* `#include "urlutils.h"`
* `#include "valuemapmodel.h"`
* `#include "vertexmodel.h"`
* `#include "webdavconnection.h"`
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
* `#include <qgslocatorcontext.h>`
* `#include <qgslocatormodel.h>`
* `#include <qgsmaplayer.h>`
* `#include <qgsmaplayerstyle.h>`
* `#include <qgsmapthemecollection.h>`
* `#include <qgsmessagelog.h>`
* `#include <qgsnetworkaccessmanager.h>`
* `#include <qgsofflineediting.h>`
* `#include <qgsprintlayout.h>`
* `#include <qgsproject.h>`
* `#include <qgsprojectdisplaysettings.h>`
* `#include <qgsprojectelevationproperties.h>`
* `#include <qgsprojectstorage.h>`
* `#include <qgsprojectstorageregistry.h>`
* `#include <qgsprojectstylesettings.h>`
* `#include <qgsprojectviewsettings.h>`
* `#include <qgsrasterlayer.h>`
* `#include <qgsrasterresamplefilter.h>`
* `#include <qgsrelationmanager.h>`
* `#include <qgssettingsregistrycore.h>`
* `#include <qgssinglesymbolrenderer.h>`
* `#include <qgssnappingutils.h>`
* `#include <qgstemporalutils.h>`
* `#include <qgsterrainprovider.h>`
* `#include <qgsunittypes.h>`
* `#include <qgsvectorlayer.h>`
* `#include <qgsvectorlayereditbuffer.h>`
* `#include <qgsvectorlayertemporalproperties.h>`
































































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
The documentation for this class was generated from the following file `src/core/qgismobileapp.cpp`

