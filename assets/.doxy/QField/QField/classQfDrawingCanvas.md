

# Class QfDrawingCanvas



[**ClassList**](annotated.md) **>** [**QfDrawingCanvas**](classQfDrawingCanvas.md)








Inherits the following classes: QQuickPaintedItem


























## Public Properties

| Type | Name |
| ---: | :--- |
| property [**QfDrawingStroke**](structQfDrawingStroke.md) | [**currentStroke**](classQfDrawingCanvas.md#property-currentstroke-12)  <br> |
| property QColor | [**frameColor**](classQfDrawingCanvas.md#property-framecolor-12)  <br> |
| property bool | [**isDirty**](classQfDrawingCanvas.md#property-isdirty-12)  <br> |
| property bool | [**isEmpty**](classQfDrawingCanvas.md#property-isempty-12)  <br> |
| property QPointF | [**offset**](classQfDrawingCanvas.md#property-offset-12)  <br> |
| property double | [**zoomFactor**](classQfDrawingCanvas.md#property-zoomfactor-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**currentStrokeChanged**](classQfDrawingCanvas.md#signal-currentstrokechanged)  <br> |
| signal void | [**frameColorChanged**](classQfDrawingCanvas.md#signal-framecolorchanged)  <br> |
| signal void | [**isDirtyChanged**](classQfDrawingCanvas.md#signal-isdirtychanged)  <br> |
| signal void | [**isEmptyChanged**](classQfDrawingCanvas.md#signal-isemptychanged)  <br> |
| signal void | [**offsetChanged**](classQfDrawingCanvas.md#signal-offsetchanged)  <br> |
| signal void | [**zoomFactorChanged**](classQfDrawingCanvas.md#signal-zoomfactorchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfDrawingCanvas**](#function-qfdrawingcanvas) (QQuickItem \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**clear**](#function-clear) () <br> |
|  Q\_INVOKABLE void | [**createBlankCanvas**](#function-createblankcanvas) (int width, int height, QColor backgroundColor=QColor(255, 255, 255)) <br> |
|  Q\_INVOKABLE void | [**createCanvasFromImage**](#function-createcanvasfromimage) (const QString & path) <br> |
|  [**QfDrawingStroke**](structQfDrawingStroke.md) | [**currentStroke**](#function-currentstroke-22) () const<br> |
|  Q\_INVOKABLE void | [**fitCanvas**](#function-fitcanvas) () <br> |
|  QColor | [**frameColor**](#function-framecolor-22) () const<br> |
|  bool | [**isDirty**](#function-isdirty-22) () const<br> |
|  bool | [**isEmpty**](#function-isempty-22) () const<br> |
|  QPointF | [**offset**](#function-offset-22) () const<br> |
|  void | [**paint**](#function-paint) (QPainter \* painter) override<br> |
|  Q\_INVOKABLE void | [**pan**](#function-pan) (const QPointF & oldPosition, const QPointF & newPosition) <br> |
|  Q\_INVOKABLE QString | [**save**](#function-save) () const<br> |
|  void | [**setFrameColor**](#function-setframecolor) (const QColor & color) <br> |
|  void | [**setIsDirty**](#function-setisdirty) (bool dirty) <br> |
|  void | [**setIsEmpty**](#function-setisempty) (bool empty) <br> |
|  void | [**setOffset**](#function-setoffset) (const QPointF & offset) <br> |
|  void | [**setZoomFactor**](#function-setzoomfactor) (double factor) <br> |
|  Q\_INVOKABLE void | [**strokeBegin**](#function-strokebegin) (const QPointF & point, const QColor color=QColor(0, 0, 0)) <br> |
|  Q\_INVOKABLE void | [**strokeEnd**](#function-strokeend) (const QPointF & point) <br> |
|  Q\_INVOKABLE void | [**strokeMove**](#function-strokemove) (const QPointF & point) <br> |
|  Q\_INVOKABLE void | [**undo**](#function-undo) () <br> |
|  Q\_INVOKABLE void | [**zoom**](#function-zoom) (double scale) <br> |
|  double | [**zoomFactor**](#function-zoomfactor-22) () const<br> |
|   | [**~QfDrawingCanvas**](#function-qfdrawingcanvas) () = default<br> |




























## Public Properties Documentation




### property currentStroke [1/2]

```C++
QfDrawingStroke QfDrawingCanvas::currentStroke;
```




<hr>



### property frameColor [1/2]

```C++
QColor QfDrawingCanvas::frameColor;
```



This property holds the color of the canvas frame. 


        

<hr>



### property isDirty [1/2]

```C++
bool QfDrawingCanvas::isDirty;
```



This property is set to TRUE when a created canvas has been modified. 


        

<hr>



### property isEmpty [1/2]

```C++
bool QfDrawingCanvas::isEmpty;
```



This property is set to TRUE when no canvas has been created or a previously created canvas has been cleared. 


        

<hr>



### property offset [1/2]

```C++
QPointF QfDrawingCanvas::offset;
```



This property holds the offset from the center of the canvas as a result of panning operations. 


        

<hr>



### property zoomFactor [1/2]

```C++
double QfDrawingCanvas::zoomFactor;
```



This property holds the current zoom factor of the canvas. A value of 1.0 means the canvas is at native resolutio (i.e. one screen pixel represents one canvas pixel). 


        

<hr>
## Public Signals Documentation




### signal currentStrokeChanged 

```C++
void QfDrawingCanvas::currentStrokeChanged;
```




<hr>



### signal frameColorChanged 

```C++
void QfDrawingCanvas::frameColorChanged;
```




<hr>



### signal isDirtyChanged 

```C++
void QfDrawingCanvas::isDirtyChanged;
```




<hr>



### signal isEmptyChanged 

```C++
void QfDrawingCanvas::isEmptyChanged;
```




<hr>



### signal offsetChanged 

```C++
void QfDrawingCanvas::offsetChanged;
```




<hr>



### signal zoomFactorChanged 

```C++
void QfDrawingCanvas::zoomFactorChanged;
```




<hr>
## Public Functions Documentation




### function QfDrawingCanvas 

```C++
QfDrawingCanvas::QfDrawingCanvas (
    QQuickItem * parent=nullptr
) 
```




<hr>



### function clear 

```C++
Q_INVOKABLE void QfDrawingCanvas::clear () 
```



Clears the drawing canvas. 

**See also:** [**isEmpty()**](classQfDrawingCanvas.md#property-isempty-12) 



        

<hr>



### function createBlankCanvas 

```C++
Q_INVOKABLE void QfDrawingCanvas::createBlankCanvas (
    int width,
    int height,
    QColor backgroundColor=QColor(255, 255, 255)
) 
```



Creates a blank drawing canvas. 

**Parameters:**


* `width` the width of the canvas. 
* `height` the height of the canvas. 
* `backgroundColor` the background color of the canvas. 




        

<hr>



### function createCanvasFromImage 

```C++
Q_INVOKABLE void QfDrawingCanvas::createCanvasFromImage (
    const QString & path
) 
```



Creates a drawing canvas from a given image which will be the background on which the drawing will be overlayed. 

**Parameters:**


* `path` the image path. 




        

<hr>



### function currentStroke [2/2]

```C++
QfDrawingStroke QfDrawingCanvas::currentStroke () const
```




<hr>



### function fitCanvas 

```C++
Q_INVOKABLE void QfDrawingCanvas::fitCanvas () 
```



Fits the drawing canvas to match available width and height. 


        

<hr>



### function frameColor [2/2]

```C++
QColor QfDrawingCanvas::frameColor () const
```



This property holds the color of the canvas frame.   


        

<hr>



### function isDirty [2/2]

```C++
bool QfDrawingCanvas::isDirty () const
```



This property is set to TRUE when a created canvas has been modified.   


        

<hr>



### function isEmpty [2/2]

```C++
bool QfDrawingCanvas::isEmpty () const
```



This property is set to TRUE when no canvas has been created or a previously created canvas has been cleared.   


        

<hr>



### function offset [2/2]

```C++
QPointF QfDrawingCanvas::offset () const
```



This property holds the offset from the center of the canvas as a result of panning operations.   


        

<hr>



### function paint 

```C++
void QfDrawingCanvas::paint (
    QPainter * painter
) override
```




<hr>



### function pan 

```C++
Q_INVOKABLE void QfDrawingCanvas::pan (
    const QPointF & oldPosition,
    const QPointF & newPosition
) 
```



Pans the drawing canvas by the distance between two points. 


        

<hr>



### function save 

```C++
Q_INVOKABLE QString QfDrawingCanvas::save () const
```



Saves the drawing canvas to a temporary location. 

**Returns:**

the temporary file path of the saved image. 





        

<hr>



### function setFrameColor 

```C++
void QfDrawingCanvas::setFrameColor (
    const QColor & color
) 
```



This property holds the color of the canvas frame.   


        

<hr>



### function setIsDirty 

```C++
void QfDrawingCanvas::setIsDirty (
    bool dirty
) 
```



This property is set to TRUE when a created canvas has been modified.   


        

<hr>



### function setIsEmpty 

```C++
void QfDrawingCanvas::setIsEmpty (
    bool empty
) 
```



This property is set to TRUE when no canvas has been created or a previously created canvas has been cleared.   


        

<hr>



### function setOffset 

```C++
void QfDrawingCanvas::setOffset (
    const QPointF & offset
) 
```



This property holds the offset from the center of the canvas as a result of panning operations.   


        

<hr>



### function setZoomFactor 

```C++
void QfDrawingCanvas::setZoomFactor (
    double factor
) 
```



This property holds the current zoom factor of the canvas. A value of 1.0 means the canvas is at native resolutio (i.e. one screen pixel represents one canvas pixel).   


        

<hr>



### function strokeBegin 

```C++
Q_INVOKABLE void QfDrawingCanvas::strokeBegin (
    const QPointF & point,
    const QColor color=QColor(0, 0, 0)
) 
```



Begins a stroke operation. 

**Parameters:**


* `point` the first point of the stroke 
* `color` the color of the stroke 




        

<hr>



### function strokeEnd 

```C++
Q_INVOKABLE void QfDrawingCanvas::strokeEnd (
    const QPointF & point
) 
```



Ends the current stroke with a final _point_ added to the stroke path. 

**Note:**

The function strokeBegin must have been called prior to this function. 





        

<hr>



### function strokeMove 

```C++
Q_INVOKABLE void QfDrawingCanvas::strokeMove (
    const QPointF & point
) 
```



Adds a new _point_ to the current stroke path. 

**Note:**

The function strokeBegin must have been called prior to this function. 





        

<hr>



### function undo 

```C++
Q_INVOKABLE void QfDrawingCanvas::undo () 
```



Undos the last drawing operation. 

**See also:** [**isDirty()**](classQfDrawingCanvas.md#property-isdirty-12) 



        

<hr>



### function zoom 

```C++
Q_INVOKABLE void QfDrawingCanvas::zoom (
    double scale
) 
```



Zooms the drawing canvas by the provided _scale_ value. 


        

<hr>



### function zoomFactor [2/2]

```C++
double QfDrawingCanvas::zoomFactor () const
```



This property holds the current zoom factor of the canvas. A value of 1.0 means the canvas is at native resolutio (i.e. one screen pixel represents one canvas pixel).   


        

<hr>



### function ~QfDrawingCanvas 

```C++
QfDrawingCanvas::~QfDrawingCanvas () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfdrawingcanvas.h`

