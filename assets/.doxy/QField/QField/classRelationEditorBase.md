

# Class RelationEditorBase



[**ClassList**](annotated.md) **>** [**RelationEditorBase**](classRelationEditorBase.md)








Inherits the following classes: [EditorWidgetBase](classEditorWidgetBase.md)


Inherited by the following classes: [ordered\_relation\_editor](classordered__relation__editor.md),  [relation\_editor](classrelation__editor.md)




































## Public Properties

| Type | Name |
| ---: | :--- |
| property int | [**bottomMargin**](classRelationEditorBase.md#property-bottommargin)  <br> |
| property Menu | [**childMenu**](classRelationEditorBase.md#property-childmenu)  <br> |
| property [**QfDialog**](classTheme_1_1QfDialog.md) | [**deleteDialog**](classRelationEditorBase.md#property-deletedialog)  <br> |
| property [**EmbeddedFeatureForm**](classEmbeddedFeatureForm.md) | [**embeddedPopup**](classRelationEditorBase.md#property-embeddedpopup)  <br> |
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
|  void | [**ensureEmbeddedFormLoaded**](#function-ensureembeddedformloaded) () <br> |
|  void | [**isActionEnabled**](#function-isactionenabled) (buttonType) <br> |
|  void | [**requestedGeometryReceived**](#function-requestedgeometryreceived) (geometry) <br> |
|  void | [**showAddFeaturePopup**](#function-showaddfeaturepopup) (geometry) <br> |
|  void | [**showAtlasMenu**](#function-showatlasmenu) () <br> |
























































## Public Properties Documentation




### property bottomMargin 

```C++
int RelationEditorBase::bottomMargin;
```




<hr>



### property childMenu 

```C++
Menu RelationEditorBase::childMenu;
```




<hr>



### property deleteDialog 

```C++
QfDialog RelationEditorBase::deleteDialog;
```




<hr>



### property embeddedPopup 

```C++
EmbeddedFeatureForm RelationEditorBase::embeddedPopup;
```




<hr>



### property itemHeight 

```C++
int RelationEditorBase::itemHeight;
```




<hr>



### property listView [1/2]

```C++
alias RelationEditorBase::listView;
```




<hr>



### property maximumVisibleItems 

```C++
int RelationEditorBase::maximumVisibleItems;
```




<hr>



### property relationEditorModel 

```C++
var RelationEditorBase::relationEditorModel;
```




<hr>



### property showAllItems 

```C++
bool RelationEditorBase::showAllItems;
```




<hr>



### property showSortButton 

```C++
bool RelationEditorBase::showSortButton;
```




<hr>
## Public Signals Documentation




### signal toggleSortAction 

```C++
void RelationEditorBase::toggleSortAction;
```




<hr>
## Public Functions Documentation




### function ensureEmbeddedFormLoaded 

```C++
void RelationEditorBase::ensureEmbeddedFormLoaded () 
```




<hr>



### function isActionEnabled 

```C++
void RelationEditorBase::isActionEnabled (
    buttonType
) 
```




<hr>



### function requestedGeometryReceived 

```C++
void RelationEditorBase::requestedGeometryReceived (
    geometry
) 
```




<hr>



### function showAddFeaturePopup 

```C++
void RelationEditorBase::showAddFeaturePopup (
    geometry
) 
```




<hr>



### function showAtlasMenu 

```C++
void RelationEditorBase::showAtlasMenu () 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/editorwidgets/RelationEditorBase.qml`

