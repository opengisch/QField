

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
| property QString | [**uuid**](classQfMarkupCollection.md#property-uuid-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**countChanged**](classQfMarkupCollection.md#signal-countchanged)  <br>_Emitted when the number of items in the collection changed._  |
| signal void | [**itemsChanged**](classQfMarkupCollection.md#signal-itemschanged)  <br>_Emitted when the list of items in the collection changed._  |
| signal void | [**nameChanged**](classQfMarkupCollection.md#signal-namechanged)  <br>_Emitted when the collection name changed._  |
| signal void | [**uuidChanged**](classQfMarkupCollection.md#signal-uuidchanged)  <br>_Emitted when the UUID changed._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfMarkupCollection**](#function-qfmarkupcollection) (const QString & name=QString(), QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE QString | [**addItem**](#function-additem) (const [**QfMarkupItem**](classQfMarkupItem.md) & item, bool resetVectorLayer=true) <br> |
|  QgsAnnotationLayer \* | [**asAnnotationLayer**](#function-asannotationlayer) () <br> |
|  QgsVectorLayer \* | [**asVectorLayer**](#function-asvectorlayer) () <br> |
|  qsizetype | [**count**](#function-count-22) () const<br> |
|  QList&lt; [**QfMarkupItem**](classQfMarkupItem.md) &gt; | [**items**](#function-items-22) () const<br> |
|  QString | [**name**](#function-name-22) () const<br> |
|  bool | [**readGeoJson**](#function-readgeojson) (const QString & path) <br> |
|  Q\_INVOKABLE void | [**removeItem**](#function-removeitem) (const QString & uuid, bool resetVectorLayer=true) <br> |
|  Q\_INVOKABLE void | [**replaceItem**](#function-replaceitem) (const QString & uuid, const [**QfMarkupItem**](classQfMarkupItem.md) & item, bool resetVectorLayer=true) <br> |
|  void | [**setName**](#function-setname) (const QString & name) <br> |
|  QString | [**uuid**](#function-uuid-22) () const<br> |
|  bool | [**writeGeoJson**](#function-writegeojson) (const QString & path) <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  Q\_INVOKABLE [**QfMarkupItem**](classQfMarkupItem.md) | [**createItem**](#function-createitem) (const QString & label, const QString & description, const QgsGeometry & geometry, const QColor & color) <br> |


























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



### property uuid [1/2]

```C++
QString QfMarkupCollection::uuid;
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



### signal uuidChanged 

_Emitted when the UUID changed._ 
```C++
void QfMarkupCollection::uuidChanged;
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
Q_INVOKABLE QString QfMarkupCollection::addItem (
    const QfMarkupItem & item,
    bool resetVectorLayer=true
) 
```



Adds an _item_ into the collection and return its UUID. 


        

<hr>



### function asAnnotationLayer 

```C++
QgsAnnotationLayer * QfMarkupCollection::asAnnotationLayer () 
```



Returns a pointer to an annotation layer matching the content of the collection. 

**Note:**

The ownership remains with the collection. 





        

<hr>



### function asVectorLayer 

```C++
QgsVectorLayer * QfMarkupCollection::asVectorLayer () 
```



Returns a pointer to a memory vector layer matching the content of the collection.


The layer geometry type is a GeometryCollection and it can be used to modify the attributes and geometries of individual markup items within the collection.




**Note:**

The ownership remains with the collection. 





        

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
Q_INVOKABLE void QfMarkupCollection::removeItem (
    const QString & uuid,
    bool resetVectorLayer=true
) 
```



Removes an item matching the provided _uuid_ from the collection. 


        

<hr>



### function replaceItem 

```C++
Q_INVOKABLE void QfMarkupCollection::replaceItem (
    const QString & uuid,
    const QfMarkupItem & item,
    bool resetVectorLayer=true
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



### function uuid [2/2]

```C++
inline QString QfMarkupCollection::uuid () const
```



Returns the UUID of the collection. 


        

<hr>



### function writeGeoJson 

```C++
bool QfMarkupCollection::writeGeoJson (
    const QString & path
) 
```



Saves the collection from the content of a GeoJSON at the provided _path_. 


        

<hr>
## Public Static Functions Documentation




### function createItem 

```C++
static Q_INVOKABLE QfMarkupItem QfMarkupCollection::createItem (
    const QString & label,
    const QString & description,
    const QgsGeometry & geometry,
    const QColor & color
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfmarkupcollection.h`

