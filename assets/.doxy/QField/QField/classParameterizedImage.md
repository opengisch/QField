

# Class ParameterizedImage



[**ClassList**](annotated.md) **>** [**ParameterizedImage**](classParameterizedImage.md)








Inherits the following classes: QQuickPaintedItem


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QColor | [**fillColor**](classParameterizedImage.md#property-fillcolor-12)  <br> |
| property QVariantMap | [**parameters**](classParameterizedImage.md#property-parameters-12)  <br> |
| property QString | [**source**](classParameterizedImage.md#property-source-12)  <br> |
| property QColor | [**strokeColor**](classParameterizedImage.md#property-strokecolor-12)  <br> |
| property double | [**strokeWidth**](classParameterizedImage.md#property-strokewidth-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**fillColorChanged**](classParameterizedImage.md#signal-fillcolorchanged)  <br> |
| signal void | [**parametersChanged**](classParameterizedImage.md#signal-parameterschanged)  <br> |
| signal void | [**sourceChanged**](classParameterizedImage.md#signal-sourcechanged)  <br> |
| signal void | [**strokeColorChanged**](classParameterizedImage.md#signal-strokecolorchanged)  <br> |
| signal void | [**strokeWidthChanged**](classParameterizedImage.md#signal-strokewidthchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ParameterizedImage**](#function-parameterizedimage) (QQuickItem \* parent=nullptr) <br> |
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
|   | [**~ParameterizedImage**](#function-parameterizedimage) () = default<br> |




























## Public Properties Documentation




### property fillColor [1/2]

```C++
QColor ParameterizedImage::fillColor;
```



The fill color used to paint an image. 

**Note:**

Compatible with parameterized SVG images only. 





        

<hr>



### property parameters [1/2]

```C++
QVariantMap ParameterizedImage::parameters;
```



The parameters used to paint an image. 

**Note:**

Compatible with parameterized SVG images only. 





        

<hr>



### property source [1/2]

```C++
QString ParameterizedImage::source;
```



The source image. 


        

<hr>



### property strokeColor [1/2]

```C++
QColor ParameterizedImage::strokeColor;
```



The stroke color used to paint an image. 

**Note:**

Compatible with parameterized SVG images only. 





        

<hr>



### property strokeWidth [1/2]

```C++
double ParameterizedImage::strokeWidth;
```



The stroke width used to paint an image. 

**Note:**

Compatible with parameterized SVG images only. 





        

<hr>
## Public Signals Documentation




### signal fillColorChanged 

```C++
void ParameterizedImage::fillColorChanged;
```




<hr>



### signal parametersChanged 

```C++
void ParameterizedImage::parametersChanged;
```




<hr>



### signal sourceChanged 

```C++
void ParameterizedImage::sourceChanged;
```




<hr>



### signal strokeColorChanged 

```C++
void ParameterizedImage::strokeColorChanged;
```




<hr>



### signal strokeWidthChanged 

```C++
void ParameterizedImage::strokeWidthChanged;
```




<hr>
## Public Functions Documentation




### function ParameterizedImage 

```C++
ParameterizedImage::ParameterizedImage (
    QQuickItem * parent=nullptr
) 
```




<hr>



### function fillColor [2/2]

```C++
QColor ParameterizedImage::fillColor () const
```



The fill color used to paint an image. 

**Note:**

Compatible with parameterized SVG images only.   





        

<hr>



### function paint 

```C++
void ParameterizedImage::paint (
    QPainter * painter
) override
```




<hr>



### function parameters [2/2]

```C++
QVariantMap ParameterizedImage::parameters () const
```



The parameters used to paint an image. 

**Note:**

Compatible with parameterized SVG images only.   





        

<hr>



### function setFillColor 

```C++
void ParameterizedImage::setFillColor (
    const QColor & color
) 
```



The fill color used to paint an image. 

**Note:**

Compatible with parameterized SVG images only.   





        

<hr>



### function setParameters 

```C++
void ParameterizedImage::setParameters (
    const QVariantMap & parameters
) 
```



The parameters used to paint an image. 

**Note:**

Compatible with parameterized SVG images only.   





        

<hr>



### function setSource 

```C++
void ParameterizedImage::setSource (
    const QString & source
) 
```



The source image.   


        

<hr>



### function setStrokeColor 

```C++
void ParameterizedImage::setStrokeColor (
    const QColor & color
) 
```



The stroke color used to paint an image. 

**Note:**

Compatible with parameterized SVG images only.   





        

<hr>



### function setStrokeWidth 

```C++
void ParameterizedImage::setStrokeWidth (
    double width
) 
```



The stroke width used to paint an image. 

**Note:**

Compatible with parameterized SVG images only.   





        

<hr>



### function source [2/2]

```C++
QString ParameterizedImage::source () const
```



The source image.   


        

<hr>



### function strokeColor [2/2]

```C++
QColor ParameterizedImage::strokeColor () const
```



The stroke color used to paint an image. 

**Note:**

Compatible with parameterized SVG images only.   





        

<hr>



### function strokeWidth [2/2]

```C++
double ParameterizedImage::strokeWidth () const
```



The stroke width used to paint an image. 

**Note:**

Compatible with parameterized SVG images only.   





        

<hr>



### function ~ParameterizedImage 

```C++
ParameterizedImage::~ParameterizedImage () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/parameterizedimage.h`

