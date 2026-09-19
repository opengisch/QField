

# Class QfMarkupItem



[**ClassList**](annotated.md) **>** [**QfMarkupItem**](classQfMarkupItem.md)



_This class represents a markup item._ 

* `#include <qfmarkupitem.h>`





























## Public Properties

| Type | Name |
| ---: | :--- |
| property QColor | [**color**](classQfMarkupItem.md#property-color-12)  <br> |
| property QString | [**description**](classQfMarkupItem.md#property-description-12)  <br> |
| property QgsGeometry | [**geometry**](classQfMarkupItem.md#property-geometry-12)  <br> |
| property QString | [**label**](classQfMarkupItem.md#property-label-12)  <br> |








## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfMarkupItem**](#function-qfmarkupitem-12) () = default<br> |
|   | [**QfMarkupItem**](#function-qfmarkupitem-22) (const QString & label, const QString & description, const QgsGeometry & geometry, const QColor & color) <br> |
|  QColor | [**color**](#function-color-22) () const<br> |
|  QString | [**description**](#function-description-22) () const<br> |
|  QgsGeometry | [**geometry**](#function-geometry-22) () const<br> |
|  QString | [**label**](#function-label-22) () const<br> |




























## Public Properties Documentation




### property color [1/2]

```C++
QColor QfMarkupItem::color;
```




<hr>



### property description [1/2]

```C++
QString QfMarkupItem::description;
```




<hr>



### property geometry [1/2]

```C++
QgsGeometry QfMarkupItem::geometry;
```




<hr>



### property label [1/2]

```C++
QString QfMarkupItem::label;
```




<hr>
## Public Functions Documentation




### function QfMarkupItem [1/2]

```C++
QfMarkupItem::QfMarkupItem () = default
```




<hr>



### function QfMarkupItem [2/2]

```C++
explicit QfMarkupItem::QfMarkupItem (
    const QString & label,
    const QString & description,
    const QgsGeometry & geometry,
    const QColor & color
) 
```




<hr>



### function color [2/2]

```C++
inline QColor QfMarkupItem::color () const
```



Returns the markup item color. 


        

<hr>



### function description [2/2]

```C++
inline QString QfMarkupItem::description () const
```



Returns the markup item description. 


        

<hr>



### function geometry [2/2]

```C++
inline QgsGeometry QfMarkupItem::geometry () const
```



Returns the markup item geometry. 

**Note:**

The geometry CRS must be WGS84 





        

<hr>



### function label [2/2]

```C++
inline QString QfMarkupItem::label () const
```



Returns the markup item label. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfmarkupitem.h`

