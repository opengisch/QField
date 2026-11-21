

# Class QfParameterizedImage



[**ClassList**](annotated.md) **>** [**QfParameterizedImage**](classQfParameterizedImage.md)








Inherits the following classes: QQuickPaintedItem


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QColor | [**fillColor**](classQfParameterizedImage.md#property-fillcolor-12)  <br> |
| property QVariantMap | [**parameters**](classQfParameterizedImage.md#property-parameters-12)  <br> |
| property QString | [**source**](classQfParameterizedImage.md#property-source-12)  <br> |
| property QColor | [**strokeColor**](classQfParameterizedImage.md#property-strokecolor-12)  <br> |
| property double | [**strokeWidth**](classQfParameterizedImage.md#property-strokewidth-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**fillColorChanged**](classQfParameterizedImage.md#signal-fillcolorchanged)  <br> |
| signal void | [**parametersChanged**](classQfParameterizedImage.md#signal-parameterschanged)  <br> |
| signal void | [**sourceChanged**](classQfParameterizedImage.md#signal-sourcechanged)  <br> |
| signal void | [**strokeColorChanged**](classQfParameterizedImage.md#signal-strokecolorchanged)  <br> |
| signal void | [**strokeWidthChanged**](classQfParameterizedImage.md#signal-strokewidthchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfParameterizedImage**](#function-qfparameterizedimage) (QQuickItem \* parent=nullptr) <br> |
|  QColor | [**fillColor**](#function-fillcolor-22) () const<br> |
|  void | [**paint**](#function-paint) (QPainter \* painter) override<br> |
|  QVariantMap | [**parameters**](#function-parameters-22) () const<br> |
|  void | [**setFillColor**](#function-setfillcolor) (const QColor & color) <br> |
|  void | [**setParameters**](#function-setparameters) (const QVariantMap & parameters) <br> |
|  void | [**setSource**](#function-setsource) (const QString & source) <br> |
|  void | [**setStrokeColor**](#function-setstrokecolor) (const QColor & color) <br> |
|  void | [**setStrokeWidth**](#function-setstrokewidth) (double width) <br> |
|  QString | [**source**](#function-source-22) () const<br> |
|  QColor | [**strokeColor**](#function-strokecolor-22) () const<br> |
|  double | [**strokeWidth**](#function-strokewidth-22) () const<br> |
|   | [**~QfParameterizedImage**](#function-qfparameterizedimage) () = default<br> |




























## Public Properties Documentation




### property fillColor [1/2]

```C++
QColor QfParameterizedImage::fillColor;
```



The fill color used to paint an image. 

**Note:**

Compatible with parameterized SVG images only. 





        

<hr>



### property parameters [1/2]

```C++
QVariantMap QfParameterizedImage::parameters;
```



The parameters used to paint an image. 

**Note:**

Compatible with parameterized SVG images only. 





        

<hr>



### property source [1/2]

```C++
QString QfParameterizedImage::source;
```



The source image. 


        

<hr>



### property strokeColor [1/2]

```C++
QColor QfParameterizedImage::strokeColor;
```



The stroke color used to paint an image. 

**Note:**

Compatible with parameterized SVG images only. 





        

<hr>



### property strokeWidth [1/2]

```C++
double QfParameterizedImage::strokeWidth;
```



The stroke width used to paint an image. 

**Note:**

Compatible with parameterized SVG images only. 





        

<hr>
## Public Signals Documentation




### signal fillColorChanged 

```C++
void QfParameterizedImage::fillColorChanged;
```




<hr>



### signal parametersChanged 

```C++
void QfParameterizedImage::parametersChanged;
```




<hr>



### signal sourceChanged 

```C++
void QfParameterizedImage::sourceChanged;
```




<hr>



### signal strokeColorChanged 

```C++
void QfParameterizedImage::strokeColorChanged;
```




<hr>



### signal strokeWidthChanged 

```C++
void QfParameterizedImage::strokeWidthChanged;
```




<hr>
## Public Functions Documentation




### function QfParameterizedImage 

```C++
QfParameterizedImage::QfParameterizedImage (
    QQuickItem * parent=nullptr
) 
```




<hr>



### function fillColor [2/2]

```C++
QColor QfParameterizedImage::fillColor () const
```



The fill color used to paint an image. 

**Note:**

Compatible with parameterized SVG images only.   





        

<hr>



### function paint 

```C++
void QfParameterizedImage::paint (
    QPainter * painter
) override
```




<hr>



### function parameters [2/2]

```C++
QVariantMap QfParameterizedImage::parameters () const
```



The parameters used to paint an image. 

**Note:**

Compatible with parameterized SVG images only.   





        

<hr>



### function setFillColor 

```C++
void QfParameterizedImage::setFillColor (
    const QColor & color
) 
```



The fill color used to paint an image. 

**Note:**

Compatible with parameterized SVG images only.   





        

<hr>



### function setParameters 

```C++
void QfParameterizedImage::setParameters (
    const QVariantMap & parameters
) 
```



The parameters used to paint an image. 

**Note:**

Compatible with parameterized SVG images only.   





        

<hr>



### function setSource 

```C++
void QfParameterizedImage::setSource (
    const QString & source
) 
```



The source image.   


        

<hr>



### function setStrokeColor 

```C++
void QfParameterizedImage::setStrokeColor (
    const QColor & color
) 
```



The stroke color used to paint an image. 

**Note:**

Compatible with parameterized SVG images only.   





        

<hr>



### function setStrokeWidth 

```C++
void QfParameterizedImage::setStrokeWidth (
    double width
) 
```



The stroke width used to paint an image. 

**Note:**

Compatible with parameterized SVG images only.   





        

<hr>



### function source [2/2]

```C++
QString QfParameterizedImage::source () const
```



The source image.   


        

<hr>



### function strokeColor [2/2]

```C++
QColor QfParameterizedImage::strokeColor () const
```



The stroke color used to paint an image. 

**Note:**

Compatible with parameterized SVG images only.   





        

<hr>



### function strokeWidth [2/2]

```C++
double QfParameterizedImage::strokeWidth () const
```



The stroke width used to paint an image. 

**Note:**

Compatible with parameterized SVG images only.   





        

<hr>



### function ~QfParameterizedImage 

```C++
QfParameterizedImage::~QfParameterizedImage () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qfparameterizedimage.h`

