

# Class CogoOperation



[**ClassList**](annotated.md) **>** [**CogoOperation**](classCogoOperation.md)



_A COGO operation._ 

* `#include <cogooperation.h>`





Inherited by the following classes: [CogoOperationPointAtDistanceAngle](classCogoOperationPointAtDistanceAngle.md),  [CogoOperationPointAtIntersectionCircles](classCogoOperationPointAtIntersectionCircles.md),  [CogoOperationPointAtXYZ](classCogoOperationPointAtXYZ.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**CogoOperation**](#function-cogooperation) () <br> |
| virtual bool | [**checkReadiness**](#function-checkreadiness) (const QVariantMap & parameters, Qgis::WkbType wkbType=Qgis::WkbType::Unknown) const<br> |
| virtual QString | [**displayName**](#function-displayname) () const<br> |
| virtual bool | [**execute**](#function-execute) ([**RubberbandModel**](classRubberbandModel.md) \* rubberbandModel, const QVariantMap & parameters, Qgis::WkbType wkbType=Qgis::WkbType::Unknown) const<br> |
| virtual QString | [**icon**](#function-icon) () const<br> |
| virtual QString | [**name**](#function-name) () const<br> |
| virtual QList&lt; [**CogoParameter**](classCogoParameter.md) &gt; | [**parameters**](#function-parameters) (Qgis::WkbType wkbType=Qgis::WkbType::Unknown) const<br> |
| virtual QList&lt; [**CogoVisualGuide**](classCogoVisualGuide.md) &gt; | [**visualGuides**](#function-visualguides) (const QVariantMap & parameters, [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) const<br> |
| virtual  | [**~CogoOperation**](#function-cogooperation) () = default<br> |




























## Public Functions Documentation




### function CogoOperation 

```C++
inline CogoOperation::CogoOperation () 
```



The COGO operation constructor. 


        

<hr>



### function checkReadiness 

```C++
inline virtual bool CogoOperation::checkReadiness (
    const QVariantMap & parameters,
    Qgis::WkbType wkbType=Qgis::WkbType::Unknown
) const
```



Returns TRUE is the provided parameters allow for the operation to be executed. 

**Parameters:**


* `parameters` the parameters to be validated for readiness 
* `wkbType` An optional WKB type to reflect ability of the geometry being digitized 




        

<hr>



### function displayName 

```C++
inline virtual QString CogoOperation::displayName () const
```



Returns a translatable display name. 


        

<hr>



### function execute 

```C++
inline virtual bool CogoOperation::execute (
    RubberbandModel * rubberbandModel,
    const QVariantMap & parameters,
    Qgis::WkbType wkbType=Qgis::WkbType::Unknown
) const
```



Executes the operation and add generated vertex or vertices into the rubberband model. 

**Parameters:**


* `rubberbandModel` the rubberband model within which one or more vertices will be added 
* `parameters` the parameters used to execute the operation 
* `wkbType` An optional WKB type to reflect ability of the geometry being digitized 




        

<hr>



### function icon 

```C++
inline virtual QString CogoOperation::icon () const
```



Returns the icon name. 


        

<hr>



### function name 

```C++
inline virtual QString CogoOperation::name () const
```



Returns the name string identifier. 


        

<hr>



### function parameters 

```C++
inline virtual QList< CogoParameter > CogoOperation::parameters (
    Qgis::WkbType wkbType=Qgis::WkbType::Unknown
) const
```



Returns the list of available parameters to configure the operation. 

**Parameters:**


* `wkbType` An optional WKB type to reflect ability of the geometry being digitized 




        

<hr>



### function visualGuides 

```C++
inline virtual QList< CogoVisualGuide > CogoOperation::visualGuides (
    const QVariantMap & parameters,
    QgsQuickMapSettings * mapSettings
) const
```



Returns a list of visual guides based on provided parameters. 

**Parameters:**


* `parameters` the parameters used to generate the visual guides 
* `mapSettings` the map settings object used to georeference the visual guides 




        

<hr>



### function ~CogoOperation 

```C++
virtual CogoOperation::~CogoOperation () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/cogo/cogooperation.h`

