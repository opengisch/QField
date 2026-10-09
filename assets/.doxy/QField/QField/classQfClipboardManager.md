

# Class QfClipboardManager



[**ClassList**](annotated.md) **>** [**QfClipboardManager**](classQfClipboardManager.md)



[More...](#detailed-description)

* `#include <qfclipboardmanager.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**holdsFeature**](classQfClipboardManager.md#property-holdsfeature-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**holdsFeatureChanged**](classQfClipboardManager.md#signal-holdsfeaturechanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfClipboardManager**](#function-qfclipboardmanager) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**copyFeatureToClipboard**](#function-copyfeaturetoclipboard-12) (const QgsFeature & feature, bool includeGeometry=false) <br> |
|  Q\_INVOKABLE void | [**copyFeatureToClipboard**](#function-copyfeaturetoclipboard-22) (QgsVectorLayer \* layer, QgsFeatureId fid, bool includeGeometry=false, bool isCutOperation=false) <br> |
|  bool | [**holdsFeature**](#function-holdsfeature-22) () const<br> |
|  Q\_INVOKABLE QgsFeature | [**pasteFeatureFromClipboard**](#function-pastefeaturefromclipboard) () <br> |
|  Q\_INVOKABLE bool | [**pasteFeatureFromClipboardIntoLayer**](#function-pastefeaturefromclipboardintolayer) (QgsVectorLayer \* layer) <br> |
|   | [**~QfClipboardManager**](#function-qfclipboardmanager) () = default<br> |




























## Detailed Description


This class handles clipboard operations. 


    
## Public Properties Documentation




### property holdsFeature [1/2]

```C++
bool QfClipboardManager::holdsFeature;
```




<hr>
## Public Signals Documentation




### signal holdsFeatureChanged 

```C++
void QfClipboardManager::holdsFeatureChanged;
```




<hr>
## Public Functions Documentation




### function QfClipboardManager 

```C++
explicit QfClipboardManager::QfClipboardManager (
    QObject * parent=nullptr
) 
```




<hr>



### function copyFeatureToClipboard [1/2]

```C++
Q_INVOKABLE void QfClipboardManager::copyFeatureToClipboard (
    const QgsFeature & feature,
    bool includeGeometry=false
) 
```



Copies a feature into the system's clipboard in both plain text as well as HTML. 

**Parameters:**


* `feature` the feature from which the attributes will be copied into memory 
* `includeGeometry` set to TRUE when a WKT string representation of the geometry should be included 




        

<hr>



### function copyFeatureToClipboard [2/2]

```C++
Q_INVOKABLE void QfClipboardManager::copyFeatureToClipboard (
    QgsVectorLayer * layer,
    QgsFeatureId fid,
    bool includeGeometry=false,
    bool isCutOperation=false
) 
```



Copies a feature into the system's clipboard in both plain text as well as HTML. 

**Parameters:**


* `layer` the vector layer associated to the feature 
* `fid` the id of the feature 
* `includeGeometry` set to TRUE when a WKT string representation of the geometry should be included 
* `isCutOperation` set to TRUE if this is a cut operation. In that case, when pasting into a different layer, the feature will be deleted from the original layer. 




        

<hr>



### function holdsFeature [2/2]

```C++
inline bool QfClipboardManager::holdsFeature () const
```




<hr>



### function pasteFeatureFromClipboard 

```C++
Q_INVOKABLE QgsFeature QfClipboardManager::pasteFeatureFromClipboard () 
```



Returns a feature with attributes and geometry matching clipboard values when a feature has been copied into the clipboard. 


        

<hr>



### function pasteFeatureFromClipboardIntoLayer 

```C++
Q_INVOKABLE bool QfClipboardManager::pasteFeatureFromClipboardIntoLayer (
    QgsVectorLayer * layer
) 
```



Pastes the copied feature from the clipboard into the given editable layer. The feature is made compatible with the layer before being inserted. If the feature was previously cut and the destination layer differs from the source, the feature will be deleted from the original layer after paste. 

**Parameters:**


* `layer` destination vector layer 



**Returns:**

TRUE if insertion was successful 





        

<hr>



### function ~QfClipboardManager 

```C++
QfClipboardManager::~QfClipboardManager () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/app/qfclipboardmanager.h`

