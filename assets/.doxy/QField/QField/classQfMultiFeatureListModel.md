

# Class QfMultiFeatureListModel



[**ClassList**](annotated.md) **>** [**QfMultiFeatureListModel**](classQfMultiFeatureListModel.md)








Inherits the following classes: QSortFilterProxyModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**FeatureListRoles**](#enum-featurelistroles)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**canDeleteSelection**](classQfMultiFeatureListModel.md#property-candeleteselection-12)  <br> |
| property bool | [**canDuplicateSelection**](classQfMultiFeatureListModel.md#property-canduplicateselection-12)  <br> |
| property bool | [**canEditAttributesSelection**](classQfMultiFeatureListModel.md#property-caneditattributesselection-12)  <br> |
| property bool | [**canMergeSelection**](classQfMultiFeatureListModel.md#property-canmergeselection-12)  <br> |
| property bool | [**canMoveSelection**](classQfMultiFeatureListModel.md#property-canmoveselection-12)  <br> |
| property bool | [**canProcessSelection**](classQfMultiFeatureListModel.md#property-canprocessselection-12)  <br> |
| property bool | [**canRotateSelection**](classQfMultiFeatureListModel.md#property-canrotateselection-12)  <br> |
| property int | [**count**](classQfMultiFeatureListModel.md#property-count-12)  <br> |
| property int | [**selectedCount**](classQfMultiFeatureListModel.md#property-selectedcount-12)  <br> |
| property QList&lt; QgsFeature &gt; | [**selectedFeatures**](classQfMultiFeatureListModel.md#property-selectedfeatures-12)  <br> |
| property QgsVectorLayer \* | [**selectedLayer**](classQfMultiFeatureListModel.md#property-selectedlayer-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**countChanged**](classQfMultiFeatureListModel.md#signal-countchanged)  <br> |
| signal void | [**selectedCountChanged**](classQfMultiFeatureListModel.md#signal-selectedcountchanged)  <br> |
| signal void | [**selectedLayerChanged**](classQfMultiFeatureListModel.md#signal-selectedlayerchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfMultiFeatureListModel**](#function-qfmultifeaturelistmodel) (QObject \* parent=nullptr) <br> |
|  void | [**appendFeatures**](#function-appendfeatures) (const QList&lt; [**QfIdentifyTool::IdentifyResult**](structQfIdentifyTool_1_1IdentifyResult.md) &gt; & results) <br> |
|  bool | [**canDeleteSelection**](#function-candeleteselection-22) () const<br>_Returns TRUE if the selected features can be deleted._  |
|  bool | [**canDuplicateSelection**](#function-canduplicateselection-22) () const<br>_Returns TRUE if the selected features can be duplicated onto their associated layer._  |
|  bool | [**canEditAttributesSelection**](#function-caneditattributesselection-22) () const<br>_Returns TRUE if the selected features can have their attributes value changed._  |
|  bool | [**canMergeSelection**](#function-canmergeselection-22) () const<br>_Returns TRUE if the selected features can be merged._  |
|  bool | [**canMoveSelection**](#function-canmoveselection-22) () const<br>_Returns TRUE if the selected features' geometry can be moved._  |
|  bool | [**canProcessSelection**](#function-canprocessselection-22) () const<br>_Returns TRUE if the selected features can run processing algorithms._  |
|  bool | [**canRotateSelection**](#function-canrotateselection-22) () const<br>_Returns TRUE if the selected features' geometry can be rotated._  |
|  Q\_INVOKABLE void | [**clear**](#function-clear) (const bool keepSelected=false) <br> |
|  Q\_INVOKABLE void | [**clearSelection**](#function-clearselection) () <br> |
|  int | [**count**](#function-count-22) () const<br> |
|  Q\_INVOKABLE bool | [**deleteFeature**](#function-deletefeature) (QgsVectorLayer \* layer, QgsFeatureId fid) <br> |
|  Q\_INVOKABLE bool | [**deleteSelection**](#function-deleteselection) () <br>_Deletes selected features._  |
|  Q\_INVOKABLE bool | [**duplicateFeature**](#function-duplicatefeature) (QgsVectorLayer \* layer, const QgsFeature & feature) <br> |
|  Q\_INVOKABLE bool | [**duplicateSelection**](#function-duplicateselection) () <br>_Duplicates selected features onto their associated layer._  |
|  Q\_INVOKABLE bool | [**mergeSelection**](#function-mergeselection) () <br> |
|  Q\_INVOKABLE bool | [**moveSelection**](#function-moveselection) (const double x, const double y, const QgsPoint & destinationPoint) <br> |
|  Q\_INVOKABLE bool | [**rotateSelection**](#function-rotateselection) (const double angle) <br>_Rotate selected features along a given_ _vector_ _._ |
|  int | [**selectedCount**](#function-selectedcount-22) () const<br> |
|  QList&lt; QgsFeature &gt; | [**selectedFeatures**](#function-selectedfeatures-22) () <br> |
|  QgsVectorLayer \* | [**selectedLayer**](#function-selectedlayer-22) () <br> |
|  void | [**setFeatures**](#function-setfeatures-12) (const QMap&lt; QgsVectorLayer \*, QgsFeatureRequest &gt; & requests) <br> |
|  Q\_INVOKABLE void | [**setFeatures**](#function-setfeatures-22) (QgsVectorLayer \* vl, const QString & filter, const QgsRectangle & extent=QgsRectangle()) <br> |
|  Q\_INVOKABLE void | [**toggleSelectedItem**](#function-toggleselecteditem) (int item) <br> |
























## Protected Functions

| Type | Name |
| ---: | :--- |
|  void | [**adjustFilterToSelectedCount**](#function-adjustfiltertoselectedcount) () <br> |
| virtual bool | [**filterAcceptsRow**](#function-filteracceptsrow) (int source\_row, const QModelIndex & source\_parent) override const<br> |




## Public Types Documentation




### enum FeatureListRoles 

```C++
enum QfMultiFeatureListModel::FeatureListRoles {
    FeatureIdRole = Qt::UserRole + 1,
    FeatureSelectedRole,
    FeatureNameRole,
    FeatureRole,
    LayerNameRole,
    LayerRole,
    GeometryRole,
    CrsRole,
    DeleteFeatureRole,
    EditGeometryRole,
    ConditionalTextColorRole,
    ConditionalBackgroundColorRole,
    ConditionalFontBoldRole,
    ConditionalFontItalicRole,
    ConditionalFontUnderlineRole,
    ConditionalFontStrikeOutRole,
    ExtrusionRole
};
```




<hr>
## Public Properties Documentation




### property canDeleteSelection [1/2]

```C++
bool QfMultiFeatureListModel::canDeleteSelection;
```




<hr>



### property canDuplicateSelection [1/2]

```C++
bool QfMultiFeatureListModel::canDuplicateSelection;
```




<hr>



### property canEditAttributesSelection [1/2]

```C++
bool QfMultiFeatureListModel::canEditAttributesSelection;
```




<hr>



### property canMergeSelection [1/2]

```C++
bool QfMultiFeatureListModel::canMergeSelection;
```




<hr>



### property canMoveSelection [1/2]

```C++
bool QfMultiFeatureListModel::canMoveSelection;
```




<hr>



### property canProcessSelection [1/2]

```C++
bool QfMultiFeatureListModel::canProcessSelection;
```




<hr>



### property canRotateSelection [1/2]

```C++
bool QfMultiFeatureListModel::canRotateSelection;
```




<hr>



### property count [1/2]

```C++
int QfMultiFeatureListModel::count;
```




<hr>



### property selectedCount [1/2]

```C++
int QfMultiFeatureListModel::selectedCount;
```




<hr>



### property selectedFeatures [1/2]

```C++
QList< QgsFeature > QfMultiFeatureListModel::selectedFeatures;
```




<hr>



### property selectedLayer [1/2]

```C++
QgsVectorLayer * QfMultiFeatureListModel::selectedLayer;
```




<hr>
## Public Signals Documentation




### signal countChanged 

```C++
void QfMultiFeatureListModel::countChanged;
```




<hr>



### signal selectedCountChanged 

```C++
void QfMultiFeatureListModel::selectedCountChanged;
```




<hr>



### signal selectedLayerChanged 

```C++
void QfMultiFeatureListModel::selectedLayerChanged;
```




<hr>
## Public Functions Documentation




### function QfMultiFeatureListModel 

```C++
explicit QfMultiFeatureListModel::QfMultiFeatureListModel (
    QObject * parent=nullptr
) 
```




<hr>



### function appendFeatures 

```C++
void QfMultiFeatureListModel::appendFeatures (
    const QList< QfIdentifyTool::IdentifyResult > & results
) 
```



Appends features from a list of _results_. 


        

<hr>



### function canDeleteSelection [2/2]

_Returns TRUE if the selected features can be deleted._ 
```C++
bool QfMultiFeatureListModel::canDeleteSelection () const
```




<hr>



### function canDuplicateSelection [2/2]

_Returns TRUE if the selected features can be duplicated onto their associated layer._ 
```C++
bool QfMultiFeatureListModel::canDuplicateSelection () const
```




<hr>



### function canEditAttributesSelection [2/2]

_Returns TRUE if the selected features can have their attributes value changed._ 
```C++
bool QfMultiFeatureListModel::canEditAttributesSelection () const
```




<hr>



### function canMergeSelection [2/2]

_Returns TRUE if the selected features can be merged._ 
```C++
bool QfMultiFeatureListModel::canMergeSelection () const
```




<hr>



### function canMoveSelection [2/2]

_Returns TRUE if the selected features' geometry can be moved._ 
```C++
bool QfMultiFeatureListModel::canMoveSelection () const
```




<hr>



### function canProcessSelection [2/2]

_Returns TRUE if the selected features can run processing algorithms._ 
```C++
bool QfMultiFeatureListModel::canProcessSelection () const
```




<hr>



### function canRotateSelection [2/2]

_Returns TRUE if the selected features' geometry can be rotated._ 
```C++
bool QfMultiFeatureListModel::canRotateSelection () const
```




<hr>



### function clear 

```C++
Q_INVOKABLE void QfMultiFeatureListModel::clear (
    const bool keepSelected=false
) 
```



Resets the model to either an empty feature list or one that contains only the selected features. 

**Parameters:**


* `keepSelected` if set to TRUE, selected features will be kept when resetting the model. 




        

<hr>



### function clearSelection 

```C++
Q_INVOKABLE void QfMultiFeatureListModel::clearSelection () 
```



Empties the list of selected features. 


        

<hr>



### function count [2/2]

```C++
int QfMultiFeatureListModel::count () const
```



Returns the number of features in the model. 


        

<hr>



### function deleteFeature 

```C++
Q_INVOKABLE bool QfMultiFeatureListModel::deleteFeature (
    QgsVectorLayer * layer,
    QgsFeatureId fid
) 
```



Deletes a feature from a vector layer




**Parameters:**


* `layer` The layer from which a feature will be removed 
* `fid` The id of the feature to remove 




        

<hr>



### function deleteSelection 

_Deletes selected features._ 
```C++
Q_INVOKABLE bool QfMultiFeatureListModel::deleteSelection () 
```




<hr>



### function duplicateFeature 

```C++
Q_INVOKABLE bool QfMultiFeatureListModel::duplicateFeature (
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
Q_INVOKABLE bool QfMultiFeatureListModel::duplicateSelection () 
```




<hr>



### function mergeSelection 

```C++
Q_INVOKABLE bool QfMultiFeatureListModel::mergeSelection () 
```



Merges selected features by updating the first seleted feature's geometry to a combination (i.e. union) of geometries of all selected features.


All but the first feature will then be removed from the vector layer containing the selected features. 


        

<hr>



### function moveSelection 

```C++
Q_INVOKABLE bool QfMultiFeatureListModel::moveSelection (
    const double x,
    const double y,
    const QgsPoint & destinationPoint
) 
```



Moves selected feature(s) geometry by _x_ and _y_. For a single feature with a singlepart point geometry containing Z and/or M values will adopt non-null values from the _destinationPoint_. 


        

<hr>



### function rotateSelection 

_Rotate selected features along a given_ _vector_ _._
```C++
Q_INVOKABLE bool QfMultiFeatureListModel::rotateSelection (
    const double angle
) 
```




<hr>



### function selectedCount [2/2]

```C++
int QfMultiFeatureListModel::selectedCount () const
```



Returns the number of selected features in the model. 


        

<hr>



### function selectedFeatures [2/2]

```C++
QList< QgsFeature > QfMultiFeatureListModel::selectedFeatures () 
```



Returns the list of currently selected features. 


        

<hr>



### function selectedLayer [2/2]

```C++
QgsVectorLayer * QfMultiFeatureListModel::selectedLayer () 
```



Returns the vector layer within which one or more features are currently selected 


        

<hr>



### function setFeatures [1/2]

```C++
void QfMultiFeatureListModel::setFeatures (
    const QMap< QgsVectorLayer *, QgsFeatureRequest > & requests
) 
```



Resets the model to contain features found from a list of _requests_. 


        

<hr>



### function setFeatures [2/2]

```C++
Q_INVOKABLE void QfMultiFeatureListModel::setFeatures (
    QgsVectorLayer * vl,
    const QString & filter,
    const QgsRectangle & extent=QgsRectangle()
) 
```



Resets the model to contain features found from a _vl_ with the possibility of filtering by a _filter_ expression and/or an _extent_. 


        

<hr>



### function toggleSelectedItem 

```C++
Q_INVOKABLE void QfMultiFeatureListModel::toggleSelectedItem (
    int item
) 
```



Toggles the selection state of a given item. 

**Parameters:**


* `item` the item's row number 




        

<hr>
## Protected Functions Documentation




### function adjustFilterToSelectedCount 

```C++
void QfMultiFeatureListModel::adjustFilterToSelectedCount () 
```




<hr>



### function filterAcceptsRow 

```C++
virtual bool QfMultiFeatureListModel::filterAcceptsRow (
    int source_row,
    const QModelIndex & source_parent
) override const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qfmultifeaturelistmodel.h`

