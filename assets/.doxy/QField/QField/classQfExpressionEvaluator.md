

# Class QfExpressionEvaluator



[**ClassList**](annotated.md) **>** [**QfExpressionEvaluator**](classQfExpressionEvaluator.md)



_The_ [_**QfExpressionEvaluator**_](classQfExpressionEvaluator.md) _class enables evaluation of expression strings and expression templates._

* `#include <qfexpressionevaluator.h>`



Inherits the following classes: QObject














## Public Types

| Type | Name |
| ---: | :--- |
| enum  | [**Mode**](#enum-mode)  <br>_Expression evaluator modes._  |












## Public Properties

| Type | Name |
| ---: | :--- |
| property [**QfAppExpressionContextScopesGenerator**](classQfAppExpressionContextScopesGenerator.md) \* | [**appExpressionContextScopesGenerator**](classQfExpressionEvaluator.md#property-appexpressioncontextscopesgenerator-12)  <br> |
| property [**QfAttributeFormModel**](classQfAttributeFormModel.md) \* | [**attributeFormModel**](classQfExpressionEvaluator.md#property-attributeformmodel-12)  <br> |
| property QString | [**expressionText**](classQfExpressionEvaluator.md#property-expressiontext-12)  <br> |
| property QgsFeature | [**feature**](classQfExpressionEvaluator.md#property-feature-12)  <br> |
| property QgsMapLayer \* | [**layer**](classQfExpressionEvaluator.md#property-layer-12)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classQfExpressionEvaluator.md#property-mapsettings-12)  <br> |
| property [**Mode**](classQfExpressionEvaluator.md#enum-mode) | [**mode**](classQfExpressionEvaluator.md#property-mode-12)  <br> |
| property QgsProject \* | [**project**](classQfExpressionEvaluator.md#property-project-12)  <br> |
| property QVariantMap | [**variables**](classQfExpressionEvaluator.md#property-variables-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**appExpressionContextScopesGeneratorChanged**](classQfExpressionEvaluator.md#signal-appexpressioncontextscopesgeneratorchanged)  <br> |
| signal void | [**attributeFormModelChanged**](classQfExpressionEvaluator.md#signal-attributeformmodelchanged)  <br> |
| signal void | [**expressionTextChanged**](classQfExpressionEvaluator.md#signal-expressiontextchanged)  <br> |
| signal void | [**featureChanged**](classQfExpressionEvaluator.md#signal-featurechanged)  <br> |
| signal void | [**layerChanged**](classQfExpressionEvaluator.md#signal-layerchanged)  <br> |
| signal void | [**mapSettingsChanged**](classQfExpressionEvaluator.md#signal-mapsettingschanged)  <br> |
| signal void | [**modeChanged**](classQfExpressionEvaluator.md#signal-modechanged)  <br> |
| signal void | [**projectChanged**](classQfExpressionEvaluator.md#signal-projectchanged)  <br> |
| signal void | [**variablesChanged**](classQfExpressionEvaluator.md#signal-variableschanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfExpressionEvaluator**](#function-qfexpressionevaluator) (QObject \* parent=nullptr) <br> |
|  [**QfAppExpressionContextScopesGenerator**](classQfAppExpressionContextScopesGenerator.md) \* | [**appExpressionContextScopesGenerator**](#function-appexpressioncontextscopesgenerator-22) () const<br>_Returns the application expression context scopes generator object._  |
|  [**QfAttributeFormModel**](classQfAttributeFormModel.md) \* | [**attributeFormModel**](#function-attributeformmodel-22) () const<br>_Returns the attribute form model providing an expression context._  |
|  Q\_INVOKABLE QVariant | [**evaluate**](#function-evaluate-12) () <br>_Evalutes and returns the current expression text property value._  |
|  Q\_INVOKABLE QVariant | [**evaluate**](#function-evaluate-22) (const QString & expressionText) <br>_Evalutes and returns the expression text passed as a parameter._  |
|  QString | [**expressionText**](#function-expressiontext-22) () <br>_Returns the expression text used when evaluating._  |
|  QgsFeature | [**feature**](#function-feature-22) () const<br>_Returns the feature attached to the expression context._  |
|  QgsMapLayer \* | [**layer**](#function-layer-22) () const<br>_Returns the map layer attached to the expression context._  |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () const<br>_Returns the map settings attached to the expression context._  |
|  [**Mode**](classQfExpressionEvaluator.md#enum-mode) | [**mode**](#function-mode-22) () const<br>_Returns the expression evaluator mode._  |
|  QgsProject \* | [**project**](#function-project-22) () const<br>_Returns the project attached to the expression context._  |
|  void | [**setAppExpressionContextScopesGenerator**](#function-setappexpressioncontextscopesgenerator) ([**QfAppExpressionContextScopesGenerator**](classQfAppExpressionContextScopesGenerator.md) \* generator) <br>_Sets the application expression context scopes_ _generator_ _object._ |
|  void | [**setAttributeFormModel**](#function-setattributeformmodel) ([**QfAttributeFormModel**](classQfAttributeFormModel.md) \* attributeFormModel) <br>_Sets the attribute form model providing an expression context._  |
|  void | [**setExpressionText**](#function-setexpressiontext) (const QString & expressionText) <br>_Sets the expression text used when evaluating._  |
|  void | [**setFeature**](#function-setfeature) (const QgsFeature & feature) <br>_Sets the feature attached to the expression context._  |
|  void | [**setLayer**](#function-setlayer) (QgsMapLayer \* layer) <br>_Sets the map layer attached to the expression context._  |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br>_Sets the map settings attached to the expression context._  |
|  void | [**setMode**](#function-setmode) ([**Mode**](classQfExpressionEvaluator.md#enum-mode) mode) <br>_Sets the expression evaluator_ _mode_ _._ |
|  void | [**setProject**](#function-setproject) (QgsProject \* project) <br>_Sets the project attached to the expression context._  |
|  void | [**setVariables**](#function-setvariables) (const QVariantMap & variables) <br>_Sets the map of variables injected into the expression context when evaluating._  |
|  QVariantMap | [**variables**](#function-variables-22) () const<br>_Returns the map of variables injected into the expression context when evaluating._  |




























## Public Types Documentation




### enum Mode 

_Expression evaluator modes._ 
```C++
enum QfExpressionEvaluator::Mode {
    ExpressionMode,
    ExpressionTemplateMode
};
```




<hr>
## Public Properties Documentation




### property appExpressionContextScopesGenerator [1/2]

```C++
QfAppExpressionContextScopesGenerator * QfExpressionEvaluator::appExpressionContextScopesGenerator;
```




<hr>



### property attributeFormModel [1/2]

```C++
QfAttributeFormModel * QfExpressionEvaluator::attributeFormModel;
```




<hr>



### property expressionText [1/2]

```C++
QString QfExpressionEvaluator::expressionText;
```




<hr>



### property feature [1/2]

```C++
QgsFeature QfExpressionEvaluator::feature;
```




<hr>



### property layer [1/2]

```C++
QgsMapLayer * QfExpressionEvaluator::layer;
```




<hr>



### property mapSettings [1/2]

```C++
QgsQuickMapSettings * QfExpressionEvaluator::mapSettings;
```




<hr>



### property mode [1/2]

```C++
Mode QfExpressionEvaluator::mode;
```




<hr>



### property project [1/2]

```C++
QgsProject * QfExpressionEvaluator::project;
```




<hr>



### property variables [1/2]

```C++
QVariantMap QfExpressionEvaluator::variables;
```




<hr>
## Public Signals Documentation




### signal appExpressionContextScopesGeneratorChanged 

```C++
void QfExpressionEvaluator::appExpressionContextScopesGeneratorChanged;
```




<hr>



### signal attributeFormModelChanged 

```C++
void QfExpressionEvaluator::attributeFormModelChanged;
```




<hr>



### signal expressionTextChanged 

```C++
void QfExpressionEvaluator::expressionTextChanged;
```




<hr>



### signal featureChanged 

```C++
void QfExpressionEvaluator::featureChanged;
```




<hr>



### signal layerChanged 

```C++
void QfExpressionEvaluator::layerChanged;
```




<hr>



### signal mapSettingsChanged 

```C++
void QfExpressionEvaluator::mapSettingsChanged;
```




<hr>



### signal modeChanged 

```C++
void QfExpressionEvaluator::modeChanged;
```




<hr>



### signal projectChanged 

```C++
void QfExpressionEvaluator::projectChanged;
```




<hr>



### signal variablesChanged 

```C++
void QfExpressionEvaluator::variablesChanged;
```




<hr>
## Public Functions Documentation




### function QfExpressionEvaluator 

```C++
explicit QfExpressionEvaluator::QfExpressionEvaluator (
    QObject * parent=nullptr
) 
```




<hr>



### function appExpressionContextScopesGenerator [2/2]

_Returns the application expression context scopes generator object._ 
```C++
QfAppExpressionContextScopesGenerator * QfExpressionEvaluator::appExpressionContextScopesGenerator () const
```




<hr>



### function attributeFormModel [2/2]

_Returns the attribute form model providing an expression context._ 
```C++
QfAttributeFormModel * QfExpressionEvaluator::attributeFormModel () const
```




<hr>



### function evaluate [1/2]

_Evalutes and returns the current expression text property value._ 
```C++
Q_INVOKABLE QVariant QfExpressionEvaluator::evaluate () 
```




<hr>



### function evaluate [2/2]

_Evalutes and returns the expression text passed as a parameter._ 
```C++
Q_INVOKABLE QVariant QfExpressionEvaluator::evaluate (
    const QString & expressionText
) 
```




<hr>



### function expressionText [2/2]

_Returns the expression text used when evaluating._ 
```C++
inline QString QfExpressionEvaluator::expressionText () 
```




<hr>



### function feature [2/2]

_Returns the feature attached to the expression context._ 
```C++
inline QgsFeature QfExpressionEvaluator::feature () const
```




<hr>



### function layer [2/2]

_Returns the map layer attached to the expression context._ 
```C++
inline QgsMapLayer * QfExpressionEvaluator::layer () const
```




<hr>



### function mapSettings [2/2]

_Returns the map settings attached to the expression context._ 
```C++
inline QgsQuickMapSettings * QfExpressionEvaluator::mapSettings () const
```




<hr>



### function mode [2/2]

_Returns the expression evaluator mode._ 
```C++
inline Mode QfExpressionEvaluator::mode () const
```




<hr>



### function project [2/2]

_Returns the project attached to the expression context._ 
```C++
inline QgsProject * QfExpressionEvaluator::project () const
```




<hr>



### function setAppExpressionContextScopesGenerator 

_Sets the application expression context scopes_ _generator_ _object._
```C++
void QfExpressionEvaluator::setAppExpressionContextScopesGenerator (
    QfAppExpressionContextScopesGenerator * generator
) 
```




<hr>



### function setAttributeFormModel 

_Sets the attribute form model providing an expression context._ 
```C++
void QfExpressionEvaluator::setAttributeFormModel (
    QfAttributeFormModel * attributeFormModel
) 
```




<hr>



### function setExpressionText 

_Sets the expression text used when evaluating._ 
```C++
void QfExpressionEvaluator::setExpressionText (
    const QString & expressionText
) 
```




<hr>



### function setFeature 

_Sets the feature attached to the expression context._ 
```C++
void QfExpressionEvaluator::setFeature (
    const QgsFeature & feature
) 
```




<hr>



### function setLayer 

_Sets the map layer attached to the expression context._ 
```C++
void QfExpressionEvaluator::setLayer (
    QgsMapLayer * layer
) 
```




<hr>



### function setMapSettings 

_Sets the map settings attached to the expression context._ 
```C++
void QfExpressionEvaluator::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```




<hr>



### function setMode 

_Sets the expression evaluator_ _mode_ _._
```C++
void QfExpressionEvaluator::setMode (
    Mode mode
) 
```




<hr>



### function setProject 

_Sets the project attached to the expression context._ 
```C++
void QfExpressionEvaluator::setProject (
    QgsProject * project
) 
```




<hr>



### function setVariables 

_Sets the map of variables injected into the expression context when evaluating._ 
```C++
void QfExpressionEvaluator::setVariables (
    const QVariantMap & variables
) 
```




<hr>



### function variables [2/2]

_Returns the map of variables injected into the expression context when evaluating._ 
```C++
inline QVariantMap QfExpressionEvaluator::variables () const
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/gui/qfexpressionevaluator.h`

