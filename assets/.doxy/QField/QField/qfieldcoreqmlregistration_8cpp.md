

# File qfieldcoreqmlregistration.cpp



[**FileList**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldcoreqmlregistration.cpp**](qfieldcoreqmlregistration_8cpp.md)

[Go to the source code of this file](qfieldcoreqmlregistration_8cpp_source.md)



* `#include "appexpressioncontextscopesgenerator.h"`
* `#include "appinterface.h"`
* `#include "audioanalyzer.h"`
* `#include "audiorecorder.h"`
* `#include "barcodedecoder.h"`
* `#include "bookmarkmodel.h"`
* `#include "cameraorientationnormalizer.h"`
* `#include "cogo/cogoexecutor.h"`
* `#include "cogo/cogooperation.h"`
* `#include "cogo/cogooperationsmodel.h"`
* `#include "digitizinglogger.h"`
* `#include "distancearea.h"`
* `#include "drawingcanvas.h"`
* `#include "externalstorage.h"`
* `#include "featurelistextentcontroller.h"`
* `#include "featurelistmodel.h"`
* `#include "featurelistmodelselection.h"`
* `#include "featuremodel.h"`
* `#include "geometry.h"`
* `#include "gridmodel.h"`
* `#include "identifytool.h"`
* `#include "layerresolver.h"`
* `#include "layertreemodel.h"`
* `#include "linepolygonshape.h"`
* `#include "locator/locatormodelsuperbridge.h"`
* `#include "locator/qfieldlocatorfilter.h"`
* `#include "maplayermodel.h"`
* `#include "maptoscreen.h"`
* `#include "multifeaturelistmodel.h"`
* `#include "navigation.h"`
* `#include "navigationmodel.h"`
* `#include "nearfieldreader.h"`
* `#include "platforms/platformutilities.h"`
* `#include "positioning/abstractgnssreceiver.h"`
* `#include "positioning/geofencer.h"`
* `#include "positioning/gnsspositioninformation.h"`
* `#include "positioning/ntripsettings.h"`
* `#include "positioning/ntripsourcetablefetcher.h"`
* `#include "positioning/positioning.h"`
* `#include "positioning/positioningdevicemodel.h"`
* `#include "positioning/positioninginformationmodel.h"`
* `#include "processing/processingalgorithm.h"`
* `#include "processing/processingalgorithmparametersmodel.h"`
* `#include "processing/processingalgorithmsmodel.h"`
* `#include "projectinfo.h"`
* `#include "projectsource.h"`
* `#include "qfieldcloud/deltafilewrapper.h"`
* `#include "qfieldcloud/deltalistmodel.h"`
* `#include "qfieldcloud/layerobserver.h"`
* `#include "qfieldcloud/qfieldcloudconnection.h"`
* `#include "qfieldcloud/qfieldcloudproject.h"`
* `#include "qfieldcloud/qfieldcloudprojectsmodel.h"`
* `#include "qfieldcloud/qfieldcloudstatus.h"`
* `#include "qfieldcoreqmlregistration.h"`
* `#include "qgsgeometrywrapper.h"`
* `#include "qgsgpkgflusher.h"`
* `#include "qgsquick/qgsquickcoordinatetransformer.h"`
* `#include "qgsquick/qgsquickelevationprofilecanvas.h"`
* `#include "qgsquick/qgsquickmapcanvasmap.h"`
* `#include "qgsquick/qgsquickmapsettings.h"`
* `#include "qgsquick/qgsquickmaptransform.h"`
* `#include "resourcesource.h"`
* `#include "rubberbandmodel.h"`
* `#include "rubberbandshape.h"`
* `#include "scalebarmeasurement.h"`
* `#include "settings.h"`
* `#include "submodel.h"`
* `#include "tracker.h"`
* `#include "trackingmodel.h"`
* `#include "utils/authutils.h"`
* `#include "utils/coordinatereferencesystemutils.h"`
* `#include "utils/expressioncontextutils.h"`
* `#include "utils/featureutils.h"`
* `#include "utils/fileutils.h"`
* `#include "utils/geometryutils.h"`
* `#include "utils/layerutils.h"`
* `#include "utils/positioningutils.h"`
* `#include "utils/processingutils.h"`
* `#include "utils/projectutils.h"`
* `#include "utils/qfieldcloudutils.h"`
* `#include "utils/relationutils.h"`
* `#include "utils/snappingutils.h"`
* `#include "utils/stringutils.h"`
* `#include "utils/urlutils.h"`
* `#include "vertexmodel.h"`
* `#include "videosinkcapture.h"`
* `#include "viewstatus.h"`
* `#include "webdavconnection.h"`
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
| namespace | [**QFieldCore**](namespaceQFieldCore.md) <br> |



















































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**REGISTER\_SINGLETON**](qfieldcoreqmlregistration_8cpp.md#define-register_singleton) (uri, \_class, name) `qmlRegisterSingletonType&lt;\_class&gt;( uri, 1, 0, name, []( QQmlEngine \*engine, QJSEngine \*scriptEngine ) -&gt; QObject \* { Q\_UNUSED( engine ); Q\_UNUSED( scriptEngine ); return new \_class(); } )`<br> |

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
The documentation for this class was generated from the following file `src/core/qfieldcoreqmlregistration.cpp`

