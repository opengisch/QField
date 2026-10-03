

# Class QfEditorWidgetExternalResource



[**ClassList**](annotated.md) **>** [**QfEditorWidgetExternalResource**](classQfEditorWidgetExternalResource.md)








Inherits the following classes: [QfEditorWidgetBase](classQfEditorWidgetBase.md)














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**DocumentViewer**](#enum-documentviewer)  <br> |
























## Public Properties

| Type | Name |
| ---: | :--- |
| property string | [**audioSourcePath**](classQfEditorWidgetExternalResource.md#property-audiosourcepath)  <br> |
| property var | [**currentValue**](classQfEditorWidgetExternalResource.md#property-currentvalue)  <br> |
| property int | [**documentViewer**](classQfEditorWidgetExternalResource.md#property-documentviewer)  <br> |
| property bool | [**isAudio**](classQfEditorWidgetExternalResource.md#property-isaudio)  <br> |
| property bool | [**isImage**](classQfEditorWidgetExternalResource.md#property-isimage)  <br> |
| property bool | [**isVideo**](classQfEditorWidgetExternalResource.md#property-isvideo)  <br> |
| property string | [**prefixToRelativePath**](classQfEditorWidgetExternalResource.md#property-prefixtorelativepath)  <br> |


## Public Properties inherited from QfEditorWidgetBase

See [QfEditorWidgetBase](classQfEditorWidgetBase.md)

| Type | Name |
| ---: | :--- |
| property bool | [**hasMenu**](classQfEditorWidgetBase.md#property-hasmenu)  <br> |
| property bool | [**isEmpty**](classQfEditorWidgetBase.md#property-isempty)  <br> |
| property bool | [**isLoaded**](classQfEditorWidgetBase.md#property-isloaded)  <br> |
| property bool | [**isNull**](classQfEditorWidgetBase.md#property-isnull)  <br> |
| property Menu | [**menu**](classQfEditorWidgetBase.md#property-menu)  <br> |








## Public Signals inherited from QfEditorWidgetBase

See [QfEditorWidgetBase](classQfEditorWidgetBase.md)

| Type | Name |
| ---: | :--- |
| signal void | [**requestBarcode**](classQfEditorWidgetBase.md#signal-requestbarcode) (var item) <br> |
| signal void | [**requestGeometry**](classQfEditorWidgetBase.md#signal-requestgeometry) (var item, var layer) <br> |
| signal void | [**requestJumpToPoint**](classQfEditorWidgetBase.md#signal-requestjumptopoint) (var center, real scale, bool handleMargins) <br> |
| signal void | [**valueChangeRequested**](classQfEditorWidgetBase.md#signal-valuechangerequested) (var value, bool isNull) <br> |






## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**getResourceFilePath**](#function-getresourcefilepath) () <br> |
|  void | [**prepareValue**](#function-preparevalue) (fullValue) <br> |
























































## Public Types Documentation




### enum DocumentViewer 

```C++
enum QfEditorWidgetExternalResource::DocumentViewer {
    DocumentFile,
    DocumentImage,
    DocumentWeb,
    DocumentAudio,
    DocumentVideo
};
```




<hr>
## Public Properties Documentation




### property audioSourcePath 

```C++
string QfEditorWidgetExternalResource::audioSourcePath;
```




<hr>



### property currentValue 

```C++
var QfEditorWidgetExternalResource::currentValue;
```




<hr>



### property documentViewer 

```C++
int QfEditorWidgetExternalResource::documentViewer;
```




<hr>



### property isAudio 

```C++
bool QfEditorWidgetExternalResource::isAudio;
```




<hr>



### property isImage 

```C++
bool QfEditorWidgetExternalResource::isImage;
```




<hr>



### property isVideo 

```C++
bool QfEditorWidgetExternalResource::isVideo;
```




<hr>



### property prefixToRelativePath 

```C++
string QfEditorWidgetExternalResource::prefixToRelativePath;
```




<hr>
## Public Functions Documentation




### function getResourceFilePath 

```C++
void QfEditorWidgetExternalResource::getResourceFilePath () 
```




<hr>



### function prepareValue 

```C++
void QfEditorWidgetExternalResource::prepareValue (
    fullValue
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qml/editorwidgets/QfEditorWidgetExternalResource.qml`

