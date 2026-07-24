

# Class FlatLayerTreeModelBase



[**ClassList**](annotated.md) **>** [**FlatLayerTreeModelBase**](classFlatLayerTreeModelBase.md)








Inherits the following classes: QAbstractProxyModel






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**hasCollapsibleItemsChanged**](classFlatLayerTreeModelBase.md#signal-hascollapsibleitemschanged)  <br> |
| signal void | [**isCollapsedChanged**](classFlatLayerTreeModelBase.md#signal-iscollapsedchanged)  <br> |
| signal void | [**isFrozenChanged**](classFlatLayerTreeModelBase.md#signal-isfrozenchanged)  <br> |
| signal void | [**isTemporalChanged**](classFlatLayerTreeModelBase.md#signal-istemporalchanged)  <br> |
| signal void | [**layersAdded**](classFlatLayerTreeModelBase.md#signal-layersadded)  <br> |
| signal void | [**layersRemoved**](classFlatLayerTreeModelBase.md#signal-layersremoved)  <br> |
| signal void | [**mapThemeChanged**](classFlatLayerTreeModelBase.md#signal-mapthemechanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**FlatLayerTreeModelBase**](#function-flatlayertreemodelbase) (QgsLayerTree \* layerTree, QgsProject \* project, QObject \* parent=nullptr) <br> |
|  int | [**columnCount**](#function-columncount) (const QModelIndex & parent=QModelIndex()) override const<br> |
|  QVariant | [**data**](#function-data) (const QModelIndex & index, int role) override const<br> |
|  Q\_INVOKABLE void | [**freeze**](#function-freeze) () <br>_Freezes the model as is, with any source model signals ignored._  |
|  bool | [**hasCollapsibleItems**](#function-hascollapsibleitems) () const<br>_Returns TRUE if the layer tree has at least one collapsible item._  |
|  QModelIndex | [**index**](#function-index) (int row, int column, const QModelIndex & parent=QModelIndex()) override const<br> |
|  bool | [**isCollapsed**](#function-iscollapsed) () const<br>_Returns TRUE if all collapsible items are currently collapsed._  |
|  bool | [**isFrozen**](#function-isfrozen) () const<br>_Returns TRUE if the model is frozen._  |
|  bool | [**isTemporal**](#function-istemporal) () const<br>_Returns whether the current layer tree has temporal awareness._  |
|  QgsLayerTree \* | [**layerTree**](#function-layertree) () const<br> |
|  QgsLayerTreeModel \* | [**layerTreeModel**](#function-layertreemodel) () const<br> |
|  QModelIndex | [**mapFromSource**](#function-mapfromsource) (const QModelIndex & sourceIndex) override const<br> |
|  QString | [**mapTheme**](#function-maptheme) () const<br> |
|  QModelIndex | [**mapToSource**](#function-maptosource) (const QModelIndex & proxyIndex) override const<br> |
|  QgsRectangle | [**nodeExtent**](#function-nodeextent) (const QModelIndex & index, [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings, const float buffer) <br>_Calculate layer tree node extent and add optional buffer._  |
|  QModelIndex | [**parent**](#function-parent) (const QModelIndex & child) override const<br> |
|  QgsProject \* | [**project**](#function-project) () const<br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  int | [**rowCount**](#function-rowcount) (const QModelIndex & parent=QModelIndex()) override const<br> |
|  void | [**setAllCollapsed**](#function-setallcollapsed) (bool collapsed) <br>_Collapses or expands all collapsible items in the layer tree._  |
|  bool | [**setData**](#function-setdata) (const QModelIndex & index, const QVariant & value, int role) override<br> |
|  void | [**setLayerInTracking**](#function-setlayerintracking) (QgsLayerTreeLayer \* nodeLayer, bool tracking) <br>_Sets the information if the_ _nodeLayer_ _is currently in__tracking_ _state._ |
|  void | [**setMapTheme**](#function-setmaptheme) (const QString & mapTheme) <br> |
|  void | [**setSourceModel**](#function-setsourcemodel) (QAbstractItemModel \* sourceModel) override<br> |
|  Q\_INVOKABLE void | [**unfreeze**](#function-unfreeze) (bool resetModel=false) <br>_Unfreezes the model and resume listening to source model signals._  |
|  Q\_INVOKABLE void | [**updateCurrentMapTheme**](#function-updatecurrentmaptheme) () <br> |




























## Public Signals Documentation




### signal hasCollapsibleItemsChanged 

```C++
void FlatLayerTreeModelBase::hasCollapsibleItemsChanged;
```




<hr>



### signal isCollapsedChanged 

```C++
void FlatLayerTreeModelBase::isCollapsedChanged;
```




<hr>



### signal isFrozenChanged 

```C++
void FlatLayerTreeModelBase::isFrozenChanged;
```




<hr>



### signal isTemporalChanged 

```C++
void FlatLayerTreeModelBase::isTemporalChanged;
```




<hr>



### signal layersAdded 

```C++
void FlatLayerTreeModelBase::layersAdded;
```




<hr>



### signal layersRemoved 

```C++
void FlatLayerTreeModelBase::layersRemoved;
```




<hr>



### signal mapThemeChanged 

```C++
void FlatLayerTreeModelBase::mapThemeChanged;
```




<hr>
## Public Functions Documentation




### function FlatLayerTreeModelBase 

```C++
explicit FlatLayerTreeModelBase::FlatLayerTreeModelBase (
    QgsLayerTree * layerTree,
    QgsProject * project,
    QObject * parent=nullptr
) 
```




<hr>



### function columnCount 

```C++
int FlatLayerTreeModelBase::columnCount (
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function data 

```C++
QVariant FlatLayerTreeModelBase::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function freeze 

_Freezes the model as is, with any source model signals ignored._ 
```C++
Q_INVOKABLE void FlatLayerTreeModelBase::freeze () 
```




<hr>



### function hasCollapsibleItems 

_Returns TRUE if the layer tree has at least one collapsible item._ 
```C++
bool FlatLayerTreeModelBase::hasCollapsibleItems () const
```




<hr>



### function index 

```C++
QModelIndex FlatLayerTreeModelBase::index (
    int row,
    int column,
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function isCollapsed 

_Returns TRUE if all collapsible items are currently collapsed._ 
```C++
bool FlatLayerTreeModelBase::isCollapsed () const
```




<hr>



### function isFrozen 

_Returns TRUE if the model is frozen._ 
```C++
bool FlatLayerTreeModelBase::isFrozen () const
```




<hr>



### function isTemporal 

_Returns whether the current layer tree has temporal awareness._ 
```C++
inline bool FlatLayerTreeModelBase::isTemporal () const
```




<hr>



### function layerTree 

```C++
QgsLayerTree * FlatLayerTreeModelBase::layerTree () const
```




<hr>



### function layerTreeModel 

```C++
QgsLayerTreeModel * FlatLayerTreeModelBase::layerTreeModel () const
```




<hr>



### function mapFromSource 

```C++
QModelIndex FlatLayerTreeModelBase::mapFromSource (
    const QModelIndex & sourceIndex
) override const
```




<hr>



### function mapTheme 

```C++
QString FlatLayerTreeModelBase::mapTheme () const
```




<hr>



### function mapToSource 

```C++
QModelIndex FlatLayerTreeModelBase::mapToSource (
    const QModelIndex & proxyIndex
) override const
```




<hr>



### function nodeExtent 

_Calculate layer tree node extent and add optional buffer._ 
```C++
QgsRectangle FlatLayerTreeModelBase::nodeExtent (
    const QModelIndex & index,
    QgsQuickMapSettings * mapSettings,
    const float buffer
) 
```




<hr>



### function parent 

```C++
QModelIndex FlatLayerTreeModelBase::parent (
    const QModelIndex & child
) override const
```




<hr>



### function project 

```C++
QgsProject * FlatLayerTreeModelBase::project () const
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > FlatLayerTreeModelBase::roleNames () override const
```




<hr>



### function rowCount 

```C++
int FlatLayerTreeModelBase::rowCount (
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function setAllCollapsed 

_Collapses or expands all collapsible items in the layer tree._ 
```C++
void FlatLayerTreeModelBase::setAllCollapsed (
    bool collapsed
) 
```




<hr>



### function setData 

```C++
bool FlatLayerTreeModelBase::setData (
    const QModelIndex & index,
    const QVariant & value,
    int role
) override
```




<hr>



### function setLayerInTracking 

_Sets the information if the_ _nodeLayer_ _is currently in__tracking_ _state._
```C++
void FlatLayerTreeModelBase::setLayerInTracking (
    QgsLayerTreeLayer * nodeLayer,
    bool tracking
) 
```




<hr>



### function setMapTheme 

```C++
void FlatLayerTreeModelBase::setMapTheme (
    const QString & mapTheme
) 
```




<hr>



### function setSourceModel 

```C++
void FlatLayerTreeModelBase::setSourceModel (
    QAbstractItemModel * sourceModel
) override
```




<hr>



### function unfreeze 

_Unfreezes the model and resume listening to source model signals._ 
```C++
Q_INVOKABLE void FlatLayerTreeModelBase::unfreeze (
    bool resetModel=false
) 
```




<hr>



### function updateCurrentMapTheme 

```C++
Q_INVOKABLE void FlatLayerTreeModelBase::updateCurrentMapTheme () 
```



Update map theme as currently used by the model This should be triggered after a project has been loaded 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/layertreemodel.h`

