

# Class QfCogoVisualGuide



[**ClassList**](annotated.md) **>** [**QfCogoVisualGuide**](classQfCogoVisualGuide.md)



_A COGO visual guide._ 

* `#include <qfcogooperation.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Type**](#enum-type)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property QColor | [**color**](classQfCogoVisualGuide.md#property-color)  <br> |
| property QVariantMap | [**details**](classQfCogoVisualGuide.md#property-details)  <br> |
| property QColor | [**outlineColor**](classQfCogoVisualGuide.md#property-outlinecolor)  <br> |
| property Type | [**type**](classQfCogoVisualGuide.md#property-type)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfCogoVisualGuide**](#function-qfcogovisualguide) (Type type=Point, const QVariantMap & details=QVariantMap(), const QColor & color=COLOR\_NEUTRAL, const QColor & outlineColor=QColor(255, 255, 255, 127)) <br> |




























## Public Types Documentation




### enum Type 

```C++
enum QfCogoVisualGuide::Type {
    Point,
    Line,
    Circle,
    Label
};
```




<hr>
## Public Properties Documentation




### property color 

```C++
QColor QfCogoVisualGuide::color;
```



The COGO visual guide color. 


        

<hr>



### property details 

```C++
QVariantMap QfCogoVisualGuide::details;
```



The COGO visual guide details. 


        

<hr>



### property outlineColor 

```C++
QColor QfCogoVisualGuide::outlineColor;
```



The COGO visual guide outline color. 


        

<hr>



### property type 

```C++
Type QfCogoVisualGuide::type;
```



The COGO visual guide type (point, line, circle, label, etc.) 


        

<hr>
## Public Functions Documentation




### function QfCogoVisualGuide 

```C++
inline explicit QfCogoVisualGuide::QfCogoVisualGuide (
    Type type=Point,
    const QVariantMap & details=QVariantMap(),
    const QColor & color=COLOR_NEUTRAL,
    const QColor & outlineColor=QColor(255, 255, 255, 127)
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/cogo/qfcogooperation.h`

