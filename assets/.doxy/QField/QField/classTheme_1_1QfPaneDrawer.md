

# Class Theme::QfPaneDrawer



[**ClassList**](annotated.md) **>** [**Theme**](namespaceTheme.md) **>** [**QfPaneDrawer**](classTheme_1_1QfPaneDrawer.md)



[More...](#detailed-description)




Inherits the following classes: Pane


Inherited by the following classes: [BookmarkList](classBookmarkList.md),  [FeatureListForm](classFeatureListForm.md)
























## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**contentVisible**](classTheme_1_1QfPaneDrawer.md#property-contentvisible)  <br>_When TRUE the pane is laid out at its resting size, when FALSE it collapses to zero._  |
| property string | [**freezeKey**](classTheme_1_1QfPaneDrawer.md#property-freezekey)  <br>_Key passed to mapCanvasMap freeze/unfreeze so concurrent panes does not clear each other._  |
| property real | [**headerHeight**](classTheme_1_1QfPaneDrawer.md#property-headerheight)  <br>_Height of the header, used to compute the collapse threshold._  |
| property bool | [**isDragging**](classTheme_1_1QfPaneDrawer.md#property-isdragging)  <br>_TRUE while the header is being dragged to resize the pane._  |
| property bool | [**isFullscreen**](classTheme_1_1QfPaneDrawer.md#property-isfullscreen)  <br>_When TRUE the pane expands to fill the whole available area._  |
| property bool | [**isVertical**](classTheme_1_1QfPaneDrawer.md#property-isvertical)  <br>_TRUE when the pane is laid out vertically (portrait or narrow), driving the drag axis and resize animation direction._  |
| property real | [**minContentHeight**](classTheme_1_1QfPaneDrawer.md#property-mincontentheight)  <br>_Content driven minimum height the pane snaps to at rest, clamped to half the available height._  |
| property bool | [**useDefaultMinHeight**](classTheme_1_1QfPaneDrawer.md#property-usedefaultminheight)  <br>_When TRUE the resting height ignores a minContentHeight and uses the default minimum._  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**collapsed**](classTheme_1_1QfPaneDrawer.md#signal-collapsed)  <br>_Emitted when a drag releases below the minimum size without entering fullscreen._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**statusIndicatorDragReleased**](#function-statusindicatordragreleased) () <br> |
|  void | [**statusIndicatorDragged**](#function-statusindicatordragged) (deltaX, deltaY) <br> |




























## Detailed Description



 
**Import Statement** 
 
```C++
import Theme 
```
 

**Version:**

1.0 





    
## Public Properties Documentation




### property contentVisible 

_When TRUE the pane is laid out at its resting size, when FALSE it collapses to zero._ 
```C++
bool Theme::QfPaneDrawer::contentVisible;
```




<hr>



### property freezeKey 

_Key passed to mapCanvasMap freeze/unfreeze so concurrent panes does not clear each other._ 
```C++
string Theme::QfPaneDrawer::freezeKey;
```




<hr>



### property headerHeight 

_Height of the header, used to compute the collapse threshold._ 
```C++
real Theme::QfPaneDrawer::headerHeight;
```




<hr>



### property isDragging 

_TRUE while the header is being dragged to resize the pane._ 
```C++
bool Theme::QfPaneDrawer::isDragging;
```




<hr>



### property isFullscreen 

_When TRUE the pane expands to fill the whole available area._ 
```C++
bool Theme::QfPaneDrawer::isFullscreen;
```




<hr>



### property isVertical 

_TRUE when the pane is laid out vertically (portrait or narrow), driving the drag axis and resize animation direction._ 
```C++
bool Theme::QfPaneDrawer::isVertical;
```





**Remark:**

This property is read-only 





        

<hr>



### property minContentHeight 

_Content driven minimum height the pane snaps to at rest, clamped to half the available height._ 
```C++
real Theme::QfPaneDrawer::minContentHeight;
```




<hr>



### property useDefaultMinHeight 

_When TRUE the resting height ignores a minContentHeight and uses the default minimum._ 
```C++
bool Theme::QfPaneDrawer::useDefaultMinHeight;
```




<hr>
## Public Signals Documentation




### signal collapsed 

_Emitted when a drag releases below the minimum size without entering fullscreen._ 
```C++
void Theme::QfPaneDrawer::collapsed;
```




<hr>
## Public Functions Documentation




### function statusIndicatorDragReleased 

```C++
void Theme::QfPaneDrawer::statusIndicatorDragReleased () 
```




<hr>



### function statusIndicatorDragged 

```C++
void Theme::QfPaneDrawer::statusIndicatorDragged (
    deltaX,
    deltaY
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/imports/Theme/QfPaneDrawer.qml`

