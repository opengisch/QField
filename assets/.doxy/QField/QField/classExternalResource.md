

# Class ExternalResource



[**ClassList**](annotated.md) **>** [**ExternalResource**](classExternalResource.md)








Inherits the following classes: [EditorWidgetBase](classEditorWidgetBase.md)














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**DocumentViewer**](#enum-documentviewer)  <br> |
























## Public Properties

| Type | Name |
| ---: | :--- |
| property string | [**audioSourcePath**](classExternalResource.md#property-audiosourcepath)  <br> |
| property var | [**currentValue**](classExternalResource.md#property-currentvalue)  <br> |
| property int | [**documentViewer**](classExternalResource.md#property-documentviewer)  <br> |
| property bool | [**isAudio**](classExternalResource.md#property-isaudio)  <br> |
| property bool | [**isImage**](classExternalResource.md#property-isimage)  <br> |
| property bool | [**isVideo**](classExternalResource.md#property-isvideo)  <br> |
| property string | [**prefixToRelativePath**](classExternalResource.md#property-prefixtorelativepath)  <br> |


## Public Properties inherited from EditorWidgetBase

See [EditorWidgetBase](classEditorWidgetBase.md)

| Type | Name |
| ---: | :--- |
| property bool | [**hasMenu**](classEditorWidgetBase.md#property-hasmenu)  <br> |
| property bool | [**isEmpty**](classEditorWidgetBase.md#property-isempty)  <br> |
| property bool | [**isLoaded**](classEditorWidgetBase.md#property-isloaded)  <br> |
| property bool | [**isNull**](classEditorWidgetBase.md#property-isnull)  <br> |
| property Menu | [**menu**](classEditorWidgetBase.md#property-menu)  <br> |








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
|  void | [**getResourceFilePath**](#function-getresourcefilepath) () <br> |
|  void | [**prepareValue**](#function-preparevalue) (fullValue) <br> |
























































## Public Types Documentation




### enum DocumentViewer 

```C++
enum ExternalResource::DocumentViewer {
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
string ExternalResource::audioSourcePath;
```




<hr>



### property currentValue 

```C++
var ExternalResource::currentValue;
```




<hr>



### property documentViewer 

```C++
int ExternalResource::documentViewer;
```




<hr>



### property isAudio 

```C++
bool ExternalResource::isAudio;
```




<hr>



### property isImage 

```C++
bool ExternalResource::isImage;
```




<hr>



### property isVideo 

```C++
bool ExternalResource::isVideo;
```




<hr>



### property prefixToRelativePath 

```C++
string ExternalResource::prefixToRelativePath;
```




<hr>
## Public Functions Documentation




### function getResourceFilePath 

```C++
void ExternalResource::getResourceFilePath () 
```




<hr>



### function prepareValue 

```C++
void ExternalResource::prepareValue (
    fullValue
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/editorwidgets/ExternalResource.qml`

