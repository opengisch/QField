

# Class ClipboardManager



[**ClassList**](annotated.md) **>** [**ClipboardManager**](classClipboardManager.md)



[More...](#detailed-description)

* `#include <clipboardmanager.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**holdsFeature**](classClipboardManager.md#property-holdsfeature-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**holdsFeatureChanged**](classClipboardManager.md#signal-holdsfeaturechanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ClipboardManager**](#function-clipboardmanager) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**copyFeatureToClipboard**](#function-copyfeaturetoclipboard-12) (const QgsFeature & feature, bool includeGeometry=false) <br> |
|  Q\_INVOKABLE void | [**copyFeatureToClipboard**](#function-copyfeaturetoclipboard-22) (QgsVectorLayer \* layer, QgsFeatureId fid, bool includeGeometry=false, bool isCutOperation=false) <br> |
|  bool | [**holdsFeature**](#function-holdsfeature-22) () const<br> |
|  Q\_INVOKABLE QgsFeature | [**pasteFeatureFromClipboard**](#function-pastefeaturefromclipboard) () <br> |
|  Q\_INVOKABLE bool | [**pasteFeatureFromClipboardIntoLayer**](#function-pastefeaturefromclipboardintolayer) (QgsVectorLayer \* layer) <br> |
|   | [**~ClipboardManager**](#function-clipboardmanager) () <br> |




























## Detailed Description


This class handles clipboard operations. 


    
## Public Properties Documentation




### property holdsFeature [1/2]

```C++
bool ClipboardManager::holdsFeature;
```




<hr>
## Public Signals Documentation




### signal holdsFeatureChanged 

```C++
void ClipboardManager::holdsFeatureChanged;
```




<hr>
## Public Functions Documentation




### function ClipboardManager 

```C++
explicit ClipboardManager::ClipboardManager (
    QObject * parent=nullptr
) 
```




<hr>



### function copyFeatureToClipboard [1/2]

```C++
Q_INVOKABLE void ClipboardManager::copyFeatureToClipboard (
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
Q_INVOKABLE void ClipboardManager::copyFeatureToClipboard (
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
inline bool ClipboardManager::holdsFeature () const
```




<hr>



### function pasteFeatureFromClipboard 

```C++
Q_INVOKABLE QgsFeature ClipboardManager::pasteFeatureFromClipboard () 
```



Returns a feature with attributes and geometry matching clipboard values when a feature has been copied into the clipboard. 


        

<hr>



### function pasteFeatureFromClipboardIntoLayer 

```C++
Q_INVOKABLE bool ClipboardManager::pasteFeatureFromClipboardIntoLayer (
    QgsVectorLayer * layer
) 
```



Pastes the copied feature from the clipboard into the given editable layer. The feature is made compatible with the layer before being inserted. If the feature was previously cut and the destination layer differs from the source, the feature will be deleted from the original layer after paste. 

**Parameters:**


* `layer` destination vector layer 



**Returns:**

TRUE if insertion was successful 





        

<hr>



### function ~ClipboardManager 

```C++
inline ClipboardManager::~ClipboardManager () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/clipboardmanager.h`

