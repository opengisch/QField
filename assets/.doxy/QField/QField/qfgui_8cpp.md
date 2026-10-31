

# File qfgui.cpp



[**FileList**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qfgui.cpp**](qfgui_8cpp.md)

[Go to the source code of this file](qfgui_8cpp_source.md)



* `#include "qfattributeformmodel.h"`
* `#include "qfexpressionevaluator.h"`
* `#include "qfexpressionvariablemodel.h"`
* `#include "qffeaturechecklistmodel.h"`
* `#include "qffocusstack.h"`
* `#include "qfgeometryeditorsmodel.h"`
* `#include "qfgui.h"`
* `#include "qflocalfilesmodel.h"`
* `#include "qfmessagelogmodel.h"`
* `#include "qforderedrelationmodel.h"`
* `#include "qfparameterizedimage.h"`
* `#include "qfpermissions.h"`
* `#include "qfplatformutilities.h"`
* `#include "qfprintlayoutlistmodel.h"`
* `#include "qfrecentprojectlistmodel.h"`
* `#include "qfreferencingfeaturelistmodel.h"`
* `#include "qfsensorlistmodel.h"`
* `#include "qftheme.h"`
* `#include "qfvaluemapmodel.h"`
* `#include <QGuiApplication>`
* `#include <QPermissions>`
* `#include <QScreen>`
* `#include <QtQml>`













## Namespaces

| Type | Name |
| ---: | :--- |
| namespace | [**QfGui**](namespaceQfGui.md) <br> |



















































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**REGISTER\_SINGLETON**](qfgui_8cpp.md#define-register_singleton) (uri, \_class, name) `qmlRegisterSingletonType&lt;\_class&gt;( uri, 1, 0, name, []( QQmlEngine \*engine, QJSEngine \*scriptEngine ) -&gt; QObject \* { Q\_UNUSED( engine ); Q\_UNUSED( scriptEngine ); return new \_class(); } )`<br> |

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
The documentation for this class was generated from the following file `src/gui/qfgui.cpp`

