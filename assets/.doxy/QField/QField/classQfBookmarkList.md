

# Class QfBookmarkList



[**ClassList**](annotated.md) **>** [**QfBookmarkList**](classQfBookmarkList.md)








Inherits the following classes: [QfPaneDrawer](classQfPaneDrawer.md)


Inherited by the following classes: [BookmarkList](classBookmarkList.md)




































## Public Properties

| Type | Name |
| ---: | :--- |
| property alias | [**model**](classQfBookmarkList.md#property-model)  <br> |
| property bool | [**multiSelection**](classQfBookmarkList.md#property-multiselection)  <br> |


## Public Properties inherited from QfPaneDrawer

See [QfPaneDrawer](classQfPaneDrawer.md)

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
| signal void | [**requestBookmarkNavigation**](classQfBookmarkList.md#signal-requestbookmarknavigation) (int bookmarkIndex) <br> |
| signal void | [**requestBookmarkProperties**](classQfBookmarkList.md#signal-requestbookmarkproperties) (string bookmarkId, string bookmarkName, string bookmarkGroup) <br> |


## Public Signals inherited from QfPaneDrawer

See [QfPaneDrawer](classQfPaneDrawer.md)

| Type | Name |
| ---: | :--- |
| signal void | [**collapsed**](classQfPaneDrawer.md#signal-collapsed)  <br>_Emitted when a drag releases below the minimum size without entering fullscreen._  |








## Public Functions inherited from QfPaneDrawer

See [QfPaneDrawer](classQfPaneDrawer.md)

| Type | Name |
| ---: | :--- |
|  void | [**statusIndicatorDragReleased**](classQfPaneDrawer.md#function-statusindicatordragreleased) () <br> |
|  void | [**statusIndicatorDragged**](classQfPaneDrawer.md#function-statusindicatordragged) (deltaX, deltaY) <br> |






















































## Public Properties Documentation




### property model 

```C++
alias QfBookmarkList::model;
```




<hr>



### property multiSelection 

```C++
bool QfBookmarkList::multiSelection;
```




<hr>
## Public Signals Documentation




### signal requestBookmarkNavigation 

```C++
void QfBookmarkList::requestBookmarkNavigation;
```




<hr>



### signal requestBookmarkProperties 

```C++
void QfBookmarkList::requestBookmarkProperties;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qml/QfBookmarkList.qml`

