

# File qfieldguiqmlregistration.cpp



[**FileList**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qfieldguiqmlregistration.cpp**](qfieldguiqmlregistration_8cpp.md)

[Go to the source code of this file](qfieldguiqmlregistration_8cpp_source.md)



* `#include "attributeformmodel.h"`
* `#include "expressionevaluator.h"`
* `#include "expressionvariablemodel.h"`
* `#include "featurechecklistmodel.h"`
* `#include "focusstack.h"`
* `#include "geometryeditorsmodel.h"`
* `#include "localfilesmodel.h"`
* `#include "messagelogmodel.h"`
* `#include "orderedrelationmodel.h"`
* `#include "parameterizedimage.h"`
* `#include "permissions.h"`
* `#include "platformutilities.h"`
* `#include "printlayoutlistmodel.h"`
* `#include "qfieldguiqmlregistration.h"`
* `#include "recentprojectlistmodel.h"`
* `#include "referencingfeaturelistmodel.h"`
* `#include "sensorlistmodel.h"`
* `#include "theme.h"`
* `#include "valuemapmodel.h"`
* `#include <QGuiApplication>`
* `#include <QPermissions>`
* `#include <QScreen>`
* `#include <QtQml>`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**QFieldGui**](namespaceQFieldGui.md) <br> |



















































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**REGISTER\_SINGLETON**](qfieldguiqmlregistration_8cpp.md#define-register_singleton) (uri, \_class, name) `qmlRegisterSingletonType&lt;\_class&gt;( uri, 1, 0, name, []( QQmlEngine \*engine, QJSEngine \*scriptEngine ) -&gt; QObject \* { Q\_UNUSED( engine ); Q\_UNUSED( scriptEngine ); return new \_class(); } )`<br> |

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
The documentation for this class was generated from the following file `src/gui/qfieldguiqmlregistration.cpp`

