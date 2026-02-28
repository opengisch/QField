

# Class Theme::QfToggleButtonGroup



[**ClassList**](annotated.md) **>** [**Theme**](namespaceTheme.md) **>** [**QfToggleButtonGroup**](classTheme_1_1QfToggleButtonGroup.md)



[More...](#detailed-description)




Inherits the following classes: QtQuick::Item


























## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**allowDeselect**](classTheme_1_1QfToggleButtonGroup.md#property-allowdeselect)  <br> |
| property bool | [**allowMultipleSelection**](classTheme_1_1QfToggleButtonGroup.md#property-allowmultipleselection)  <br> |
| property real | [**buttonMininumWidth**](classTheme_1_1QfToggleButtonGroup.md#property-buttonmininumwidth)  <br> |
| property real | [**buttonSpacing**](classTheme_1_1QfToggleButtonGroup.md#property-buttonspacing)  <br> |
| property string | [**checkedRole**](classTheme_1_1QfToggleButtonGroup.md#property-checkedrole)  <br> |
| property bool | [**editable**](classTheme_1_1QfToggleButtonGroup.md#property-editable)  <br> |
| property bool | [**editing**](classTheme_1_1QfToggleButtonGroup.md#property-editing)  <br> |
| property alias | [**font**](classTheme_1_1QfToggleButtonGroup.md#property-font)  <br> |
| property alias | [**model**](classTheme_1_1QfToggleButtonGroup.md#property-model)  <br> |
| property int | [**selectedIndex**](classTheme_1_1QfToggleButtonGroup.md#property-selectedindex)  <br> |
| property string | [**textRole**](classTheme_1_1QfToggleButtonGroup.md#property-textrole)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**itemDeselected**](classTheme_1_1QfToggleButtonGroup.md#signal-itemdeselected)  <br> |
| signal void | [**itemSelected**](classTheme_1_1QfToggleButtonGroup.md#signal-itemselected) (int index, var modelData) <br> |
































## Detailed Description


Provides a consistent UI for selecting single values from a list of options. 
 
**Import Statement** 
 
```C++
import Theme 
```
 

**Version:**

1.0 





    
## Public Properties Documentation




### property allowDeselect 

```C++
bool Theme::QfToggleButtonGroup::allowDeselect;
```



When true, clicking on an already-selected button will deselect it, setting selectedIndex to -1 and emitting the deselected() signal. 


        

<hr>



### property allowMultipleSelection 

```C++
bool Theme::QfToggleButtonGroup::allowMultipleSelection;
```



When true, multiple items can be selected, the checkedRole mdoel value will define whether a given index is selected or not. 


        

<hr>



### property buttonMininumWidth 

```C++
real Theme::QfToggleButtonGroup::buttonMininumWidth;
```



Minimum width for buttons to handle empty text gracefully 


        

<hr>



### property buttonSpacing 

```C++
real Theme::QfToggleButtonGroup::buttonSpacing;
```



Spacing between buttons 


        

<hr>



### property checkedRole 

```C++
string Theme::QfToggleButtonGroup::checkedRole;
```




<hr>



### property editable 

```C++
bool Theme::QfToggleButtonGroup::editable;
```




<hr>



### property editing 

```C++
bool Theme::QfToggleButtonGroup::editing;
```




<hr>



### property font 

```C++
alias Theme::QfToggleButtonGroup::font;
```




<hr>



### property model 

```C++
alias Theme::QfToggleButtonGroup::model;
```




<hr>



### property selectedIndex 

```C++
int Theme::QfToggleButtonGroup::selectedIndex;
```




<hr>



### property textRole 

```C++
string Theme::QfToggleButtonGroup::textRole;
```




<hr>
## Public Signals Documentation




### signal itemDeselected 

```C++
void Theme::QfToggleButtonGroup::itemDeselected;
```



Emitted when the user deselects the currently selected item (only when allowDeselect is true). The selectedIndex will be -1 after this signal is emitted. 


        

<hr>



### signal itemSelected 

```C++
void Theme::QfToggleButtonGroup::itemSelected;
```



Emitted when user selects a button 

**Parameters:**


* `index` The index of the selected item 
* `modelData` The model data for the selected item 




        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/imports/Theme/QfToggleButtonGroup.qml`

