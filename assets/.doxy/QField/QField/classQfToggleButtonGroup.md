

# Class QfToggleButtonGroup



[**ClassList**](annotated.md) **>** [**QfToggleButtonGroup**](classQfToggleButtonGroup.md)



[More...](#detailed-description)




Inherits the following classes: QtQuick::Item


























## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**allowDeselect**](classQfToggleButtonGroup.md#property-allowdeselect)  <br> |
| property bool | [**allowMultipleSelection**](classQfToggleButtonGroup.md#property-allowmultipleselection)  <br> |
| property real | [**buttonMininumWidth**](classQfToggleButtonGroup.md#property-buttonmininumwidth)  <br> |
| property real | [**buttonSpacing**](classQfToggleButtonGroup.md#property-buttonspacing)  <br> |
| property string | [**checkedRole**](classQfToggleButtonGroup.md#property-checkedrole)  <br> |
| property bool | [**editable**](classQfToggleButtonGroup.md#property-editable)  <br> |
| property bool | [**editing**](classQfToggleButtonGroup.md#property-editing)  <br> |
| property alias | [**font**](classQfToggleButtonGroup.md#property-font)  <br> |
| property alias | [**model**](classQfToggleButtonGroup.md#property-model)  <br> |
| property int | [**selectedIndex**](classQfToggleButtonGroup.md#property-selectedindex)  <br> |
| property string | [**textRole**](classQfToggleButtonGroup.md#property-textrole)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**itemDeselected**](classQfToggleButtonGroup.md#signal-itemdeselected)  <br> |
| signal void | [**itemSelected**](classQfToggleButtonGroup.md#signal-itemselected) (int index, var modelData) <br> |
































## Detailed Description


Provides a consistent UI for selecting single values from a list of options. 


    
## Public Properties Documentation




### property allowDeselect 

```C++
bool QfToggleButtonGroup::allowDeselect;
```



When true, clicking on an already-selected button will deselect it, setting selectedIndex to -1 and emitting the deselected() signal. 


        

<hr>



### property allowMultipleSelection 

```C++
bool QfToggleButtonGroup::allowMultipleSelection;
```



When true, multiple items can be selected, the checkedRole mdoel value will define whether a given index is selected or not. 


        

<hr>



### property buttonMininumWidth 

```C++
real QfToggleButtonGroup::buttonMininumWidth;
```



Minimum width for buttons to handle empty text gracefully 


        

<hr>



### property buttonSpacing 

```C++
real QfToggleButtonGroup::buttonSpacing;
```



Spacing between buttons 


        

<hr>



### property checkedRole 

```C++
string QfToggleButtonGroup::checkedRole;
```




<hr>



### property editable 

```C++
bool QfToggleButtonGroup::editable;
```




<hr>



### property editing 

```C++
bool QfToggleButtonGroup::editing;
```




<hr>



### property font 

```C++
alias QfToggleButtonGroup::font;
```




<hr>



### property model 

```C++
alias QfToggleButtonGroup::model;
```




<hr>



### property selectedIndex 

```C++
int QfToggleButtonGroup::selectedIndex;
```




<hr>



### property textRole 

```C++
string QfToggleButtonGroup::textRole;
```




<hr>
## Public Signals Documentation




### signal itemDeselected 

```C++
void QfToggleButtonGroup::itemDeselected;
```



Emitted when the user deselects the currently selected item (only when allowDeselect is true). The selectedIndex will be -1 after this signal is emitted. 


        

<hr>



### signal itemSelected 

```C++
void QfToggleButtonGroup::itemSelected;
```



Emitted when user selects a button 

**Parameters:**


* `index` The index of the selected item 
* `modelData` The model data for the selected item 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qml/QfToggleButtonGroup.qml`

