

# Class MapCanvasPointHandler



[**ClassList**](annotated.md) **>** [**MapCanvasPointHandler**](classMapCanvasPointHandler.md)








Inherits the following classes: QtQuick::Item














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Priority**](#enum-priority)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property var | [**handlers**](classMapCanvasPointHandler.md#property-handlers)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**clicked**](#function-clicked) ([**point**](classpoint.md), type) <br> |
|  void | [**deregisterHandler**](#function-deregisterhandler) (name) <br> |
|  void | [**doubleClicked**](#function-doubleclicked) ([**point**](classpoint.md), type) <br> |
|  void | [**pointInItem**](#function-pointinitem) ([**point**](classpoint.md), item) <br> |
|  void | [**pressAndHold**](#function-pressandhold) ([**point**](classpoint.md), type) <br> |
|  void | [**processInteraction**](#function-processinteraction) ([**point**](classpoint.md), type, interactionType) <br> |
|  void | [**registerHandler**](#function-registerhandler) (name, handler, priority=MapCanvasPointHandler.Priority.Normal) <br> |




























## Public Types Documentation




### enum Priority 

```C++
enum MapCanvasPointHandler::Priority {
    Low,
    Normal = 50,
    High = 100
};
```




<hr>
## Public Properties Documentation




### property handlers 

```C++
var MapCanvasPointHandler::handlers;
```




<hr>
## Public Functions Documentation




### function clicked 

```C++
void MapCanvasPointHandler::clicked (
    point,
    type
) 
```




<hr>



### function deregisterHandler 

```C++
void MapCanvasPointHandler::deregisterHandler (
    name
) 
```




<hr>



### function doubleClicked 

```C++
void MapCanvasPointHandler::doubleClicked (
    point,
    type
) 
```




<hr>



### function pointInItem 

```C++
void MapCanvasPointHandler::pointInItem (
    point,
    item
) 
```




<hr>



### function pressAndHold 

```C++
void MapCanvasPointHandler::pressAndHold (
    point,
    type
) 
```




<hr>



### function processInteraction 

```C++
void MapCanvasPointHandler::processInteraction (
    point,
    type,
    interactionType
) 
```




<hr>



### function registerHandler 

```C++
void MapCanvasPointHandler::registerHandler (
    name,
    handler,
    priority=MapCanvasPointHandler.Priority.Normal
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/qml/MapCanvasPointHandler.qml`

