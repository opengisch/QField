

# Class QfGridAnnotation



[**ClassList**](annotated.md) **>** [**QfGridAnnotation**](classQfGridAnnotation.md)



[More...](#detailed-description)

* `#include <qfgridmodel.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Positions**](#enum-positions)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property QPointF | [**coordinate**](classQfGridAnnotation.md#property-coordinate)  <br> |
| property Positions | [**position**](classQfGridAnnotation.md#property-position)  <br> |
| property double | [**value**](classQfGridAnnotation.md#property-value)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfGridAnnotation**](#function-qfgridannotation) (const Positions position=Top, const QPointF coordinate=QPointF(), const double value=0.0) <br> |




























## Detailed Description


Holds details for a given grid annotation. 


    
## Public Types Documentation




### enum Positions 

```C++
enum QfGridAnnotation::Positions {
    Top,
    Bottom,
    Left,
    Right
};
```




<hr>
## Public Properties Documentation




### property coordinate 

```C++
QPointF QfGridAnnotation::coordinate;
```




<hr>



### property position 

```C++
Positions QfGridAnnotation::position;
```




<hr>



### property value 

```C++
double QfGridAnnotation::value;
```




<hr>
## Public Functions Documentation




### function QfGridAnnotation 

```C++
inline QfGridAnnotation::QfGridAnnotation (
    const Positions position=Top,
    const QPointF coordinate=QPointF(),
    const double value=0.0
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfgridmodel.h`

