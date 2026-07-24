

# Class gallery\_relation\_editor



[**ClassList**](annotated.md) **>** [**gallery\_relation\_editor**](classgallery__relation__editor.md)








Inherits the following classes: [RelationEditorBase](classRelationEditorBase.md)


















































## Public Properties

| Type | Name |
| ---: | :--- |
| property var | [**activeMediaItem**](classgallery__relation__editor.md#property-activemediaitem)  <br> |
| property int | [**cardMargin**](classgallery__relation__editor.md#property-cardmargin)  <br> |
| property int | [**cardSize**](classgallery__relation__editor.md#property-cardsize)  <br> |
| property string | [**currentFetchKey**](classgallery__relation__editor.md#property-currentfetchkey)  <br> |
| property list&lt; string &gt; | [**failedDownloads**](classgallery__relation__editor.md#property-faileddownloads)  <br> |
| property list&lt; var &gt; | [**fetchQueue**](classgallery__relation__editor.md#property-fetchqueue)  <br> |
| property var | [**fetchedPaths**](classgallery__relation__editor.md#property-fetchedpaths)  <br> |
| property string | [**imagePrefix**](classgallery__relation__editor.md#property-imageprefix)  <br> |
| property bool | [**isCardView**](classgallery__relation__editor.md#property-iscardview)  <br> |
| property int | [**listItemHeight**](classgallery__relation__editor.md#property-listitemheight)  <br> |
| property list&lt; string &gt; | [**pendingDownloads**](classgallery__relation__editor.md#property-pendingdownloads)  <br> |
| property [**ResourceSource**](classResourceSource.md) | [**resourceSource**](classgallery__relation__editor.md#property-resourcesource)  <br> |
| property int | [**toggleHeight**](classgallery__relation__editor.md#property-toggleheight)  <br> |
| property list&lt; var &gt; | [**videoQueue**](classgallery__relation__editor.md#property-videoqueue)  <br> |
| property bool | [**videoQueueBusy**](classgallery__relation__editor.md#property-videoqueuebusy)  <br> |
| property int | [**visibleCards**](classgallery__relation__editor.md#property-visiblecards)  <br> |


## Public Properties inherited from RelationEditorBase

See [RelationEditorBase](classRelationEditorBase.md)

| Type | Name |
| ---: | :--- |
| property int | [**bottomMargin**](classRelationEditorBase.md#property-bottommargin)  <br> |
| property Menu | [**childMenu**](classRelationEditorBase.md#property-childmenu)  <br> |
| property [**QfDialog**](classTheme_1_1QfDialog.md) | [**deleteDialog**](classRelationEditorBase.md#property-deletedialog)  <br> |
| property [**EmbeddedFeatureForm**](classEmbeddedFeatureForm.md) | [**embeddedPopup**](classRelationEditorBase.md#property-embeddedpopup)  <br> |
| property alias | [**footer**](classRelationEditorBase.md#property-footer-12)  <br> |
| property alias | [**footerContent**](classRelationEditorBase.md#property-footercontent)  <br> |
| property alias | [**headerActions**](classRelationEditorBase.md#property-headeractions)  <br> |
| property alias | [**headerEntry**](classRelationEditorBase.md#property-headerentry-12)  <br> |
| property alias | [**itemCount**](classRelationEditorBase.md#property-itemcount)  <br> |
| property int | [**itemHeight**](classRelationEditorBase.md#property-itemheight)  <br> |
| property alias | [**listView**](classRelationEditorBase.md#property-listview-12)  <br> |
| property int | [**maximumVisibleItems**](classRelationEditorBase.md#property-maximumvisibleitems)  <br> |
| property var | [**relationEditorModel**](classRelationEditorBase.md#property-relationeditormodel)  <br> |
| property bool | [**showAllItems**](classRelationEditorBase.md#property-showallitems)  <br> |
| property bool | [**showSortButton**](classRelationEditorBase.md#property-showsortbutton)  <br> |


## Public Properties inherited from EditorWidgetBase

See [EditorWidgetBase](classEditorWidgetBase.md)

| Type | Name |
| ---: | :--- |
| property bool | [**hasMenu**](classEditorWidgetBase.md#property-hasmenu)  <br> |
| property bool | [**isEmpty**](classEditorWidgetBase.md#property-isempty)  <br> |
| property bool | [**isLoaded**](classEditorWidgetBase.md#property-isloaded)  <br> |
| property bool | [**isNull**](classEditorWidgetBase.md#property-isnull)  <br> |
| property Menu | [**menu**](classEditorWidgetBase.md#property-menu)  <br> |








## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**downloadFinished**](classgallery__relation__editor.md#signal-downloadfinished) (string path) <br> |


## Public Signals inherited from RelationEditorBase

See [RelationEditorBase](classRelationEditorBase.md)

| Type | Name |
| ---: | :--- |
| signal void | [**toggleSortAction**](classRelationEditorBase.md#signal-togglesortaction)  <br> |


## Public Signals inherited from EditorWidgetBase

See [EditorWidgetBase](classEditorWidgetBase.md)

| Type | Name |
| ---: | :--- |
| signal void | [**requestBarcode**](classEditorWidgetBase.md#signal-requestbarcode) (var item) <br> |
| signal void | [**requestGeometry**](classEditorWidgetBase.md#signal-requestgeometry) (var item, var layer) <br> |
| signal void | [**requestJumpToPoint**](classEditorWidgetBase.md#signal-requestjumptopoint) (var center, real scale, bool handleMargins) <br> |
| signal void | [**valueChangeRequested**](classEditorWidgetBase.md#signal-valuechangerequested) (var value, bool isNull) <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**captureAudio**](#function-captureaudio) () <br> |
|  void | [**capturePhoto**](#function-capturephoto) () <br> |
|  void | [**captureVideo**](#function-capturevideo) () <br> |
|  void | [**enqueueExternalFetch**](#function-enqueueexternalfetch) (relativePath, fetchUrl, authConfigId) <br> |
|  void | [**processNextFetch**](#function-processnextfetch) () <br> |
|  void | [**queueVideoLoad**](#function-queuevideoload) (fn) <br> |
|  void | [**releaseMediaFocus**](#function-releasemediafocus) (item) <br> |
|  void | [**requestMediaFocus**](#function-requestmediafocus) (item) <br> |
|  void | [**stopAllMedia**](#function-stopallmedia) () <br> |


## Public Functions inherited from RelationEditorBase

See [RelationEditorBase](classRelationEditorBase.md)

| Type | Name |
| ---: | :--- |
|  void | [**ensureEmbeddedFormLoaded**](classRelationEditorBase.md#function-ensureembeddedformloaded) () <br> |
|  void | [**isActionEnabled**](classRelationEditorBase.md#function-isactionenabled) (buttonType) <br> |
|  void | [**prepareParent**](classRelationEditorBase.md#function-prepareparent) () <br> |
|  void | [**requestedGeometryReceived**](classRelationEditorBase.md#function-requestedgeometryreceived) (geometry) <br> |
|  void | [**showAddFeaturePopup**](classRelationEditorBase.md#function-showaddfeaturepopup) (geometry) <br> |
|  void | [**showAtlasMenu**](classRelationEditorBase.md#function-showatlasmenu) () <br> |
|  void | [**showViewFeaturePopup**](classRelationEditorBase.md#function-showviewfeaturepopup) (feature) <br> |


















































































## Public Properties Documentation




### property activeMediaItem 

```C++
var gallery_relation_editor::activeMediaItem;
```




<hr>



### property cardMargin 

```C++
int gallery_relation_editor::cardMargin;
```





**Remark:**

This property is read-only 





        

<hr>



### property cardSize 

```C++
int gallery_relation_editor::cardSize;
```





**Remark:**

This property is read-only 





        

<hr>



### property currentFetchKey 

```C++
string gallery_relation_editor::currentFetchKey;
```




<hr>



### property failedDownloads 

```C++
list<string> gallery_relation_editor::failedDownloads;
```




<hr>



### property fetchQueue 

```C++
list<var> gallery_relation_editor::fetchQueue;
```




<hr>



### property fetchedPaths 

```C++
var gallery_relation_editor::fetchedPaths;
```




<hr>



### property imagePrefix 

```C++
string gallery_relation_editor::imagePrefix;
```




<hr>



### property isCardView 

```C++
bool gallery_relation_editor::isCardView;
```




<hr>



### property listItemHeight 

```C++
int gallery_relation_editor::listItemHeight;
```





**Remark:**

This property is read-only 





        

<hr>



### property pendingDownloads 

```C++
list<string> gallery_relation_editor::pendingDownloads;
```




<hr>



### property resourceSource 

```C++
ResourceSource gallery_relation_editor::resourceSource;
```




<hr>



### property toggleHeight 

```C++
int gallery_relation_editor::toggleHeight;
```





**Remark:**

This property is read-only 





        

<hr>



### property videoQueue 

```C++
list<var> gallery_relation_editor::videoQueue;
```




<hr>



### property videoQueueBusy 

```C++
bool gallery_relation_editor::videoQueueBusy;
```




<hr>



### property visibleCards 

```C++
int gallery_relation_editor::visibleCards;
```





**Remark:**

This property is read-only 





        

<hr>
## Public Signals Documentation




### signal downloadFinished 

```C++
void gallery_relation_editor::downloadFinished;
```




<hr>
## Public Functions Documentation




### function captureAudio 

```C++
void gallery_relation_editor::captureAudio () 
```




<hr>



### function capturePhoto 

```C++
void gallery_relation_editor::capturePhoto () 
```




<hr>



### function captureVideo 

```C++
void gallery_relation_editor::captureVideo () 
```




<hr>



### function enqueueExternalFetch 

```C++
void gallery_relation_editor::enqueueExternalFetch (
    relativePath,
    fetchUrl,
    authConfigId
) 
```




<hr>



### function processNextFetch 

```C++
void gallery_relation_editor::processNextFetch () 
```




<hr>



### function queueVideoLoad 

```C++
void gallery_relation_editor::queueVideoLoad (
    fn
) 
```




<hr>



### function releaseMediaFocus 

```C++
void gallery_relation_editor::releaseMediaFocus (
    item
) 
```




<hr>



### function requestMediaFocus 

```C++
void gallery_relation_editor::requestMediaFocus (
    item
) 
```




<hr>



### function stopAllMedia 

```C++
void gallery_relation_editor::stopAllMedia () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/editorwidgets/relationeditors/gallery_relation_editor.qml`

