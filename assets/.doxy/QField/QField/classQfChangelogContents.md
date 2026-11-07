

# Class QfChangelogContents



[**ClassList**](annotated.md) **>** [**QfChangelogContents**](classQfChangelogContents.md)



[More...](#detailed-description)

* `#include <qfchangelogcontents.h>`



Inherits the following classes: QObject














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Status**](#enum-status)  <br>_Changelog contents status._  |












## Public Properties

| Type | Name |
| ---: | :--- |
| property QString | [**markdown**](classQfChangelogContents.md#property-markdown-12)  <br>_Holds the current changelog contents formatted as markdown._  |
| property [**Status**](classQfChangelogContents.md#enum-status) | [**status**](classQfChangelogContents.md#property-status-12)  <br>_Holds the current changelog contents status._  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**markdownChanged**](classQfChangelogContents.md#signal-markdownchanged)  <br>_Emitted when the markdown contents has been changed._  |
| signal void | [**statusChanged**](classQfChangelogContents.md#signal-statuschanged)  <br>_Emitted when the status has been changed._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfChangelogContents**](#function-qfchangelogcontents) (QObject \* parent=nullptr) <br>_Constructor._  |
|  QString | [**markdown**](#function-markdown-22) () <br>_Returns the current changelog contents formatted as markdown. Null string if no changelog._  |
|  Q\_INVOKABLE void | [**request**](#function-request) () <br> |
|  [**Status**](classQfChangelogContents.md#enum-status) | [**status**](#function-status-22) () <br>_Returns the current changelog contents status._  |




























## Detailed Description


Obtain the QField changelog contents from the GitHub releases API. 


    
## Public Types Documentation




### enum Status 

_Changelog contents status._ 
```C++
enum QfChangelogContents::Status {
    IdleStatus,
    LoadingStatus,
    SuccessStatus,
    ErrorStatus
};
```




<hr>
## Public Properties Documentation




### property markdown [1/2]

_Holds the current changelog contents formatted as markdown._ 
```C++
QString QfChangelogContents::markdown;
```




<hr>



### property status [1/2]

_Holds the current changelog contents status._ 
```C++
QfChangelogContents::Status QfChangelogContents::status;
```




<hr>
## Public Signals Documentation




### signal markdownChanged 

_Emitted when the markdown contents has been changed._ 
```C++
void QfChangelogContents::markdownChanged;
```




<hr>



### signal statusChanged 

_Emitted when the status has been changed._ 
```C++
void QfChangelogContents::statusChanged;
```




<hr>
## Public Functions Documentation




### function QfChangelogContents 

_Constructor._ 
```C++
explicit QfChangelogContents::QfChangelogContents (
    QObject * parent=nullptr
) 
```




<hr>



### function markdown [2/2]

_Returns the current changelog contents formatted as markdown. Null string if no changelog._ 
```C++
QString QfChangelogContents::markdown () 
```




<hr>



### function request 

```C++
Q_INVOKABLE void QfChangelogContents::request () 
```



Initiates a HTTP request to obtain the changelog contents. 

**See also:** [**markdownChanged()**](classQfChangelogContents.md#signal-markdownchanged) 


**See also:** [**statusChanged()**](classQfChangelogContents.md#signal-statuschanged) 



        

<hr>



### function status [2/2]

_Returns the current changelog contents status._ 
```C++
Status QfChangelogContents::status () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/app/qfchangelogcontents.h`

