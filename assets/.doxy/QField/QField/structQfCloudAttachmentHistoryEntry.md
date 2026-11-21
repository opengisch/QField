

# Struct QfCloudAttachmentHistoryEntry



[**ClassList**](annotated.md) **>** [**QfCloudAttachmentHistoryEntry**](structQfCloudAttachmentHistoryEntry.md)



[More...](#detailed-description)

* `#include <qfcloudutils.h>`





















## Public Attributes

| Type | Name |
| ---: | :--- |
|  QString | [**fileName**](#variable-filename)  <br>_The absolute path of the uploaded file._  |
|  int | [**httpCode**](#variable-httpcode)   = `0`<br>_The HTTP status code returned by the server for the upload._  |
|  QDateTime | [**timestamp**](#variable-timestamp)  <br>_The time at which the upload finished._  |












































## Detailed Description


Holds the final outcome of an attachment upload to QFieldCloud. 


    
## Public Attributes Documentation




### variable fileName 

_The absolute path of the uploaded file._ 
```C++
QString QfCloudAttachmentHistoryEntry::fileName;
```




<hr>



### variable httpCode 

_The HTTP status code returned by the server for the upload._ 
```C++
int QfCloudAttachmentHistoryEntry::httpCode;
```




<hr>



### variable timestamp 

_The time at which the upload finished._ 
```C++
QDateTime QfCloudAttachmentHistoryEntry::timestamp;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/qfcloudutils.h`

