

# Class ExternalResource



[**ClassList**](annotated.md) **>** [**ExternalResource**](classExternalResource.md)








Inherits the following classes: [EditorWidgetBase](classEditorWidgetBase.md)






































## Public Properties

| Type | Name |
| ---: | :--- |
| property var | [**currentValue**](classExternalResource.md#property-currentvalue)  <br> |
| property int | [**documentViewer**](classExternalResource.md#property-documentviewer)  <br> |
| property int | [**document\_AUDIO**](classExternalResource.md#property-document_audio)  <br> |
| property int | [**document\_FILE**](classExternalResource.md#property-document_file)  <br> |
| property int | [**document\_IMAGE**](classExternalResource.md#property-document_image)  <br> |
| property int | [**document\_VIDEO**](classExternalResource.md#property-document_video)  <br> |
| property int | [**document\_WEB**](classExternalResource.md#property-document_web)  <br> |
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
|  void | [**attachFile**](#function-attachfile) () <br> |
|  void | [**attachGallery**](#function-attachgallery) () <br> |
|  void | [**captureAudio**](#function-captureaudio) () <br> |
|  void | [**capturePhoto**](#function-capturephoto) () <br> |
|  void | [**captureVideo**](#function-capturevideo) () <br> |
|  void | [**getResourceFilePath**](#function-getresourcefilepath) () <br> |
|  void | [**prepareValue**](#function-preparevalue) (fullValue) <br> |
























































## Public Properties Documentation




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



### property document\_AUDIO 

```C++
int ExternalResource::document_AUDIO;
```





**Remark:**

This property is read-only 





        

<hr>



### property document\_FILE 

```C++
int ExternalResource::document_FILE;
```





**Remark:**

This property is read-only 





        

<hr>



### property document\_IMAGE 

```C++
int ExternalResource::document_IMAGE;
```





**Remark:**

This property is read-only 





        

<hr>



### property document\_VIDEO 

```C++
int ExternalResource::document_VIDEO;
```





**Remark:**

This property is read-only 





        

<hr>



### property document\_WEB 

```C++
int ExternalResource::document_WEB;
```





**Remark:**

This property is read-only 





        

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




### function attachFile 

```C++
void ExternalResource::attachFile () 
```




<hr>



### function attachGallery 

```C++
void ExternalResource::attachGallery () 
```




<hr>



### function captureAudio 

```C++
void ExternalResource::captureAudio () 
```




<hr>



### function capturePhoto 

```C++
void ExternalResource::capturePhoto () 
```




<hr>



### function captureVideo 

```C++
void ExternalResource::captureVideo () 
```




<hr>



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

