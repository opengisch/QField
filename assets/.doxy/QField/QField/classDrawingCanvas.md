

# Class DrawingCanvas



[**ClassList**](annotated.md) **>** [**DrawingCanvas**](classDrawingCanvas.md)








Inherits the following classes: QQuickPaintedItem


























## Public Properties

| Type | Name |
| ---: | :--- |
| property [**DrawingStroke**](structDrawingStroke.md) | [**currentStroke**](classDrawingCanvas.md#property-currentstroke-12)  <br> |
| property QColor | [**frameColor**](classDrawingCanvas.md#property-framecolor-12)  <br> |
| property bool | [**isDirty**](classDrawingCanvas.md#property-isdirty-12)  <br> |
| property bool | [**isEmpty**](classDrawingCanvas.md#property-isempty-12)  <br> |
| property QPointF | [**offset**](classDrawingCanvas.md#property-offset-12)  <br> |
| property double | [**zoomFactor**](classDrawingCanvas.md#property-zoomfactor-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**currentStrokeChanged**](classDrawingCanvas.md#signal-currentstrokechanged)  <br> |
| signal void | [**frameColorChanged**](classDrawingCanvas.md#signal-framecolorchanged)  <br> |
| signal void | [**isDirtyChanged**](classDrawingCanvas.md#signal-isdirtychanged)  <br> |
| signal void | [**isEmptyChanged**](classDrawingCanvas.md#signal-isemptychanged)  <br> |
| signal void | [**offsetChanged**](classDrawingCanvas.md#signal-offsetchanged)  <br> |
| signal void | [**zoomFactorChanged**](classDrawingCanvas.md#signal-zoomfactorchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**DrawingCanvas**](#function-drawingcanvas) (QQuickItem \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**clear**](#function-clear) () <br> |
|  Q\_INVOKABLE void | [**createBlankCanvas**](#function-createblankcanvas) (int width, int height, QColor backgroundColor=QColor(255, 255, 255)) <br> |
|  Q\_INVOKABLE void | [**createCanvasFromImage**](#function-createcanvasfromimage) (const QString & path) <br> |
|  [**DrawingStroke**](structDrawingStroke.md) | [**currentStroke**](#function-currentstroke-22) () const<br> |
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
|   | [**~DrawingCanvas**](#function-drawingcanvas) () = default<br> |




























## Public Properties Documentation




### property currentStroke [1/2]

```C++
DrawingStroke DrawingCanvas::currentStroke;
```




<hr>



### property frameColor [1/2]

```C++
QColor DrawingCanvas::frameColor;
```



This property holds the color of the canvas frame. 


        

<hr>



### property isDirty [1/2]

```C++
bool DrawingCanvas::isDirty;
```



This property is set to TRUE when a created canvas has been modified. 


        

<hr>



### property isEmpty [1/2]

```C++
bool DrawingCanvas::isEmpty;
```



This property is set to TRUE when no canvas has been created or a previously created canvas has been cleared. 


        

<hr>



### property offset [1/2]

```C++
QPointF DrawingCanvas::offset;
```



This property holds the offset from the center of the canvas as a result of panning operations. 


        

<hr>



### property zoomFactor [1/2]

```C++
double DrawingCanvas::zoomFactor;
```



This property holds the current zoom factor of the canvas. A value of 1.0 means the canvas is at native resolutio (i.e. one screen pixel represents one canvas pixel). 


        

<hr>
## Public Signals Documentation




### signal currentStrokeChanged 

```C++
void DrawingCanvas::currentStrokeChanged;
```




<hr>



### signal frameColorChanged 

```C++
void DrawingCanvas::frameColorChanged;
```




<hr>



### signal isDirtyChanged 

```C++
void DrawingCanvas::isDirtyChanged;
```




<hr>



### signal isEmptyChanged 

```C++
void DrawingCanvas::isEmptyChanged;
```




<hr>



### signal offsetChanged 

```C++
void DrawingCanvas::offsetChanged;
```




<hr>



### signal zoomFactorChanged 

```C++
void DrawingCanvas::zoomFactorChanged;
```




<hr>
## Public Functions Documentation




### function DrawingCanvas 

```C++
DrawingCanvas::DrawingCanvas (
    QQuickItem * parent=nullptr
) 
```




<hr>



### function clear 

```C++
Q_INVOKABLE void DrawingCanvas::clear () 
```



Clears the drawing canvas. 

**See also:** [**isEmpty()**](classDrawingCanvas.md#property-isempty-12) 



        

<hr>



### function createBlankCanvas 

```C++
Q_INVOKABLE void DrawingCanvas::createBlankCanvas (
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
Q_INVOKABLE void DrawingCanvas::createCanvasFromImage (
    const QString & path
) 
```



Creates a drawing canvas from a given image which will be the background on which the drawing will be overlayed. 

**Parameters:**


* `path` the image path. 




        

<hr>



### function currentStroke [2/2]

```C++
DrawingStroke DrawingCanvas::currentStroke () const
```




<hr>



### function fitCanvas 

```C++
Q_INVOKABLE void DrawingCanvas::fitCanvas () 
```



Fits the drawing canvas to match available width and height. 


        

<hr>



### function frameColor [2/2]

```C++
QColor DrawingCanvas::frameColor () const
```



This property holds the color of the canvas frame.   


        

<hr>



### function isDirty [2/2]

```C++
bool DrawingCanvas::isDirty () const
```



This property is set to TRUE when a created canvas has been modified.   


        

<hr>



### function isEmpty [2/2]

```C++
bool DrawingCanvas::isEmpty () const
```



This property is set to TRUE when no canvas has been created or a previously created canvas has been cleared.   


        

<hr>



### function offset [2/2]

```C++
QPointF DrawingCanvas::offset () const
```



This property holds the offset from the center of the canvas as a result of panning operations.   


        

<hr>



### function paint 

```C++
void DrawingCanvas::paint (
    QPainter * painter
) override
```




<hr>



### function pan 

```C++
Q_INVOKABLE void DrawingCanvas::pan (
    const QPointF & oldPosition,
    const QPointF & newPosition
) 
```



Pans the drawing canvas by the distance between two points. 


        

<hr>



### function save 

```C++
Q_INVOKABLE QString DrawingCanvas::save () const
```



Saves the drawing canvas to a temporary location. 

**Returns:**

the temporary file path of the saved image. 





        

<hr>



### function setFrameColor 

```C++
void DrawingCanvas::setFrameColor (
    const QColor & color
) 
```



This property holds the color of the canvas frame.   


        

<hr>



### function setIsDirty 

```C++
void DrawingCanvas::setIsDirty (
    bool dirty
) 
```



This property is set to TRUE when a created canvas has been modified.   


        

<hr>



### function setIsEmpty 

```C++
void DrawingCanvas::setIsEmpty (
    bool empty
) 
```



This property is set to TRUE when no canvas has been created or a previously created canvas has been cleared.   


        

<hr>



### function setOffset 

```C++
void DrawingCanvas::setOffset (
    const QPointF & offset
) 
```



This property holds the offset from the center of the canvas as a result of panning operations.   


        

<hr>



### function setZoomFactor 

```C++
void DrawingCanvas::setZoomFactor (
    double factor
) 
```



This property holds the current zoom factor of the canvas. A value of 1.0 means the canvas is at native resolutio (i.e. one screen pixel represents one canvas pixel).   


        

<hr>



### function strokeBegin 

```C++
Q_INVOKABLE void DrawingCanvas::strokeBegin (
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
Q_INVOKABLE void DrawingCanvas::strokeEnd (
    const QPointF & point
) 
```



Ends the current stroke with a final _point_ added to the stroke path. 

**Note:**

The function strokeBegin must have been called prior to this function. 





        

<hr>



### function strokeMove 

```C++
Q_INVOKABLE void DrawingCanvas::strokeMove (
    const QPointF & point
) 
```



Adds a new _point_ to the current stroke path. 

**Note:**

The function strokeBegin must have been called prior to this function. 





        

<hr>



### function undo 

```C++
Q_INVOKABLE void DrawingCanvas::undo () 
```



Undos the last drawing operation. 

**See also:** [**isDirty()**](classDrawingCanvas.md#property-isdirty-12) 



        

<hr>



### function zoom 

```C++
Q_INVOKABLE void DrawingCanvas::zoom (
    double scale
) 
```



Zooms the drawing canvas by the provided _scale_ value. 


        

<hr>



### function zoomFactor [2/2]

```C++
double DrawingCanvas::zoomFactor () const
```



This property holds the current zoom factor of the canvas. A value of 1.0 means the canvas is at native resolutio (i.e. one screen pixel represents one canvas pixel).   


        

<hr>



### function ~DrawingCanvas 

```C++
DrawingCanvas::~DrawingCanvas () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/drawingcanvas.h`

