

# Class RelationUtils



[**ClassList**](annotated.md) **>** [**RelationUtils**](classRelationUtils.md)








Inherits the following classes: QObject


































## Public Functions

| Type | Name |
| ---: | :--- |
|   | [**RelationUtils**](#function-relationutils) (QObject \* parent=nullptr) <br> |
|  Q\_INVOKABLE QgsPolymorphicRelation | [**addPolymorphicRelation**](#function-addpolymorphicrelation) (QgsProject \* project, const QVariantList & referencedLayers, const QgsVectorLayer \* referencingLayer, const QVariantMap & fieldPairs, const QString & referencedLayerField, const QString & referencedLayerExpression) <br> |
|  Q\_INVOKABLE QgsPolymorphicRelation | [**createPolymorphicRelation**](#function-createpolymorphicrelation) (const QVariantList & referencedLayers, const QgsVectorLayer \* referencingLayer, const QVariantMap & fieldPairs, const QString & referencedLayerField, const QString & referencedLayerExpression) <br> |
|  Q\_INVOKABLE QgsRelation | [**createRelation**](#function-createrelation) (const QgsVectorLayer \* referencedLayer, const QgsVectorLayer \* referencingLayer, const QVariantMap & fieldPairs) <br> |
|  Q\_INVOKABLE QgsRelation | [**resolveReferencingRelation**](#function-resolvereferencingrelation) (QgsProject \* project, QgsVectorLayer \* layer, const QString & fieldName, const QString & relationId) <br> |




























## Public Functions Documentation




### function RelationUtils 

```C++
explicit RelationUtils::RelationUtils (
    QObject * parent=nullptr
) 
```




<hr>



### function addPolymorphicRelation 

```C++
Q_INVOKABLE QgsPolymorphicRelation RelationUtils::addPolymorphicRelation (
    QgsProject * project,
    const QVariantList & referencedLayers,
    const QgsVectorLayer * referencingLayer,
    const QVariantMap & fieldPairs,
    const QString & referencedLayerField,
    const QString & referencedLayerExpression
) 
```



Creates and adds a polymorphic relation to the project. This is a convenience function that creates the relation and adds it to the project's relation manager.




**Parameters:**


* `project` The project to add the relation to 
* `referencedLayers` The list of referenced (parent) layers 
* `referencingLayer` The referencing (child) layer 
* `fieldPairs` A map where keys are referenced field names and values are referencing field names 
* `referencedLayerField` The field name in the child layer that stores the parent layer name 
* `referencedLayerExpression` The expression to identify the parent layer name (e.g., "@layer\_name") 



**Returns:**

A QgsPolymorphicRelation object 





        

<hr>



### function createPolymorphicRelation 

```C++
Q_INVOKABLE QgsPolymorphicRelation RelationUtils::createPolymorphicRelation (
    const QVariantList & referencedLayers,
    const QgsVectorLayer * referencingLayer,
    const QVariantMap & fieldPairs,
    const QString & referencedLayerField,
    const QString & referencedLayerExpression
) 
```



Creates a polymorphic relation between multiple referenced layers and a referencing layer. Note: Layers must be added to the project first




**Parameters:**


* `referencedLayers` The list of referenced (parent) layers 
* `referencingLayer` The referencing (child) layer 
* `fieldPairs` A map where keys are referenced field names and values are referencing field names 
* `referencedLayerField` The field name in the child layer that stores the parent layer name 
* `referencedLayerExpression` The expression to identify the parent layer name (e.g., "@layer\_name") 



**Returns:**

A QgsPolymorphicRelation object 





        

<hr>



### function createRelation 

```C++
Q_INVOKABLE QgsRelation RelationUtils::createRelation (
    const QgsVectorLayer * referencedLayer,
    const QgsVectorLayer * referencingLayer,
    const QVariantMap & fieldPairs
) 
```



Creates a relation between referenced and referencing layers. Note: Layers must be added to the project first




**Parameters:**


* `referencedLayer` The referenced (parent) layer 
* `referencingLayer` The referencing (child) layer 
* `fieldPairs` A map where keys are referenced field names and values are referencing field names 



**Returns:**

A QgsRelation object 





        

<hr>



### function resolveReferencingRelation 

```C++
Q_INVOKABLE QgsRelation RelationUtils::resolveReferencingRelation (
    QgsProject * project,
    QgsVectorLayer * layer,
    const QString & fieldName,
    const QString & relationId
) 
```




<hr>

------------------------------
The documentation for this class was generated from the following file `src/core/utils/relationutils.h`

