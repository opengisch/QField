

# Class ExpressionEvaluator



[**ClassList**](annotated.md) **>** [**ExpressionEvaluator**](classExpressionEvaluator.md)



_The_ [_**ExpressionEvaluator**_](classExpressionEvaluator.md) _class enables evaluation of expression strings and expression templates._

* `#include <expressionevaluator.h>`



Inherits the following classes: QObject














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Mode**](#enum-mode)  <br>_Expression evaluator modes._  |












## Public Properties

| Type | Name |
| ---: | :--- |
| property [**AppExpressionContextScopesGenerator**](classAppExpressionContextScopesGenerator.md) \* | [**appExpressionContextScopesGenerator**](classExpressionEvaluator.md#property-appexpressioncontextscopesgenerator-12)  <br> |
| property [**AttributeFormModel**](classAttributeFormModel.md) \* | [**attributeFormModel**](classExpressionEvaluator.md#property-attributeformmodel-12)  <br> |
| property QString | [**expressionText**](classExpressionEvaluator.md#property-expressiontext-12)  <br> |
| property QgsFeature | [**feature**](classExpressionEvaluator.md#property-feature-12)  <br> |
| property QgsMapLayer \* | [**layer**](classExpressionEvaluator.md#property-layer-12)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classExpressionEvaluator.md#property-mapsettings-12)  <br> |
| property [**Mode**](classExpressionEvaluator.md#enum-mode) | [**mode**](classExpressionEvaluator.md#property-mode-12)  <br> |
| property QgsProject \* | [**project**](classExpressionEvaluator.md#property-project-12)  <br> |
| property QVariantMap | [**variables**](classExpressionEvaluator.md#property-variables-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**appExpressionContextScopesGeneratorChanged**](classExpressionEvaluator.md#signal-appexpressioncontextscopesgeneratorchanged)  <br> |
| signal void | [**attributeFormModelChanged**](classExpressionEvaluator.md#signal-attributeformmodelchanged)  <br> |
| signal void | [**expressionTextChanged**](classExpressionEvaluator.md#signal-expressiontextchanged)  <br> |
| signal void | [**featureChanged**](classExpressionEvaluator.md#signal-featurechanged)  <br> |
| signal void | [**layerChanged**](classExpressionEvaluator.md#signal-layerchanged)  <br> |
| signal void | [**mapSettingsChanged**](classExpressionEvaluator.md#signal-mapsettingschanged)  <br> |
| signal void | [**modeChanged**](classExpressionEvaluator.md#signal-modechanged)  <br> |
| signal void | [**projectChanged**](classExpressionEvaluator.md#signal-projectchanged)  <br> |
| signal void | [**variablesChanged**](classExpressionEvaluator.md#signal-variableschanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ExpressionEvaluator**](#function-expressionevaluator) (QObject \* parent=nullptr) <br> |
|  [**AppExpressionContextScopesGenerator**](classAppExpressionContextScopesGenerator.md) \* | [**appExpressionContextScopesGenerator**](#function-appexpressioncontextscopesgenerator-22) () const<br>_Returns the application expression context scopes generator object._  |
|  [**AttributeFormModel**](classAttributeFormModel.md) \* | [**attributeFormModel**](#function-attributeformmodel-22) () const<br>_Returns the attribute form model providing an expression context._  |
|  Q\_INVOKABLE QVariant | [**evaluate**](#function-evaluate-12) () <br>_Evalutes and returns the current expression text property value._  |
|  Q\_INVOKABLE QVariant | [**evaluate**](#function-evaluate-22) (const QString & expressionText) <br>_Evalutes and returns the expression text passed as a parameter._  |
|  QString | [**expressionText**](#function-expressiontext-22) () <br>_Returns the expression text used when evaluating._  |
|  QgsFeature | [**feature**](#function-feature-22) () const<br>_Returns the feature attached to the expression context._  |
|  QgsMapLayer \* | [**layer**](#function-layer-22) () const<br>_Returns the map layer attached to the expression context._  |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () const<br>_Returns the map settings attached to the expression context._  |
|  [**Mode**](classExpressionEvaluator.md#enum-mode) | [**mode**](#function-mode-22) () const<br>_Returns the expression evaluator mode._  |
|  QgsProject \* | [**project**](#function-project-22) () const<br>_Returns the project attached to the expression context._  |
|  void | [**setAppExpressionContextScopesGenerator**](#function-setappexpressioncontextscopesgenerator) ([**AppExpressionContextScopesGenerator**](classAppExpressionContextScopesGenerator.md) \* generator) <br>_Sets the application expression context scopes_ _generator_ _object._ |
|  void | [**setAttributeFormModel**](#function-setattributeformmodel) ([**AttributeFormModel**](classAttributeFormModel.md) \* attributeFormModel) <br>_Sets the attribute form model providing an expression context._  |
|  void | [**setExpressionText**](#function-setexpressiontext) (const QString & expressionText) <br>_Sets the expression text used when evaluating._  |
|  void | [**setFeature**](#function-setfeature) (const QgsFeature & feature) <br>_Sets the feature attached to the expression context._  |
|  void | [**setLayer**](#function-setlayer) (QgsMapLayer \* layer) <br>_Sets the map layer attached to the expression context._  |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br>_Sets the map settings attached to the expression context._  |
|  void | [**setMode**](#function-setmode) ([**Mode**](classExpressionEvaluator.md#enum-mode) mode) <br>_Sets the expression evaluator_ _mode_ _._ |
|  void | [**setProject**](#function-setproject) (QgsProject \* project) <br>_Sets the project attached to the expression context._  |
|  void | [**setVariables**](#function-setvariables) (const QVariantMap & variables) <br>_Sets the map of variables injected into the expression context when evaluating._  |
|  QVariantMap | [**variables**](#function-variables-22) () const<br>_Returns the map of variables injected into the expression context when evaluating._  |




























## Public Types Documentation




### enum Mode 

_Expression evaluator modes._ 
```C++
enum ExpressionEvaluator::Mode {
    ExpressionMode,
    ExpressionTemplateMode
};
```




<hr>
## Public Properties Documentation




### property appExpressionContextScopesGenerator [1/2]

```C++
AppExpressionContextScopesGenerator * ExpressionEvaluator::appExpressionContextScopesGenerator;
```




<hr>



### property attributeFormModel [1/2]

```C++
AttributeFormModel * ExpressionEvaluator::attributeFormModel;
```




<hr>



### property expressionText [1/2]

```C++
QString ExpressionEvaluator::expressionText;
```




<hr>



### property feature [1/2]

```C++
QgsFeature ExpressionEvaluator::feature;
```




<hr>



### property layer [1/2]

```C++
QgsMapLayer * ExpressionEvaluator::layer;
```




<hr>



### property mapSettings [1/2]

```C++
QgsQuickMapSettings * ExpressionEvaluator::mapSettings;
```




<hr>



### property mode [1/2]

```C++
Mode ExpressionEvaluator::mode;
```




<hr>



### property project [1/2]

```C++
QgsProject * ExpressionEvaluator::project;
```




<hr>



### property variables [1/2]

```C++
QVariantMap ExpressionEvaluator::variables;
```




<hr>
## Public Signals Documentation




### signal appExpressionContextScopesGeneratorChanged 

```C++
void ExpressionEvaluator::appExpressionContextScopesGeneratorChanged;
```




<hr>



### signal attributeFormModelChanged 

```C++
void ExpressionEvaluator::attributeFormModelChanged;
```




<hr>



### signal expressionTextChanged 

```C++
void ExpressionEvaluator::expressionTextChanged;
```




<hr>



### signal featureChanged 

```C++
void ExpressionEvaluator::featureChanged;
```




<hr>



### signal layerChanged 

```C++
void ExpressionEvaluator::layerChanged;
```




<hr>



### signal mapSettingsChanged 

```C++
void ExpressionEvaluator::mapSettingsChanged;
```




<hr>



### signal modeChanged 

```C++
void ExpressionEvaluator::modeChanged;
```




<hr>



### signal projectChanged 

```C++
void ExpressionEvaluator::projectChanged;
```




<hr>



### signal variablesChanged 

```C++
void ExpressionEvaluator::variablesChanged;
```




<hr>
## Public Functions Documentation




### function ExpressionEvaluator 

```C++
explicit ExpressionEvaluator::ExpressionEvaluator (
    QObject * parent=nullptr
) 
```




<hr>



### function appExpressionContextScopesGenerator [2/2]

_Returns the application expression context scopes generator object._ 
```C++
AppExpressionContextScopesGenerator * ExpressionEvaluator::appExpressionContextScopesGenerator () const
```




<hr>



### function attributeFormModel [2/2]

_Returns the attribute form model providing an expression context._ 
```C++
AttributeFormModel * ExpressionEvaluator::attributeFormModel () const
```




<hr>



### function evaluate [1/2]

_Evalutes and returns the current expression text property value._ 
```C++
Q_INVOKABLE QVariant ExpressionEvaluator::evaluate () 
```




<hr>



### function evaluate [2/2]

_Evalutes and returns the expression text passed as a parameter._ 
```C++
Q_INVOKABLE QVariant ExpressionEvaluator::evaluate (
    const QString & expressionText
) 
```




<hr>



### function expressionText [2/2]

_Returns the expression text used when evaluating._ 
```C++
inline QString ExpressionEvaluator::expressionText () 
```




<hr>



### function feature [2/2]

_Returns the feature attached to the expression context._ 
```C++
inline QgsFeature ExpressionEvaluator::feature () const
```




<hr>



### function layer [2/2]

_Returns the map layer attached to the expression context._ 
```C++
inline QgsMapLayer * ExpressionEvaluator::layer () const
```




<hr>



### function mapSettings [2/2]

_Returns the map settings attached to the expression context._ 
```C++
inline QgsQuickMapSettings * ExpressionEvaluator::mapSettings () const
```




<hr>



### function mode [2/2]

_Returns the expression evaluator mode._ 
```C++
inline Mode ExpressionEvaluator::mode () const
```




<hr>



### function project [2/2]

_Returns the project attached to the expression context._ 
```C++
inline QgsProject * ExpressionEvaluator::project () const
```




<hr>



### function setAppExpressionContextScopesGenerator 

_Sets the application expression context scopes_ _generator_ _object._
```C++
void ExpressionEvaluator::setAppExpressionContextScopesGenerator (
    AppExpressionContextScopesGenerator * generator
) 
```




<hr>



### function setAttributeFormModel 

_Sets the attribute form model providing an expression context._ 
```C++
void ExpressionEvaluator::setAttributeFormModel (
    AttributeFormModel * attributeFormModel
) 
```




<hr>



### function setExpressionText 

_Sets the expression text used when evaluating._ 
```C++
void ExpressionEvaluator::setExpressionText (
    const QString & expressionText
) 
```




<hr>



### function setFeature 

_Sets the feature attached to the expression context._ 
```C++
void ExpressionEvaluator::setFeature (
    const QgsFeature & feature
) 
```




<hr>



### function setLayer 

_Sets the map layer attached to the expression context._ 
```C++
void ExpressionEvaluator::setLayer (
    QgsMapLayer * layer
) 
```




<hr>



### function setMapSettings 

_Sets the map settings attached to the expression context._ 
```C++
void ExpressionEvaluator::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```




<hr>



### function setMode 

_Sets the expression evaluator_ _mode_ _._
```C++
void ExpressionEvaluator::setMode (
    Mode mode
) 
```




<hr>



### function setProject 

_Sets the project attached to the expression context._ 
```C++
void ExpressionEvaluator::setProject (
    QgsProject * project
) 
```




<hr>



### function setVariables 

_Sets the map of variables injected into the expression context when evaluating._ 
```C++
void ExpressionEvaluator::setVariables (
    const QVariantMap & variables
) 
```




<hr>



### function variables [2/2]

_Returns the map of variables injected into the expression context when evaluating._ 
```C++
inline QVariantMap ExpressionEvaluator::variables () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/expressionevaluator.h`

