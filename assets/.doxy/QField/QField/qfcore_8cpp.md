

# File qfcore.cpp



[**FileList**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfcore.cpp**](qfcore_8cpp.md)

[Go to the source code of this file](qfcore_8cpp_source.md)



* `#include "cogo/qfcogoexecutor.h"`
* `#include "cogo/qfcogooperation.h"`
* `#include "cogo/qfcogooperationsmodel.h"`
* `#include "locator/qflocatorfilter.h"`
* `#include "locator/qflocatormodelsuperbridge.h"`
* `#include "platforms/qfplatformutilities.h"`
* `#include "positioning/qfabstractgnssreceiver.h"`
* `#include "positioning/qfgeofencer.h"`
* `#include "positioning/qfgnsspositioninformation.h"`
* `#include "positioning/qfntripsettings.h"`
* `#include "positioning/qfntripsourcetablefetcher.h"`
* `#include "positioning/qfpositioning.h"`
* `#include "positioning/qfpositioningdevicemodel.h"`
* `#include "positioning/qfpositioninginformationmodel.h"`
* `#include "processing/qfprocessingalgorithm.h"`
* `#include "processing/qfprocessingalgorithmparametersmodel.h"`
* `#include "processing/qfprocessingalgorithmsmodel.h"`
* `#include "qfappexpressioncontextscopesgenerator.h"`
* `#include "qfappinterface.h"`
* `#include "qfaudioanalyzer.h"`
* `#include "qfaudiorecorder.h"`
* `#include "qfbarcodedecoder.h"`
* `#include "qfbookmarkmodel.h"`
* `#include "qfcameraorientationnormalizer.h"`
* `#include "qfcore.h"`
* `#include "qfdigitizinglogger.h"`
* `#include "qfdistancearea.h"`
* `#include "qfdrawingcanvas.h"`
* `#include "qfexternalstorage.h"`
* `#include "qffeaturelistextentcontroller.h"`
* `#include "qffeaturelistmodel.h"`
* `#include "qffeaturelistmodelselection.h"`
* `#include "qffeaturemodel.h"`
* `#include "qfgeometry.h"`
* `#include "qfgeometrywrapper.h"`
* `#include "qfgridmodel.h"`
* `#include "qfidentifytool.h"`
* `#include "qfield.h"`
* `#include "qfieldcloud/qfcloudconnection.h"`
* `#include "qfieldcloud/qfcloudproject.h"`
* `#include "qfieldcloud/qfcloudprojectsmodel.h"`
* `#include "qfieldcloud/qfcloudstatus.h"`
* `#include "qfieldcloud/qfdeltachangesmodel.h"`
* `#include "qfieldcloud/qfdeltafilewrapper.h"`
* `#include "qfieldcloud/qfdeltalistmodel.h"`
* `#include "qfieldcloud/qflayerobserver.h"`
* `#include "qflayerresolver.h"`
* `#include "qflayertreemodel.h"`
* `#include "qflinepolygonshape.h"`
* `#include "qfmaplayermodel.h"`
* `#include "qfmaptoscreen.h"`
* `#include "qfmultifeaturelistmodel.h"`
* `#include "qfnavigation.h"`
* `#include "qfnavigationmodel.h"`
* `#include "qfnearfieldreader.h"`
* `#include "qfprojectinfo.h"`
* `#include "qfprojectsource.h"`
* `#include "qfresourcesource.h"`
* `#include "qfrubberbandmodel.h"`
* `#include "qfrubberbandshape.h"`
* `#include "qfscalebarmeasurement.h"`
* `#include "qfsettings.h"`
* `#include "qfsubmodel.h"`
* `#include "qftracker.h"`
* `#include "qftrackingmodel.h"`
* `#include "qfvertexmodel.h"`
* `#include "qfvideosinkcapture.h"`
* `#include "qfviewstatus.h"`
* `#include "qfwebdavconnection.h"`
* `#include "qgsgpkgflusher.h"`
* `#include "qgsquick/qgsquickcoordinatetransformer.h"`
* `#include "qgsquick/qgsquickelevationprofilecanvas.h"`
* `#include "qgsquick/qgsquickmapcanvasmap.h"`
* `#include "qgsquick/qgsquickmapsettings.h"`
* `#include "qgsquick/qgsquickmaptransform.h"`
* `#include "utils/qfauthutils.h"`
* `#include "utils/qfcloudutils.h"`
* `#include "utils/qfcoordinatereferencesystemutils.h"`
* `#include "utils/qfexpressioncontextutils.h"`
* `#include "utils/qffeatureutils.h"`
* `#include "utils/qffileutils.h"`
* `#include "utils/qfgeometryutils.h"`
* `#include "utils/qflayerutils.h"`
* `#include "utils/qfpositioningutils.h"`
* `#include "utils/qfprocessingutils.h"`
* `#include "utils/qfprojectutils.h"`
* `#include "utils/qfrelationutils.h"`
* `#include "utils/qfsnappingutils.h"`
* `#include "utils/qfstringutils.h"`
* `#include "utils/qfurlutils.h"`
* `#include <QAbstractSocket>`
* `#include <QtQml>`
* `#include <qgis.h>`
* `#include <qgslocatorcontext.h>`
* `#include <qgslocatormodel.h>`
* `#include <qgsmaplayer.h>`
* `#include <qgsmapthemecollection.h>`
* `#include <qgsproject.h>`
* `#include <qgsprojectdisplaysettings.h>`
* `#include <qgsrasterlayer.h>`
* `#include <qgsrelationmanager.h>`
* `#include <qgssnappingutils.h>`
* `#include <qgsvectorlayer.h>`
* `#include <qgsvectorlayereditbuffer.h>`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**QfCore**](namespaceQfCore.md) <br> |



















































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**REGISTER\_SINGLETON**](qfcore_8cpp.md#define-register_singleton) (uri, \_class, name) `qmlRegisterSingletonType&lt;\_class&gt;( uri, 1, 0, name, []( QQmlEngine \*engine, QJSEngine \*scriptEngine ) -&gt; QObject \* { Q\_UNUSED( engine ); Q\_UNUSED( scriptEngine ); return new \_class(); } )`<br> |

## Macro Definition Documentation





### define REGISTER\_SINGLETON 

```C++
#define REGISTER_SINGLETON (
    uri,
    _class,
    name
) `qmlRegisterSingletonType<_class>( uri, 1, 0, name, []( QQmlEngine *engine, QJSEngine *scriptEngine ) -> QObject * { Q_UNUSED( engine ); Q_UNUSED( scriptEngine ); return new _class(); } )`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfcore.cpp`

