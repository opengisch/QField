

# Class ChangelogContents



[**ClassList**](annotated.md) **>** [**ChangelogContents**](classChangelogContents.md)



[More...](#detailed-description)

* `#include <changelogcontents.h>`



Inherits the following classes: QObject














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Status**](#enum-status)  <br>[_**Changelog**_](classChangelog.md) _contents status._ |












## Public Properties

| Type | Name |
| ---: | :--- |
| property QString | [**markdown**](classChangelogContents.md#property-markdown-12)  <br>_Holds the current changelog contents formatted as markdown._  |
| property [**Status**](classChangelogContents.md#enum-status) | [**status**](classChangelogContents.md#property-status-12)  <br>_Holds the current changelog contents status._  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**markdownChanged**](classChangelogContents.md#signal-markdownchanged)  <br>_Emitted when the markdown contents has been changed._  |
| signal void | [**statusChanged**](classChangelogContents.md#signal-statuschanged)  <br>_Emitted when the status has been changed._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ChangelogContents**](#function-changelogcontents) (QObject \* parent=nullptr) <br>_Constructor._  |
|  QString | [**markdown**](#function-markdown-22) () <br>_Returns the current changelog contents formatted as markdown. Null string if no changelog._  |
|  Q\_INVOKABLE void | [**request**](#function-request) () <br> |
|  [**Status**](classChangelogContents.md#enum-status) | [**status**](#function-status-22) () <br>_Returns the current changelog contents status._  |




























## Detailed Description


Obtain the QField changelog contents from the GitHub releases API. 


    
## Public Types Documentation




### enum Status 

[_**Changelog**_](classChangelog.md) _contents status._
```C++
enum ChangelogContents::Status {
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
QString ChangelogContents::markdown;
```




<hr>



### property status [1/2]

_Holds the current changelog contents status._ 
```C++
ChangelogContents::Status ChangelogContents::status;
```




<hr>
## Public Signals Documentation




### signal markdownChanged 

_Emitted when the markdown contents has been changed._ 
```C++
void ChangelogContents::markdownChanged;
```




<hr>



### signal statusChanged 

_Emitted when the status has been changed._ 
```C++
void ChangelogContents::statusChanged;
```




<hr>
## Public Functions Documentation




### function ChangelogContents 

_Constructor._ 
```C++
explicit ChangelogContents::ChangelogContents (
    QObject * parent=nullptr
) 
```




<hr>



### function markdown [2/2]

_Returns the current changelog contents formatted as markdown. Null string if no changelog._ 
```C++
QString ChangelogContents::markdown () 
```




<hr>



### function request 

```C++
Q_INVOKABLE void ChangelogContents::request () 
```



Initiates a HTTP request to obtain the changelog contents. 

**See also:** [**markdownChanged()**](classChangelogContents.md#signal-markdownchanged) 


**See also:** [**statusChanged()**](classChangelogContents.md#signal-statuschanged) 



        

<hr>



### function status [2/2]

_Returns the current changelog contents status._ 
```C++
Status ChangelogContents::status () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/changelogcontents.h`

