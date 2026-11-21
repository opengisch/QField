

# Class QfNewsItem



[**ClassList**](annotated.md) **>** [**QfNewsItem**](classQfNewsItem.md)



_A class containing information on a news item._ 

* `#include <qfnewsparser.h>`





























## Public Properties

| Type | Name |
| ---: | :--- |
| property QString | [**description**](classQfNewsItem.md#property-description)  <br>_The news item short plain text description._  |
| property QUrl | [**imageUrl**](classQfNewsItem.md#property-imageurl)  <br>_The news item illustration image URL._  |
| property QDateTime | [**publicationDate**](classQfNewsItem.md#property-publicationdate)  <br>_The news item publication date._  |
| property QString | [**title**](classQfNewsItem.md#property-title)  <br>_The news item title._  |
| property QUrl | [**url**](classQfNewsItem.md#property-url)  <br>_The news item URL._  |
| property QString | [**uuid**](classQfNewsItem.md#property-uuid)  <br>_The news item unique identifier._  |








## Public Functions

| Type | Name |
| ---: | :--- |
|  bool | [**operator!=**](#function-operator) (const [**QfNewsItem**](classQfNewsItem.md) & other) const<br> |
|  bool | [**operator==**](#function-operator_1) (const [**QfNewsItem**](classQfNewsItem.md) & other) const<br> |




























## Public Properties Documentation




### property description 

_The news item short plain text description._ 
```C++
QString QfNewsItem::description;
```




<hr>



### property imageUrl 

_The news item illustration image URL._ 
```C++
QUrl QfNewsItem::imageUrl;
```




<hr>



### property publicationDate 

_The news item publication date._ 
```C++
QDateTime QfNewsItem::publicationDate;
```




<hr>



### property title 

_The news item title._ 
```C++
QString QfNewsItem::title;
```




<hr>



### property url 

_The news item URL._ 
```C++
QUrl QfNewsItem::url;
```




<hr>



### property uuid 

_The news item unique identifier._ 
```C++
QString QfNewsItem::uuid;
```




<hr>
## Public Functions Documentation




### function operator!= 

```C++
inline bool QfNewsItem::operator!= (
    const QfNewsItem & other
) const
```




<hr>



### function operator== 

```C++
inline bool QfNewsItem::operator== (
    const QfNewsItem & other
) const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfnewsparser.h`

