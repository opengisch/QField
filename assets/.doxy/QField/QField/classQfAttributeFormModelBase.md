

# Class QfAttributeFormModelBase



[**ClassList**](annotated.md) **>** [**QfAttributeFormModelBase**](classQfAttributeFormModelBase.md)








Inherits the following classes: QStandardItemModel






























## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**constraintsHardValidChanged**](classQfAttributeFormModelBase.md#signal-constraintshardvalidchanged)  <br> |
| signal void | [**constraintsSoftValidChanged**](classQfAttributeFormModelBase.md#signal-constraintssoftvalidchanged)  <br> |
| signal void | [**featureModelChanged**](classQfAttributeFormModelBase.md#signal-featuremodelchanged)  <br> |
| signal void | [**hasConstraintsChanged**](classQfAttributeFormModelBase.md#signal-hasconstraintschanged)  <br> |
| signal void | [**hasRemembranceChanged**](classQfAttributeFormModelBase.md#signal-hasremembrancechanged)  <br> |
| signal void | [**hasTabsChanged**](classQfAttributeFormModelBase.md#signal-hastabschanged)  <br> |
| signal void | [**isWizardChanged**](classQfAttributeFormModelBase.md#signal-iswizardchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfAttributeFormModelBase**](#function-qfattributeformmodelbase) (QObject \* parent=nullptr) <br> |
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
|  QgsExpressionContext | [**createExpressionContext**](#function-createexpressioncontext) () const<br>_Creates an expression context._  |
|  void | [**deactivateAllRememberValues**](#function-deactivateallremembervalues) () <br>_Deactivate all available value that can be remembered and reused._  |
|  bool | [**deleteFeature**](#function-deletefeature) () <br> |
|  [**QfFeatureModel**](classQfFeatureModel.md) \* | [**featureModel**](#function-featuremodel) () const<br> |
|  bool | [**hasConstraints**](#function-hasconstraints) () const<br> |
|  bool | [**hasRemembrance**](#function-hasremembrance) () const<br> |
|  bool | [**hasTabs**](#function-hastabs) () const<br> |
|  bool | [**isWizard**](#function-iswizard) () const<br> |
|  QHash&lt; int, QByteArray &gt; | [**roleNames**](#function-rolenames) () override const<br> |
|  bool | [**save**](#function-save) () <br> |
|  bool | [**setData**](#function-setdata) (const QModelIndex & index, const QVariant & value, int role=Qt::EditRole) override<br> |
|  void | [**setFeatureModel**](#function-setfeaturemodel) ([**QfFeatureModel**](classQfFeatureModel.md) \* featureModel) <br> |
|  void | [**setHasConstraints**](#function-sethasconstraints) (bool hasConstraints) <br> |
|  void | [**setHasRemembrance**](#function-sethasremembrance) (bool hasRemembrance) <br> |
|  void | [**setHasTabs**](#function-sethastabs) (bool hasTabs) <br> |
|  void | [**setIsWizard**](#function-setiswizard) (bool isWizard) <br> |




























## Public Signals Documentation




### signal constraintsHardValidChanged 

```C++
void QfAttributeFormModelBase::constraintsHardValidChanged;
```




<hr>



### signal constraintsSoftValidChanged 

```C++
void QfAttributeFormModelBase::constraintsSoftValidChanged;
```




<hr>



### signal featureModelChanged 

```C++
void QfAttributeFormModelBase::featureModelChanged;
```




<hr>



### signal hasConstraintsChanged 

```C++
void QfAttributeFormModelBase::hasConstraintsChanged;
```




<hr>



### signal hasRemembranceChanged 

```C++
void QfAttributeFormModelBase::hasRemembranceChanged;
```




<hr>



### signal hasTabsChanged 

```C++
void QfAttributeFormModelBase::hasTabsChanged;
```




<hr>



### signal isWizardChanged 

```C++
void QfAttributeFormModelBase::isWizardChanged;
```




<hr>
## Public Functions Documentation




### function QfAttributeFormModelBase 

```C++
explicit QfAttributeFormModelBase::QfAttributeFormModelBase (
    QObject * parent=nullptr
) 
```




<hr>



### function activateAllRememberValues 

_Activate all available value that can be remembered and reused._ 
```C++
void QfAttributeFormModelBase::activateAllRememberValues () 
```




<hr>



### function applyFeatureModel 

_Applies feature model data such as attribute values, constraints, visibility to the attribute form model._ 
```C++
void QfAttributeFormModelBase::applyFeatureModel () 
```




<hr>



### function applyParentDefaultValues 

_Applies default values linked to a parent feature._ 
```C++
void QfAttributeFormModelBase::applyParentDefaultValues () 
```




<hr>



### function applyRelationshipDefaultValues 

_Applies default values linked to relationships._ 
```C++
void QfAttributeFormModelBase::applyRelationshipDefaultValues () 
```




<hr>



### function attribute 

```C++
QVariant QfAttributeFormModelBase::attribute (
    const QString & name
) 
```



Returns the value for an attribute _name_ of the current feature. 

**Returns:**

value of the attribute   





        

<hr>



### function changeAttribute 

```C++
bool QfAttributeFormModelBase::changeAttribute (
    const QString & name,
    const QVariant & value
) 
```




<hr>



### function changeGeometry 

```C++
bool QfAttributeFormModelBase::changeGeometry (
    const QgsGeometry & geometry
) 
```



Sets the _geometry_ of the current feature   


        

<hr>



### function constraintsHardValid 

```C++
bool QfAttributeFormModelBase::constraintsHardValid () const
```




<hr>



### function constraintsSoftValid 

```C++
bool QfAttributeFormModelBase::constraintsSoftValid () const
```




<hr>



### function create 

```C++
bool QfAttributeFormModelBase::create () 
```



Create the current (not existing yet) feature   


        

<hr>



### function createExpressionContext 

_Creates an expression context._ 
```C++
QgsExpressionContext QfAttributeFormModelBase::createExpressionContext () const
```




<hr>



### function deactivateAllRememberValues 

_Deactivate all available value that can be remembered and reused._ 
```C++
void QfAttributeFormModelBase::deactivateAllRememberValues () 
```




<hr>



### function deleteFeature 

```C++
bool QfAttributeFormModelBase::deleteFeature () 
```



Delete the current feature   


        

<hr>



### function featureModel 

```C++
QfFeatureModel * QfAttributeFormModelBase::featureModel () const
```




<hr>



### function hasConstraints 

```C++
bool QfAttributeFormModelBase::hasConstraints () const
```




<hr>



### function hasRemembrance 

```C++
bool QfAttributeFormModelBase::hasRemembrance () const
```




<hr>



### function hasTabs 

```C++
bool QfAttributeFormModelBase::hasTabs () const
```




<hr>



### function isWizard 

```C++
bool QfAttributeFormModelBase::isWizard () const
```




<hr>



### function roleNames 

```C++
QHash< int, QByteArray > QfAttributeFormModelBase::roleNames () override const
```




<hr>



### function save 

```C++
bool QfAttributeFormModelBase::save () 
```



Save the current (already existing) feature   


        

<hr>



### function setData 

```C++
bool QfAttributeFormModelBase::setData (
    const QModelIndex & index,
    const QVariant & value,
    int role=Qt::EditRole
) override
```




<hr>



### function setFeatureModel 

```C++
void QfAttributeFormModelBase::setFeatureModel (
    QfFeatureModel * featureModel
) 
```




<hr>



### function setHasConstraints 

```C++
void QfAttributeFormModelBase::setHasConstraints (
    bool hasConstraints
) 
```




<hr>



### function setHasRemembrance 

```C++
void QfAttributeFormModelBase::setHasRemembrance (
    bool hasRemembrance
) 
```




<hr>



### function setHasTabs 

```C++
void QfAttributeFormModelBase::setHasTabs (
    bool hasTabs
) 
```




<hr>



### function setIsWizard 

```C++
void QfAttributeFormModelBase::setIsWizard (
    bool isWizard
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qfattributeformmodelbase.h`

