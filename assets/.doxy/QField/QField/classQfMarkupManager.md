

# Class QfMarkupManager



[**ClassList**](annotated.md) **>** [**QfMarkupManager**](classQfMarkupManager.md)



_This class to manage markup collections._ 

* `#include <qfmarkupmanager.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QStringList | [**collectionNames**](classQfMarkupManager.md#property-collectionnames-12)  <br> |
| property QList&lt; [**QfMarkupCollection**](classQfMarkupCollection.md) \* &gt; | [**collections**](classQfMarkupManager.md#property-collections-12)  <br> |
| property bool | [**hasItems**](classQfMarkupManager.md#property-hasitems-12)  <br> |
| property QStringList | [**hiddenCollectionUuids**](classQfMarkupManager.md#property-hiddencollectionuuids-12)  <br> |
| property QList&lt; [**QfMarkupCollection**](classQfMarkupCollection.md) \* &gt; | [**visibleCollections**](classQfMarkupManager.md#property-visiblecollections-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**collectionItemsChanged**](classQfMarkupManager.md#signal-collectionitemschanged) (const QString & uuid) <br>_Emitted when a collection's list of items has changed._  |
| signal void | [**collectionsAdded**](classQfMarkupManager.md#signal-collectionsadded) (const QStringList & uuids) <br>_Emitted when collections are about to be removed._  |
| signal void | [**collectionsChanged**](classQfMarkupManager.md#signal-collectionschanged)  <br>_Emitted when the list of collections has changed._  |
| signal void | [**collectionsWillBeRemoved**](classQfMarkupManager.md#signal-collectionswillberemoved) (const QStringList & uuids) <br>_Emitted when collections are about to be removed._  |
| signal void | [**hasItemsChanged**](classQfMarkupManager.md#signal-hasitemschanged)  <br>_Emitted when the item count of a loaded collection has changed._  |
| signal void | [**hiddenCollectionUuidsChanged**](classQfMarkupManager.md#signal-hiddencollectionuuidschanged)  <br>_Emitted when the list of hidden collection UUIDs has changed._  |
| signal void | [**visibleCollectionsChanged**](classQfMarkupManager.md#signal-visiblecollectionschanged)  <br>_Emitted when the list of visible collections has changed._  |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfMarkupManager**](#function-qfmarkupmanager) (QObject \* parent=nullptr) <br> |
|  [**QfMarkupCollection**](classQfMarkupCollection.md) \* | [**collection**](#function-collection) (const QString & uuid) const<br> |
|  QStringList | [**collectionNames**](#function-collectionnames-22) () const<br> |
|  QString | [**collectionUuid**](#function-collectionuuid) ([**QfMarkupCollection**](classQfMarkupCollection.md) \* collection) const<br> |
|  QStringList | [**collectionUuids**](#function-collectionuuids) () const<br> |
|  QList&lt; [**QfMarkupCollection**](classQfMarkupCollection.md) \* &gt; | [**collections**](#function-collections-22) () const<br> |
|  bool | [**hasItems**](#function-hasitems-22) () const<br> |
|  QStringList | [**hiddenCollectionUuids**](#function-hiddencollectionuuids-22) () const<br> |
|  Q\_INVOKABLE void | [**reset**](#function-reset) (const QString & path, const QString & prefix=QString()) <br> |
|  void | [**setHiddenCollectionUuids**](#function-sethiddencollectionuuids) (const QStringList & hiddenCollectionUuids) <br> |
|  QList&lt; [**QfMarkupCollection**](classQfMarkupCollection.md) \* &gt; | [**visibleCollections**](#function-visiblecollections-22) () const<br> |




























## Public Properties Documentation




### property collectionNames [1/2]

```C++
QStringList QfMarkupManager::collectionNames;
```




<hr>



### property collections [1/2]

```C++
QList<QfMarkupCollection *> QfMarkupManager::collections;
```




<hr>



### property hasItems [1/2]

```C++
bool QfMarkupManager::hasItems;
```




<hr>



### property hiddenCollectionUuids [1/2]

```C++
QStringList QfMarkupManager::hiddenCollectionUuids;
```




<hr>



### property visibleCollections [1/2]

```C++
QList<QfMarkupCollection *> QfMarkupManager::visibleCollections;
```




<hr>
## Public Signals Documentation




### signal collectionItemsChanged 

_Emitted when a collection's list of items has changed._ 
```C++
void QfMarkupManager::collectionItemsChanged;
```




<hr>



### signal collectionsAdded 

_Emitted when collections are about to be removed._ 
```C++
void QfMarkupManager::collectionsAdded;
```




<hr>



### signal collectionsChanged 

_Emitted when the list of collections has changed._ 
```C++
void QfMarkupManager::collectionsChanged;
```




<hr>



### signal collectionsWillBeRemoved 

_Emitted when collections are about to be removed._ 
```C++
void QfMarkupManager::collectionsWillBeRemoved;
```




<hr>



### signal hasItemsChanged 

_Emitted when the item count of a loaded collection has changed._ 
```C++
void QfMarkupManager::hasItemsChanged;
```




<hr>



### signal hiddenCollectionUuidsChanged 

_Emitted when the list of hidden collection UUIDs has changed._ 
```C++
void QfMarkupManager::hiddenCollectionUuidsChanged;
```




<hr>



### signal visibleCollectionsChanged 

_Emitted when the list of visible collections has changed._ 
```C++
void QfMarkupManager::visibleCollectionsChanged;
```




<hr>
## Public Functions Documentation




### function QfMarkupManager 

```C++
explicit QfMarkupManager::QfMarkupManager (
    QObject * parent=nullptr
) 
```




<hr>



### function collection 

```C++
QfMarkupCollection * QfMarkupManager::collection (
    const QString & uuid
) const
```



Returns the collection present in the markup manager matching the _uuid_. 


        

<hr>



### function collectionNames [2/2]

```C++
QStringList QfMarkupManager::collectionNames () const
```



Returns the list of collection names present in the markup manager. 


        

<hr>



### function collectionUuid 

```C++
QString QfMarkupManager::collectionUuid (
    QfMarkupCollection * collection
) const
```



Returns the list UUID matching the _collection_. 


        

<hr>



### function collectionUuids 

```C++
inline QStringList QfMarkupManager::collectionUuids () const
```



Returns the list of collection UUIDs present in the markup manager. 


        

<hr>



### function collections [2/2]

```C++
inline QList< QfMarkupCollection * > QfMarkupManager::collections () const
```



Returns the list of collections present in the markup manager. 


        

<hr>



### function hasItems [2/2]

```C++
bool QfMarkupManager::hasItems () const
```



Returns TRUE if any of the collections present in the markup manager contains one or more items. 


        

<hr>



### function hiddenCollectionUuids [2/2]

```C++
inline QStringList QfMarkupManager::hiddenCollectionUuids () const
```



Returns the list of collection UUIDs that are hidden. Matching collection will not be part of the list of collections returned by the markup manager visibleCollections() function. 


        

<hr>



### function reset 

```C++
Q_INVOKABLE void QfMarkupManager::reset (
    const QString & path,
    const QString & prefix=QString()
) 
```



Resets the markup manager's list of collections by loading compatible GeoJSON files present in the provided _path_. If a _prefix_ is provided, the markup manager will only load GeoJSON files beginning by the prefix. 


        

<hr>



### function setHiddenCollectionUuids 

```C++
void QfMarkupManager::setHiddenCollectionUuids (
    const QStringList & hiddenCollectionUuids
) 
```



Sets the list of collection UUIDs that are hidden. Matching collection will not be part of the list of collections returned by the markup manager visibleCollections() function. 


        

<hr>



### function visibleCollections [2/2]

```C++
QList< QfMarkupCollection * > QfMarkupManager::visibleCollections () const
```



Returns the list of visible collections present in the markup manager. 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfmarkupmanager.h`

