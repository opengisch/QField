

# Class QfExpressionContextUtils



[**ClassList**](annotated.md) **>** [**QfExpressionContextUtils**](classQfExpressionContextUtils.md)








Inherits the following classes: QObject


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfExpressionContextUtils**](#function-qfexpressioncontextutils) (QObject \* parent=nullptr) <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  QgsExpressionContextScope \* | [**cloudUserScope**](#function-clouduserscope) (const [**QfCloudUserInformation**](structQfCloudUserInformation.md) & cloudUserInformation) <br> |
|  Q\_INVOKABLE QVariantMap | [**globalVariables**](#function-globalvariables) () <br> |
|  Q\_INVOKABLE QVariantMap | [**layerVariables**](#function-layervariables) (QgsMapLayer \* layer) <br> |
|  QgsExpressionContextScope \* | [**mapToolCaptureScope**](#function-maptoolcapturescope) (const [**QfSnappingResult**](classQfSnappingResult.md) & topSnappingResult) <br> |
|  QgsExpressionContextScope \* | [**positionScope**](#function-positionscope) (const [**QfGnssPositionInformation**](classQfGnssPositionInformation.md) & positionInformation, bool positionLocked) <br> |
|  Q\_INVOKABLE QVariantMap | [**projectVariables**](#function-projectvariables) (QgsProject \* project) <br> |
|  Q\_INVOKABLE void | [**removeGlobalVariable**](#function-removeglobalvariable) (const QString & name) <br> |
|  Q\_INVOKABLE void | [**removeLayerVariable**](#function-removelayervariable) (QgsMapLayer \* layer, const QString & name) <br> |
|  Q\_INVOKABLE void | [**removeProjectVariable**](#function-removeprojectvariable) (QgsProject \* project, const QString & name) <br> |
|  Q\_INVOKABLE void | [**setGlobalVariable**](#function-setglobalvariable) (const QString & name, const QVariant & value) <br> |
|  Q\_INVOKABLE void | [**setGlobalVariables**](#function-setglobalvariables) (const QVariantMap & variables) <br> |
|  Q\_INVOKABLE void | [**setLayerVariable**](#function-setlayervariable) (QgsMapLayer \* layer, const QString & name, const QVariant & value) <br> |
|  Q\_INVOKABLE void | [**setLayerVariables**](#function-setlayervariables) (QgsMapLayer \* layer, const QVariantMap & variables) <br> |
|  Q\_INVOKABLE void | [**setProjectVariable**](#function-setprojectvariable) (QgsProject \* project, const QString & name, const QVariant & value) <br> |
|  Q\_INVOKABLE void | [**setProjectVariables**](#function-setprojectvariables) (QgsProject \* project, const QVariantMap & variables) <br> |


























## Public Functions Documentation




### function QfExpressionContextUtils 

```C++
explicit QfExpressionContextUtils::QfExpressionContextUtils (
    QObject * parent=nullptr
) 
```




<hr>
## Public Static Functions Documentation




### function cloudUserScope 

```C++
static QgsExpressionContextScope * QfExpressionContextUtils::cloudUserScope (
    const QfCloudUserInformation & cloudUserInformation
) 
```




<hr>



### function globalVariables 

```C++
static Q_INVOKABLE QVariantMap QfExpressionContextUtils::globalVariables () 
```



Returns the global context variables. 

**See also:** [**setGlobalVariable()**](classQfExpressionContextUtils.md#function-setglobalvariable) 


**See also:** [**setGlobalVariables()**](classQfExpressionContextUtils.md#function-setglobalvariables) 


**See also:** [**removeGlobalVariable()**](classQfExpressionContextUtils.md#function-removeglobalvariable) 



        

<hr>



### function layerVariables 

```C++
static Q_INVOKABLE QVariantMap QfExpressionContextUtils::layerVariables (
    QgsMapLayer * layer
) 
```



Returns a layer context variables. 

**Parameters:**


* `layer` map layer 



**See also:** [**setLayerVariable()**](classQfExpressionContextUtils.md#function-setlayervariable) 


**See also:** [**setLayerVariables()**](classQfExpressionContextUtils.md#function-setlayervariables) 


**See also:** [**removeLayerVariable()**](classQfExpressionContextUtils.md#function-removelayervariable) 



        

<hr>



### function mapToolCaptureScope 

```C++
static QgsExpressionContextScope * QfExpressionContextUtils::mapToolCaptureScope (
    const QfSnappingResult & topSnappingResult
) 
```




<hr>



### function positionScope 

```C++
static QgsExpressionContextScope * QfExpressionContextUtils::positionScope (
    const QfGnssPositionInformation & positionInformation,
    bool positionLocked
) 
```




<hr>



### function projectVariables 

```C++
static Q_INVOKABLE QVariantMap QfExpressionContextUtils::projectVariables (
    QgsProject * project
) 
```



Returns a project context variables. 

**Parameters:**


* `project` project 



**See also:** [**setProjectVariable()**](classQfExpressionContextUtils.md#function-setprojectvariable) 


**See also:** [**setProjectVariables()**](classQfExpressionContextUtils.md#function-setprojectvariables) 


**See also:** [**removeProjectVariable()**](classQfExpressionContextUtils.md#function-removeprojectvariable) 



        

<hr>



### function removeGlobalVariable 

```C++
static Q_INVOKABLE void QfExpressionContextUtils::removeGlobalVariable (
    const QString & name
) 
```



Removes a global context variable. 

**Parameters:**


* `name` variable name 



**See also:** [**globalVariables()**](classQfExpressionContextUtils.md#function-globalvariables) 


**See also:** [**setGlobalVariable()**](classQfExpressionContextUtils.md#function-setglobalvariable) 


**See also:** [**setGlobalVariables()**](classQfExpressionContextUtils.md#function-setglobalvariables) 



        

<hr>



### function removeLayerVariable 

```C++
static Q_INVOKABLE void QfExpressionContextUtils::removeLayerVariable (
    QgsMapLayer * layer,
    const QString & name
) 
```



Removes a layer context variable. 

**Parameters:**


* `layer` map layer 
* `name` variable name 



**See also:** [**layerVariables()**](classQfExpressionContextUtils.md#function-layervariables) 


**See also:** [**setLayerVariable()**](classQfExpressionContextUtils.md#function-setlayervariable) 


**See also:** [**setLayerVariables()**](classQfExpressionContextUtils.md#function-setlayervariables) 



        

<hr>



### function removeProjectVariable 

```C++
static Q_INVOKABLE void QfExpressionContextUtils::removeProjectVariable (
    QgsProject * project,
    const QString & name
) 
```



Removes a project context variable. 

**Parameters:**


* `project` project 
* `name` variable name 



**See also:** [**projectVariables()**](classQfExpressionContextUtils.md#function-projectvariables) 


**See also:** [**setProjectVariable()**](classQfExpressionContextUtils.md#function-setprojectvariable) 


**See also:** [**setProjectVariables()**](classQfExpressionContextUtils.md#function-setprojectvariables) 



        

<hr>



### function setGlobalVariable 

```C++
static Q_INVOKABLE void QfExpressionContextUtils::setGlobalVariable (
    const QString & name,
    const QVariant & value
) 
```



Sets a global context variable. 

**Parameters:**


* `name` variable name 
* `value` variable value 



**See also:** [**globalVariables()**](classQfExpressionContextUtils.md#function-globalvariables) 


**See also:** [**setGlobalVariables()**](classQfExpressionContextUtils.md#function-setglobalvariables) 


**See also:** [**removeGlobalVariable()**](classQfExpressionContextUtils.md#function-removeglobalvariable) 



        

<hr>



### function setGlobalVariables 

```C++
static Q_INVOKABLE void QfExpressionContextUtils::setGlobalVariables (
    const QVariantMap & variables
) 
```



Sets the global context variables. 

**Parameters:**


* `variables` new set of global variables 



**See also:** [**globalVariables()**](classQfExpressionContextUtils.md#function-globalvariables) 


**See also:** [**setGlobalVariable()**](classQfExpressionContextUtils.md#function-setglobalvariable) 


**See also:** [**removeGlobalVariable()**](classQfExpressionContextUtils.md#function-removeglobalvariable) 



        

<hr>



### function setLayerVariable 

```C++
static Q_INVOKABLE void QfExpressionContextUtils::setLayerVariable (
    QgsMapLayer * layer,
    const QString & name,
    const QVariant & value
) 
```



Sets a layer context variable. 

**Parameters:**


* `layer` map layer 
* `name` variable name 
* `value` variable value 



**See also:** [**layerVariables()**](classQfExpressionContextUtils.md#function-layervariables) 


**See also:** [**setLayerVariables()**](classQfExpressionContextUtils.md#function-setlayervariables) 


**See also:** [**removeLayerVariable()**](classQfExpressionContextUtils.md#function-removelayervariable) 



        

<hr>



### function setLayerVariables 

```C++
static Q_INVOKABLE void QfExpressionContextUtils::setLayerVariables (
    QgsMapLayer * layer,
    const QVariantMap & variables
) 
```



Sets a layer context variables. 

**Parameters:**


* `layer` map layer 
* `variables` new set of layer variables 



**See also:** [**layerVariables()**](classQfExpressionContextUtils.md#function-layervariables) 


**See also:** [**setLayerVariable()**](classQfExpressionContextUtils.md#function-setlayervariable) 


**See also:** [**removeLayerVariable()**](classQfExpressionContextUtils.md#function-removelayervariable) 



        

<hr>



### function setProjectVariable 

```C++
static Q_INVOKABLE void QfExpressionContextUtils::setProjectVariable (
    QgsProject * project,
    const QString & name,
    const QVariant & value
) 
```



Sets a project context variable. 

**Parameters:**


* `project` project 
* `name` variable name 
* `value` variable value 



**See also:** [**projectVariables()**](classQfExpressionContextUtils.md#function-projectvariables) 


**See also:** [**setProjectVariables()**](classQfExpressionContextUtils.md#function-setprojectvariables) 


**See also:** [**removeProjectVariable()**](classQfExpressionContextUtils.md#function-removeprojectvariable) 



        

<hr>



### function setProjectVariables 

```C++
static Q_INVOKABLE void QfExpressionContextUtils::setProjectVariables (
    QgsProject * project,
    const QVariantMap & variables
) 
```



Sets a project context variables. 

**Parameters:**


* `project` project 
* `variables` new set of project variables 



**See also:** [**projectVariables()**](classQfExpressionContextUtils.md#function-projectvariables) 


**See also:** [**setProjectVariable()**](classQfExpressionContextUtils.md#function-setprojectvariable) 


**See also:** [**removeProjectVariable()**](classQfExpressionContextUtils.md#function-removeprojectvariable) 



        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/qfexpressioncontextutils.h`

