

# Class QfFlatLayerTreeModelBase



[**ClassList**](annotated.md) **>** [**QfFlatLayerTreeModelBase**](classQfFlatLayerTreeModelBase.md)








Inherits the following classes: QAbstractProxyModel






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**hasCollapsibleItemsChanged**](classQfFlatLayerTreeModelBase.md#signal-hascollapsibleitemschanged)  <br> |
| signal void | [**isCollapsedChanged**](classQfFlatLayerTreeModelBase.md#signal-iscollapsedchanged)  <br> |
| signal void | [**isFrozenChanged**](classQfFlatLayerTreeModelBase.md#signal-isfrozenchanged)  <br> |
| signal void | [**isTemporalChanged**](classQfFlatLayerTreeModelBase.md#signal-istemporalchanged)  <br> |
| signal void | [**layersAdded**](classQfFlatLayerTreeModelBase.md#signal-layersadded)  <br> |
| signal void | [**layersRemoved**](classQfFlatLayerTreeModelBase.md#signal-layersremoved)  <br> |
| signal void | [**mapThemeChanged**](classQfFlatLayerTreeModelBase.md#signal-mapthemechanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfFlatLayerTreeModelBase**](#function-qfflatlayertreemodelbase) (QgsLayerTree \* layerTree, QgsProject \* project, QObject \* parent=nullptr) <br> |
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
void QfFlatLayerTreeModelBase::hasCollapsibleItemsChanged;
```




<hr>



### signal isCollapsedChanged 

```C++
void QfFlatLayerTreeModelBase::isCollapsedChanged;
```




<hr>



### signal isFrozenChanged 

```C++
void QfFlatLayerTreeModelBase::isFrozenChanged;
```




<hr>



### signal isTemporalChanged 

```C++
void QfFlatLayerTreeModelBase::isTemporalChanged;
```




<hr>



### signal layersAdded 

```C++
void QfFlatLayerTreeModelBase::layersAdded;
```




<hr>



### signal layersRemoved 

```C++
void QfFlatLayerTreeModelBase::layersRemoved;
```




<hr>



### signal mapThemeChanged 

```C++
void QfFlatLayerTreeModelBase::mapThemeChanged;
```




<hr>
## Public Functions Documentation




### function QfFlatLayerTreeModelBase 

```C++
explicit QfFlatLayerTreeModelBase::QfFlatLayerTreeModelBase (
    QgsLayerTree * layerTree,
    QgsProject * project,
    QObject * parent=nullptr
) 
```




<hr>



### function columnCount 

```C++
int QfFlatLayerTreeModelBase::columnCount (
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function data 

```C++
QVariant QfFlatLayerTreeModelBase::data (
    const QModelIndex & index,
    int role
) override const
```




<hr>



### function freeze 

_Freezes the model as is, with any source model signals ignored._ 
```C++
Q_INVOKABLE void QfFlatLayerTreeModelBase::freeze () 
```




<hr>



### function hasCollapsibleItems 

_Returns TRUE if the layer tree has at least one collapsible item._ 
```C++
bool QfFlatLayerTreeModelBase::hasCollapsibleItems () const
```




<hr>



### function index 

```C++
QModelIndex QfFlatLayerTreeModelBase::index (
    int row,
    int column,
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function isCollapsed 

_Returns TRUE if all collapsible items are currently collapsed._ 
```C++
bool QfFlatLayerTreeModelBase::isCollapsed () const
```




<hr>



### function isFrozen 

_Returns TRUE if the model is frozen._ 
```C++
bool QfFlatLayerTreeModelBase::isFrozen () const
```




<hr>



### function isTemporal 

_Returns whether the current layer tree has temporal awareness._ 
```C++
inline bool QfFlatLayerTreeModelBase::isTemporal () const
```




<hr>



### function layerTree 

```C++
QgsLayerTree * QfFlatLayerTreeModelBase::layerTree () const
```




<hr>



### function layerTreeModel 

```C++
QgsLayerTreeModel * QfFlatLayerTreeModelBase::layerTreeModel () const
```




<hr>



### function mapFromSource 

```C++
QModelIndex QfFlatLayerTreeModelBase::mapFromSource (
    const QModelIndex & sourceIndex
) override const
```




<hr>



### function mapTheme 

```C++
QString QfFlatLayerTreeModelBase::mapTheme () const
```




<hr>



### function mapToSource 

```C++
QModelIndex QfFlatLayerTreeModelBase::mapToSource (
    const QModelIndex & proxyIndex
) override const
```




<hr>



### function nodeExtent 

_Calculate layer tree node extent and add optional buffer._ 
```C++
QgsRectangle QfFlatLayerTreeModelBase::nodeExtent (
    const QModelIndex & index,
    QgsQuickMapSettings * mapSettings,
    const float buffer
) 
```




<hr>



### function parent 

```C++
QModelIndex QfFlatLayerTreeModelBase::parent (
    const QModelIndex & child
) override const
```




<hr>



### function project 

```C++
QgsProject * QfFlatLayerTreeModelBase::project () const
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfFlatLayerTreeModelBase::roleNames () override const
```




<hr>



### function rowCount 

```C++
int QfFlatLayerTreeModelBase::rowCount (
    const QModelIndex & parent=QModelIndex()
) override const
```




<hr>



### function setAllCollapsed 

_Collapses or expands all collapsible items in the layer tree._ 
```C++
void QfFlatLayerTreeModelBase::setAllCollapsed (
    bool collapsed
) 
```




<hr>



### function setData 

```C++
bool QfFlatLayerTreeModelBase::setData (
    const QModelIndex & index,
    const QVariant & value,
    int role
) override
```




<hr>



### function setLayerInTracking 

_Sets the information if the_ _nodeLayer_ _is currently in__tracking_ _state._
```C++
void QfFlatLayerTreeModelBase::setLayerInTracking (
    QgsLayerTreeLayer * nodeLayer,
    bool tracking
) 
```




<hr>



### function setMapTheme 

```C++
void QfFlatLayerTreeModelBase::setMapTheme (
    const QString & mapTheme
) 
```




<hr>



### function setSourceModel 

```C++
void QfFlatLayerTreeModelBase::setSourceModel (
    QAbstractItemModel * sourceModel
) override
```




<hr>



### function unfreeze 

_Unfreezes the model and resume listening to source model signals._ 
```C++
Q_INVOKABLE void QfFlatLayerTreeModelBase::unfreeze (
    bool resetModel=false
) 
```




<hr>



### function updateCurrentMapTheme 

```C++
Q_INVOKABLE void QfFlatLayerTreeModelBase::updateCurrentMapTheme () 
```



Update map theme as currently used by the model This should be triggered after a project has been loaded 


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qflayertreemodel.h`

