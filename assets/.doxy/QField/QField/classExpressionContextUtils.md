

# Class ExpressionContextUtils



[**ClassList**](annotated.md) **>** [**ExpressionContextUtils**](classExpressionContextUtils.md)








Inherits the following classes: QObject


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**ExpressionContextUtils**](#function-expressioncontextutils) (QObject \* parent=nullptr) <br> |


## Public Static Functions

| Type | Name |
| ---: | :--- |
|  QgsExpressionContextScope \* | [**cloudUserScope**](#function-clouduserscope) (const [**CloudUserInformation**](structCloudUserInformation.md) & cloudUserInformation) <br> |
|  Q\_INVOKABLE QVariantMap | [**globalVariables**](#function-globalvariables) () <br> |
|  Q\_INVOKABLE QVariantMap | [**layerVariables**](#function-layervariables) (QgsMapLayer \* layer) <br> |
|  QgsExpressionContextScope \* | [**mapToolCaptureScope**](#function-maptoolcapturescope) (const [**SnappingResult**](classSnappingResult.md) & topSnappingResult) <br> |
|  QgsExpressionContextScope \* | [**positionScope**](#function-positionscope) (const [**GnssPositionInformation**](classGnssPositionInformation.md) & positionInformation, bool positionLocked) <br> |
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




### function ExpressionContextUtils 

```C++
explicit ExpressionContextUtils::ExpressionContextUtils (
    QObject * parent=nullptr
) 
```




<hr>
## Public Static Functions Documentation




### function cloudUserScope 

```C++
static QgsExpressionContextScope * ExpressionContextUtils::cloudUserScope (
    const CloudUserInformation & cloudUserInformation
) 
```




<hr>



### function globalVariables 

```C++
static Q_INVOKABLE QVariantMap ExpressionContextUtils::globalVariables () 
```



Returns the global context variables. 

**See also:** [**setGlobalVariable()**](classExpressionContextUtils.md#function-setglobalvariable) 


**See also:** [**setGlobalVariables()**](classExpressionContextUtils.md#function-setglobalvariables) 


**See also:** [**removeGlobalVariable()**](classExpressionContextUtils.md#function-removeglobalvariable) 



        

<hr>



### function layerVariables 

```C++
static Q_INVOKABLE QVariantMap ExpressionContextUtils::layerVariables (
    QgsMapLayer * layer
) 
```



Returns a layer context variables. 

**Parameters:**


* `layer` map layer 



**See also:** [**setLayerVariable()**](classExpressionContextUtils.md#function-setlayervariable) 


**See also:** [**setLayerVariables()**](classExpressionContextUtils.md#function-setlayervariables) 


**See also:** [**removeLayerVariable()**](classExpressionContextUtils.md#function-removelayervariable) 



        

<hr>



### function mapToolCaptureScope 

```C++
static QgsExpressionContextScope * ExpressionContextUtils::mapToolCaptureScope (
    const SnappingResult & topSnappingResult
) 
```




<hr>



### function positionScope 

```C++
static QgsExpressionContextScope * ExpressionContextUtils::positionScope (
    const GnssPositionInformation & positionInformation,
    bool positionLocked
) 
```




<hr>



### function projectVariables 

```C++
static Q_INVOKABLE QVariantMap ExpressionContextUtils::projectVariables (
    QgsProject * project
) 
```



Returns a project context variables. 

**Parameters:**


* `project` project 



**See also:** [**setProjectVariable()**](classExpressionContextUtils.md#function-setprojectvariable) 


**See also:** [**setProjectVariables()**](classExpressionContextUtils.md#function-setprojectvariables) 


**See also:** [**removeProjectVariable()**](classExpressionContextUtils.md#function-removeprojectvariable) 



        

<hr>



### function removeGlobalVariable 

```C++
static Q_INVOKABLE void ExpressionContextUtils::removeGlobalVariable (
    const QString & name
) 
```



Removes a global context variable. 

**Parameters:**


* `name` variable name 



**See also:** [**globalVariables()**](classExpressionContextUtils.md#function-globalvariables) 


**See also:** [**setGlobalVariable()**](classExpressionContextUtils.md#function-setglobalvariable) 


**See also:** [**setGlobalVariables()**](classExpressionContextUtils.md#function-setglobalvariables) 



        

<hr>



### function removeLayerVariable 

```C++
static Q_INVOKABLE void ExpressionContextUtils::removeLayerVariable (
    QgsMapLayer * layer,
    const QString & name
) 
```



Removes a layer context variable. 

**Parameters:**


* `layer` map layer 
* `name` variable name 



**See also:** [**layerVariables()**](classExpressionContextUtils.md#function-layervariables) 


**See also:** [**setLayerVariable()**](classExpressionContextUtils.md#function-setlayervariable) 


**See also:** [**setLayerVariables()**](classExpressionContextUtils.md#function-setlayervariables) 



        

<hr>



### function removeProjectVariable 

```C++
static Q_INVOKABLE void ExpressionContextUtils::removeProjectVariable (
    QgsProject * project,
    const QString & name
) 
```



Removes a project context variable. 

**Parameters:**


* `project` project 
* `name` variable name 



**See also:** [**projectVariables()**](classExpressionContextUtils.md#function-projectvariables) 


**See also:** [**setProjectVariable()**](classExpressionContextUtils.md#function-setprojectvariable) 


**See also:** [**setProjectVariables()**](classExpressionContextUtils.md#function-setprojectvariables) 



        

<hr>



### function setGlobalVariable 

```C++
static Q_INVOKABLE void ExpressionContextUtils::setGlobalVariable (
    const QString & name,
    const QVariant & value
) 
```



Sets a global context variable. 

**Parameters:**


* `name` variable name 
* `value` variable value 



**See also:** [**globalVariables()**](classExpressionContextUtils.md#function-globalvariables) 


**See also:** [**setGlobalVariables()**](classExpressionContextUtils.md#function-setglobalvariables) 


**See also:** [**removeGlobalVariable()**](classExpressionContextUtils.md#function-removeglobalvariable) 



        

<hr>



### function setGlobalVariables 

```C++
static Q_INVOKABLE void ExpressionContextUtils::setGlobalVariables (
    const QVariantMap & variables
) 
```



Sets the global context variables. 

**Parameters:**


* `variables` new set of global variables 



**See also:** [**globalVariables()**](classExpressionContextUtils.md#function-globalvariables) 


**See also:** [**setGlobalVariable()**](classExpressionContextUtils.md#function-setglobalvariable) 


**See also:** [**removeGlobalVariable()**](classExpressionContextUtils.md#function-removeglobalvariable) 



        

<hr>



### function setLayerVariable 

```C++
static Q_INVOKABLE void ExpressionContextUtils::setLayerVariable (
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



**See also:** [**layerVariables()**](classExpressionContextUtils.md#function-layervariables) 


**See also:** [**setLayerVariables()**](classExpressionContextUtils.md#function-setlayervariables) 


**See also:** [**removeLayerVariable()**](classExpressionContextUtils.md#function-removelayervariable) 



        

<hr>



### function setLayerVariables 

```C++
static Q_INVOKABLE void ExpressionContextUtils::setLayerVariables (
    QgsMapLayer * layer,
    const QVariantMap & variables
) 
```



Sets a layer context variables. 

**Parameters:**


* `layer` map layer 
* `variables` new set of layer variables 



**See also:** [**layerVariables()**](classExpressionContextUtils.md#function-layervariables) 


**See also:** [**setLayerVariable()**](classExpressionContextUtils.md#function-setlayervariable) 


**See also:** [**removeLayerVariable()**](classExpressionContextUtils.md#function-removelayervariable) 



        

<hr>



### function setProjectVariable 

```C++
static Q_INVOKABLE void ExpressionContextUtils::setProjectVariable (
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



**See also:** [**projectVariables()**](classExpressionContextUtils.md#function-projectvariables) 


**See also:** [**setProjectVariables()**](classExpressionContextUtils.md#function-setprojectvariables) 


**See also:** [**removeProjectVariable()**](classExpressionContextUtils.md#function-removeprojectvariable) 



        

<hr>



### function setProjectVariables 

```C++
static Q_INVOKABLE void ExpressionContextUtils::setProjectVariables (
    QgsProject * project,
    const QVariantMap & variables
) 
```



Sets a project context variables. 

**Parameters:**


* `project` project 
* `variables` new set of project variables 



**See also:** [**projectVariables()**](classExpressionContextUtils.md#function-projectvariables) 


**See also:** [**setProjectVariable()**](classExpressionContextUtils.md#function-setprojectvariable) 


**See also:** [**removeProjectVariable()**](classExpressionContextUtils.md#function-removeprojectvariable) 



        

<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/expressioncontextutils.h`

