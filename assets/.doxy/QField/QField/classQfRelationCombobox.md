

# Class QfRelationCombobox



[**ClassList**](annotated.md) **>** [**QfRelationCombobox**](classQfRelationCombobox.md)








Inherits the following classes: QtQuick::Item


Inherited by the following classes: [RelationCombobox](classRelationCombobox.md)
























## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**allowAddFeature**](classQfRelationCombobox.md#property-allowaddfeature)  <br> |
| property alias | [**count**](classQfRelationCombobox.md#property-count)  <br> |
| property var | [**currentKeyValue**](classQfRelationCombobox.md#property-currentkeyvalue)  <br> |
| property color | [**displayedTextColor**](classQfRelationCombobox.md#property-displayedtextcolor)  <br> |
| property [**QfEmbeddedFeatureForm**](classQfEmbeddedFeatureForm.md) | [**embeddedFeatureForm**](classQfRelationCombobox.md#property-embeddedfeatureform)  <br> |
| property [**QfFeatureCheckListModel**](classQfFeatureCheckListModel.md) | [**featureListModel**](classQfRelationCombobox.md#property-featurelistmodel)  <br> |
| property var | [**layerResolver**](classQfRelationCombobox.md#property-layerresolver)  <br> |
| property var | [**relation**](classQfRelationCombobox.md#property-relation)  <br> |
| property alias | [**searchPopup**](classQfRelationCombobox.md#property-searchpopup)  <br> |
| property bool | [**useCompleter**](classQfRelationCombobox.md#property-usecompleter)  <br> |
| property bool | [**useSearch**](classQfRelationCombobox.md#property-usesearch)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**requestJumpToPoint**](classQfRelationCombobox.md#signal-requestjumptopoint) (var center, real scale, bool handleMargins) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**ensureEmbeddedFormLoaded**](#function-ensureembeddedformloaded) () <br> |
|  void | [**requestedGeometryReceived**](#function-requestedgeometryreceived) (geometry) <br> |
|  void | [**showAddFeaturePopup**](#function-showaddfeaturepopup) (geometry) <br> |




























## Public Properties Documentation




### property allowAddFeature 

```C++
bool QfRelationCombobox::allowAddFeature;
```




<hr>



### property count 

```C++
alias QfRelationCombobox::count;
```





**Remark:**

This property is read-only 





        

<hr>



### property currentKeyValue 

```C++
var QfRelationCombobox::currentKeyValue;
```




<hr>



### property displayedTextColor 

```C++
color QfRelationCombobox::displayedTextColor;
```




<hr>



### property embeddedFeatureForm 

```C++
QfEmbeddedFeatureForm QfRelationCombobox::embeddedFeatureForm;
```




<hr>



### property featureListModel 

```C++
QfFeatureCheckListModel QfRelationCombobox::featureListModel;
```




<hr>



### property layerResolver 

```C++
var QfRelationCombobox::layerResolver;
```




<hr>



### property relation 

```C++
var QfRelationCombobox::relation;
```




<hr>



### property searchPopup 

```C++
alias QfRelationCombobox::searchPopup;
```





**Remark:**

This property is read-only 





        

<hr>



### property useCompleter 

```C++
bool QfRelationCombobox::useCompleter;
```




<hr>



### property useSearch 

```C++
bool QfRelationCombobox::useSearch;
```




<hr>
## Public Signals Documentation




### signal requestJumpToPoint 

```C++
void QfRelationCombobox::requestJumpToPoint;
```




<hr>
## Public Functions Documentation




### function ensureEmbeddedFormLoaded 

```C++
void QfRelationCombobox::ensureEmbeddedFormLoaded () 
```




<hr>



### function requestedGeometryReceived 

```C++
void QfRelationCombobox::requestedGeometryReceived (
    geometry
) 
```




<hr>



### function showAddFeaturePopup 

```C++
void QfRelationCombobox::showAddFeaturePopup (
    geometry
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qml/QfRelationCombobox.qml`

