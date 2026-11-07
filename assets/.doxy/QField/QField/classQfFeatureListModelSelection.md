

# Class QfFeatureListModelSelection



[**ClassList**](annotated.md) **>** [**QfFeatureListModelSelection**](classQfFeatureListModelSelection.md)








Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QgsFeature | [**focusedFeature**](classQfFeatureListModelSelection.md#property-focusedfeature-12)  <br> |
| property QgsGeometry | [**focusedGeometry**](classQfFeatureListModelSelection.md#property-focusedgeometry-12)  <br> |
| property int | [**focusedItem**](classQfFeatureListModelSelection.md#property-focuseditem-12)  <br> |
| property QgsVectorLayer \* | [**focusedLayer**](classQfFeatureListModelSelection.md#property-focusedlayer-12)  <br> |
| property [**QfMultiFeatureListModel**](classQfMultiFeatureListModel.md) \* | [**model**](classQfFeatureListModelSelection.md#property-model-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**focusedItemChanged**](classQfFeatureListModelSelection.md#signal-focuseditemchanged)  <br> |
| signal void | [**modelChanged**](classQfFeatureListModelSelection.md#signal-modelchanged)  <br> |
| signal void | [**selectedFeaturesChanged**](classQfFeatureListModelSelection.md#signal-selectedfeatureschanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfFeatureListModelSelection**](#function-qffeaturelistmodelselection) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**clear**](#function-clear) () <br> |
|  QgsFeature | [**focusedFeature**](#function-focusedfeature-22) () const<br> |
|  QgsGeometry | [**focusedGeometry**](#function-focusedgeometry-22) () const<br> |
|  int | [**focusedItem**](#function-focuseditem-22) () const<br> |
|  QgsVectorLayer \* | [**focusedLayer**](#function-focusedlayer-22) () const<br> |
|  [**QfMultiFeatureListModel**](classQfMultiFeatureListModel.md) \* | [**model**](#function-model-22) () const<br> |
|  void | [**setFocusedItem**](#function-setfocuseditem) (int item) <br> |
|  void | [**setModel**](#function-setmodel) ([**QfMultiFeatureListModel**](classQfMultiFeatureListModel.md) \* model) <br> |
|  Q\_INVOKABLE void | [**toggleSelectedItem**](#function-toggleselecteditem) (int item) <br> |




























## Public Properties Documentation




### property focusedFeature [1/2]

```C++
QgsFeature QfFeatureListModelSelection::focusedFeature;
```




<hr>



### property focusedGeometry [1/2]

```C++
QgsGeometry QfFeatureListModelSelection::focusedGeometry;
```




<hr>



### property focusedItem [1/2]

```C++
int QfFeatureListModelSelection::focusedItem;
```




<hr>



### property focusedLayer [1/2]

```C++
QgsVectorLayer * QfFeatureListModelSelection::focusedLayer;
```




<hr>



### property model [1/2]

```C++
QfMultiFeatureListModel * QfFeatureListModelSelection::model;
```




<hr>
## Public Signals Documentation




### signal focusedItemChanged 

```C++
void QfFeatureListModelSelection::focusedItemChanged;
```




<hr>



### signal modelChanged 

```C++
void QfFeatureListModelSelection::modelChanged;
```




<hr>



### signal selectedFeaturesChanged 

```C++
void QfFeatureListModelSelection::selectedFeaturesChanged;
```




<hr>
## Public Functions Documentation




### function QfFeatureListModelSelection 

```C++
explicit QfFeatureListModelSelection::QfFeatureListModelSelection (
    QObject * parent=nullptr
) 
```




<hr>



### function clear 

```C++
Q_INVOKABLE void QfFeatureListModelSelection::clear () 
```




<hr>



### function focusedFeature [2/2]

```C++
QgsFeature QfFeatureListModelSelection::focusedFeature () const
```




<hr>



### function focusedGeometry [2/2]

```C++
QgsGeometry QfFeatureListModelSelection::focusedGeometry () const
```




<hr>



### function focusedItem [2/2]

```C++
int QfFeatureListModelSelection::focusedItem () const
```




<hr>



### function focusedLayer [2/2]

```C++
QgsVectorLayer * QfFeatureListModelSelection::focusedLayer () const
```




<hr>



### function model [2/2]

```C++
QfMultiFeatureListModel * QfFeatureListModelSelection::model () const
```




<hr>



### function setFocusedItem 

```C++
void QfFeatureListModelSelection::setFocusedItem (
    int item
) 
```




<hr>



### function setModel 

```C++
void QfFeatureListModelSelection::setModel (
    QfMultiFeatureListModel * model
) 
```




<hr>



### function toggleSelectedItem 

```C++
Q_INVOKABLE void QfFeatureListModelSelection::toggleSelectedItem (
    int item
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/qffeaturelistmodelselection.h`

