

# File deltafilewrapper.cpp



[**FileList**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qfieldcloud**](dir_d27ac98b23548f5810403c734ce9817b.md) **>** [**deltafilewrapper.cpp**](deltafilewrapper_8cpp.md)

[Go to the source code of this file](deltafilewrapper_8cpp_source.md)



* `#include "deltafilewrapper.h"`
* `#include "qfield.h"`
* `#include "utils/fileutils.h"`
* `#include "utils/qfieldcloudutils.h"`
* `#include <QDebug>`
* `#include <QFile>`
* `#include <QFileInfo>`
* `#include <QUuid>`
* `#include <qgsmessagelog.h>`
* `#include <qgsproject.h>`
* `#include <qgsvectorlayerutils.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| typedef QMap&lt; QString, QStringList &gt; | [**CacheAttachmentFieldNamesMap**](#typedef-cacheattachmentfieldnamesmap)  <br> |




















## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**Q\_GLOBAL\_STATIC**](#function-q_global_static) (CacheAttachmentFieldNamesMap, sCacheAttachmentFieldNames) <br> |
|   | [**Q\_GLOBAL\_STATIC**](#function-q_global_static) (QSet&lt; QString &gt;, sFileLocks) <br> |




























## Public Types Documentation




### typedef CacheAttachmentFieldNamesMap 

```C++
typedef QMap<QString, QStringList> CacheAttachmentFieldNamesMap;
```



Attachment fields cache. 


        

<hr>
## Public Functions Documentation




### function Q\_GLOBAL\_STATIC 

```C++
Q_GLOBAL_STATIC (
    CacheAttachmentFieldNamesMap,
    sCacheAttachmentFieldNames
) 
```




<hr>



### function Q\_GLOBAL\_STATIC 

```C++
Q_GLOBAL_STATIC (
    QSet< QString >,
    sFileLocks
) 
```



Storage to keep track of the currently opened files. The stored paths are absolute, to ensure they are unique. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfieldcloud/deltafilewrapper.cpp`

