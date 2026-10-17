

# Class QfCogoOperation



[**ClassList**](annotated.md) **>** [**QfCogoOperation**](classQfCogoOperation.md)



_A COGO operation._ 

* `#include <qfcogooperation.h>`





Inherited by the following classes: [QfCogoOperationPointAtDistanceAngle](classQfCogoOperationPointAtDistanceAngle.md),  [QfCogoOperationPointAtIntersectionCircles](classQfCogoOperationPointAtIntersectionCircles.md),  [QfCogoOperationPointAtXYZ](classQfCogoOperationPointAtXYZ.md)
































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**QfCogoOperation**](#function-qfcogooperation) () <br> |
| virtual bool | [**checkReadiness**](#function-checkreadiness) (const QVariantMap & parameters, Qgis::WkbType wkbType=Qgis::WkbType::Unknown) const<br> |
| virtual QString | [**displayName**](#function-displayname) () const<br> |
| virtual bool | [**execute**](#function-execute) ([**QfRubberbandModel**](classQfRubberbandModel.md) \* rubberbandModel, const QVariantMap & parameters, Qgis::WkbType wkbType=Qgis::WkbType::Unknown) const<br> |
| virtual QString | [**icon**](#function-icon) () const<br> |
| virtual QString | [**name**](#function-name) () const<br> |
| virtual QList&lt; [**QfCogoParameter**](classQfCogoParameter.md) &gt; | [**parameters**](#function-parameters) (Qgis::WkbType wkbType=Qgis::WkbType::Unknown) const<br> |
| virtual QList&lt; [**QfCogoVisualGuide**](classQfCogoVisualGuide.md) &gt; | [**visualGuides**](#function-visualguides) (const QVariantMap & parameters, [**QgsQuickMapSettings**](classQgsQuickMapSettings.md) \* mapSettings) const<br> |
| virtual  | [**~QfCogoOperation**](#function-qfcogooperation) () = default<br> |




























## Public Functions Documentation




### function QfCogoOperation 

```C++
inline QfCogoOperation::QfCogoOperation () 
```



The COGO operation constructor. 


        

<hr>



### function checkReadiness 

```C++
inline virtual bool QfCogoOperation::checkReadiness (
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
inline virtual QString QfCogoOperation::displayName () const
```



Returns a translatable display name. 


        

<hr>



### function execute 

```C++
inline virtual bool QfCogoOperation::execute (
    QfRubberbandModel * rubberbandModel,
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
inline virtual QString QfCogoOperation::icon () const
```



Returns the icon name. 


        

<hr>



### function name 

```C++
inline virtual QString QfCogoOperation::name () const
```



Returns the name string identifier. 


        

<hr>



### function parameters 

```C++
inline virtual QList< QfCogoParameter > QfCogoOperation::parameters (
    Qgis::WkbType wkbType=Qgis::WkbType::Unknown
) const
```



Returns the list of available parameters to configure the operation. 

**Parameters:**


* `wkbType` An optional WKB type to reflect ability of the geometry being digitized 




        

<hr>



### function visualGuides 

```C++
inline virtual QList< QfCogoVisualGuide > QfCogoOperation::visualGuides (
    const QVariantMap & parameters,
    QgsQuickMapSettings * mapSettings
) const
```



Returns a list of visual guides based on provided parameters. 

**Parameters:**


* `parameters` the parameters used to generate the visual guides 
* `mapSettings` the map settings object used to georeference the visual guides 




        

<hr>



### function ~QfCogoOperation 

```C++
virtual QfCogoOperation::~QfCogoOperation () = default
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/cogo/qfcogooperation.h`

