

# Class BookmarkList



[**ClassList**](annotated.md) **>** [**BookmarkList**](classBookmarkList.md)








Inherits the following classes: [Theme::QfPaneDrawer](classTheme_1_1QfPaneDrawer.md)






































## Public Properties

| Type | Name |
| ---: | :--- |
| property alias | [**model**](classBookmarkList.md#property-model)  <br> |
| property bool | [**multiSelection**](classBookmarkList.md#property-multiselection)  <br> |


## Public Properties inherited from Theme::QfPaneDrawer

See [Theme::QfPaneDrawer](classTheme_1_1QfPaneDrawer.md)

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
| signal void | [**requestBookmarkProperties**](classBookmarkList.md#signal-requestbookmarkproperties) (string bookmarkId, string bookmarkName, string bookmarkGroup) <br> |


## Public Signals inherited from Theme::QfPaneDrawer

See [Theme::QfPaneDrawer](classTheme_1_1QfPaneDrawer.md)

| Type | Name |
| ---: | :--- |
| signal void | [**collapsed**](classTheme_1_1QfPaneDrawer.md#signal-collapsed)  <br>_Emitted when a drag releases below the minimum size without entering fullscreen._  |








## Public Functions inherited from Theme::QfPaneDrawer

See [Theme::QfPaneDrawer](classTheme_1_1QfPaneDrawer.md)

| Type | Name |
| ---: | :--- |
|  void | [**statusIndicatorDragReleased**](classTheme_1_1QfPaneDrawer.md#function-statusindicatordragreleased) () <br> |
|  void | [**statusIndicatorDragged**](classTheme_1_1QfPaneDrawer.md#function-statusindicatordragged) (deltaX, deltaY) <br> |






















































## Public Properties Documentation




### property model 

```C++
alias BookmarkList::model;
```




<hr>



### property multiSelection 

```C++
bool BookmarkList::multiSelection;
```




<hr>
## Public Signals Documentation




### signal requestBookmarkProperties 

```C++
void BookmarkList::requestBookmarkProperties;
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/BookmarkList.qml`

