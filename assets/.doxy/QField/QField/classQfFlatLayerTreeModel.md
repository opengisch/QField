

# Class QfFlatLayerTreeModel



[**ClassList**](annotated.md) **>** [**QfFlatLayerTreeModel**](classQfFlatLayerTreeModel.md)








Inherits the following classes: QSortFilterProxyModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Roles**](#enum-roles)  <br> |
| enum  | [**Types**](#enum-types)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**hasCollapsibleItems**](classQfFlatLayerTreeModel.md#property-hascollapsibleitems-12)  <br> |
| property bool | [**isCollapsed**](classQfFlatLayerTreeModel.md#property-iscollapsed-12)  <br> |
| property bool | [**isFrozen**](classQfFlatLayerTreeModel.md#property-isfrozen-12)  <br> |
| property bool | [**isTemporal**](classQfFlatLayerTreeModel.md#property-istemporal-12)  <br> |
| property QString | [**mapTheme**](classQfFlatLayerTreeModel.md#property-maptheme-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**hasCollapsibleItemsChanged**](classQfFlatLayerTreeModel.md#signal-hascollapsibleitemschanged)  <br> |
| signal void | [**isCollapsedChanged**](classQfFlatLayerTreeModel.md#signal-iscollapsedchanged)  <br> |
| signal void | [**isFrozenChanged**](classQfFlatLayerTreeModel.md#signal-isfrozenchanged)  <br> |
| signal void | [**isTemporalChanged**](classQfFlatLayerTreeModel.md#signal-istemporalchanged)  <br> |
| signal void | [**layersAdded**](classQfFlatLayerTreeModel.md#signal-layersadded)  <br> |
| signal void | [**layersRemoved**](classQfFlatLayerTreeModel.md#signal-layersremoved)  <br> |
| signal void | [**mapThemeChanged**](classQfFlatLayerTreeModel.md#signal-mapthemechanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfFlatLayerTreeModel**](#function-qfflatlayertreemodel) (QgsLayerTree \* layerTree, QgsProject \* project, QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br> |
|  Q\_INVOKABLE void | [**freeze**](#function-freeze) () <br>_Freezes the model as is, with any source model signals ignored._  |
|  bool | [**hasCollapsibleItems**](#function-hascollapsibleitems-22) () const<br>_Returns TRUE if the layer tree has at least one collapsible item._  |
|  bool | [**isCollapsed**](#function-iscollapsed-22) () const<br>_Returns TRUE if all collapsible items are currently collapsed._  |
|  bool | [**isFrozen**](#function-isfrozen-22) () const<br>_Returns TRUE if the model is frozen._  |
|  bool | [**isTemporal**](#function-istemporal-22) () const<br> |
|  QgsLayerTree \* | [**layerTree**](#function-layertree) () const<br> |
|  QgsLayerTreeModel \* | [**layerTreeModel**](#function-layertreemodel) () const<br> |
|  QString | [**mapTheme**](#function-maptheme-22) () const<br> |
|  Q\_INVOKABLE QgsRectangle | [**nodeExtent**](#function-nodeextent) (const QModelIndex & index, [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings, const float buffer=0.02) <br>_Calculate layer tree node extent._  |
|  QgsProject \* | [**project**](#function-project) () const<br> |
|  Q\_INVOKABLE void | [**setAllCollapsed**](#function-setallcollapsed) (bool collapsed) <br>_Collapses or expands all collapsible items in the layer tree._  |
|  Q\_INVOKABLE bool | [**setData**](#function-setdata) (const QModelIndex & index, const QVariant & value, int role) override<br> |
|  void | [**setLayerInTracking**](#function-setlayerintracking) (QgsLayerTreeLayer \* nodeLayer, bool tracking) <br>_Sets the information if the_ _nodeLayer_ _is currently in__tracking_ _state._ |
|  void | [**setMapTheme**](#function-setmaptheme) (const QString & mapTheme) <br> |
|  Q\_INVOKABLE void | [**unfreeze**](#function-unfreeze) (bool resetModel=false) <br>_Unfreezes the model and resume listening to source model signals._  |
|  Q\_INVOKABLE void | [**updateCurrentMapTheme**](#function-updatecurrentmaptheme) () <br> |
























## Protected Functions

| Type | Name |
| ---: | :--- |
| virtual bool | [**filterAcceptsRow**](#function-filteracceptsrow) (int source\_row, const QModelIndex & source\_parent) override const<br> |




## Public Types Documentation




### enum Roles 

```C++
enum QfFlatLayerTreeModel::Roles {
    VectorLayerPointer = Qt::UserRole + 1,
    MapLayerPointer,
    HasSpatialExtent,
    LegendImage,
    Type,
    Name,
    Visible,
    Trackable,
    InTracking,
    ReadOnly,
    FeatureAdditionLocked,
    AttributeEditingLocked,
    GeometryEditingLocked,
    FeatureDeletionLocked,
    TreeLevel,
    LayerType,
    IsValid,
    FeatureCount,
    IsCollapsed,
    IsParentCollapsed,
    HasChildren,
    CanReloadData,
    HasLabels,
    LabelsVisible,
    Opacity,
    FilterExpression,
    Credits,
    SnappingEnabled,
    HasNotes,
    Notes,
    Checkable
};
```




<hr>



### enum Types 

```C++
enum QfFlatLayerTreeModel::Types {
    Layer,
    Group,
    Image,
    Legend
};
```




<hr>
## Public Properties Documentation




### property hasCollapsibleItems [1/2]

```C++
bool QfFlatLayerTreeModel::hasCollapsibleItems;
```




<hr>



### property isCollapsed [1/2]

```C++
bool QfFlatLayerTreeModel::isCollapsed;
```




<hr>



### property isFrozen [1/2]

```C++
bool QfFlatLayerTreeModel::isFrozen;
```




<hr>



### property isTemporal [1/2]

```C++
bool QfFlatLayerTreeModel::isTemporal;
```




<hr>



### property mapTheme [1/2]

```C++
QString QfFlatLayerTreeModel::mapTheme;
```




<hr>
## Public Signals Documentation




### signal hasCollapsibleItemsChanged 

```C++
void QfFlatLayerTreeModel::hasCollapsibleItemsChanged;
```




<hr>



### signal isCollapsedChanged 

```C++
void QfFlatLayerTreeModel::isCollapsedChanged;
```




<hr>



### signal isFrozenChanged 

```C++
void QfFlatLayerTreeModel::isFrozenChanged;
```




<hr>



### signal isTemporalChanged 

```C++
void QfFlatLayerTreeModel::isTemporalChanged;
```




<hr>



### signal layersAdded 

```C++
void QfFlatLayerTreeModel::layersAdded;
```




<hr>



### signal layersRemoved 

```C++
void QfFlatLayerTreeModel::layersRemoved;
```




<hr>



### signal mapThemeChanged 

```C++
void QfFlatLayerTreeModel::mapThemeChanged;
```




<hr>
## Public Functions Documentation




### function QfFlatLayerTreeModel 

```C++
explicit QfFlatLayerTreeModel::QfFlatLayerTreeModel (
    QgsLayerTree * layerTree,
    QgsProject * project,
    QObject * parent=nullptr
) 
```




<hr>



### function data 

```C++
Q_INVOKABLE QVariant QfFlatLayerTreeModel::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function freeze 

_Freezes the model as is, with any source model signals ignored._ 
```C++
Q_INVOKABLE void QfFlatLayerTreeModel::freeze () 
```




<hr>



### function hasCollapsibleItems [2/2]

_Returns TRUE if the layer tree has at least one collapsible item._ 
```C++
bool QfFlatLayerTreeModel::hasCollapsibleItems () const
```




<hr>



### function isCollapsed [2/2]

_Returns TRUE if all collapsible items are currently collapsed._ 
```C++
bool QfFlatLayerTreeModel::isCollapsed () const
```




<hr>



### function isFrozen [2/2]

_Returns TRUE if the model is frozen._ 
```C++
bool QfFlatLayerTreeModel::isFrozen () const
```




<hr>



### function isTemporal [2/2]

```C++
bool QfFlatLayerTreeModel::isTemporal () const
```




<hr>



### function layerTree 

```C++
QgsLayerTree * QfFlatLayerTreeModel::layerTree () const
```




<hr>



### function layerTreeModel 

```C++
QgsLayerTreeModel * QfFlatLayerTreeModel::layerTreeModel () const
```




<hr>



### function mapTheme [2/2]

```C++
QString QfFlatLayerTreeModel::mapTheme () const
```




<hr>



### function nodeExtent 

_Calculate layer tree node extent._ 
```C++
Q_INVOKABLE QgsRectangle QfFlatLayerTreeModel::nodeExtent (
    const QModelIndex & index,
    QgsQuickMapSettings * mapSettings,
    const float buffer=0.02
) 
```




<hr>



### function project 

```C++
QgsProject * QfFlatLayerTreeModel::project () const
```




<hr>



### function setAllCollapsed 

_Collapses or expands all collapsible items in the layer tree._ 
```C++
Q_INVOKABLE void QfFlatLayerTreeModel::setAllCollapsed (
    bool collapsed
) 
```




<hr>



### function setData 

```C++
Q_INVOKABLE bool QfFlatLayerTreeModel::setData (
    const QModelIndex & index,
    const QVariant & value,
    int role
) override
```




<hr>



### function setLayerInTracking 

_Sets the information if the_ _nodeLayer_ _is currently in__tracking_ _state._
```C++
void QfFlatLayerTreeModel::setLayerInTracking (
    QgsLayerTreeLayer * nodeLayer,
    bool tracking
) 
```




<hr>



### function setMapTheme 

```C++
void QfFlatLayerTreeModel::setMapTheme (
    const QString & mapTheme
) 
```




<hr>



### function unfreeze 

_Unfreezes the model and resume listening to source model signals._ 
```C++
Q_INVOKABLE void QfFlatLayerTreeModel::unfreeze (
    bool resetModel=false
) 
```




<hr>



### function updateCurrentMapTheme 

```C++
Q_INVOKABLE void QfFlatLayerTreeModel::updateCurrentMapTheme () 
```



Update map theme as currently used by the model This should be triggered after a project has been loaded 


        

<hr>
## Protected Functions Documentation




### function filterAcceptsRow 

```C++
virtual bool QfFlatLayerTreeModel::filterAcceptsRow (
    int source_row,
    const QModelIndex & source_parent
) override const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qflayertreemodel.h`

