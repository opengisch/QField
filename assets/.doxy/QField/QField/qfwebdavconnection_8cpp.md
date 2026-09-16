

# File qfwebdavconnection.cpp



[**FileList**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfwebdavconnection.cpp**](qfwebdavconnection_8cpp.md)

[Go to the source code of this file](qfwebdavconnection_8cpp_source.md)



* `#include "qfplatformutilities.h"`
* `#include "qfwebdavconnection.h"`
* `#include <QCryptographicHash>`
* `#include <QDir>`
* `#include <QDirIterator>`
* `#include <QFile>`
* `#include <QJsonDocument>`
* `#include <QJsonParseError>`
* `#include <QLockFile>`
* `#include <QNetworkReply>`
* `#include <QRegularExpression>`
* `#include <QSettings>`
* `#include <QTemporaryFile>`
* `#include <QUrl>`
* `#include <QtWebDAV/qwebdavitem.h>`
* `#include <qgsapplication.h>`
* `#include <qgsauthmanager.h>`
* `#include <algorithm>`
































































## Macros

| Type | Name |
| ---: | :--- |
| define  | [**WEBDAV\_CONFIGURATION\_FILENAME**](qfwebdavconnection_8cpp.md#define-webdav_configuration_filename)  `QStringLiteral( "qfield\_webdav\_configuration.json" )`<br> |
| define  | [**WEBDAV\_LOCK\_FILENAME**](qfwebdavconnection_8cpp.md#define-webdav_lock_filename)  `QStringLiteral( "qfield\_webdav\_configuration.lock" )`<br> |

## Macro Definition Documentation





### define WEBDAV\_CONFIGURATION\_FILENAME 

```C++
#define WEBDAV_CONFIGURATION_FILENAME `QStringLiteral( "qfield_webdav_configuration.json" )`
```




<hr>



### define WEBDAV\_LOCK\_FILENAME 

```C++
#define WEBDAV_LOCK_FILENAME `QStringLiteral( "qfield_webdav_configuration.lock" )`
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfwebdavconnection.cpp`

