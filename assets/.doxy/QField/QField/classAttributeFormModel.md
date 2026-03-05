

# Class AttributeFormModel



[**ClassList**](annotated.md) **>** [**AttributeFormModel**](classAttributeFormModel.md)








Inherits the following classes: QSortFilterProxyModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**FeatureRoles**](#enum-featureroles)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**constraintsHardValid**](classAttributeFormModel.md#property-constraintshardvalid-12)  <br> |
| property bool | [**constraintsSoftValid**](classAttributeFormModel.md#property-constraintssoftvalid-12)  <br> |
| property [**FeatureModel**](classFeatureModel.md) \* | [**featureModel**](classAttributeFormModel.md#property-featuremodel-12)  <br> |
| property bool | [**hasConstraints**](classAttributeFormModel.md#property-hasconstraints-12)  <br> |
| property bool | [**hasRemembrance**](classAttributeFormModel.md#property-hasremembrance-12)  <br> |
| property bool | [**hasTabs**](classAttributeFormModel.md#property-hastabs-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**constraintsHardValidChanged**](classAttributeFormModel.md#signal-constraintshardvalidchanged)  <br> |
| signal void | [**constraintsSoftValidChanged**](classAttributeFormModel.md#signal-constraintssoftvalidchanged)  <br> |
| signal void | [**featureChanged**](classAttributeFormModel.md#signal-featurechanged)  <br> |
| signal void | [**featureModelChanged**](classAttributeFormModel.md#signal-featuremodelchanged)  <br> |
| signal void | [**hasConstraintsChanged**](classAttributeFormModel.md#signal-hasconstraintschanged)  <br> |
| signal void | [**hasRemembranceChanged**](classAttributeFormModel.md#signal-hasremembrancechanged)  <br> |
| signal void | [**hasTabsChanged**](classAttributeFormModel.md#signal-hastabschanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**AttributeFormModel**](#function-attributeformmodel) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE void | [**activateAllRememberValues**](#function-activateallremembervalues) () <br>_Activate all available value that can be remembered and reused._  |
|  Q\_INVOKABLE void | [**applyFeatureModel**](#function-applyfeaturemodel) () <br>_Applies feature model data such as attribute values, constraints, visibility to the attribute form model._  |
|  Q\_INVOKABLE void | [**applyParentDefaultValues**](#function-applyparentdefaultvalues) () <br>_Applies default values linked to a parent feature._  |
|  Q\_INVOKABLE void | [**applyRelationshipDefaultValues**](#function-applyrelationshipdefaultvalues) () <br>_Applies default values linked to relationships._  |
|  Q\_INVOKABLE QVariant | [**attribute**](#function-attribute) (const QString & name) <br> |
|  Q\_INVOKABLE bool | [**changeAttribute**](#function-changeattribute) (const QString & name, const QVariant & value) <br> |
|  Q\_INVOKABLE bool | [**changeGeometry**](#function-changegeometry) (const QgsGeometry & geometry) <br> |
|  bool | [**constraintsHardValid**](#function-constraintshardvalid-22) () const<br> |
|  bool | [**constraintsSoftValid**](#function-constraintssoftvalid-22) () const<br> |
|  Q\_INVOKABLE bool | [**create**](#function-create) () <br> |
|  Q\_INVOKABLE void | [**deactivateAllRememberValues**](#function-deactivateallremembervalues) () <br>_Deactivate all available value that can be remembered and reused._  |
|  Q\_INVOKABLE bool | [**deleteFeature**](#function-deletefeature) () <br> |
|  [**FeatureModel**](classFeatureModel.md) \* | [**featureModel**](#function-featuremodel-22) () const<br> |
|  bool | [**hasConstraints**](#function-hasconstraints-22) () const<br> |
|  bool | [**hasRemembrance**](#function-hasremembrance-22) () const<br> |
|  bool | [**hasTabs**](#function-hastabs-22) () const<br> |
|  Q\_INVOKABLE bool | [**save**](#function-save) () <br> |
|  void | [**setFeatureModel**](#function-setfeaturemodel) ([**FeatureModel**](classFeatureModel.md) \* featureModel) <br> |
























## Protected Functions

| Type | Name |
| ---: | :--- |
| virtual bool | [**filterAcceptsRow**](#function-filteracceptsrow) (int sourceRow, const QModelIndex & sourceParent) override const<br> |




## Public Types Documentation




### enum FeatureRoles 

```C++
enum AttributeFormModel::FeatureRoles {
    ElementType = Qt::UserRole + 1,
    Name,
    AttributeValue,
    AttributeEditable,
    EditorWidget,
    EditorWidgetConfig,
    RelationEditorWidget,
    RelationEditorWidgetConfig,
    CanRememberValue,
    RememberValue,
    Field,
    RelationId,
    NmRelationId,
    FieldIndex,
    AttributeEditorElement,
    CurrentlyVisible,
    ConstraintHardValid,
    ConstraintSoftValid,
    ConstraintDescription,
    AttributeAllowEdit,
    EditorWidgetCode,
    TabIndex,
    GroupColor,
    GroupName,
    GroupIndex,
    ColumnCount,
    LabelOverrideColor,
    LabelColor,
    LabelOverrideFont,
    LabelFont
};
```




<hr>
## Public Properties Documentation




### property constraintsHardValid [1/2]

```C++
bool AttributeFormModel::constraintsHardValid;
```




<hr>



### property constraintsSoftValid [1/2]

```C++
bool AttributeFormModel::constraintsSoftValid;
```




<hr>



### property featureModel [1/2]

```C++
FeatureModel * AttributeFormModel::featureModel;
```




<hr>



### property hasConstraints [1/2]

```C++
bool AttributeFormModel::hasConstraints;
```




<hr>



### property hasRemembrance [1/2]

```C++
bool AttributeFormModel::hasRemembrance;
```




<hr>



### property hasTabs [1/2]

```C++
bool AttributeFormModel::hasTabs;
```




<hr>
## Public Signals Documentation




### signal constraintsHardValidChanged 

```C++
void AttributeFormModel::constraintsHardValidChanged;
```




<hr>



### signal constraintsSoftValidChanged 

```C++
void AttributeFormModel::constraintsSoftValidChanged;
```




<hr>



### signal featureChanged 

```C++
void AttributeFormModel::featureChanged;
```




<hr>



### signal featureModelChanged 

```C++
void AttributeFormModel::featureModelChanged;
```




<hr>



### signal hasConstraintsChanged 

```C++
void AttributeFormModel::hasConstraintsChanged;
```




<hr>



### signal hasRemembranceChanged 

```C++
void AttributeFormModel::hasRemembranceChanged;
```




<hr>



### signal hasTabsChanged 

```C++
void AttributeFormModel::hasTabsChanged;
```




<hr>
## Public Functions Documentation




### function AttributeFormModel 

```C++
explicit AttributeFormModel::AttributeFormModel (
    QObject * parent=nullptr
) 
```




<hr>



### function activateAllRememberValues 

_Activate all available value that can be remembered and reused._ 
```C++
Q_INVOKABLE void AttributeFormModel::activateAllRememberValues () 
```




<hr>



### function applyFeatureModel 

_Applies feature model data such as attribute values, constraints, visibility to the attribute form model._ 
```C++
Q_INVOKABLE void AttributeFormModel::applyFeatureModel () 
```




<hr>



### function applyParentDefaultValues 

_Applies default values linked to a parent feature._ 
```C++
Q_INVOKABLE void AttributeFormModel::applyParentDefaultValues () 
```




<hr>



### function applyRelationshipDefaultValues 

_Applies default values linked to relationships._ 
```C++
Q_INVOKABLE void AttributeFormModel::applyRelationshipDefaultValues () 
```




<hr>



### function attribute 

```C++
Q_INVOKABLE QVariant AttributeFormModel::attribute (
    const QString & name
) 
```



Returns the value for an attribute _name_ of the current feature. 

**Returns:**

value of the attribute 





        

<hr>



### function changeAttribute 

```C++
Q_INVOKABLE bool AttributeFormModel::changeAttribute (
    const QString & name,
    const QVariant & value
) 
```



Gets the _value_ for an attribute _name_ of the current feature 

**Parameters:**


* `value` the value of the attribute 




        

<hr>



### function changeGeometry 

```C++
Q_INVOKABLE bool AttributeFormModel::changeGeometry (
    const QgsGeometry & geometry
) 
```



Sets the _geometry_ of the current feature 


        

<hr>



### function constraintsHardValid [2/2]

```C++
bool AttributeFormModel::constraintsHardValid () const
```




<hr>



### function constraintsSoftValid [2/2]

```C++
bool AttributeFormModel::constraintsSoftValid () const
```




<hr>



### function create 

```C++
Q_INVOKABLE bool AttributeFormModel::create () 
```



Create the current (not existing yet) feature 


        

<hr>



### function deactivateAllRememberValues 

_Deactivate all available value that can be remembered and reused._ 
```C++
Q_INVOKABLE void AttributeFormModel::deactivateAllRememberValues () 
```




<hr>



### function deleteFeature 

```C++
Q_INVOKABLE bool AttributeFormModel::deleteFeature () 
```



Delete the current feature 


        

<hr>



### function featureModel [2/2]

```C++
FeatureModel * AttributeFormModel::featureModel () const
```




<hr>



### function hasConstraints [2/2]

```C++
bool AttributeFormModel::hasConstraints () const
```




<hr>



### function hasRemembrance [2/2]

```C++
bool AttributeFormModel::hasRemembrance () const
```




<hr>



### function hasTabs [2/2]

```C++
bool AttributeFormModel::hasTabs () const
```




<hr>



### function save 

```C++
Q_INVOKABLE bool AttributeFormModel::save () 
```



Save the current (already existing) feature 


        

<hr>



### function setFeatureModel 

```C++
void AttributeFormModel::setFeatureModel (
    FeatureModel * featureModel
) 
```




<hr>
## Protected Functions Documentation




### function filterAcceptsRow 

```C++
virtual bool AttributeFormModel::filterAcceptsRow (
    int sourceRow,
    const QModelIndex & sourceParent
) override const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/attributeformmodel.h`

