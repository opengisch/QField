

# Class QfRelationEditorBase



[**ClassList**](annotated.md) **>** [**QfRelationEditorBase**](classQfRelationEditorBase.md)








Inherits the following classes: [QfEditorWidgetBase](classQfEditorWidgetBase.md)


Inherited by the following classes: [gallery\_relation\_editor](classgallery__relation__editor.md),  [ordered\_relation\_editor](classordered__relation__editor.md),  [relation\_editor](classrelation__editor.md)




































## Public Properties

| Type | Name |
| ---: | :--- |
| property int | [**bottomMargin**](classQfRelationEditorBase.md#property-bottommargin)  <br> |
| property Menu | [**childMenu**](classQfRelationEditorBase.md#property-childmenu)  <br> |
| property [**QfDialog**](classQfDialog.md) | [**deleteDialog**](classQfRelationEditorBase.md#property-deletedialog)  <br> |
| property [**QfEmbeddedFeatureForm**](classQfEmbeddedFeatureForm.md) | [**embeddedPopup**](classQfRelationEditorBase.md#property-embeddedpopup)  <br> |
| property alias | [**footer**](classQfRelationEditorBase.md#property-footer-12)  <br> |
| property alias | [**footerContent**](classQfRelationEditorBase.md#property-footercontent)  <br> |
| property alias | [**headerActions**](classQfRelationEditorBase.md#property-headeractions)  <br> |
| property alias | [**headerEntry**](classQfRelationEditorBase.md#property-headerentry-12)  <br> |
| property alias | [**itemCount**](classQfRelationEditorBase.md#property-itemcount)  <br> |
| property int | [**itemHeight**](classQfRelationEditorBase.md#property-itemheight)  <br> |
| property alias | [**listView**](classQfRelationEditorBase.md#property-listview-12)  <br> |
| property int | [**maximumVisibleItems**](classQfRelationEditorBase.md#property-maximumvisibleitems)  <br> |
| property var | [**relationEditorModel**](classQfRelationEditorBase.md#property-relationeditormodel)  <br> |
| property bool | [**showAllItems**](classQfRelationEditorBase.md#property-showallitems)  <br> |
| property bool | [**showSortButton**](classQfRelationEditorBase.md#property-showsortbutton)  <br> |


## Public Properties inherited from QfEditorWidgetBase

See [QfEditorWidgetBase](classQfEditorWidgetBase.md)

| Type | Name |
| ---: | :--- |
| property bool | [**hasMenu**](classQfEditorWidgetBase.md#property-hasmenu)  <br> |
| property bool | [**isEmpty**](classQfEditorWidgetBase.md#property-isempty)  <br> |
| property bool | [**isLoaded**](classQfEditorWidgetBase.md#property-isloaded)  <br> |
| property bool | [**isNull**](classQfEditorWidgetBase.md#property-isnull)  <br> |
| property Menu | [**menu**](classQfEditorWidgetBase.md#property-menu)  <br> |






## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**toggleSortAction**](classQfRelationEditorBase.md#signal-togglesortaction)  <br> |


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
|  void | [**ensureEmbeddedFormLoaded**](#function-ensureembeddedformloaded) () <br> |
|  void | [**isActionEnabled**](#function-isactionenabled) (buttonType) <br> |
|  void | [**prepareParent**](#function-prepareparent) () <br> |
|  void | [**requestedGeometryReceived**](#function-requestedgeometryreceived) (geometry) <br> |
|  void | [**showAddFeaturePopup**](#function-showaddfeaturepopup) (geometry) <br> |
|  void | [**showAtlasMenu**](#function-showatlasmenu) () <br> |
|  void | [**showViewFeaturePopup**](#function-showviewfeaturepopup) (feature) <br> |
























































## Public Properties Documentation




### property bottomMargin 

```C++
int QfRelationEditorBase::bottomMargin;
```




<hr>



### property childMenu 

```C++
Menu QfRelationEditorBase::childMenu;
```




<hr>



### property deleteDialog 

```C++
QfDialog QfRelationEditorBase::deleteDialog;
```




<hr>



### property embeddedPopup 

```C++
QfEmbeddedFeatureForm QfRelationEditorBase::embeddedPopup;
```




<hr>



### property footer [1/2]

```C++
alias QfRelationEditorBase::footer;
```




<hr>



### property footerContent 

```C++
alias QfRelationEditorBase::footerContent;
```




<hr>



### property headerActions 

```C++
alias QfRelationEditorBase::headerActions;
```




<hr>



### property headerEntry [1/2]

```C++
alias QfRelationEditorBase::headerEntry;
```




<hr>



### property itemCount 

```C++
alias QfRelationEditorBase::itemCount;
```




<hr>



### property itemHeight 

```C++
int QfRelationEditorBase::itemHeight;
```




<hr>



### property listView [1/2]

```C++
alias QfRelationEditorBase::listView;
```




<hr>



### property maximumVisibleItems 

```C++
int QfRelationEditorBase::maximumVisibleItems;
```




<hr>



### property relationEditorModel 

```C++
var QfRelationEditorBase::relationEditorModel;
```




<hr>



### property showAllItems 

```C++
bool QfRelationEditorBase::showAllItems;
```




<hr>



### property showSortButton 

```C++
bool QfRelationEditorBase::showSortButton;
```




<hr>
## Public Signals Documentation




### signal toggleSortAction 

```C++
void QfRelationEditorBase::toggleSortAction;
```




<hr>
## Public Functions Documentation




### function ensureEmbeddedFormLoaded 

```C++
void QfRelationEditorBase::ensureEmbeddedFormLoaded () 
```




<hr>



### function isActionEnabled 

```C++
void QfRelationEditorBase::isActionEnabled (
    buttonType
) 
```




<hr>



### function prepareParent 

```C++
void QfRelationEditorBase::prepareParent () 
```




<hr>



### function requestedGeometryReceived 

```C++
void QfRelationEditorBase::requestedGeometryReceived (
    geometry
) 
```




<hr>



### function showAddFeaturePopup 

```C++
void QfRelationEditorBase::showAddFeaturePopup (
    geometry
) 
```




<hr>



### function showAtlasMenu 

```C++
void QfRelationEditorBase::showAtlasMenu () 
```




<hr>



### function showViewFeaturePopup 

```C++
void QfRelationEditorBase::showViewFeaturePopup (
    feature
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qml/editorwidgets/QfRelationEditorBase.qml`

