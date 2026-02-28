

# Class FeatureListModelSelection



[**ClassList**](annotated.md) **>** [**FeatureListModelSelection**](classFeatureListModelSelection.md)








Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property QgsFeature | [**focusedFeature**](classFeatureListModelSelection.md#property-focusedfeature-12)  <br> |
| property QgsGeometry | [**focusedGeometry**](classFeatureListModelSelection.md#property-focusedgeometry-12)  <br> |
| property int | [**focusedItem**](classFeatureListModelSelection.md#property-focuseditem-12)  <br> |
| property QgsVectorLayer \* | [**focusedLayer**](classFeatureListModelSelection.md#property-focusedlayer-12)  <br> |
| property [**MultiFeatureListModel**](classMultiFeatureListModel.md) \* | [**model**](classFeatureListModelSelection.md#property-model-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**focusedItemChanged**](classFeatureListModelSelection.md#signal-focuseditemchanged)  <br> |
| signal void | [**modelChanged**](classFeatureListModelSelection.md#signal-modelchanged)  <br> |
| signal void | [**selectedFeaturesChanged**](classFeatureListModelSelection.md#signal-selectedfeatureschanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**FeatureListModelSelection**](#function-featurelistmodelselection) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**clear**](#function-clear) () <br> |
|  QgsFeature | [**focusedFeature**](#function-focusedfeature-22) () const<br> |
|  QgsGeometry | [**focusedGeometry**](#function-focusedgeometry-22) () const<br> |
|  int | [**focusedItem**](#function-focuseditem-22) () const<br> |
|  QgsVectorLayer \* | [**focusedLayer**](#function-focusedlayer-22) () const<br> |
|  [**MultiFeatureListModel**](classMultiFeatureListModel.md) \* | [**model**](#function-model-22) () const<br> |
|  void | [**setFocusedItem**](#function-setfocuseditem) (int item) <br> |
|  void | [**setModel**](#function-setmodel) ([**MultiFeatureListModel**](classMultiFeatureListModel.md) \* model) <br> |
|  Q\_INVOKABLE void | [**toggleSelectedItem**](#function-toggleselecteditem) (int item) <br> |




























## Public Properties Documentation




### property focusedFeature [1/2]

```C++
QgsFeature FeatureListModelSelection::focusedFeature;
```




<hr>



### property focusedGeometry [1/2]

```C++
QgsGeometry FeatureListModelSelection::focusedGeometry;
```




<hr>



### property focusedItem [1/2]

```C++
int FeatureListModelSelection::focusedItem;
```




<hr>



### property focusedLayer [1/2]

```C++
QgsVectorLayer * FeatureListModelSelection::focusedLayer;
```




<hr>



### property model [1/2]

```C++
MultiFeatureListModel * FeatureListModelSelection::model;
```




<hr>
## Public Signals Documentation




### signal focusedItemChanged 

```C++
void FeatureListModelSelection::focusedItemChanged;
```




<hr>



### signal modelChanged 

```C++
void FeatureListModelSelection::modelChanged;
```




<hr>



### signal selectedFeaturesChanged 

```C++
void FeatureListModelSelection::selectedFeaturesChanged;
```




<hr>
## Public Functions Documentation




### function FeatureListModelSelection 

```C++
explicit FeatureListModelSelection::FeatureListModelSelection (
    QObject * parent=nullptr
) 
```




<hr>



### function clear 

```C++
Q_INVOKABLE void FeatureListModelSelection::clear () 
```




<hr>



### function focusedFeature [2/2]

```C++
QgsFeature FeatureListModelSelection::focusedFeature () const
```




<hr>



### function focusedGeometry [2/2]

```C++
QgsGeometry FeatureListModelSelection::focusedGeometry () const
```




<hr>



### function focusedItem [2/2]

```C++
int FeatureListModelSelection::focusedItem () const
```




<hr>



### function focusedLayer [2/2]

```C++
QgsVectorLayer * FeatureListModelSelection::focusedLayer () const
```




<hr>



### function model [2/2]

```C++
MultiFeatureListModel * FeatureListModelSelection::model () const
```




<hr>



### function setFocusedItem 

```C++
void FeatureListModelSelection::setFocusedItem (
    int item
) 
```




<hr>



### function setModel 

```C++
void FeatureListModelSelection::setModel (
    MultiFeatureListModel * model
) 
```




<hr>



### function toggleSelectedItem 

```C++
Q_INVOKABLE void FeatureListModelSelection::toggleSelectedItem (
    int item
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/featurelistmodelselection.h`

