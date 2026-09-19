

# Class QfMarkupCollection



[**ClassList**](annotated.md) **>** [**QfMarkupCollection**](classQfMarkupCollection.md)



_This class holds a collection of markup items._ 

* `#include <qfmarkupcollection.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property qsizetype | [**count**](classQfMarkupCollection.md#property-count-12)  <br> |
| property QList&lt; [**QfMarkupItem**](classQfMarkupItem.md) &gt; | [**items**](classQfMarkupCollection.md#property-items-12)  <br> |
| property QString | [**name**](classQfMarkupCollection.md#property-name-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**countChanged**](classQfMarkupCollection.md#signal-countchanged)  <br>_Emitted when the number of items in the collection changed._  |
| signal void | [**itemsChanged**](classQfMarkupCollection.md#signal-itemschanged)  <br>_Emitted when the list of items in the collection changed._  |
| signal void | [**nameChanged**](classQfMarkupCollection.md#signal-namechanged)  <br>_Emitted when the collection name changed._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfMarkupCollection**](#function-qfmarkupcollection) (const QString & name=QString(), QObject \* parent=nullptr) <br> |
|  void | [**addItem**](#function-additem) (const [**QfMarkupItem**](classQfMarkupItem.md) & item) <br> |
|  qsizetype | [**count**](#function-count-22) () const<br> |
|  QList&lt; [**QfMarkupItem**](classQfMarkupItem.md) &gt; | [**items**](#function-items-22) () const<br> |
|  QString | [**name**](#function-name-22) () const<br> |
|  bool | [**readGeoJson**](#function-readgeojson) (const QString & path) <br> |
|  void | [**removeItem**](#function-removeitem) (const QString & uuid) <br> |
|  void | [**replaceItem**](#function-replaceitem) (const QString & uuid, const [**QfMarkupItem**](classQfMarkupItem.md) & item) <br> |
|  void | [**setName**](#function-setname) (const QString & name) <br> |
|  bool | [**writeGeoJson**](#function-writegeojson) (const QString & path) <br> |




























## Public Properties Documentation




### property count [1/2]

```C++
qsizetype QfMarkupCollection::count;
```




<hr>



### property items [1/2]

```C++
QList<QfMarkupItem> QfMarkupCollection::items;
```




<hr>



### property name [1/2]

```C++
QString QfMarkupCollection::name;
```




<hr>
## Public Signals Documentation




### signal countChanged 

_Emitted when the number of items in the collection changed._ 
```C++
void QfMarkupCollection::countChanged;
```




<hr>



### signal itemsChanged 

_Emitted when the list of items in the collection changed._ 
```C++
void QfMarkupCollection::itemsChanged;
```




<hr>



### signal nameChanged 

_Emitted when the collection name changed._ 
```C++
void QfMarkupCollection::nameChanged;
```




<hr>
## Public Functions Documentation




### function QfMarkupCollection 

```C++
explicit QfMarkupCollection::QfMarkupCollection (
    const QString & name=QString(),
    QObject * parent=nullptr
) 
```




<hr>



### function addItem 

```C++
void QfMarkupCollection::addItem (
    const QfMarkupItem & item
) 
```



Adds an _item_ into the collection. 


        

<hr>



### function count [2/2]

```C++
inline qsizetype QfMarkupCollection::count () const
```



Returns the number of items in the collection. 


        

<hr>



### function items [2/2]

```C++
inline QList< QfMarkupItem > QfMarkupCollection::items () const
```



Returns the list of items in the collection. 


        

<hr>



### function name [2/2]

```C++
inline QString QfMarkupCollection::name () const
```



Returns the name of the collection. 


        

<hr>



### function readGeoJson 

```C++
bool QfMarkupCollection::readGeoJson (
    const QString & path
) 
```



Restore a collection from the content of a GeoJSON at the provided _path_. 


        

<hr>



### function removeItem 

```C++
void QfMarkupCollection::removeItem (
    const QString & uuid
) 
```



Removes an item matching the provided _uuid_ from the collection. 


        

<hr>



### function replaceItem 

```C++
void QfMarkupCollection::replaceItem (
    const QString & uuid,
    const QfMarkupItem & item
) 
```



Replaces an item matching the provided _uuid_ with a new _item_ within the collection. 


        

<hr>



### function setName 

```C++
void QfMarkupCollection::setName (
    const QString & name
) 
```



Sets the name of the collection. 


        

<hr>



### function writeGeoJson 

```C++
bool QfMarkupCollection::writeGeoJson (
    const QString & path
) 
```



Saves the collection from the content of a GeoJSON at the provided _path_. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfmarkupcollection.h`

