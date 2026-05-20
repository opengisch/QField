

# Class GridAnnotation



[**ClassList**](annotated.md) **>** [**GridAnnotation**](classGridAnnotation.md)



[More...](#detailed-description)

* `#include <gridmodel.h>`

















## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Positions**](#enum-positions)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property QPointF | [**coordinate**](classGridAnnotation.md#property-coordinate)  <br> |
| property Positions | [**position**](classGridAnnotation.md#property-position)  <br> |
| property double | [**value**](classGridAnnotation.md#property-value)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**GridAnnotation**](#function-gridannotation) (const Positions position=Top, const QPointF coordinate=QPointF(), const double value=0.0) <br> |




























## Detailed Description


Holds details for a given grid annotation. 


    
## Public Types Documentation




### enum Positions 

```C++
enum GridAnnotation::Positions {
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
QPointF GridAnnotation::coordinate;
```




<hr>



### property position 

```C++
Positions GridAnnotation::position;
```




<hr>



### property value 

```C++
double GridAnnotation::value;
```




<hr>
## Public Functions Documentation




### function GridAnnotation 

```C++
inline GridAnnotation::GridAnnotation (
    const Positions position=Top,
    const QPointF coordinate=QPointF(),
    const double value=0.0
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/gridmodel.h`

