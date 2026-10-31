

# File qgsquickelevationprofilecanvas.cpp



[**FileList**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qgsquick**](dir_19c3d7c8ba6dc95926fecc96295d1560.md) **>** [**qgsquickelevationprofilecanvas.cpp**](qgsquickelevationprofilecanvas_8cpp.md)

[Go to the source code of this file](qgsquickelevationprofilecanvas_8cpp_source.md)



* `#include "qgsabstractprofilegenerator.h"`
* `#include "qgsabstractprofilesource.h"`
* `#include "qgscolorutils.h"`
* `#include "qgsexpressioncontextutils.h"`
* `#include "qgsfillsymbol.h"`
* `#include "qgsfillsymbollayer.h"`
* `#include "qgslinesymbol.h"`
* `#include "qgslinesymbollayer.h"`
* `#include "qgsmaplayerelevationproperties.h"`
* `#include "qgsmaplayerutils.h"`
* `#include "qgsplot.h"`
* `#include "qgsprofilerenderer.h"`
* `#include "qgsprofilerequest.h"`
* `#include "qgsprojectelevationproperties.h"`
* `#include "qgsquickelevationprofilecanvas.h"`
* `#include "qgsterrainprovider.h"`
* `#include <QQuickWindow>`
* `#include <QSGSimpleRectNode>`
* `#include <QSGSimpleTextureNode>`
* `#include <QScreen>`
* `#include <QTimer>`















## Classes

| Type | Name |
| ---: | :--- |
| class | [**QgsElevationProfilePlotItem**](classQgsElevationProfilePlotItem.md) <br> |






















## Public Functions

| Type | Name |
| ---: | :--- |
|  QList&lt; QgsMapLayer \* &gt; | [**\_qgis\_listQPointerToRaw**](#function-_qgis_listqpointertoraw) (const QgsWeakMapLayerPointerList & layers) <br> |
|  QgsWeakMapLayerPointerList | [**\_qgis\_listRawToQPointer**](#function-_qgis_listrawtoqpointer) (const QList&lt; QgsMapLayer \* &gt; & layers) <br> |




























## Public Functions Documentation




### function \_qgis\_listQPointerToRaw 

```C++
inline QList< QgsMapLayer * > _qgis_listQPointerToRaw (
    const QgsWeakMapLayerPointerList & layers
) 
```




<hr>



### function \_qgis\_listRawToQPointer 

```C++
inline QgsWeakMapLayerPointerList _qgis_listRawToQPointer (
    const QList< QgsMapLayer * > & layers
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qgsquick/qgsquickelevationprofilecanvas.cpp`

