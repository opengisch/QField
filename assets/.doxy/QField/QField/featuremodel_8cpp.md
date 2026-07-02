

# File featuremodel.cpp



[**FileList**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**featuremodel.cpp**](featuremodel_8cpp.md)

[Go to the source code of this file](featuremodel_8cpp_source.md)



* `#include "expressioncontextutils.h"`
* `#include "featuremodel.h"`
* `#include "layerutils.h"`
* `#include "qgsquickmapsettings.h"`
* `#include "vertexmodel.h"`
* `#include <QJSValue>`
* `#include <QMutex>`
* `#include <qgscurvepolygon.h>`
* `#include <qgsexpressioncontextutils.h>`
* `#include <qgsgeometrycollection.h>`
* `#include <qgsgeometryoptions.h>`
* `#include <qgsmessagelog.h>`
* `#include <qgsproject.h>`
* `#include <qgsrelationmanager.h>`
* `#include <qgsvectorlayer.h>`
* `#include <qgsvectorlayereditutils.h>`
* `#include <qgsvectorlayerutils.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef QMap&lt; QgsVectorLayer \*, [**FeatureModel::RememberValues**](structFeatureModel_1_1RememberValues.md) &gt; | [**Rememberings**](#typedef-rememberings)  <br> |
















































## Public Types Documentation




### typedef Rememberings 

```C++
typedef QMap<QgsVectorLayer *, FeatureModel::RememberValues> Rememberings;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/featuremodel.cpp`

