

# Class CogoExecutor



[**ClassList**](annotated.md) **>** [**CogoExecutor**](classCogoExecutor.md)



_A COGO operation exeuctor object allowing for property-driven access to COGO operations._ 

* `#include <cogoexecutor.h>`



Inherits the following classes: QObject


























## Public Properties

| Type | Name |
| ---: | :--- |
| property bool | [**isReady**](classCogoExecutor.md#property-isready-12)  <br> |
| property [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](classCogoExecutor.md#property-mapsettings-12)  <br> |
| property QString | [**name**](classCogoExecutor.md#property-name-12)  <br> |
| property QVariantMap | [**parameterValues**](classCogoExecutor.md#property-parametervalues-12)  <br> |
| property QList&lt; [**CogoParameter**](classCogoParameter.md) &gt; | [**parameters**](classCogoExecutor.md#property-parameters-12)  <br> |
| property [**RubberbandModel**](classRubberbandModel.md) \* | [**rubberbandModel**](classCogoExecutor.md#property-rubberbandmodel-12)  <br> |
| property QList&lt; [**CogoVisualGuide**](classCogoVisualGuide.md) &gt; | [**visualGuides**](classCogoExecutor.md#property-visualguides-12)  <br> |




## Public Signals

| Type | Name |
| ---: | :--- |
| signal void | [**isReadyChanged**](classCogoExecutor.md#signal-isreadychanged)  <br> |
| signal void | [**mapSettingsChanged**](classCogoExecutor.md#signal-mapsettingschanged)  <br> |
| signal void | [**nameChanged**](classCogoExecutor.md#signal-namechanged)  <br> |
| signal void | [**parameterValuesChanged**](classCogoExecutor.md#signal-parametervalueschanged)  <br> |
| signal void | [**parametersChanged**](classCogoExecutor.md#signal-parameterschanged)  <br> |
| signal void | [**rubberbandModelChanged**](classCogoExecutor.md#signal-rubberbandmodelchanged)  <br> |
| signal void | [**visualGuidesChanged**](classCogoExecutor.md#signal-visualguideschanged)  <br> |




## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CogoExecutor**](#function-cogoexecutor) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE bool | [**execute**](#function-execute) () <br> |
|  bool | [**isReady**](#function-isready-22) () const<br> |
|  [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* | [**mapSettings**](#function-mapsettings-22) () const<br> |
|  QString | [**name**](#function-name-22) () const<br> |
|  QVariantMap | [**parameterValues**](#function-parametervalues-22) () const<br> |
|  QList&lt; [**CogoParameter**](classCogoParameter.md) &gt; | [**parameters**](#function-parameters-22) () const<br> |
|  [**RubberbandModel**](classRubberbandModel.md) \* | [**rubberbandModel**](#function-rubberbandmodel-22) () const<br> |
|  void | [**setMapSettings**](#function-setmapsettings) ([**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) <br> |
|  void | [**setName**](#function-setname) (const QString & name) <br> |
|  void | [**setParameterValues**](#function-setparametervalues) (const QVariantMap & parameterValues) <br> |
|  void | [**setRubberbandModel**](#function-setrubberbandmodel) ([**RubberbandModel**](classRubberbandModel.md) \* rubberbandModel) <br> |
|  QList&lt; [**CogoVisualGuide**](classCogoVisualGuide.md) &gt; | [**visualGuides**](#function-visualguides-22) () const<br> |




























## Public Properties Documentation




### property isReady [1/2]

```C++
bool CogoExecutor::isReady;
```



COGO operation paired with the parameters' readiness to be executed. 


        

<hr>



### property mapSettings [1/2]

```C++
QgsQuickMapSettings * CogoExecutor::mapSettings;
```



The map settings used when generating visual guides. 


        

<hr>



### property name [1/2]

```C++
QString CogoExecutor::name;
```



The name of the COGO operation to be executed. 


        

<hr>



### property parameterValues [1/2]

```C++
QVariantMap CogoExecutor::parameterValues;
```



The map of parameter values to be used when executing the COGO operation. 


        

<hr>



### property parameters [1/2]

```C++
QList<CogoParameter> CogoExecutor::parameters;
```



The list of parameters of the COGO operation to be executed. 


        

<hr>



### property rubberbandModel [1/2]

```C++
RubberbandModel * CogoExecutor::rubberbandModel;
```



The rubberband model to be used when executing the COGO operation. 


        

<hr>



### property visualGuides [1/2]

```C++
QList<CogoVisualGuide> CogoExecutor::visualGuides;
```



The visual guides returned by the COGO operation paired with the parameters. 


        

<hr>
## Public Signals Documentation




### signal isReadyChanged 

```C++
void CogoExecutor::isReadyChanged;
```



COGO operation paired with the parameters' readiness to be executed.   


        

<hr>



### signal mapSettingsChanged 

```C++
void CogoExecutor::mapSettingsChanged;
```



The map settings used when generating visual guides.   


        

<hr>



### signal nameChanged 

```C++
void CogoExecutor::nameChanged;
```



The name of the COGO operation to be executed.   


        

<hr>



### signal parameterValuesChanged 

```C++
void CogoExecutor::parameterValuesChanged;
```




<hr>



### signal parametersChanged 

```C++
void CogoExecutor::parametersChanged;
```



The list of parameters of the COGO operation to be executed.   


        

<hr>



### signal rubberbandModelChanged 

```C++
void CogoExecutor::rubberbandModelChanged;
```



The rubberband model to be used when executing the COGO operation.   


        

<hr>



### signal visualGuidesChanged 

```C++
void CogoExecutor::visualGuidesChanged;
```



The visual guides returned by the COGO operation paired with the parameters.   


        

<hr>
## Public Functions Documentation




### function CogoExecutor 

```C++
explicit CogoExecutor::CogoExecutor (
    QObject * parent=nullptr
) 
```




<hr>



### function execute 

```C++
Q_INVOKABLE bool CogoExecutor::execute () 
```



Executes the COGO operation. 


        

<hr>



### function isReady [2/2]

```C++
inline bool CogoExecutor::isReady () const
```



COGO operation paired with the parameters' readiness to be executed.   


        

<hr>



### function mapSettings [2/2]

```C++
inline QgsQuickMapSettings * CogoExecutor::mapSettings () const
```



The map settings used when generating visual guides.   


        

<hr>



### function name [2/2]

```C++
inline QString CogoExecutor::name () const
```



The name of the COGO operation to be executed.   


        

<hr>



### function parameterValues [2/2]

```C++
inline QVariantMap CogoExecutor::parameterValues () const
```



The map of parameter values to be used when executing the COGO operation.   


        

<hr>



### function parameters [2/2]

```C++
inline QList< CogoParameter > CogoExecutor::parameters () const
```



The list of parameters of the COGO operation to be executed.   


        

<hr>



### function rubberbandModel [2/2]

```C++
inline RubberbandModel * CogoExecutor::rubberbandModel () const
```



The rubberband model to be used when executing the COGO operation.   


        

<hr>



### function setMapSettings 

```C++
void CogoExecutor::setMapSettings (
    QgsQuickMapSettings * mapSettings
) 
```



The map settings used when generating visual guides.   


        

<hr>



### function setName 

```C++
void CogoExecutor::setName (
    const QString & name
) 
```



The name of the COGO operation to be executed.   


        

<hr>



### function setParameterValues 

```C++
void CogoExecutor::setParameterValues (
    const QVariantMap & parameterValues
) 
```



The map of parameter values to be used when executing the COGO operation.   


        

<hr>



### function setRubberbandModel 

```C++
void CogoExecutor::setRubberbandModel (
    RubberbandModel * rubberbandModel
) 
```



The rubberband model to be used when executing the COGO operation.   


        

<hr>



### function visualGuides [2/2]

```C++
QList< CogoVisualGuide > CogoExecutor::visualGuides () const
```



The visual guides returned by the COGO operation paired with the parameters.   


        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/cogo/cogoexecutor.h`

