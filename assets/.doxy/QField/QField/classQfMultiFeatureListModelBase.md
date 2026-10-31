

# Class QfMultiFeatureListModelBase



[**ClassList**](annotated.md) **>** [**QfMultiFeatureListModelBase**](classQfMultiFeatureListModelBase.md)








Inherits the following classes: QAbstractItemModel






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**countChanged**](classQfMultiFeatureListModelBase.md#signal-countchanged)  <br> |
| signal void | [**selectedCountChanged**](classQfMultiFeatureListModelBase.md#signal-selectedcountchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfMultiFeatureListModelBase**](#function-qfmultifeaturelistmodelbase) (QObject \* parent=nullptr) <br> |
|  void | [**appendFeatures**](#function-appendfeatures) (const QList&lt; [**QfIdentifyTool::IdentifyResult**](structQfIdentifyTool_1_1IdentifyResult.md) &gt; & results) <br> |
|  bool | [**canDeleteSelection**](#function-candeleteselection) () const<br> |
|  bool | [**canDuplicateSelection**](#function-canduplicateselection) () const<br> |
|  bool | [**canEditAttributesSelection**](#function-caneditattributesselection) () const<br> |
|  bool | [**canMergeSelection**](#function-canmergeselection) () const<br> |
|  bool | [**canMoveSelection**](#function-canmoveselection) () const<br> |
|  bool | [**canProcessSelection**](#function-canprocessselection) () const<br> |
|  bool | [**canRotateSelection**](#function-canrotateselection) () const<br> |
|  void | [**clear**](#function-clear) (const bool keepSelected=false) <br> |
|  void | [**clearSelection**](#function-clearselection) () <br> |
|  int | [**columnCount**](#function-columncount) (const QModelIndex & parent) override const<br> |
|  int | [**count**](#function-count) () const<br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br> |
|  bool | [**deleteFeature**](#function-deletefeature) (QgsVectorLayer \* layer, QgsFeatureId fid, bool selectionAction=false) <br> |
|  bool | [**deleteSelection**](#function-deleteselection) () <br>_Deletes selected features._  |
|  bool | [**duplicateFeature**](#function-duplicatefeature) (QgsVectorLayer \* layer, const QgsFeature & feature) <br> |
|  bool | [**duplicateSelection**](#function-duplicateselection) () <br>_Duplicates selected features onto their associated layer._  |
|  QModelIndex | [**index**](#function-index) (int row, int column, const QModelIndex & parent=QModelIndex()) override const<br> |
|  bool | [**mergeSelection**](#function-mergeselection) () <br> |
|  bool | [**moveSelection**](#function-moveselection) (const double x, const double y, const QgsPoint & destinationPoint) <br> |
|  QModelIndex | [**parent**](#function-parent) (const QModelIndex & child) override const<br> |
| virtual bool | [**removeRows**](#function-removerows) (int row, int count, const QModelIndex & parent) override<br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  bool | [**rotateSelection**](#function-rotateselection) (const double angle) <br>_Rotate selected features along a given_ _vector_ _._ |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent) override const<br> |
|  int | [**selectedCount**](#function-selectedcount) () const<br> |
|  QList&lt; QgsFeature &gt; | [**selectedFeatures**](#function-selectedfeatures) () const<br> |
|  QgsVectorLayer \* | [**selectedLayer**](#function-selectedlayer) () const<br> |
|  void | [**setFeatures**](#function-setfeatures) (const QMap&lt; QgsVectorLayer \*, QgsFeatureRequest &gt; & requests) <br> |
|  void | [**toggleSelectedItem**](#function-toggleselecteditem) (int item) <br> |




























## Public Signals Documentation




### signal countChanged 

```C++
void QfMultiFeatureListModelBase::countChanged;
```




<hr>



### signal selectedCountChanged 

```C++
void QfMultiFeatureListModelBase::selectedCountChanged;
```




<hr>
## Public Functions Documentation




### function QfMultiFeatureListModelBase 

```C++
explicit QfMultiFeatureListModelBase::QfMultiFeatureListModelBase (
    QObject * parent=nullptr
) 
```




<hr>



### function appendFeatures 

```C++
void QfMultiFeatureListModelBase::appendFeatures (
    const QList< QfIdentifyTool::IdentifyResult > & results
) 
```



Appends features from a list of _results_.   


        

<hr>



### function canDeleteSelection 

```C++
bool QfMultiFeatureListModelBase::canDeleteSelection () const
```




<hr>



### function canDuplicateSelection 

```C++
bool QfMultiFeatureListModelBase::canDuplicateSelection () const
```




<hr>



### function canEditAttributesSelection 

```C++
bool QfMultiFeatureListModelBase::canEditAttributesSelection () const
```




<hr>



### function canMergeSelection 

```C++
bool QfMultiFeatureListModelBase::canMergeSelection () const
```




<hr>



### function canMoveSelection 

```C++
bool QfMultiFeatureListModelBase::canMoveSelection () const
```




<hr>



### function canProcessSelection 

```C++
bool QfMultiFeatureListModelBase::canProcessSelection () const
```




<hr>



### function canRotateSelection 

```C++
bool QfMultiFeatureListModelBase::canRotateSelection () const
```




<hr>



### function clear 

```C++
void QfMultiFeatureListModelBase::clear (
    const bool keepSelected=false
) 
```



Resets the model to either an empty feature list or one that contains only the selected features. 

**Parameters:**


* `keepSelected` if set to TRUE, selected features will be kept when resetting the model.   




        

<hr>



### function clearSelection 

```C++
void QfMultiFeatureListModelBase::clearSelection () 
```



Empties the list of selected features.   


        

<hr>



### function columnCount 

```C++
int QfMultiFeatureListModelBase::columnCount (
    const QModelIndex & parent
) override const
```




<hr>



### function count 

```C++
int QfMultiFeatureListModelBase::count () const
```




<hr>



### function data 

```C++
QVariant QfMultiFeatureListModelBase::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function deleteFeature 

```C++
bool QfMultiFeatureListModelBase::deleteFeature (
    QgsVectorLayer * layer,
    QgsFeatureId fid,
    bool selectionAction=false
) 
```



Deletes a feature from a vector layer




**Parameters:**


* `layer` The layer from which a feature will be removed 
* `fid` The id of the feature to remove 
* `selectionAction` 




        

<hr>



### function deleteSelection 

_Deletes selected features._ 
```C++
bool QfMultiFeatureListModelBase::deleteSelection () 
```




<hr>



### function duplicateFeature 

```C++
bool QfMultiFeatureListModelBase::duplicateFeature (
    QgsVectorLayer * layer,
    const QgsFeature & feature
) 
```



Duplicates a feature on a given layer




**Parameters:**


* `layer` The layer within which the feature will be duplicated 
* `feature` The feature to be duplicated   




        

<hr>



### function duplicateSelection 

_Duplicates selected features onto their associated layer._ 
```C++
bool QfMultiFeatureListModelBase::duplicateSelection () 
```




<hr>



### function index 

```C++
QModelIndex QfMultiFeatureListModelBase::index (
    int row,
    int column,
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function mergeSelection 

```C++
bool QfMultiFeatureListModelBase::mergeSelection () 
```



Merges selected features by updating the first seleted feature's geometry to a combination (i.e. union) of geometries of all selected features.


All but the first feature will then be removed from the vector layer containing the selected features.   


        

<hr>



### function moveSelection 

```C++
bool QfMultiFeatureListModelBase::moveSelection (
    const double x,
    const double y,
    const QgsPoint & destinationPoint
) 
```



Moves selected feature(s) geometry by _x_ and _y_. For a single feature with a singlepart point geometry containing Z and/or M values will adopt non-null values from the _destinationPoint_.   


        

<hr>



### function parent 

```C++
QModelIndex QfMultiFeatureListModelBase::parent (
    const QModelIndex & child
) override const
```




<hr>



### function removeRows 

```C++
virtual bool QfMultiFeatureListModelBase::removeRows (
    int row,
    int count,
    const QModelIndex & parent
) override
```



Removes a defined number of rows starting from a given position. The parent index is not used as we have a list only.




**Parameters:**


* `row` The first row to remove 
* `count` The numbe rof rows to remove 
* `parent` Can savely be omitted as it is unused and defaults to an invalid index 




        

<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfMultiFeatureListModelBase::roleNames () override const
```




<hr>



### function rotateSelection 

_Rotate selected features along a given_ _vector_ _._
```C++
bool QfMultiFeatureListModelBase::rotateSelection (
    const double angle
) 
```




<hr>



### function rowCount 

```C++
int QfMultiFeatureListModelBase::rowCount (
    const QModelIndex & parent
) override const
```




<hr>



### function selectedCount 

```C++
int QfMultiFeatureListModelBase::selectedCount () const
```




<hr>



### function selectedFeatures 

```C++
QList< QgsFeature > QfMultiFeatureListModelBase::selectedFeatures () const
```




<hr>



### function selectedLayer 

```C++
QgsVectorLayer * QfMultiFeatureListModelBase::selectedLayer () const
```




<hr>



### function setFeatures 

```C++
void QfMultiFeatureListModelBase::setFeatures (
    const QMap< QgsVectorLayer *, QgsFeatureRequest > & requests
) 
```



Resets the model to contain features found from a list of _requests_.   


        

<hr>



### function toggleSelectedItem 

```C++
void QfMultiFeatureListModelBase::toggleSelectedItem (
    int item
) 
```



Toggles the selection state of a given item. 

**Parameters:**


* `item` the item's row number   




        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfmultifeaturelistmodelbase.h`

