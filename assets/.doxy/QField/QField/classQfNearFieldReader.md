

# Class QfNearFieldReader



[**ClassList**](annotated.md) **>** [**QfNearFieldReader**](classQfNearFieldReader.md)



[More...](#detailed-description)

* `#include <qfnearfieldreader.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**active**](classQfNearFieldReader.md#property-active-12)  <br> |
| property QString | [**readString**](classQfNearFieldReader.md#property-readstring-12)  <br> |
| property bool | [**targetInRange**](classQfNearFieldReader.md#property-targetinrange-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**activeChanged**](classQfNearFieldReader.md#signal-activechanged)  <br>_Emitted when the active state has changed._  |
| signal void | [**readStringChanged**](classQfNearFieldReader.md#signal-readstringchanged)  <br>_Emitted when the last text string read from a near-field target has changed._  |
| signal void | [**targetDetected**](classQfNearFieldReader.md#signal-targetdetected) (const QString & targetId) <br>_Emitted when a near-field target has been detected._  |
| signal void | [**targetInRangeChanged**](classQfNearFieldReader.md#signal-targetinrangechanged)  <br>_Emitted when a near-field target is within range of the device._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfNearFieldReader**](#function-qfnearfieldreader) (QObject \* parent=nullptr) <br> |
|  bool | [**active**](#function-active-22) () const<br> |
|  QString | [**readString**](#function-readstring-22) () const<br> |
|  void | [**setActive**](#function-setactive) (bool active) <br> |
|  bool | [**targetInRange**](#function-targetinrange-22) () const<br> |
|   | [**~QfNearFieldReader**](#function-qfnearfieldreader) () <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  bool | [**isSupported**](#function-issupported) () <br> |


























## Detailed Description


The [**QfNearFieldReader**](classQfNearFieldReader.md) class allows for the scanning of near-field targets. When text strings are present on targets, the strings will be automatically read and a signal emitted with the string values. 


    
## Public Properties Documentation




### property active [1/2]

```C++
bool QfNearFieldReader::active;
```




<hr>



### property readString [1/2]

```C++
QString QfNearFieldReader::readString;
```




<hr>



### property targetInRange [1/2]

```C++
bool QfNearFieldReader::targetInRange;
```




<hr>
## Public Signals Documentation




### signal activeChanged 

_Emitted when the active state has changed._ 
```C++
void QfNearFieldReader::activeChanged;
```




<hr>



### signal readStringChanged 

_Emitted when the last text string read from a near-field target has changed._ 
```C++
void QfNearFieldReader::readStringChanged;
```




<hr>



### signal targetDetected 

_Emitted when a near-field target has been detected._ 
```C++
void QfNearFieldReader::targetDetected;
```




<hr>



### signal targetInRangeChanged 

_Emitted when a near-field target is within range of the device._ 
```C++
void QfNearFieldReader::targetInRangeChanged;
```




<hr>
## Public Functions Documentation




### function QfNearFieldReader 

```C++
explicit QfNearFieldReader::QfNearFieldReader (
    QObject * parent=nullptr
) 
```




<hr>



### function active [2/2]

```C++
bool QfNearFieldReader::active () const
```



Returns whether the device is actively scanning and reading text strings from near-field targets. 


        

<hr>



### function readString [2/2]

```C++
QString QfNearFieldReader::readString () const
```



Returns the last read near-field string. 


        

<hr>



### function setActive 

```C++
void QfNearFieldReader::setActive (
    bool active
) 
```



Sets whether the device is actively scanning and reading text strings from near-field targets. 


        

<hr>



### function targetInRange [2/2]

```C++
bool QfNearFieldReader::targetInRange () const
```



Returns whether a near-field target is currently within range. 


        

<hr>



### function ~QfNearFieldReader 

```C++
QfNearFieldReader::~QfNearFieldReader () 
```




<hr>
## Public Static Functions Documentation




### function isSupported 

```C++
static bool QfNearFieldReader::isSupported () 
```



Returns whether the current device can read near-field targets. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfnearfieldreader.h`

