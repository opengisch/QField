

# Class QfPaneDrawer



[**ClassList**](annotated.md) **>** [**QfPaneDrawer**](classQfPaneDrawer.md)








Inherits the following classes: Pane


Inherited by the following classes: [QfBookmarkList](classQfBookmarkList.md),  [QfFeatureListForm](classQfFeatureListForm.md)
























## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**contentVisible**](classQfPaneDrawer.md#property-contentvisible)  <br>_When TRUE the pane is laid out at its resting size, when FALSE it collapses to zero._  |
| property string | [**freezeKey**](classQfPaneDrawer.md#property-freezekey)  <br>_Key passed to mapCanvasMap freeze/unfreeze so concurrent panes does not clear each other._  |
| property real | [**headerHeight**](classQfPaneDrawer.md#property-headerheight)  <br>_Height of the header, used to compute the collapse threshold._  |
| property bool | [**isDragging**](classQfPaneDrawer.md#property-isdragging)  <br>_TRUE while the header is being dragged to resize the pane._  |
| property bool | [**isFullscreen**](classQfPaneDrawer.md#property-isfullscreen)  <br>_When TRUE the pane expands to fill the whole available area._  |
| property bool | [**isVertical**](classQfPaneDrawer.md#property-isvertical)  <br>_TRUE when the pane is laid out vertically (portrait or narrow), driving the drag axis and resize animation direction._  |
| property real | [**minContentHeight**](classQfPaneDrawer.md#property-mincontentheight)  <br>_Content driven minimum height the pane snaps to at rest, clamped to half the available height._  |
| property bool | [**useDefaultMinHeight**](classQfPaneDrawer.md#property-usedefaultminheight)  <br>_When TRUE the resting height ignores a minContentHeight and uses the default minimum._  |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**collapsed**](classQfPaneDrawer.md#signal-collapsed)  <br>_Emitted when a drag releases below the minimum size without entering fullscreen._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**statusIndicatorDragReleased**](#function-statusindicatordragreleased) () <br> |
|  void | [**statusIndicatorDragged**](#function-statusindicatordragged) (deltaX, deltaY) <br> |




























## Public Properties Documentation




### property contentVisible 

_When TRUE the pane is laid out at its resting size, when FALSE it collapses to zero._ 
```C++
bool QfPaneDrawer::contentVisible;
```




<hr>



### property freezeKey 

_Key passed to mapCanvasMap freeze/unfreeze so concurrent panes does not clear each other._ 
```C++
string QfPaneDrawer::freezeKey;
```




<hr>



### property headerHeight 

_Height of the header, used to compute the collapse threshold._ 
```C++
real QfPaneDrawer::headerHeight;
```




<hr>



### property isDragging 

_TRUE while the header is being dragged to resize the pane._ 
```C++
bool QfPaneDrawer::isDragging;
```




<hr>



### property isFullscreen 

_When TRUE the pane expands to fill the whole available area._ 
```C++
bool QfPaneDrawer::isFullscreen;
```




<hr>



### property isVertical 

_TRUE when the pane is laid out vertically (portrait or narrow), driving the drag axis and resize animation direction._ 
```C++
bool QfPaneDrawer::isVertical;
```





**Remark:**

This property is read-only 





        

<hr>



### property minContentHeight 

_Content driven minimum height the pane snaps to at rest, clamped to half the available height._ 
```C++
real QfPaneDrawer::minContentHeight;
```




<hr>



### property useDefaultMinHeight 

_When TRUE the resting height ignores a minContentHeight and uses the default minimum._ 
```C++
bool QfPaneDrawer::useDefaultMinHeight;
```




<hr>
## Public Signals Documentation




### signal collapsed 

_Emitted when a drag releases below the minimum size without entering fullscreen._ 
```C++
void QfPaneDrawer::collapsed;
```




<hr>
## Public Functions Documentation




### function statusIndicatorDragReleased 

```C++
void QfPaneDrawer::statusIndicatorDragReleased () 
```




<hr>



### function statusIndicatorDragged 

```C++
void QfPaneDrawer::statusIndicatorDragged (
    deltaX,
    deltaY
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qml/QfPaneDrawer.qml`

