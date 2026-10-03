

# Class QfAttributeFormModel



[**ClassList**](annotated.md) **>** [**QfAttributeFormModel**](classQfAttributeFormModel.md)








Inherits the following classes: QSortFilterProxyModel














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**FeatureRoles**](#enum-featureroles)  <br> |












## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**constraintsHardValid**](classQfAttributeFormModel.md#property-constraintshardvalid-12)  <br> |
| property bool | [**constraintsSoftValid**](classQfAttributeFormModel.md#property-constraintssoftvalid-12)  <br> |
| property [**QfFeatureModel**](classQfFeatureModel.md) \* | [**featureModel**](classQfAttributeFormModel.md#property-featuremodel-12)  <br> |
| property bool | [**hasConstraints**](classQfAttributeFormModel.md#property-hasconstraints-12)  <br> |
| property bool | [**hasRemembrance**](classQfAttributeFormModel.md#property-hasremembrance-12)  <br> |
| property bool | [**hasTabs**](classQfAttributeFormModel.md#property-hastabs-12)  <br> |
| property bool | [**isWizard**](classQfAttributeFormModel.md#property-iswizard-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**constraintsHardValidChanged**](classQfAttributeFormModel.md#signal-constraintshardvalidchanged)  <br> |
| signal void | [**constraintsSoftValidChanged**](classQfAttributeFormModel.md#signal-constraintssoftvalidchanged)  <br> |
| signal void | [**featureModelChanged**](classQfAttributeFormModel.md#signal-featuremodelchanged)  <br> |
| signal void | [**hasConstraintsChanged**](classQfAttributeFormModel.md#signal-hasconstraintschanged)  <br> |
| signal void | [**hasRemembranceChanged**](classQfAttributeFormModel.md#signal-hasremembrancechanged)  <br> |
| signal void | [**hasTabsChanged**](classQfAttributeFormModel.md#signal-hastabschanged)  <br> |
| signal void | [**isWizardChanged**](classQfAttributeFormModel.md#signal-iswizardchanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfAttributeFormModel**](#function-qfattributeformmodel) (QObject \* parent=nullptr) <br> |
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
|  QgsExpressionContext | [**createExpressionContext**](#function-createexpressioncontext) () const<br>_Creates an expression context._  |
|  Q\_INVOKABLE void | [**deactivateAllRememberValues**](#function-deactivateallremembervalues) () <br>_Deactivate all available value that can be remembered and reused._  |
|  Q\_INVOKABLE bool | [**deleteFeature**](#function-deletefeature) () <br> |
|  [**QfFeatureModel**](classQfFeatureModel.md) \* | [**featureModel**](#function-featuremodel-22) () const<br> |
|  bool | [**hasConstraints**](#function-hasconstraints-22) () const<br> |
|  bool | [**hasRemembrance**](#function-hasremembrance-22) () const<br> |
|  bool | [**hasTabs**](#function-hastabs-22) () const<br> |
|  bool | [**isWizard**](#function-iswizard-22) () const<br> |
|  Q\_INVOKABLE bool | [**save**](#function-save) () <br> |
|  void | [**setFeatureModel**](#function-setfeaturemodel) ([**QfFeatureModel**](classQfFeatureModel.md) \* featureModel) <br> |
























## Protected Functions

| Type | Name |
| ---: | :--- |
| virtual bool | [**filterAcceptsRow**](#function-filteracceptsrow) (int sourceRow, const QModelIndex & sourceParent) override const<br> |




## Public Types Documentation




### enum FeatureRoles 

```C++
enum QfAttributeFormModel::FeatureRoles {
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
bool QfAttributeFormModel::constraintsHardValid;
```




<hr>



### property constraintsSoftValid [1/2]

```C++
bool QfAttributeFormModel::constraintsSoftValid;
```




<hr>



### property featureModel [1/2]

```C++
QfFeatureModel * QfAttributeFormModel::featureModel;
```




<hr>



### property hasConstraints [1/2]

```C++
bool QfAttributeFormModel::hasConstraints;
```




<hr>



### property hasRemembrance [1/2]

```C++
bool QfAttributeFormModel::hasRemembrance;
```




<hr>



### property hasTabs [1/2]

```C++
bool QfAttributeFormModel::hasTabs;
```




<hr>



### property isWizard [1/2]

```C++
bool QfAttributeFormModel::isWizard;
```




<hr>
## Public Signals Documentation




### signal constraintsHardValidChanged 

```C++
void QfAttributeFormModel::constraintsHardValidChanged;
```




<hr>



### signal constraintsSoftValidChanged 

```C++
void QfAttributeFormModel::constraintsSoftValidChanged;
```




<hr>



### signal featureModelChanged 

```C++
void QfAttributeFormModel::featureModelChanged;
```




<hr>



### signal hasConstraintsChanged 

```C++
void QfAttributeFormModel::hasConstraintsChanged;
```




<hr>



### signal hasRemembranceChanged 

```C++
void QfAttributeFormModel::hasRemembranceChanged;
```




<hr>



### signal hasTabsChanged 

```C++
void QfAttributeFormModel::hasTabsChanged;
```




<hr>



### signal isWizardChanged 

```C++
void QfAttributeFormModel::isWizardChanged;
```




<hr>
## Public Functions Documentation




### function QfAttributeFormModel 

```C++
explicit QfAttributeFormModel::QfAttributeFormModel (
    QObject * parent=nullptr
) 
```




<hr>



### function activateAllRememberValues 

_Activate all available value that can be remembered and reused._ 
```C++
Q_INVOKABLE void QfAttributeFormModel::activateAllRememberValues () 
```




<hr>



### function applyFeatureModel 

_Applies feature model data such as attribute values, constraints, visibility to the attribute form model._ 
```C++
Q_INVOKABLE void QfAttributeFormModel::applyFeatureModel () 
```




<hr>



### function applyParentDefaultValues 

_Applies default values linked to a parent feature._ 
```C++
Q_INVOKABLE void QfAttributeFormModel::applyParentDefaultValues () 
```




<hr>



### function applyRelationshipDefaultValues 

_Applies default values linked to relationships._ 
```C++
Q_INVOKABLE void QfAttributeFormModel::applyRelationshipDefaultValues () 
```




<hr>



### function attribute 

```C++
Q_INVOKABLE QVariant QfAttributeFormModel::attribute (
    const QString & name
) 
```



Returns the value for an attribute _name_ of the current feature. 

**Returns:**

value of the attribute 





        

<hr>



### function changeAttribute 

```C++
Q_INVOKABLE bool QfAttributeFormModel::changeAttribute (
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
Q_INVOKABLE bool QfAttributeFormModel::changeGeometry (
    const QgsGeometry & geometry
) 
```



Sets the _geometry_ of the current feature 


        

<hr>



### function constraintsHardValid [2/2]

```C++
bool QfAttributeFormModel::constraintsHardValid () const
```




<hr>



### function constraintsSoftValid [2/2]

```C++
bool QfAttributeFormModel::constraintsSoftValid () const
```




<hr>



### function create 

```C++
Q_INVOKABLE bool QfAttributeFormModel::create () 
```



Create the current (not existing yet) feature 


        

<hr>



### function createExpressionContext 

_Creates an expression context._ 
```C++
QgsExpressionContext QfAttributeFormModel::createExpressionContext () const
```




<hr>



### function deactivateAllRememberValues 

_Deactivate all available value that can be remembered and reused._ 
```C++
Q_INVOKABLE void QfAttributeFormModel::deactivateAllRememberValues () 
```




<hr>



### function deleteFeature 

```C++
Q_INVOKABLE bool QfAttributeFormModel::deleteFeature () 
```



Delete the current feature 


        

<hr>



### function featureModel [2/2]

```C++
QfFeatureModel * QfAttributeFormModel::featureModel () const
```




<hr>



### function hasConstraints [2/2]

```C++
bool QfAttributeFormModel::hasConstraints () const
```




<hr>



### function hasRemembrance [2/2]

```C++
bool QfAttributeFormModel::hasRemembrance () const
```




<hr>



### function hasTabs [2/2]

```C++
bool QfAttributeFormModel::hasTabs () const
```




<hr>



### function isWizard [2/2]

```C++
bool QfAttributeFormModel::isWizard () const
```




<hr>



### function save 

```C++
Q_INVOKABLE bool QfAttributeFormModel::save () 
```



Save the current (already existing) feature 


        

<hr>



### function setFeatureModel 

```C++
void QfAttributeFormModel::setFeatureModel (
    QfFeatureModel * featureModel
) 
```




<hr>
## Protected Functions Documentation




### function filterAcceptsRow 

```C++
virtual bool QfAttributeFormModel::filterAcceptsRow (
    int sourceRow,
    const QModelIndex & sourceParent
) override const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qfattributeformmodel.h`

