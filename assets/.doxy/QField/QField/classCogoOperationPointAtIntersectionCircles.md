

# Class CogoOperationPointAtIntersectionCircles



[**ClassList**](annotated.md) **>** [**CogoOperationPointAtIntersectionCircles**](classCogoOperationPointAtIntersectionCircles.md)



_A COGO operation to generate a point at the intersection of two defined circles._ 

* `#include <cogooperation.h>`



Inherits the following classes: [CogoOperation](classCogoOperation.md)






















































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CogoOperationPointAtIntersectionCircles**](#function-cogooperationpointatintersectioncircles) () <br> |
| virtual bool | [**checkReadiness**](#function-checkreadiness) (const QVariantMap & parameters, Qgis::WkbType wkbType=Qgis::WkbType::Unknown) override const<br> |
| virtual QString | [**displayName**](#function-displayname) () override const<br> |
| virtual bool | [**execute**](#function-execute) ([**RubberbandModel**](classRubberbandModel.md) \* rubberbandModel, const QVariantMap & parameters, Qgis::WkbType wkbType) override const<br> |
| virtual QString | [**icon**](#function-icon) () override const<br> |
| virtual QString | [**name**](#function-name) () override const<br> |
| virtual QList&lt; [**CogoParameter**](classCogoParameter.md) &gt; | [**parameters**](#function-parameters) (Qgis::WkbType wkbType) override const<br> |
| virtual QList&lt; [**CogoVisualGuide**](classCogoVisualGuide.md) &gt; | [**visualGuides**](#function-visualguides) (const QVariantMap & parameters, [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) override const<br> |
|   | [**~CogoOperationPointAtIntersectionCircles**](#function-cogooperationpointatintersectioncircles) () = default<br> |


## Public Functions inherited from CogoOperation

See [CogoOperation](classCogoOperation.md)

| Type | Name |
| ---: | :--- |
|   | [**CogoOperation**](classCogoOperation.md#function-cogooperation) () <br> |
| virtual bool | [**checkReadiness**](classCogoOperation.md#function-checkreadiness) (const QVariantMap & parameters, Qgis::WkbType wkbType=Qgis::WkbType::Unknown) const<br> |
| virtual QString | [**displayName**](classCogoOperation.md#function-displayname) () const<br> |
| virtual bool | [**execute**](classCogoOperation.md#function-execute) ([**RubberbandModel**](classRubberbandModel.md) \* rubberbandModel, const QVariantMap & parameters, Qgis::WkbType wkbType=Qgis::WkbType::Unknown) const<br> |
| virtual QString | [**icon**](classCogoOperation.md#function-icon) () const<br> |
| virtual QString | [**name**](classCogoOperation.md#function-name) () const<br> |
| virtual QList&lt; [**CogoParameter**](classCogoParameter.md) &gt; | [**parameters**](classCogoOperation.md#function-parameters) (Qgis::WkbType wkbType=Qgis::WkbType::Unknown) const<br> |
| virtual QList&lt; [**CogoVisualGuide**](classCogoVisualGuide.md) &gt; | [**visualGuides**](classCogoOperation.md#function-visualguides) (const QVariantMap & parameters, [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) const<br> |
| virtual  | [**~CogoOperation**](classCogoOperation.md#function-cogooperation) () = default<br> |






















































## Public Functions Documentation




### function CogoOperationPointAtIntersectionCircles 

```C++
inline CogoOperationPointAtIntersectionCircles::CogoOperationPointAtIntersectionCircles () 
```




<hr>



### function checkReadiness 

```C++
virtual bool CogoOperationPointAtIntersectionCircles::checkReadiness (
    const QVariantMap & parameters,
    Qgis::WkbType wkbType=Qgis::WkbType::Unknown
) override const
```



Returns TRUE is the provided parameters allow for the operation to be executed. 

**Parameters:**


* `parameters` the parameters to be validated for readiness 
* `wkbType` An optional WKB type to reflect ability of the geometry being digitized 




        
Implements [*CogoOperation::checkReadiness*](classCogoOperation.md#function-checkreadiness)


<hr>



### function displayName 

```C++
inline virtual QString CogoOperationPointAtIntersectionCircles::displayName () override const
```



Returns a translatable display name. 


        
Implements [*CogoOperation::displayName*](classCogoOperation.md#function-displayname)


<hr>



### function execute 

```C++
virtual bool CogoOperationPointAtIntersectionCircles::execute (
    RubberbandModel * rubberbandModel,
    const QVariantMap & parameters,
    Qgis::WkbType wkbType
) override const
```



Executes the operation and add generated vertex or vertices into the rubberband model. 

**Parameters:**


* `rubberbandModel` the rubberband model within which one or more vertices will be added 
* `parameters` the parameters used to execute the operation 
* `wkbType` An optional WKB type to reflect ability of the geometry being digitized 




        
Implements [*CogoOperation::execute*](classCogoOperation.md#function-execute)


<hr>



### function icon 

```C++
inline virtual QString CogoOperationPointAtIntersectionCircles::icon () override const
```



Returns the icon name. 


        
Implements [*CogoOperation::icon*](classCogoOperation.md#function-icon)


<hr>



### function name 

```C++
inline virtual QString CogoOperationPointAtIntersectionCircles::name () override const
```



Returns the name string identifier. 


        
Implements [*CogoOperation::name*](classCogoOperation.md#function-name)


<hr>



### function parameters 

```C++
virtual QList< CogoParameter > CogoOperationPointAtIntersectionCircles::parameters (
    Qgis::WkbType wkbType
) override const
```



Returns the list of available parameters to configure the operation. 

**Parameters:**


* `wkbType` An optional WKB type to reflect ability of the geometry being digitized 




        
Implements [*CogoOperation::parameters*](classCogoOperation.md#function-parameters)


<hr>



### function visualGuides 

```C++
virtual QList< CogoVisualGuide > CogoOperationPointAtIntersectionCircles::visualGuides (
    const QVariantMap & parameters,
    QgsQuickMapSettings * mapSettings
) override const
```



Returns a list of visual guides based on provided parameters. 

**Parameters:**


* `parameters` the parameters used to generate the visual guides 
* `mapSettings` the map settings object used to georeference the visual guides 




        
Implements [*CogoOperation::visualGuides*](classCogoOperation.md#function-visualguides)


<hr>



### function ~CogoOperationPointAtIntersectionCircles 

```C++
CogoOperationPointAtIntersectionCircles::~CogoOperationPointAtIntersectionCircles () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/cogo/cogooperation.h`

