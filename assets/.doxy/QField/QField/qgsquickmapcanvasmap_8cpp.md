

# File qgsquickmapcanvasmap.cpp



[**FileList**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qgsquick**](dir_19c3d7c8ba6dc95926fecc96295d1560.md) **>** [**qgsquickmapcanvasmap.cpp**](qgsquickmapcanvasmap_8cpp.md)

[Go to the source code of this file](qgsquickmapcanvasmap_8cpp_source.md)



* `#include "qgsquickmapcanvasmap.h"`
* `#include "qgsquickmapsettings.h"`
* `#include <QQuickWindow>`
* `#include <QSGSimpleTextureNode>`
* `#include <QScreen>`
* `#include <qgis.h>`
* `#include <qgsannotationlayer.h>`
* `#include <qgsexpressioncontextutils.h>`
* `#include <qgsgrouplayer.h>`
* `#include <qgslabelingresults.h>`
* `#include <qgsmaplayerelevationproperties.h>`
* `#include <qgsmaplayertemporalproperties.h>`
* `#include <qgsmaprenderercache.h>`
* `#include <qgsmaprendererparalleljob.h>`
* `#include <qgsmaprenderersequentialjob.h>`
* `#include <qgsmessagelog.h>`
* `#include <qgspallabeling.h>`
* `#include <qgsproject.h>`
* `#include <qgssymbollayerutils.h>`
* `#include <qgsvectorlayer.h>`





































## Public Functions

| Type | Name |
| ---: | :--- |
|  QList&lt; QgsMapLayer \* &gt; | [**filterLayersForRender**](#function-filterlayersforrender) (const QList&lt; QgsMapLayer \* &gt; & layers) <br> |




























## Public Functions Documentation




### function filterLayersForRender 

```C++
QList< QgsMapLayer * > filterLayersForRender (
    const QList< QgsMapLayer * > & layers
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qgsquick/qgsquickmapcanvasmap.cpp`

