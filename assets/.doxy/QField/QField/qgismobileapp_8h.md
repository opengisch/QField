

# File qgismobileapp.h



[**FileList**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qgismobileapp.h**](qgismobileapp_8h.md)

[Go to the source code of this file](qgismobileapp_8h_source.md)



* `#include <QtQml/QQmlApplicationEngine>`
* `#include <qgsapplication.h>`
* `#include <qgsconfig.h>`
* `#include <qgsexiftools.h>`
* `#include <qgsmaplayerproxymodel.h>`
* `#include <qgsunittypes.h>`
* `#include "appcoordinateoperationhandlers.h"`
* `#include "bookmarkmodel.h"`
* `#include "clipboardmanager.h"`
* `#include "cogoregistry.h"`
* `#include "drawingtemplatemodel.h"`
* `#include "pluginmanager.h"`
* `#include "qfield_core_export.h"`
* `#include "qfieldappauthrequesthandler.h"`
* `#include "qfieldurlhandler.h"`
* `#include "qgsgpkgflusher.h"`
* `#include "screendimmer.h"`
* `#include "settings.h"`















## Classes

| Type | Name |
| ---: | :--- |
| class | [**QgisMobileapp**](classQgisMobileapp.md) <br> |

















































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**REGISTER\_SINGLETON**](qgismobileapp_8h.md#define-register_singleton) (uri, \_class, name) `qmlRegisterSingletonType&lt;\_class&gt;( uri, 1, 0, name, []( QQmlEngine \*engine, QJSEngine \*scriptEngine ) -&gt; QObject \* { Q\_UNUSED(engine); Q\_UNUSED(scriptEngine); return new \_class(); } )`<br> |

## Macro Definition Documentation





### define REGISTER\_SINGLETON 

```C++
#define REGISTER_SINGLETON (
    uri,
    _class,
    name
) `qmlRegisterSingletonType<_class>( uri, 1, 0, name, []( QQmlEngine *engine, QJSEngine *scriptEngine ) -> QObject * { Q_UNUSED(engine); Q_UNUSED(scriptEngine); return new _class(); } )`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qgismobileapp.h`

