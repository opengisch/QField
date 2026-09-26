

# Class QfCogoOperationPointAtIntersectionCircles



[**ClassList**](annotated.md) **>** [**QfCogoOperationPointAtIntersectionCircles**](classQfCogoOperationPointAtIntersectionCircles.md)



_A COGO operation to generate a point at the intersection of two defined circles._ 

* `#include <qfcogooperation.h>`



Inherits the following classes: [QfCogoOperation](classQfCogoOperation.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfCogoOperationPointAtIntersectionCircles**](#function-qfcogooperationpointatintersectioncircles) () <br> |
| virtual bool | [**checkReadiness**](#function-checkreadiness) (const QVariantMap & parameters, Qgis::WkbType wkbType=Qgis::WkbType::Unknown) override const<br> |
| virtual QString | [**displayName**](#function-displayname) () override const<br> |
| virtual bool | [**execute**](#function-execute) ([**QfRubberbandModel**](classQfRubberbandModel.md) \* rubberbandModel, const QVariantMap & parameters, Qgis::WkbType wkbType) override const<br> |
| virtual QString | [**icon**](#function-icon) () override const<br> |
| virtual QString | [**name**](#function-name) () override const<br> |
| virtual QList&lt; [**QfCogoParameter**](classQfCogoParameter.md) &gt; | [**parameters**](#function-parameters) (Qgis::WkbType wkbType) override const<br> |
| virtual QList&lt; [**QfCogoVisualGuide**](classQfCogoVisualGuide.md) &gt; | [**visualGuides**](#function-visualguides) (const QVariantMap & parameters, [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) override const<br> |
|   | [**~QfCogoOperationPointAtIntersectionCircles**](#function-qfcogooperationpointatintersectioncircles) () = default<br> |


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




### function QfCogoOperationPointAtIntersectionCircles 

```C++
inline QfCogoOperationPointAtIntersectionCircles::QfCogoOperationPointAtIntersectionCircles () 
```




<hr>



### function checkReadiness 

```C++
virtual bool QfCogoOperationPointAtIntersectionCircles::checkReadiness (
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
inline virtual QString QfCogoOperationPointAtIntersectionCircles::displayName () override const
```



Returns a translatable display name. 


        
Implements [*QfCogoOperation::displayName*](classQfCogoOperation.md#function-displayname)


<hr>



### function execute 

```C++
virtual bool QfCogoOperationPointAtIntersectionCircles::execute (
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
inline virtual QString QfCogoOperationPointAtIntersectionCircles::icon () override const
```



Returns the icon name. 


        
Implements [*QfCogoOperation::icon*](classQfCogoOperation.md#function-icon)


<hr>



### function name 

```C++
inline virtual QString QfCogoOperationPointAtIntersectionCircles::name () override const
```



Returns the name string identifier. 


        
Implements [*QfCogoOperation::name*](classQfCogoOperation.md#function-name)


<hr>



### function parameters 

```C++
virtual QList< QfCogoParameter > QfCogoOperationPointAtIntersectionCircles::parameters (
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
virtual QList< QfCogoVisualGuide > QfCogoOperationPointAtIntersectionCircles::visualGuides (
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



### function ~QfCogoOperationPointAtIntersectionCircles 

```C++
QfCogoOperationPointAtIntersectionCircles::~QfCogoOperationPointAtIntersectionCircles () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/cogo/qfcogooperation.h`

