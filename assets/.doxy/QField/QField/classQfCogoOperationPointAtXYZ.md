

# Class QfCogoOperationPointAtXYZ



[**ClassList**](annotated.md) **>** [**QfCogoOperationPointAtXYZ**](classQfCogoOperationPointAtXYZ.md)



_A COGO operation to generate a point at a given XY location._ 

* `#include <qfcogooperation.h>`



Inherits the following classes: [QfCogoOperation](classQfCogoOperation.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfCogoOperationPointAtXYZ**](#function-qfcogooperationpointatxyz) () <br> |
| virtual bool | [**checkReadiness**](#function-checkreadiness) (const QVariantMap & parameters, Qgis::WkbType wkbType=Qgis::WkbType::Unknown) override const<br> |
| virtual QString | [**displayName**](#function-displayname) () override const<br> |
| virtual bool | [**execute**](#function-execute) ([**QfRubberbandModel**](classQfRubberbandModel.md) \* rubberbandModel, const QVariantMap & parameters, Qgis::WkbType wkbType) override const<br> |
| virtual QString | [**icon**](#function-icon) () override const<br> |
| virtual QString | [**name**](#function-name) () override const<br> |
| virtual QList&lt; [**QfCogoParameter**](classQfCogoParameter.md) &gt; | [**parameters**](#function-parameters) (Qgis::WkbType wkbType) override const<br> |
| virtual QList&lt; [**QfCogoVisualGuide**](classQfCogoVisualGuide.md) &gt; | [**visualGuides**](#function-visualguides) (const QVariantMap & parameters, [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) override const<br> |
|   | [**~QfCogoOperationPointAtXYZ**](#function-qfcogooperationpointatxyz) () = default<br> |


## Public Functions inherited from QfCogoOperation

See [QfCogoOperation](classQfCogoOperation.md)

| Type | Name |
| ---: | :--- |
|   | [**QfCogoOperation**](classQfCogoOperation.md#function-qfcogooperation) () <br> |
| virtual bool | [**checkReadiness**](classQfCogoOperation.md#function-checkreadiness) (const QVariantMap & parameters, Qgis::WkbType wkbType=Qgis::WkbType::Unknown) const<br> |
| virtual QString | [**displayName**](classQfCogoOperation.md#function-displayname) () const<br> |
| virtual bool | [**execute**](classQfCogoOperation.md#function-execute) ([**QfRubberbandModel**](classQfRubberbandModel.md) \* rubberbandModel, const QVariantMap & parameters, Qgis::WkbType wkbType=Qgis::WkbType::Unknown) const<br> |
| virtual QString | [**icon**](classQfCogoOperation.md#function-icon) () const<br> |
| virtual QString | [**name**](classQfCogoOperation.md#function-name) () const<br> |
| virtual QList&lt; [**QfCogoParameter**](classQfCogoParameter.md) &gt; | [**parameters**](classQfCogoOperation.md#function-parameters) (Qgis::WkbType wkbType=Qgis::WkbType::Unknown) const<br> |
| virtual QList&lt; [**QfCogoVisualGuide**](classQfCogoVisualGuide.md) &gt; | [**visualGuides**](classQfCogoOperation.md#function-visualguides) (const QVariantMap & parameters, [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) const<br> |
| virtual  | [**~QfCogoOperation**](classQfCogoOperation.md#function-qfcogooperation) () = default<br> |






















































## Public Functions Documentation




### function QfCogoOperationPointAtXYZ 

```C++
inline QfCogoOperationPointAtXYZ::QfCogoOperationPointAtXYZ () 
```




<hr>



### function checkReadiness 

```C++
virtual bool QfCogoOperationPointAtXYZ::checkReadiness (
    const QVariantMap & parameters,
    Qgis::WkbType wkbType=Qgis::WkbType::Unknown
) override const
```



Returns TRUE is the provided parameters allow for the operation to be executed. 

**Parameters:**


* `parameters` the parameters to be validated for readiness 
* `wkbType` An optional WKB type to reflect ability of the geometry being digitized 




        
Implements [*QfCogoOperation::checkReadiness*](classQfCogoOperation.md#function-checkreadiness)


<hr>



### function displayName 

```C++
inline virtual QString QfCogoOperationPointAtXYZ::displayName () override const
```



Returns a translatable display name. 


        
Implements [*QfCogoOperation::displayName*](classQfCogoOperation.md#function-displayname)


<hr>



### function execute 

```C++
virtual bool QfCogoOperationPointAtXYZ::execute (
    QfRubberbandModel * rubberbandModel,
    const QVariantMap & parameters,
    Qgis::WkbType wkbType
) override const
```



Executes the operation and add generated vertex or vertices into the rubberband model. 

**Parameters:**


* `rubberbandModel` the rubberband model within which one or more vertices will be added 
* `parameters` the parameters used to execute the operation 
* `wkbType` An optional WKB type to reflect ability of the geometry being digitized 




        
Implements [*QfCogoOperation::execute*](classQfCogoOperation.md#function-execute)


<hr>



### function icon 

```C++
inline virtual QString QfCogoOperationPointAtXYZ::icon () override const
```



Returns the icon name. 


        
Implements [*QfCogoOperation::icon*](classQfCogoOperation.md#function-icon)


<hr>



### function name 

```C++
inline virtual QString QfCogoOperationPointAtXYZ::name () override const
```



Returns the name string identifier. 


        
Implements [*QfCogoOperation::name*](classQfCogoOperation.md#function-name)


<hr>



### function parameters 

```C++
virtual QList< QfCogoParameter > QfCogoOperationPointAtXYZ::parameters (
    Qgis::WkbType wkbType
) override const
```



Returns the list of available parameters to configure the operation. 

**Parameters:**


* `wkbType` An optional WKB type to reflect ability of the geometry being digitized 




        
Implements [*QfCogoOperation::parameters*](classQfCogoOperation.md#function-parameters)


<hr>



### function visualGuides 

```C++
virtual QList< QfCogoVisualGuide > QfCogoOperationPointAtXYZ::visualGuides (
    const QVariantMap & parameters,
    QgsQuickMapSettings * mapSettings
) override const
```



Returns a list of visual guides based on provided parameters. 

**Parameters:**


* `parameters` the parameters used to generate the visual guides 
* `mapSettings` the map settings object used to georeference the visual guides 




        
Implements [*QfCogoOperation::visualGuides*](classQfCogoOperation.md#function-visualguides)


<hr>



### function ~QfCogoOperationPointAtXYZ 

```C++
QfCogoOperationPointAtXYZ::~QfCogoOperationPointAtXYZ () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/cogo/qfcogooperation.h`

