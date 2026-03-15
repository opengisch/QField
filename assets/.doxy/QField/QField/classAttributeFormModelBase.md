

# Class AttributeFormModelBase



[**ClassList**](annotated.md) **>** [**AttributeFormModelBase**](classAttributeFormModelBase.md)








Inherits the following classes: QStandardItemModel






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**constraintsHardValidChanged**](classAttributeFormModelBase.md#signal-constraintshardvalidchanged)  <br> |
| signal void | [**constraintsSoftValidChanged**](classAttributeFormModelBase.md#signal-constraintssoftvalidchanged)  <br> |
| signal void | [**featureChanged**](classAttributeFormModelBase.md#signal-featurechanged)  <br> |
| signal void | [**featureModelChanged**](classAttributeFormModelBase.md#signal-featuremodelchanged)  <br> |
| signal void | [**hasConstraintsChanged**](classAttributeFormModelBase.md#signal-hasconstraintschanged)  <br> |
| signal void | [**hasRemembranceChanged**](classAttributeFormModelBase.md#signal-hasremembrancechanged)  <br> |
| signal void | [**hasTabsChanged**](classAttributeFormModelBase.md#signal-hastabschanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**AttributeFormModelBase**](#function-attributeformmodelbase) (QObject \* parent=nullptr) <br> |
|  void | [**activateAllRememberValues**](#function-activateallremembervalues) () <br>_Activate all available value that can be remembered and reused._  |
|  void | [**applyFeatureModel**](#function-applyfeaturemodel) () <br>_Applies feature model data such as attribute values, constraints, visibility to the attribute form model._  |
|  void | [**applyParentDefaultValues**](#function-applyparentdefaultvalues) () <br>_Applies default values linked to a parent feature._  |
|  void | [**applyRelationshipDefaultValues**](#function-applyrelationshipdefaultvalues) () <br>_Applies default values linked to relationships._  |
|  QVariant | [**attribute**](#function-attribute) (const QString & name) <br> |
|  bool | [**changeAttribute**](#function-changeattribute) (const QString & name, const QVariant & value) <br> |
|  bool | [**changeGeometry**](#function-changegeometry) (const QgsGeometry & geometry) <br> |
|  bool | [**constraintsHardValid**](#function-constraintshardvalid) () const<br> |
|  bool | [**constraintsSoftValid**](#function-constraintssoftvalid) () const<br> |
|  bool | [**create**](#function-create) () <br> |
|  void | [**deactivateAllRememberValues**](#function-deactivateallremembervalues) () <br>_Deactivate all available value that can be remembered and reused._  |
|  bool | [**deleteFeature**](#function-deletefeature) () <br> |
|  [**FeatureModel**](classFeatureModel.md) \* | [**featureModel**](#function-featuremodel) () const<br> |
|  bool | [**hasConstraints**](#function-hasconstraints) () const<br> |
|  bool | [**hasRemembrance**](#function-hasremembrance) () const<br> |
|  bool | [**hasTabs**](#function-hastabs) () const<br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  bool | [**save**](#function-save) () <br> |
|  bool | [**setData**](#function-setdata) (const QModelIndex & index, const QVariant & value, int role=Qt::EditRole) override<br> |
|  void | [**setFeatureModel**](#function-setfeaturemodel) ([**FeatureModel**](classFeatureModel.md) \* featureModel) <br> |
|  void | [**setHasConstraints**](#function-sethasconstraints) (bool hasConstraints) <br> |
|  void | [**setHasRemembrance**](#function-sethasremembrance) (bool hasRemembrance) <br> |
|  void | [**setHasTabs**](#function-sethastabs) (bool hasTabs) <br> |




























## Public Signals Documentation




### signal constraintsHardValidChanged 

```C++
void AttributeFormModelBase::constraintsHardValidChanged;
```




<hr>



### signal constraintsSoftValidChanged 

```C++
void AttributeFormModelBase::constraintsSoftValidChanged;
```




<hr>



### signal featureChanged 

```C++
void AttributeFormModelBase::featureChanged;
```




<hr>



### signal featureModelChanged 

```C++
void AttributeFormModelBase::featureModelChanged;
```




<hr>



### signal hasConstraintsChanged 

```C++
void AttributeFormModelBase::hasConstraintsChanged;
```




<hr>



### signal hasRemembranceChanged 

```C++
void AttributeFormModelBase::hasRemembranceChanged;
```




<hr>



### signal hasTabsChanged 

```C++
void AttributeFormModelBase::hasTabsChanged;
```




<hr>
## Public Functions Documentation




### function AttributeFormModelBase 

```C++
explicit AttributeFormModelBase::AttributeFormModelBase (
    QObject * parent=nullptr
) 
```




<hr>



### function activateAllRememberValues 

_Activate all available value that can be remembered and reused._ 
```C++
void AttributeFormModelBase::activateAllRememberValues () 
```




<hr>



### function applyFeatureModel 

_Applies feature model data such as attribute values, constraints, visibility to the attribute form model._ 
```C++
void AttributeFormModelBase::applyFeatureModel () 
```




<hr>



### function applyParentDefaultValues 

_Applies default values linked to a parent feature._ 
```C++
void AttributeFormModelBase::applyParentDefaultValues () 
```




<hr>



### function applyRelationshipDefaultValues 

_Applies default values linked to relationships._ 
```C++
void AttributeFormModelBase::applyRelationshipDefaultValues () 
```




<hr>



### function attribute 

```C++
QVariant AttributeFormModelBase::attribute (
    const QString & name
) 
```



Returns the value for an attribute _name_ of the current feature. 

**Returns:**

value of the attribute   





        

<hr>



### function changeAttribute 

```C++
bool AttributeFormModelBase::changeAttribute (
    const QString & name,
    const QVariant & value
) 
```




<hr>



### function changeGeometry 

```C++
bool AttributeFormModelBase::changeGeometry (
    const QgsGeometry & geometry
) 
```



Sets the _geometry_ of the current feature   


        

<hr>



### function constraintsHardValid 

```C++
bool AttributeFormModelBase::constraintsHardValid () const
```




<hr>



### function constraintsSoftValid 

```C++
bool AttributeFormModelBase::constraintsSoftValid () const
```




<hr>



### function create 

```C++
bool AttributeFormModelBase::create () 
```



Create the current (not existing yet) feature   


        

<hr>



### function deactivateAllRememberValues 

_Deactivate all available value that can be remembered and reused._ 
```C++
void AttributeFormModelBase::deactivateAllRememberValues () 
```




<hr>



### function deleteFeature 

```C++
bool AttributeFormModelBase::deleteFeature () 
```



Delete the current feature   


        

<hr>



### function featureModel 

```C++
FeatureModel * AttributeFormModelBase::featureModel () const
```




<hr>



### function hasConstraints 

```C++
bool AttributeFormModelBase::hasConstraints () const
```




<hr>



### function hasRemembrance 

```C++
bool AttributeFormModelBase::hasRemembrance () const
```




<hr>



### function hasTabs 

```C++
bool AttributeFormModelBase::hasTabs () const
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > AttributeFormModelBase::roleNames () override const
```




<hr>



### function save 

```C++
bool AttributeFormModelBase::save () 
```



Save the current (already existing) feature   


        

<hr>



### function setData 

```C++
bool AttributeFormModelBase::setData (
    const QModelIndex & index,
    const QVariant & value,
    int role=Qt::EditRole
) override
```




<hr>



### function setFeatureModel 

```C++
void AttributeFormModelBase::setFeatureModel (
    FeatureModel * featureModel
) 
```




<hr>



### function setHasConstraints 

```C++
void AttributeFormModelBase::setHasConstraints (
    bool hasConstraints
) 
```




<hr>



### function setHasRemembrance 

```C++
void AttributeFormModelBase::setHasRemembrance (
    bool hasRemembrance
) 
```




<hr>



### function setHasTabs 

```C++
void AttributeFormModelBase::setHasTabs (
    bool hasTabs
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/attributeformmodelbase.h`

