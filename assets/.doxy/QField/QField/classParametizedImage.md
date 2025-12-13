

# Class ParametizedImage



[**ClassList**](annotated.md) **>** [**ParametizedImage**](classParametizedImage.md)








Inherits the following classes: QQuickPaintedItem


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QColor | [**fillColor**](classParametizedImage.md#property-fillcolor-12)  <br> |
| property QString | [**source**](classParametizedImage.md#property-source-12)  <br> |
| property QColor | [**strokeColor**](classParametizedImage.md#property-strokecolor-12)  <br> |
| property double | [**strokeWidth**](classParametizedImage.md#property-strokewidth-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**fillColorChanged**](classParametizedImage.md#signal-fillcolorchanged)  <br> |
| signal void | [**sourceChanged**](classParametizedImage.md#signal-sourcechanged)  <br> |
| signal void | [**strokeColorChanged**](classParametizedImage.md#signal-strokecolorchanged)  <br> |
| signal void | [**strokeWidthChanged**](classParametizedImage.md#signal-strokewidthchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ParametizedImage**](#function-parametizedimage) (QQuickItem \* parent=nullptr) <br> |
|  QColor | [**fillColor**](#function-fillcolor-22) () const<br> |
|  void | [**paint**](#function-paint) (QPainter \* painter) override<br> |
|  void | [**setFillColor**](#function-setfillcolor) (const QColor & color) <br> |
|  void | [**setSource**](#function-setsource) (const QString & source) <br> |
|  void | [**setStrokeColor**](#function-setstrokecolor) (const QColor & color) <br> |
|  void | [**setStrokeWidth**](#function-setstrokewidth) (double width) <br> |
|  QString | [**source**](#function-source-22) () const<br> |
|  QColor | [**strokeColor**](#function-strokecolor-22) () const<br> |
|  double | [**strokeWidth**](#function-strokewidth-22) () const<br> |
|   | [**~ParametizedImage**](#function-parametizedimage) () = default<br> |




























## Public Properties Documentation




### property fillColor [1/2]

```C++
QColor ParametizedImage::fillColor;
```



Returns the current fill color used to paint an image. 

**Note:**

Compatible with parametized SVG images only. 





        

<hr>



### property source [1/2]

```C++
QString ParametizedImage::source;
```



Returns the current source image. 


        

<hr>



### property strokeColor [1/2]

```C++
QColor ParametizedImage::strokeColor;
```



Returns the current stroke color used to paint an image. 

**Note:**

Compatible with parametized SVG images only. 





        

<hr>



### property strokeWidth [1/2]

```C++
double ParametizedImage::strokeWidth;
```



Returns the current stroke width used to paint an image. 

**Note:**

Compatible with parametized SVG images only. 





        

<hr>
## Public Signals Documentation




### signal fillColorChanged 

```C++
void ParametizedImage::fillColorChanged;
```




<hr>



### signal sourceChanged 

```C++
void ParametizedImage::sourceChanged;
```




<hr>



### signal strokeColorChanged 

```C++
void ParametizedImage::strokeColorChanged;
```




<hr>



### signal strokeWidthChanged 

```C++
void ParametizedImage::strokeWidthChanged;
```




<hr>
## Public Functions Documentation




### function ParametizedImage 

```C++
ParametizedImage::ParametizedImage (
    QQuickItem * parent=nullptr
) 
```




<hr>



### function fillColor [2/2]

```C++
QColor ParametizedImage::fillColor () const
```



Returns the current fill color used to paint an image. 

**Note:**

Compatible with parametized SVG images only.   





        

<hr>



### function paint 

```C++
void ParametizedImage::paint (
    QPainter * painter
) override
```




<hr>



### function setFillColor 

```C++
void ParametizedImage::setFillColor (
    const QColor & color
) 
```



Returns the current fill color used to paint an image. 

**Note:**

Compatible with parametized SVG images only.   





        

<hr>



### function setSource 

```C++
void ParametizedImage::setSource (
    const QString & source
) 
```



Returns the current source image.   


        

<hr>



### function setStrokeColor 

```C++
void ParametizedImage::setStrokeColor (
    const QColor & color
) 
```



Returns the current stroke color used to paint an image. 

**Note:**

Compatible with parametized SVG images only.   





        

<hr>



### function setStrokeWidth 

```C++
void ParametizedImage::setStrokeWidth (
    double width
) 
```



Returns the current stroke width used to paint an image. 

**Note:**

Compatible with parametized SVG images only.   





        

<hr>



### function source [2/2]

```C++
QString ParametizedImage::source () const
```



Returns the current source image.   


        

<hr>



### function strokeColor [2/2]

```C++
QColor ParametizedImage::strokeColor () const
```



Returns the current stroke color used to paint an image. 

**Note:**

Compatible with parametized SVG images only.   





        

<hr>



### function strokeWidth [2/2]

```C++
double ParametizedImage::strokeWidth () const
```



Returns the current stroke width used to paint an image. 

**Note:**

Compatible with parametized SVG images only.   





        

<hr>



### function ~ParametizedImage 

```C++
ParametizedImage::~ParametizedImage () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/parametizedimage.h`

