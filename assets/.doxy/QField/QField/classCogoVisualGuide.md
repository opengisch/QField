

# Class CogoVisualGuide



[**ClassList**](annotated.md) **>** [**CogoVisualGuide**](classCogoVisualGuide.md)



_A COGO visual guide._ 

* `#include <cogooperation.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Type**](#enum-type)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property QColor | [**color**](classCogoVisualGuide.md#property-color)  <br> |
| property QVariantMap | [**details**](classCogoVisualGuide.md#property-details)  <br> |
| property QColor | [**outlineColor**](classCogoVisualGuide.md#property-outlinecolor)  <br> |
| property Type | [**type**](classCogoVisualGuide.md#property-type)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CogoVisualGuide**](#function-cogovisualguide) (Type type=Point, const QVariantMap & details=QVariantMap(), const QColor & color=COLOR\_NEUTRAL, const QColor & outlineColor=QColor(255, 255, 255, 127)) <br> |




























## Public Types Documentation




### enum Type 

```C++
enum CogoVisualGuide::Type {
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
QColor CogoVisualGuide::color;
```



The COGO visual guide color. 


        

<hr>



### property details 

```C++
QVariantMap CogoVisualGuide::details;
```



The COGO visual guide details. 


        

<hr>



### property outlineColor 

```C++
QColor CogoVisualGuide::outlineColor;
```



The COGO visual guide outline color. 


        

<hr>



### property type 

```C++
Type CogoVisualGuide::type;
```



The COGO visual guide type (point, line, circle, label, etc.) 


        

<hr>
## Public Functions Documentation




### function CogoVisualGuide 

```C++
inline explicit CogoVisualGuide::CogoVisualGuide (
    Type type=Point,
    const QVariantMap & details=QVariantMap(),
    const QColor & color=COLOR_NEUTRAL,
    const QColor & outlineColor=QColor(255, 255, 255, 127)
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/cogo/cogooperation.h`

