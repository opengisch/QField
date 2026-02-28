

# Class RelationCombobox



[**ClassList**](annotated.md) **>** [**RelationCombobox**](classRelationCombobox.md)








Inherits the following classes: QtQuick::Item


























## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**allowAddFeature**](classRelationCombobox.md#property-allowaddfeature)  <br> |
| property alias | [**count**](classRelationCombobox.md#property-count)  <br> |
| property var | [**currentKeyValue**](classRelationCombobox.md#property-currentkeyvalue)  <br> |
| property color | [**displayedTextColor**](classRelationCombobox.md#property-displayedtextcolor)  <br> |
| property [**EmbeddedFeatureForm**](classEmbeddedFeatureForm.md) | [**embeddedFeatureForm**](classRelationCombobox.md#property-embeddedfeatureform)  <br> |
| property [**FeatureCheckListModel**](classFeatureCheckListModel.md) | [**featureListModel**](classRelationCombobox.md#property-featurelistmodel)  <br> |
| property var | [**layerResolver**](classRelationCombobox.md#property-layerresolver)  <br> |
| property var | [**relation**](classRelationCombobox.md#property-relation)  <br> |
| property alias | [**searchPopup**](classRelationCombobox.md#property-searchpopup)  <br> |
| property bool | [**useCompleter**](classRelationCombobox.md#property-usecompleter)  <br> |
| property bool | [**useSearch**](classRelationCombobox.md#property-usesearch)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**requestJumpToPoint**](classRelationCombobox.md#signal-requestjumptopoint) (var center, real scale, bool handleMargins) <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**ensureEmbeddedFormLoaded**](#function-ensureembeddedformloaded) () <br> |
|  void | [**requestedGeometryReceived**](#function-requestedgeometryreceived) (geometry) <br> |
|  void | [**showAddFeaturePopup**](#function-showaddfeaturepopup) (geometry) <br> |




























## Public Properties Documentation




### property allowAddFeature 

```C++
bool RelationCombobox::allowAddFeature;
```




<hr>



### property count 

```C++
alias RelationCombobox::count;
```





**Remark:**

This property is read-only 





        

<hr>



### property currentKeyValue 

```C++
var RelationCombobox::currentKeyValue;
```




<hr>



### property displayedTextColor 

```C++
color RelationCombobox::displayedTextColor;
```




<hr>



### property embeddedFeatureForm 

```C++
EmbeddedFeatureForm RelationCombobox::embeddedFeatureForm;
```




<hr>



### property featureListModel 

```C++
FeatureCheckListModel RelationCombobox::featureListModel;
```




<hr>



### property layerResolver 

```C++
var RelationCombobox::layerResolver;
```




<hr>



### property relation 

```C++
var RelationCombobox::relation;
```




<hr>



### property searchPopup 

```C++
alias RelationCombobox::searchPopup;
```





**Remark:**

This property is read-only 





        

<hr>



### property useCompleter 

```C++
bool RelationCombobox::useCompleter;
```




<hr>



### property useSearch 

```C++
bool RelationCombobox::useSearch;
```




<hr>
## Public Signals Documentation




### signal requestJumpToPoint 

```C++
void RelationCombobox::requestJumpToPoint;
```




<hr>
## Public Functions Documentation




### function ensureEmbeddedFormLoaded 

```C++
void RelationCombobox::ensureEmbeddedFormLoaded () 
```




<hr>



### function requestedGeometryReceived 

```C++
void RelationCombobox::requestedGeometryReceived (
    geometry
) 
```




<hr>



### function showAddFeaturePopup 

```C++
void RelationCombobox::showAddFeaturePopup (
    geometry
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/RelationCombobox.qml`

