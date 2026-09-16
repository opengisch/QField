

# Class QfMapCanvasPointHandler



[**ClassList**](annotated.md) **>** [**QfMapCanvasPointHandler**](classQfMapCanvasPointHandler.md)








Inherits the following classes: QtQuick::Item


Inherited by the following classes: [MapCanvasPointHandler](classMapCanvasPointHandler.md)












## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Priority**](#enum-priority)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property Item | [**coordinateSpace**](classQfMapCanvasPointHandler.md#property-coordinatespace)  <br> |
| property var | [**handlers**](classQfMapCanvasPointHandler.md#property-handlers)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|  void | [**clicked**](#function-clicked) (point, type) <br> |
|  void | [**deregisterHandler**](#function-deregisterhandler) (name) <br> |
|  void | [**doubleClicked**](#function-doubleclicked) (point, type) <br> |
|  void | [**pointInItem**](#function-pointinitem) (point, item) <br> |
|  void | [**pressAndHold**](#function-pressandhold) (point, type) <br> |
|  void | [**processInteraction**](#function-processinteraction) (point, type, interactionType) <br> |
|  void | [**registerHandler**](#function-registerhandler) (name, handler, priority=QfMapCanvasPointHandler.Priority.Normal) <br> |




























## Public Types Documentation




### enum Priority 

```C++
enum QfMapCanvasPointHandler::Priority {
    Low,
    Normal = 50,
    High = 100
};
```




<hr>
## Public Properties Documentation




### property coordinateSpace 

```C++
Item QfMapCanvasPointHandler::coordinateSpace;
```




<hr>



### property handlers 

```C++
var QfMapCanvasPointHandler::handlers;
```




<hr>
## Public Functions Documentation




### function clicked 

```C++
void QfMapCanvasPointHandler::clicked (
    point,
    type
) 
```




<hr>



### function deregisterHandler 

```C++
void QfMapCanvasPointHandler::deregisterHandler (
    name
) 
```




<hr>



### function doubleClicked 

```C++
void QfMapCanvasPointHandler::doubleClicked (
    point,
    type
) 
```




<hr>



### function pointInItem 

```C++
void QfMapCanvasPointHandler::pointInItem (
    point,
    item
) 
```




<hr>



### function pressAndHold 

```C++
void QfMapCanvasPointHandler::pressAndHold (
    point,
    type
) 
```




<hr>



### function processInteraction 

```C++
void QfMapCanvasPointHandler::processInteraction (
    point,
    type,
    interactionType
) 
```




<hr>



### function registerHandler 

```C++
void QfMapCanvasPointHandler::registerHandler (
    name,
    handler,
    priority=QfMapCanvasPointHandler.Priority.Normal
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qml/QfMapCanvasPointHandler.qml`

